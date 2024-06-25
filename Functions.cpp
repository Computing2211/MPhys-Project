double fitfunction1(double *x, double *par) {
    double fitval = par[0]* pow(x[0]/par[1] + 1, par[2]);
    return fitval;
}

double fitfunction2(double *x, double *par) {
    double fitval = par[0]* pow(((x[0])*(x[0]))/(par[1]*par[1])+1, par[2]/2);
    return fitval;
}

double gauss(double *x, double *par){
  double fitval = exp(-par[0]*par[0]*x[0]*x[0]+par[1]*x[0]+par[2]);
  return fitval;
}

double polyfunc(double *x, double *par){
  double val = 1+log10(x[0]/60)*par[3]+log10(x[0]/60)*log10(x[0]/60)*par[4];
  return val;
}

double fitfunc(double *x, double *par){
  return fitfunction1(x,par)*polyfunc(x,par);
}

double fline(double *x, double *par)
{
    if (x[0] > 60 && x[0] < 100) {
      TF1::RejectPoint();
      //cout<<"Rejected"<<endl;
      return 0;
   }
   return fitfunc(x,par);
}

double constantratio(double *x, double *par){
  return 1.;
}

double constantpull(double *x, double *par){
  return 0.;
}

double correction(double lambda, int bin, bool error = false){
  TFile f2 = TFile("HEPData-ins2705040-v1-root.root");

  //f.ls();

  TDirectoryFile* dir2 = (TDirectoryFile*)f2.Get("Table 9");

  //dir->ls();

  TH1F* Factor = (TH1F*) dir2->Get("Hist1D_y1");

  double C0 = Factor->GetBinContent(bin);

  double delta = (3*C0-2)/C0;

  double value = (3+lambda)/(3+lambda*delta);

  if (lambda == 0) {
    return 1;
  }
  if (error) {
    C0 -= 0.005/pow(3,0.5);

    delta = (3*C0-2)/C0;

    value -= ((3+lambda)/(3+lambda*delta));
      return value;
  }
  else {
    return value;
}
}

double factorfunc(double *x, double *par){
  double fitval = 1+ par[0]* pow(x[0]+par[1], par[2]);
  return fitval;
}

double correctfunc(double *x, double *par){
  if (x[0] > 60 && x[0] < 120) {
    TF1::RejectPoint();
    //cout<<"Rejected"<<endl;
    return 0;
 }
  return factorfunc(x, par);
}
