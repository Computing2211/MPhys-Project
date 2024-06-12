#include<cmath>

void Graph_Data(TString particle, TString prod, TString rapidity, int low, int high) {

  TString table;
  double b;

  if (particle == "Jpsi"&& prod == "P") {
    table = "1";
    b = 8.796;
  }

  if (particle == "Jpsi" && prod == "NP") {
    table = "2";
    b = 15.62;
  }

  if (particle == "Psi2S" && prod == "P") {
    table = "3";
    b = 7.486;
  }

  if (particle == "Psi2S" && prod == "NP") {
    table = "4";
    b = 13.83;
  }

  TFile f = TFile("HEPData-ins2705040-v1-root.root");

  //f.ls();

  TDirectoryFile* dir = (TDirectoryFile*)f.Get("Table " + static_cast<TString>(table));

  //dir->ls();

  TH1F* Data = (TH1F*) dir->Get("Hist1D_y"+rapidity);

    TCanvas *c1 = new TCanvas("c1","First Set of Data");
    c1->SetLogy();
    c1->SetLogx();

    TH1F* error1 = (TH1F*) dir->Get("Hist1D_y"+rapidity+"_e1");
    TH1F* error2 = (TH1F*) dir->Get("Hist1D_y"+rapidity+"_e2");

    int n = 35;

    for (size_t i = 1; i < n; i++) {
      double binContent1 = error1 -> GetBinContent(i);
      double binContent2 = error2 -> GetBinContent(i);

      //cout << binContent << endl;

      Data->SetBinError(i, pow(binContent1*binContent1 + binContent2*binContent2, .5));
    }

    Data -> SetMarkerStyle(20);

    Data -> SetMarkerSize(.75);

    Data->GetYaxis()->SetTitle("Differential Cross Section [fb/GeV]");

    Data->SetTitle(prod+" "+particle+" at rapidity slice number "+ rapidity);

    Data->SetName("");



    //TF1 *func = new TF1("func", "[0]+[1]*pow(x,[2])", 50, 200);

    TF1 *func = new TF1("func", fitfunction1, low, high, 3);

    func-> SetParameters(5e4,8.5,-5.6);

    //func-> FixParameter(0,0);

    func-> FixParameter(1,b);

    //func-> SetParLimits(2,-5,-7);

    TFitResultPtr r = Data->Fit("func", "R S");

    // Access the covariance matrix.
    TMatrixDSym cov = r->GetCovarianceMatrix();

    // Retrieve the fit chi2.
    double chi2 = r->Chi2();

    // Retrieve the value for the parameter 0.
    double par0 = r->Parameter(0);

    // Retrieve the error for the parameter 0.
    double err0 = r->ParError(0);

    // Print the full information of the fit including covariance matrix.
    r->Print("V");

    gStyle->SetOptFit(1);
    gStyle->SetOptStat("");


    Data-> DrawCopy("E1 X");
}

