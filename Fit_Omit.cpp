TFitResultPtr Fit_Omit(TString particle, TString prod, TString rapidity, double lambda, int low, int high, bool Draw){
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
    Data->SetBinContent(i, (Data->GetBinContent(i))*(correction(lambda, i)));
  }

  Data -> SetMarkerStyle(20);

  Data -> SetMarkerSize(.75);

  Data->GetYaxis()->SetTitle("Differential Cross Section [fb/GeV]");

  Data->SetTitle(prod+" "+particle+" at rapidity slice number "+ rapidity+" (pol.= "+polarisation+")");

  Data->SetName("");

  TF1 *func = new TF1("func", fline, low, high, 5);

  func-> SetParameters(3e10,7.,-5.6,-1,2.5);

  //func-> FixParameter(4,0);

  TFitResultPtr r1 = Data->Fit("func", "Q S");

  func-> SetParameters(r1->Parameter(0),r1->Parameter(1),r1->Parameter(2),r1->Parameter(3),r1->Parameter(4));

  func-> FixParameter(4,0);

  //func-> FixParameter(1,b);

  TFitResultPtr r = Data->Fit("func", "Q R S E M");

  if (Draw) {
    // Print the full information of the fit including covariance matrix.
    r->Print("V");
  }

  TFile *file = new TFile("Fitted_Data.root","RECREATE");

  Data->Write("Data");

  return 0.0;
}
