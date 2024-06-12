double Specific_Corrections(TString particle, TString prod, TString rapidity, double lambda, int low, int high, bool Draw){
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

  TF1 *func_a = new TF1("func", correctfunc, 12,360, 3);

  func_a-> SetParameters(10,10,-1);

  TH1F* Ratio = (TH1F*)Factor->Clone();

  TFitResultPtr r2 = Factor->Fit("func", "R S E");

  TAxis* xAxis = Factor->GetXaxis();

    TF1 *func2 = new TF1("func2", factorfunc, 12,360, 3);

    func2-> SetParameters(r2->Parameter(0),r2->Parameter(1),r2->Parameter(2));

    for (size_t i = 0; i < bins; i++) {
      double entry = Factor->GetBinContent(i);
      double expected = func2->Eval(xAxis->GetBinCenter(i));
      Ratio->SetBinContent(i, entry/expected);
    }

  TString table;
double b;
TString polarisation = Form("lambda = %g", lambda);

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

TH1F* error1 = (TH1F*) dir->Get("Hist1D_y"+rapidity+"_e1");
TH1F* error2 = (TH1F*) dir->Get("Hist1D_y"+rapidity+"_e2");

int n = 35;

for (size_t i = 1; i < n; i++) {
  double binContent1 = error1 -> GetBinContent(i);
  double binContent2 = error2 -> GetBinContent(i);

  //cout << binContent << endl;

  //Data->SetBinError(i, pow(binContent1*binContent1 + binContent2*binContent2, .5));
  Data->SetBinError(i, (pow(binContent1*binContent1 + binContent2*binContent2, .5))*(correction(lambda, i)));
  //Data->SetBinError(i, (pow(binContent1*binContent1, .5))*(correction(lambda, i)));
  //Data->SetBinContent(i, (Data->GetBinContent(i))*(Factor->GetBinContent(i)));
  Data->SetBinContent(i, (Data->GetBinContent(i))*(Ratio->GetBinContent(i)));
}

Data -> SetMarkerStyle(20);

Data -> SetMarkerSize(.75);

Data->GetYaxis()->SetTitle("Differential Cross Section [fb/GeV]");

Data->SetTitle(prod+" "+particle+" at rapidity slice number "+ rapidity+" (pol.= "+polarisation+")");

Data->SetName("");

TF1 *func3 = new TF1("func3", fitfunc, low, high, 5);

func3-> SetParameters(3e10,10,-5,0,0);

func3-> FixParameter(4,0);

TFitResultPtr r1 = Data->Fit("func3", "Q S");

func3-> SetParameters(r1->Parameter(0),r1->Parameter(1),r1->Parameter(2),r1->Parameter(3),r1->Parameter(4));

func3-> FixParameter(4,0);
func3-> FixParameter(3,0);

TFitResultPtr r = Data->Fit("func3", "Q R S E M");

double Chi2 = Data->GetFunction("func3")->GetChisquare();

if (Draw) {
  // Print the full information of the fit including covariance matrix.
  r->Print("V");

  TH1F* Ratio2 = (TH1F*) dir->Get("Hist1D_y1");
  Ratio2 -> SetMarkerStyle(20);
  Ratio2 -> SetMarkerSize(.75);
  Ratio2->SetTitle("");
  TH1F* Pull = (TH1F*)Ratio2->Clone();

  for (size_t i = 1; i < n; i++) {
    Ratio2->SetBinContent(i, (Data->GetBinContent(i))/(func3->Eval(xAxis->GetBinCenter(i))));

    Ratio2->SetBinError(i, (Data->GetBinError(i))/(func3->Eval(xAxis->GetBinCenter(i))));

    Pull->SetBinContent(i, (Data->GetBinContent(i)-(func3->Eval(xAxis->GetBinCenter(i))))/(Data->GetBinError(i)));
  }

  Data->GetXaxis()->SetRangeUser(low,high);
  Ratio2->GetXaxis()->SetRangeUser(low,high);
  Pull->GetXaxis()->SetRangeUser(low,high);

  gStyle->SetOptFit(1);
  gStyle->SetOptStat("");

  TCanvas *c1 = new TCanvas("c1","Corrected Data",0,0,700,900);

  TPaveLabel *title = new TPaveLabel(0.1,0.94,0.9,0.98, prod+" "+particle+" at rapidity slice number "+ rapidity+" ("+polarisation+")");
  //title->SetFillColor(16);
  //title->SetTextFont(42);
  title->Draw();

c1->SetLogx();
c1->SetLogy();

TPad *pad3 = new TPad("pad3","pad3",0,0,1,0.32);
TPad *pad2 = new TPad("pad2","pad2",0,0.32,1,0.6);
TPad *pad1 = new TPad("pad1","pad1",0,0.6,1,0.93);
pad1->Draw(); pad2->Draw(); pad3->Draw();

pad2->cd();
pad2->SetLogx();
gStyle->SetOptFit(0);
Ratio2->GetYaxis()->SetTitle("Data to fit ratio");
Ratio2->DrawCopy("E1 X");
TF1 *constfunc1 = new TF1("constfunc1", constantratio, low, high, 0);
constfunc1->Draw("same");

pad3->cd();
pad3->SetLogx();
gStyle->SetOptFit(0);
Pull->GetYaxis()->SetTitle("Pull Distribution");
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
Data->GetFunction("func3")->ResetBit(1<<9);
Data-> DrawCopy("E1 X");
}
return Chi2;

}
