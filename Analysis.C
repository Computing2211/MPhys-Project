#include "Functions.C"
#include "Fit_Omit.C"
#include "Graphing.C"
#include "Graph_Data.C"
#include "Corrections.C"
#include "Specific_Corrections.C"
#include "Chi2lambda.C"
#include "Correlations.C"

void Analysis(){
  TString particle = "Jpsi"; //Jpsi or Psi2S
  TString prod = "NP"; //P or NP
  TString rapidity = "1"; //1,2,3
  double lambda = -0.55; //between -1 and 1, 0 doesn't work due to /0 errors
  int low = 20; //no lower than 8
  int high = 200; //no higher than 360

  //Graph_Data(particle, prod, rapidity, low, high);

  //Ratio(particle, prod, rapidity, low, high);

  //Pull(particle, prod, rapidity, low, high);

  //All(particle, prod, rapidity, low, high);

  //Corrections(lambda);

  //CorrectedGraph_Data(particle, prod, rapidity, lambda, low, high, true);

  //CorrectedRatio(particle, prod, rapidity, lambda, low, high);

  //CorrectedPull(particle, prod, rapidity, lambda, low, high);

  CorrectedAll(particle, prod, rapidity, lambda, low, high);
  //CorrectedAllPaper(particle, prod, rapidity, lambda, low, high);

  //Chi2graph(particle, prod, rapidity, low, high);

  //CorrectedAll_Omit(particle, prod, rapidity, lambda, low, high);

  //Chi2graph_Omit(particle, prod, rapidity, low, high);

  //Specific_Corrections(particle, prod, rapidity, lambda, low, high, true);

  //Correlations();
}