void Ratio(TString particle, TString prod, TString rapidity, int low, int high){
  TString table;
  double b;

  if (particle == "Jpsi"&& prod == "P") {
    table = "1";
    b = 8.796;
  }

  if (particle == "Jpsi" && prod == "NP") {
    table = "2";
    b = 15.62;
  }

  if (particle == "Psi2S" && prod == "P") {
    table = "3";
    b = 7.486;
  }

  if (particle == "Psi2S" && prod == "NP") {
    table = "4";
    b = 13.83;
  }

  TFile f = TFile("HEPData-ins2705040-v1-root.root");

  //f.ls();

  TDirectoryFile* dir = (TDirectoryFile*)f.Get("Table " + static_cast<TString>(table));

  //dir->ls();

  TH1F* Data = (TH1F*) dir->Get("Hist1D_y"+rapidity);

    TCanvas *c1 = new TCanvas("c1","First Set of Data");
    //c1->SetLogy();
    c1->SetLogx();

    TH1F* error1 = (TH1F*) dir->Get("Hist1D_y"+rapidity+"_e1");
    TH1F* error2 = (TH1F*) dir->Get("Hist1D_y"+rapidity+"_e2");

    int n = 35;

    for (size_t i = 1; i < n; i++) {
      double binContent1 = error1 -> GetBinContent(i);
      double binContent2 = error2 -> GetBinContent(i);

      //cout << binContent << endl;

      Data->SetBinError(i, pow(binContent1*binContent1 + binContent2*binContent2, .5));
    }

    Data -> SetMarkerStyle(20);

    Data -> SetMarkerSize(.75);

    Data->GetYaxis()->SetTitle("Data to fit ratio");

    Data->SetTitle(prod+" "+particle+" at rapidity slice number "+ rapidity);

    Data->SetName("");



    //TF1 *func = new TF1("func", "[0]+[1]*pow(x,[2])", 50, 200);

    TF1 *func = new TF1("func", fitfunction2, low, high, 3);

    func-> SetParameters(5e4,8.5,-5.6);

    //func-> FixParameter(0,0);

    //func-> FixParameter(1,b);

    TH1F* Ratio = (TH1F*)Data->Clone();

    //func-> SetParLimits(2,-5,-7);

    Data->Fit("func", "R S");

    //double par[3];

    //func->GetParameters(par);

    //cout<<func->Eval(60)<<endl;

    TAxis* xAxis = Ratio->GetXaxis();

    for (size_t i = 1; i < n; i++) {
      //double binContent1 = error1 -> GetBinContent(i);
      //double binContent2 = error2 -> GetBinContent(i);

      //cout << binContent << endl;

      //Data->SetBinError(i, pow(binContent1*binContent1 + binContent2*binContent2, .5));
      Ratio->SetBinContent(i, (Data->GetBinContent(i))/(func->Eval(xAxis->GetBinCenter(i))));

      Ratio->SetBinError(i, (Data->GetBinError(i))/(func->Eval(xAxis->GetBinCenter(i))));
    }

    Ratio->DrawCopy("E1 X");

    TF1 *constfunc = new TF1("constfunc", constantratio, low, high, 0);
    constfunc->Draw("same");
}

void Pull(TString particle, TString prod, TString rapidity, int low, int high){
  TString table;
  double b;

  if (particle == "Jpsi"&& prod == "P") {
    table = "1";
    b = 8.796;
  }

  if (particle == "Jpsi" && prod == "NP") {
    table = "2";
    b = 15.62;
  }

  if (particle == "Psi2S" && prod == "P") {
    table = "3";
    b = 7.486;
  }

  if (particle == "Psi2S" && prod == "NP") {
    table = "4";
    b = 13.83;
  }

  TFile f = TFile("HEPData-ins2705040-v1-root.root");

  //f.ls();

  TDirectoryFile* dir = (TDirectoryFile*)f.Get("Table " + static_cast<TString>(table));

  //dir->ls();

  TH1F* Data = (TH1F*) dir->Get("Hist1D_y"+rapidity);

    TCanvas *c1 = new TCanvas("c1","First Set of Data");
    //gStyle->SetOptStat(kFALSE);
    //c1->SetLogy();
    c1->SetLogx();

    TH1F* error1 = (TH1F*) dir->Get("Hist1D_y"+rapidity+"_e1");
    TH1F* error2 = (TH1F*) dir->Get("Hist1D_y"+rapidity+"_e2");

    int n = 35;

    for (size_t i = 1; i < n; i++) {
      double binContent1 = error1 -> GetBinContent(i);
      double binContent2 = error2 -> GetBinContent(i);

      //cout << binContent << endl;

      Data->SetBinError(i, pow(binContent1*binContent1 + binContent2*binContent2, .5));
    }

    Data -> SetMarkerStyle(20);

    Data -> SetMarkerSize(.75);

    Data->GetYaxis()->SetTitle("Pull Distribution");

    Data->SetTitle(prod+" "+particle+" at rapidity slice number "+ rapidity);

    Data->SetName("");

    TF1 *func = new TF1("func", fitfunction2, low, high, 3);

    func-> SetParameters(5e4,8.5,-5.6);

    //func-> FixParameter(1,b);

    TH1F* Pull = (TH1F*)Data->Clone();

    Data->Fit("func", "R S");

    TAxis* xAxis = Pull->GetXaxis();

    for (size_t i = 1; i < n; i++) {
      Pull->SetBinContent(i, (Data->GetBinContent(i)-(func->Eval(xAxis->GetBinCenter(i))))/(Data->GetBinError(i)));

    }

    Pull->DrawCopy("HIST X");

    TF1 *constfunc = new TF1("constfunc", constantpull, low, high, 0);
    constfunc->Draw("same");

}

