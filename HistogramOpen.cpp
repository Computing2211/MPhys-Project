#include "parameters.h"

TH1F *HistogramOpen()
{
  TString particle = PARTICLE;
  TString prod = PROD; //P or NP
  TString rapidity = RAPIDITY; //1,2,3
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

  return Data;
}