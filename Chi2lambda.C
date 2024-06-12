void Chi2graph(TString particle, TString prod, TString rapidity, int low, int high) {

  int n = 100;
  TH1* chi2hist = new TH1F("chi2hist", "Lambda Chi2 for "+prod+" "+particle+" at rapidity slice "+rapidity, n, -1,1);

  double chi2min = 10000;
  double minlambda;
  int count;

  for (size_t i = 1; i < n+1; i++) {
    double lambda = (((2.*i-1)/n)-1);
    double chi2 = CorrectedGraph_Data(particle, prod, rapidity, lambda, low, high,false);
    chi2hist->SetBinContent(i,chi2);

    if (chi2min == chi2) {
      minlambda = (count*minlambda + lambda)/(count+1);
      count+=1;
    }
    if (chi2 < chi2min) {
      chi2min = chi2;
      minlambda = lambda;
      count = 1;
    }
  }

  double lowerror;
  double higherror;
  double dist = 5;

  for (size_t i = 1; i < n+1; i++) {
    double lambda = (((2.*i-1)/n)-1);
    double chi2 = chi2hist->GetBinContent(i);
    double newdist = (chi2-(chi2min+1))*(chi2-(chi2min+1));

    if (lambda < minlambda) {
      if (newdist < dist) {
        lowerror = lambda;
        dist = newdist;
      }
    }
    if (lambda == minlambda) {
      dist = 5;
    }
    if (lambda > minlambda) {
      if (newdist < dist) {
        higherror = lambda;
        dist = newdist;
      }
    }
  }


  TCanvas *c1 = new TCanvas("c","Chi2 Data");

  chi2hist->GetYaxis()->SetTitle("Fit Chi square value");
  chi2hist->GetXaxis()->SetTitle("\\lambda");
  chi2hist->GetXaxis()->SetRange(lowerror,higherror);

  chi2hist->Draw();

  auto line = new TLine(-1, chi2min+1, 1, chi2min+1);
  line->SetLineColor(kRed);

  line->Draw("");

  cout<<"Min Chi2 of "<<chi2min<<" at lambda = "<<minlambda<<endl;
  cout<<"Low Error of "<<lowerror-minlambda<<" and High Error of "<<higherror-minlambda<<endl;
}

void Chi2graph_Omit(TString particle, TString prod, TString rapidity, int low, int high) {

  int n = 100;
  TH1* chi2hist = new TH1F("chi2hist", "Lambda Chi2 for "+prod+" "+particle+" at rapidity slice "+rapidity, n, -1,1);

  double chi2min = 10000;
  double minlambda;
  int count;

  for (size_t i = 1; i < n+1; i++) {
    double lambda = (((2.*i-1)/n)-1);
    double chi2 = Specific_Corrections(particle, prod, rapidity, lambda, low, high,false);
    chi2hist->SetBinContent(i,chi2);

    if (chi2min == chi2) {
      minlambda = (count*minlambda + lambda)/(count+1);
      count+=1;
    }
    if (chi2 < chi2min) {
      chi2min = chi2;
      minlambda = lambda;
      count = 1;
    }
  }

  double lowerror;
  double higherror;
  double dist = 5;

  for (size_t i = 1; i < n+1; i++) {
    double lambda = (((2.*i-1)/n)-1);
    double chi2 = chi2hist->GetBinContent(i);
    double newdist = (chi2-(chi2min+1))*(chi2-(chi2min+1));

    if (lambda < minlambda) {
      if (newdist < dist) {
        lowerror = lambda;
        dist = newdist;
      }
    }
    if (lambda == minlambda) {
      dist = 5;
    }
    if (lambda > minlambda) {
      if (newdist < dist) {
        higherror = lambda;
        dist = newdist;
      }
    }
  }


  TCanvas *c1 = new TCanvas("c","Chi2 Data");

  chi2hist->GetYaxis()->SetTitle("Fit Chi square value");
  chi2hist->GetXaxis()->SetTitle("\\lambda");
  chi2hist->GetXaxis()->SetRange(lowerror,higherror);

  chi2hist->Draw();

  auto line = new TLine(-1, chi2min+1, 1, chi2min+1);
  line->SetLineColor(kRed);

  line->Draw("");

  cout<<"Min Chi2 of "<<chi2min<<" at lambda = "<<minlambda<<endl;
  cout<<"Low Error of "<<lowerror-minlambda<<" and High Error of "<<higherror-minlambda<<endl;
}
