#include "parameters.h"

double inputfunc(const double *input)
{
  TString particle = PARTICLE;
  TString prod = PROD; //P or NP
  TString rapidity = RAPIDITY; //1,2,3
  int low = LOW;
  int high = HIGH;
  TString table;

  if (particle == "Jpsi"&& prod == "P") {
    table = "1";
  }
  if (particle == "Jpsi" && prod == "NP") {
    table = "2";
  }
  if (particle == "Psi2S" && prod == "P") {
    table = "3";
  }
  if (particle == "Psi2S" && prod == "NP") {
    table = "4";
  }
  TFile f = TFile("HEPData-ins2705040-v1-root.root");
  TDirectoryFile* dir = (TDirectoryFile*)f.Get("Table " + static_cast<TString>(table));
  TH1F* Data = (TH1F*) dir->Get("Hist1D_y"+rapidity);
  TH1F* error1 = (TH1F*) dir->Get("Hist1D_y"+rapidity+"_e1");
  TH1F* error2 = (TH1F*) dir->Get("Hist1D_y"+rapidity+"_e2");


  const double a = input[0];
  const double b = input[1];
  const double en = input[2];
  const double lambda = input[3];
  //const double c = input[4];
  const int n = 19;

  ROOT::Math::SVector<double,n> difference;
  ROOT::Math::SMatrix<double,n> staterror;
  ROOT::Math::SMatrix<double,n> syserror;


  int counter = 0;
  for (size_t i = 0; i < 34; i++) {
    if (Data->GetBinCenter(i+1)<high && Data->GetBinCenter(i+1)>low) {
      double x = Data->GetBinCenter(i+1);
      double y = Data->GetBinContent(i+1)*correction(lambda, i+1);
      difference[counter] = y-a*pow((1+(x/b)),en)*(1+0*log(x/60));

      staterror(counter,counter) = error1->GetBinContent(i+1)*correction(lambda, i+1);
      syserror(counter,counter) = error2->GetBinContent(i+1)*correction(lambda, i+1);
      counter+=1;
    }
  }

  ROOT::Math::SMatrix<double,n> correlation;

  double corr = 0.5;
  //Creates a correlation matrix with some bin to bin correlation
  for (size_t i = 0; i < n; i++) {
    for (size_t j = 0; j < n; j++) {
      if ((i-j)*(i-j)== 1) {
        correlation(i,j) = corr;
        /*if (i<10) {
          correlation(i,j) = corr;
        }
        else{correlation(i,j) = corr/10;}*/
      }
      else {
        correlation(i,j) = 0;
      }
      if (i==j) {
        correlation(i,j) = 1;
      }
      //if (i*j == 9*10) {
        //correlation(i,j) = 0;
      //}
    }
  }

  /*ROOT::Math::SVector<double,n> p (x,n);
  ROOT::Math::SVector<double,n> expected = a*p+b;
  ROOT::Math::SVector<double,n> sigma (y,n);
  ROOT::Math::SMatrix<double,n> m(ey,n*n);
  ROOT::Math::SMatrix<double,n> corr(rho,n*n);*/
  ROOT::Math::SMatrix<double,n> E = syserror*syserror + staterror*correlation*staterror;
  //  Invert a NxN matrix. The inverted matrix replace the existing one and returns if the result is successful
  bool ret = E.Invert();
  //bool out = correlation.Invert();
  //cout<<ROOT::Math::Similarity(difference,E)<<endl;

  //cout<<particle<<prod<<rapidity<<endl;
  return ROOT::Math::Similarity(difference,E);

}

int Correlations(const char * minName = "Minuit2",
                          const char *algoName = "Minos" ,
                          int randomSeed = -1)
{
   // create minimizer giving a name and a name (optionally) for the specific
   // algorithm
   // possible choices are:
   //     minName                  algoName
   // Minuit /Minuit2             Migrad, Simplex,Combined,Scan  (default is Migrad)
   //  Minuit2                     Fumili2
   //  Fumili
   //  GSLMultiMin                ConjugateFR, ConjugatePR, BFGS,
   //                              BFGS2, SteepestDescent
   //  GSLMultiFit
   //   GSLSimAn
   //   Genetic
   ROOT::Math::Minimizer* minimum =
      ROOT::Math::Factory::CreateMinimizer(minName, algoName);
   if (!minimum) {
      std::cerr << "Error: cannot create minimizer \"" << minName
                << "\". Maybe the required library was not built?" << std::endl;
      return 1;
   }

   // set tolerance , etc...
   minimum->SetMaxFunctionCalls(1000000); // for Minuit/Minuit2
   minimum->SetMaxIterations(10000);  // for GSL
   minimum->SetTolerance(0.001);
   minimum->SetPrintLevel(1);

   // create function wrapper for minimizer
   // a IMultiGenFunction type
   ROOT::Math::Functor f(&inputfunc,4);
   double step[4] = {0.01,0.01,0.01,0.01};
   // starting point

   double variable[4] = {1e5,15.2,-6,-0.55};
   if (randomSeed >= 0) {
      TRandom2 r(randomSeed);
      variable[0] = r.Uniform(-20,20);
      variable[1] = r.Uniform(-20,20);
      variable[2] = r.Uniform(-20,20);
      variable[3] = r.Uniform(-20,20);
      //variable[4] = r.Uniform(-20,20);
   }

   minimum->SetFunction(f);

   // Set the free variables to be minimized !
   minimum->SetVariable(0,"a",variable[0], step[0]);
   minimum->SetVariable(1,"b",variable[1], step[1]);
   minimum->SetVariable(2,"n",variable[2], step[2]);
   minimum->SetVariable(3,"lambda",variable[3], step[3]);
   //minimum->SetVariable(4,"c",variable[4], step[4]);

   // do the minimization
   minimum->Minimize();

   const double *xs = minimum->X();
   std::cout << "Minimum: f(" << xs[0] << "," << xs[1] << "," << xs[2] << "," << xs[3] << "): "
             << minimum->MinValue()  << std::endl;

   // expected minimum is 0
   /*if ( minimum->MinValue()  < 1.E-4 )
      std::cout << "Minimizer " << minName << " - " << algoName
                << "   converged to the right minimum" << std::endl;
   else {
      std::cout << "Minimizer " << minName << " - " << algoName
                << "   failed to converge !!!" << std::endl;
      Error("NumericalMinimization","fail to converge");
   }*/

   //return minimum->MinValue();

   return xs[3];
}