void All(TString particle, TString prod, TString rapidity, int low, int high) {

  TString table;
  double b;

  if (particle == "Jpsi"&& prod == "P") {
    table = "1";
    b = 8.796;
  }

  if (particle == "Jpsi" && prod == "NP") {
    table = "2";
    b = 15.62;
  }

  if (particle == "Psi2S" && prod == "P") {
    table = "3";
    b = 7.486;
  }

  if (particle == "Psi2S" && prod == "NP") {
    table = "4";
    b = 13.83;
  }

  TFile f = TFile("HEPData-ins2705040-v1-root.root");

  TDirectoryFile* dir = (TDirectoryFile*)f.Get("Table " + static_cast<TString>(table));

  TH1F* Data = (TH1F*) dir->Get("Hist1D_y"+rapidity);

    TH1F* error1 = (TH1F*) dir->Get("Hist1D_y"+rapidity+"_e1");
    TH1F* error2 = (TH1F*) dir->Get("Hist1D_y"+rapidity+"_e2");

    int n = 35;

    for (size_t i = 1; i < n; i++) {
      double binContent1 = error1 -> GetBinContent(i);
      double binContent2 = error2 -> GetBinContent(i);

      //cout << binContent << endl;

      Data->SetBinError(i, pow(binContent1*binContent1 + binContent2*binContent2, .5));
    }

    Data -> SetMarkerStyle(20);

    Data -> SetMarkerSize(.75);

    Data->SetTitle("");

    Data->GetYaxis()->SetTitle("Differential Cross Section [fb/GeV]");
    Data->GetYaxis()->SetTitleSize(0.05);

    Data->SetName("");



    //TF1 *func = new TF1("func", "[0]+[1]*pow(x,[2])", 50, 200);

    TF1 *func = new TF1("func", fitfunction2, low, high, 3);

    func-> SetParameters(5e4,8.5,-5.6);

    //func-> FixParameter(0,0);

    //func-> FixParameter(1,b);


    TH1F* Ratio = (TH1F*)Data->Clone();
    TH1F* Pull = (TH1F*)Data->Clone();

    TFitResultPtr r = Data->Fit("func", "R S");

    // Access the covariance matrix.
    TMatrixDSym cov = r->GetCovarianceMatrix();

    // Retrieve the fit chi2.
    double chi2 = r->Chi2();

    // Retrieve the value for the parameter 0.
    double par0 = r->Parameter(0);

    // Retrieve the error for the parameter 0.
    double err0 = r->ParError(0);

    // Print the full information of the fit including covariance matrix.
    r->Print("V");

    TAxis* xAxis = Ratio->GetXaxis();

    for (size_t i = 1; i < n; i++) {
      Ratio->SetBinContent(i, (Data->GetBinContent(i))/(func->Eval(xAxis->GetBinCenter(i))));

      Ratio->SetBinError(i, (Data->GetBinError(i))/(func->Eval(xAxis->GetBinCenter(i))));

      Pull->SetBinContent(i, (Data->GetBinContent(i)-(func->Eval(xAxis->GetBinCenter(i))))/(Data->GetBinError(i)));
    }




    TCanvas *c1 = new TCanvas("c1","First Set of Data",0,0,700,900);

    TPaveLabel *title = new TPaveLabel(0.1,0.94,0.9,0.98, prod+" "+particle+" at rapidity slice number "+ rapidity);
   //title->SetFillColor(16);
   //title->SetTextFont(42);
   title->Draw();

    TPad *pad3 = new TPad("pad3","pad3",0,0,1,0.32);
   TPad *pad2 = new TPad("pad2","pad2",0,0.32,1,0.6);
   TPad *pad1 = new TPad("pad1","pad1",0,0.6,1,0.93);
    pad1->Draw(); pad2->Draw(); pad3->Draw();

    pad1->cd();
    pad1->SetLogx();
    pad1->SetLogy();
    gStyle->SetOptFit(1);
    gStyle->SetOptStat("");
    Data-> DrawCopy("E1 X");

    pad2->cd();
    pad2->SetLogx();
    Ratio->GetYaxis()->SetTitle("Data to fit ratio");
    Ratio->DrawCopy("E1 X");
    TF1 *constfunc1 = new TF1("constfunc1", constantratio, low, high, 0);
    constfunc1->Draw("same");

    pad3->cd();
    pad3->SetLogx();
    Pull->GetYaxis()->SetTitle("Pull Distribution");
    Pull->DrawCopy("HIST X");
    TF1 *constfunc2 = new TF1("constfunc2", constantpull, low, high, 0);
    constfunc2->Draw("same");
}
