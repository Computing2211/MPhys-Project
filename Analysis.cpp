#include "Functions.cpp"
#include "Fit_Omit.cpp"
#include "Graphing.cpp"
#include "Graph_Data.cpp"
#include "Corrections.cpp"
#include "Specific_Corrections.cpp"
#include "Chi2lambda.cpp"
#include "Correlations.cpp"
#include <iostream>
#include "parameters.h"

void Analysis(){
  TString particle = PARTICLE; //Jpsi or Psi2S
  TString prod = "NP"; //P or NP
  TString rapidity = "1"; //1,2,3
  double lambda = -0.55; //between -1 and 1, 0 doesn't work due to  div 0 errors
  int low = 20; //no lower than 8
  int high = 200; //no higher than 360

  //Graph_Data(particle, prod, rapidity, low, high);
  //Graphs the raw data for the particle in question

  //Ratio(particle, prod, rapidity, low, high);
  //Graphs ratio of data to fit

  //Pull(particle, prod, rapidity, low, high);
  //Distance divided by error

  //All(particle, prod, rapidity, low, high);
  //All three graphs in one

  //Corrections(lambda);
  //Correction factors histogram


  //VV As above but with corrected data
  //CorrectedGraph_Data(particle, prod, rapidity, lambda, low, high, true);

  //CorrectedRatio(particle, prod, rapidity, lambda, low, high);

  //CorrectedPull(particle, prod, rapidity, lambda, low, high);

  //CorrectedAll(particle, prod, rapidity, lambda, low, high); //Has been tweaked to produce desired graphs for report

  //Chi2graph(particle, prod, rapidity, low, high); //Scans all lambda to find minimum. Also graphs

  //CorrectedAll_Omit(particle, prod, rapidity, lambda, low, high); //Omits mid bins

  //Chi2graph_Omit(particle, prod, rapidity, low, high);

  //Specific_Corrections(particle, prod, rapidity, lambda, low, high, true);

  Correlations(); //Determines values including errors with correlation possibilities. No graphs.
}
