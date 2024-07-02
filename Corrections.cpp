void Corrections(double lambda) {
  TFile f2 = TFile("HEPData-ins2705040-v1-root.root");

  //f.ls();

  TDirectoryFile* dir2 = (TDirectoryFile*)f2.Get("Table 9");

  //dir->ls();

  TH1F* Factor = (TH1F*) dir2->Get("Hist1D_y1");

  TH1F* Compare = (TH1F*) dir2->Get("Hist1D_y1");

  int bins = 35;

  for (size_t i = 0; i < bins; i++) {
    Factor->SetBinContent(i, correction(lambda, i));
    Factor->SetBinError(i, correction(lambda, i, true));
  }

  TF1 *func = new TF1("func", correctfunc, 12,360, 3);

  func-> SetParameters(10,10,-1);

  TH1F* Ratio = (TH1F*)Factor->Clone();

  TFitResultPtr r1 = Factor->Fit("func", "Q R S E M");

  TAxis* xAxis = Factor->GetXaxis();



    TCanvas *c1 = new TCanvas("c1","Corrected Data");
    c1->SetLogx();

    //c1->SetLogy();
    gStyle->SetOptFit(0);
    gStyle->SetOptStat("");

    TString polarisation = Form("lambda = %g", lambda);

    Ratio->SetTitle("\\mbox{Specific Correction Factor Provided} (\\lambda_{\\vartheta} = "+polarisation+")");
    Factor->SetTitle("\\mbox{Correction Factor Calculated} (\\lambda_{\\vartheta} = "+polarisation+")");
    Factor->SetTitle("");

    Factor->SetName("");

    Factor -> SetMarkerStyle(20);

    Factor -> SetMarkerSize(.75);

    TF1 *func2 = new TF1("func", factorfunc, 12,360, 3);

    func2-> SetParameters(r1->Parameter(0),r1->Parameter(1),r1->Parameter(2));

    for (size_t i = 0; i < bins; i++) {
      double entry = Factor->GetBinContent(i);
      double expected = func2->Eval(xAxis->GetBinCenter(i));
      //cout<<entry<<" "<<expected<<endl;
      Ratio->SetBinContent(i, entry/expected);
    }

    //Draw either the ratio of the fit to the correction, or the raw correction values
    //Ratio -> DrawCopy("HIST X 0");
    Factor ->DrawCopy("HIST");
    //func2->ResetBit(1<<9);
    func2->Draw("SAME");
}

double CorrectedGraph_Data(TString particle, TString prod, TString rapidity, double lambda, int low, int high, bool Draw) {

    TString polarisation = Form("lambda = %g", lambda);

    Fitting(particle, prod, rapidity, lambda, low, high, Draw);

    //double num = Fit_Omit(particle, prod, rapidity, lambda, low, high, Draw);

    TFile file = TFile("Fitted_Data.root");

    TH1F* Data = (TH1F*) file.Get("Data");

    TF1 *fit = Data->GetFunction("func");

    double chi2 = fit->GetChisquare();

    if (Draw) {
      TCanvas *c1 = new TCanvas("c1","Corrected Data");
      c1->SetLogy();
      c1->SetLogx();
      gStyle->SetOptFit(1);
      gStyle->SetOptStat("");

    Data-> DrawCopy("E1 X 0");
    fit->Draw("SAME 0");
}
    return chi2;
}

