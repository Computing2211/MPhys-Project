void Graph(){
  TFile f = TFile("HEPData-ins2705040-v1-root.root");

  TDirectoryFile* dir = (TDirectoryFile*)f.Get("Table 1");

  TH1F* Data = (TH1F*) dir->Get("Hist1D_y1");

  TFile *file = new TFile("test.root","RECREATE");

  Data->Write("testFile");
}

void Sandbox(){
  Graph();
  TFile file = TFile("test.root");

  file.ls();

  TH1F* Data = (TH1F*) file.Get("testFile");

  Data->DrawCopy();
}