void CorrectedRatio(TString particle, TString prod, TString rapidity, double lambda, int low, int high){
  TString table;
  double b;
  TString polarisation = Form("lambda = %g", lambda);
  /*TString polarisation;
  if (selection == "1") {
    polarisation = "1";
  }
  if (selection == "2") {
    polarisation = "0.2";
  }
  if (selection == "3") {
    polarisation = "-0.2";
  }
  if (selection == "4") {
    polarisation = "-1";
  }*/

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

  TFile f2 = TFile("HEPData-ins2705040-v1-root.root");

  //f.ls();

  TDirectoryFile* dir2 = (TDirectoryFile*)f2.Get("Table 9");

  //dir->ls();

  //TH1F* Factor = (TH1F*) dir2->Get("Hist1D_y"+selection);

    TCanvas *c1 = new TCanvas("c1","Corrected Data");
    //c1->SetLogy();
    c1->SetLogx();

    TH1F* error1 = (TH1F*) dir->Get("Hist1D_y"+rapidity+"_e1");
    TH1F* error2 = (TH1F*) dir->Get("Hist1D_y"+rapidity+"_e2");

    int n = 35;

    for (size_t i = 1; i < n; i++) {
      double binContent1 = error1 -> GetBinContent(i);
      double binContent2 = error2 -> GetBinContent(i);

      //cout << binContent << endl;

      //Data->SetBinError(i, pow(binContent1*binContent1 + binContent2*binContent2, .5));
      Data->SetBinError(i, (pow(binContent1*binContent1 + binContent2*binContent2, .5))*(correction(lambda, i)));
      //Data->SetBinContent(i, (Data->GetBinContent(i))*(Factor->GetBinContent(i)));
      Data->SetBinContent(i, (Data->GetBinContent(i))*(correction(lambda, i)));
    }

    Data -> SetMarkerStyle(20);

    Data -> SetMarkerSize(.75);

    Data->GetYaxis()->SetTitle("Data to fit ratio");

    Data->SetTitle(prod+" "+particle+" at rapidity slice number "+ rapidity+" (pol.= "+polarisation+")");

    Data->SetName("");



    //TF1 *func = new TF1("func", "[0]+[1]*pow(x,[2])", 50, 200);

    TF1 *func = new TF1("func", fitfunc, low, high, 5);

    func-> SetParameters(5e10,2,-5.6,1,1);

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

void CorrectedPull(TString particle, TString prod, TString rapidity, double lambda, int low, int high){
  TString table;
  double b;
  TString polarisation = Form("lambda = %g", lambda);
  /*TString polarisation;
  if (selection == "1") {
    polarisation = "1";
  }
  if (selection == "2") {
    polarisation = "0.2";
  }
  if (selection == "3") {
    polarisation = "-0.2";
  }
  if (selection == "4") {
    polarisation = "-1";
  }*/

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

  TFile f2 = TFile("HEPData-ins2705040-v1-root.root");

  //f.ls();

  TDirectoryFile* dir2 = (TDirectoryFile*)f2.Get("Table 9");

  //dir->ls();

  //TH1F* Factor = (TH1F*) dir2->Get("Hist1D_y"+selection);

    TCanvas *c1 = new TCanvas("c1","Corrected Data");
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

      //Data->SetBinError(i, pow(binContent1*binContent1 + binContent2*binContent2, .5));
      Data->SetBinError(i, (pow(binContent1*binContent1 + binContent2*binContent2, .5))*(correction(lambda, i)));
      //Data->SetBinContent(i, (Data->GetBinContent(i))*(Factor->GetBinContent(i)));
      Data->SetBinContent(i, (Data->GetBinContent(i))*(correction(lambda, i)));
    }

    Data -> SetMarkerStyle(20);

    Data -> SetMarkerSize(.75);

    Data->GetYaxis()->SetTitle("Pull Distribution");

    Data->SetTitle(prod+" "+particle+" at rapidity slice number "+ rapidity+" (pol.= "+polarisation+")");

    Data->SetName("");

    TF1 *func = new TF1("func", fitfunc, low, high, 5);

    func-> SetParameters(5e10,2,-5.6,1,1);

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

void CorrectedAll(TString particle, TString prod, TString rapidity, double lambda, int low, int high) {

//Define Paraters

TString polarisation = Form("lambda = %g", lambda);

  //Create and configure the three histograms required

    TFile f = TFile("HEPData-ins2705040-v1-root.root");
    TDirectoryFile* dir = (TDirectoryFile*)f.Get("Table 1");
    TH1F* Ratio = (TH1F*) dir->Get("Hist1D_y1");
    Ratio -> SetMarkerStyle(20);
    Ratio -> SetMarkerSize(.75);
    Ratio->SetTitle("");
    TH1F* Pull = (TH1F*)Ratio->Clone();

    //Create, fill, and fit to corrected data
    Fitting(particle, prod, rapidity, lambda, low, high, true);

    //Read in histogram
    TFile file = TFile("Fitted_Data.root");

    TH1F* Data = (TH1F*) file.Get("Data");

    Data->SetTitle("");

//Data fit
    TF1 *fit = Data->GetFunction("func");

//Fit equation
    TF1 *func = new TF1("function", fitfunc, low, high, 5);

    func-> SetParameters(fit->GetParameter(0),fit->GetParameter(1),fit->GetParameter(2),fit->GetParameter(3),fit->GetParameter(4));

    double chi2 = fit->GetChisquare();

//Fill Ratio and Pull histograms
    TAxis* xAxis = Ratio->GetXaxis();

    int n = 35;

    for (size_t i = 1; i < n; i++) {
      Ratio->SetBinContent(i, (Data->GetBinContent(i))/(func->Eval(xAxis->GetBinCenter(i))));

      Ratio->SetBinError(i, (Data->GetBinError(i))/(func->Eval(xAxis->GetBinCenter(i))));

      Pull->SetBinContent(i, (Data->GetBinContent(i)-(func->Eval(xAxis->GetBinCenter(i))))/(Data->GetBinError(i)));
    }

//Restrict range of all histograms
    Data->GetXaxis()->SetRangeUser(low,high);
    Ratio->GetXaxis()->SetRangeUser(low,high);
    Pull->GetXaxis()->SetRangeUser(low,high);

//Create visual graph
    TCanvas *c1 = new TCanvas("c1","Corrected Data",0,0,600,800);

    TPaveLabel *title = new TPaveLabel(0.15,0.92,0.9,0.98, prod+" "+particle+" at rapidity slice number "+ rapidity+" ("+polarisation+")");
   //title->SetFillColor(16);
   //title->SetTextFont(42);
   title->Draw();

    TPad *pad3 = new TPad("pad3","pad3",0.03,0.02,1,0.33);
   TPad *pad2 = new TPad("pad2","pad2",0.03,0.33,1,0.62);
   TPad *pad1 = new TPad("pad1","pad1",0.03,0.62,1,0.91);
    pad1->Draw(); pad2->Draw(); pad3->Draw();

    pad2->cd();
    pad2->SetLogx();
    gStyle->SetOptFit(0);
    Ratio->GetYaxis()->SetTitle("Data to fit ratio");
    Ratio->GetXaxis()->SetTitle("");
    Ratio->GetYaxis()->SetLabelSize(0.04);
    Ratio->GetXaxis()->SetLabelSize(0.05);
    Ratio->GetYaxis()->SetTitleSize(0.06);
    Ratio->GetXaxis()->SetTitleSize(0.05);
    Ratio->DrawCopy("E1 X");
    TF1 *constfunc1 = new TF1("constfunc1", constantratio, low, high, 0);
    constfunc1->Draw("same");

    pad3->cd();
    pad3->SetLogx();
    gStyle->SetOptFit(0);
    Pull->GetYaxis()->SetTitle("Pull Distribution");
    Pull->GetYaxis()->SetLabelSize(0.05);
    Pull->GetYaxis()->SetTitleSize(0.055);
    Pull->GetXaxis()->SetLabelSize(0.05);
    Pull->GetXaxis()->SetTitleSize(0.05);
    Pull->DrawCopy("HIST X");
    TF1 *constfunc2 = new TF1("constfunc2", constantpull, low, high, 0);
    constfunc2->Draw("same");
    auto line1 = new TLine(low, 1, high, 1);
    line1->SetLineColor(kBlue);
    line1->Draw("");
    auto line2 = new TLine(low, -1, high, -1);
    line2->SetLineColor(kBlue);
    line2->Draw("");

    pad1->cd();
    pad1->SetLogx();
    pad1->SetLogy();
    Data->GetYaxis()->SetTitle("Cross Section [GeV]");
    Data->GetYaxis()->SetLabelSize(0.05);
    Data->GetYaxis()->SetTitleSize(0.06);
    Data->GetXaxis()->SetLabelSize(0.001);
    Data->GetXaxis()->SetTitle("");
    gStyle->SetOptFit(1111); // Set 0 for no data, 1111 for all data
    gStyle->SetOptStat("");
    Data->GetFunction("func")->ResetBit(1<<9);
    Data-> DrawCopy("E1 X");
}

void CorrectedAll_Omit(TString particle, TString prod, TString rapidity, double lambda, int low, int high) {

    TFile f = TFile("HEPData-ins2705040-v1-root.root");
    TDirectoryFile* dir = (TDirectoryFile*)f.Get("Table 1");
    TH1F* Ratio = (TH1F*) dir->Get("Hist1D_y1");
    Ratio -> SetMarkerStyle(20);
    Ratio -> SetMarkerSize(.75);
    Ratio->SetTitle("");
    TH1F* Pull = (TH1F*)Ratio->Clone();

    TString polarisation = Form("lambda = %g", lambda);

    Fitting(particle, prod, rapidity, lambda, low, high, true);

    TFile file = TFile("Fitted_Data.root");

    TH1F* Data = (TH1F*) file.Get("Data");

    Data->SetTitle("");

    TF1 *fit = Data->GetFunction("func");

    TF1 *func = new TF1("function", fitfunc, low, high, 5);

    func-> SetParameters(fit->GetParameter(0),fit->GetParameter(1),fit->GetParameter(2),fit->GetParameter(3),fit->GetParameter(4));

    double chi2 = fit->GetChisquare();

    TAxis* xAxis = Ratio->GetXaxis();

    int n = 35;

    for (size_t i = 1; i < n; i++) {
      Ratio->SetBinContent(i, (Data->GetBinContent(i))/(func->Eval(xAxis->GetBinCenter(i))));

      Ratio->SetBinError(i, (Data->GetBinError(i))/(func->Eval(xAxis->GetBinCenter(i))));

      Pull->SetBinContent(i, (Data->GetBinContent(i)-(func->Eval(xAxis->GetBinCenter(i))))/(Data->GetBinError(i)));
    }

    Data->GetXaxis()->SetRangeUser(low,high);
    Ratio->GetXaxis()->SetRangeUser(low,high);
    Pull->GetXaxis()->SetRangeUser(low,high);

    TCanvas *c1 = new TCanvas("c1","Corrected Data",0,0,700,900);

    TPaveLabel *title = new TPaveLabel(0.1,0.94,0.9,0.98, prod+" "+particle+" at rapidity slice number "+ rapidity+" ("+polarisation+")");
   //title->SetFillColor(16);
   //title->SetTextFont(42);
   title->Draw();

    TPad *pad3 = new TPad("pad3","pad3",0,0,1,0.32);
   TPad *pad2 = new TPad("pad2","pad2",0,0.32,1,0.6);
   TPad *pad1 = new TPad("pad1","pad1",0,0.6,1,0.93);
    pad1->Draw(); pad2->Draw(); pad3->Draw();

    pad2->cd();
    pad2->SetLogx();
    gStyle->SetOptFit(0);
    Ratio->GetYaxis()->SetTitle("Data to fit ratio");
    Ratio->DrawCopy("E1 X");
    TF1 *constfunc1 = new TF1("constfunc1", constantratio, low, high, 0);
    constfunc1->Draw("same");

    pad3->cd();
    pad3->SetLogx();
    gStyle->SetOptFit(0);
    Data->GetYaxis()->SetTitle("Pull Distribution");
    Pull->DrawCopy("HIST X");
    TF1 *constfunc2 = new TF1("constfunc2", constantpull, low, high, 0);
    constfunc2->Draw("same");
    auto line1 = new TLine(low, 1, high, 1);
    line1->SetLineColor(kBlue);
    line1->Draw("");
    auto line2 = new TLine(low, -1, high, -1);
    line2->SetLineColor(kBlue);
    line2->Draw("");

    pad1->cd();
    pad1->SetLogx();
    pad1->SetLogy();
    gStyle->SetOptFit(1111);
    gStyle->SetOptStat("");
    Data->GetFunction("func")->ResetBit(1<<9);
    Data-> DrawCopy("E1 X");
}
