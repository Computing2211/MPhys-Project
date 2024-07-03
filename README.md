This program was originally designed to analyse data taken by ATLAS which can be found at https://www.hepdata.net/record/ins2705040 and is also saved in the .root file HEPData-ins2705040-v1-root.root

To use this program:
1. Run in CERN's ROOT framework
2. Set the desired parameters in parameters.h:
   a. The particle in question (J/psi or psi(2S) mesons)
   b. The production mechanism (prompt or non-prompt)
   c. The rapidity slice, of three
   d. The upper and lower bounds of the transverse momentum (20-200 is recommended for the fit equations used, the extremeties did not match the fit functions well
3. Run the pre-processor file "Binfinder.cpp". It sets values that need to be constants before the main file is run
4. Run the file "Analysis.cpp". Within this, several files can be run:
   a. Correlations() (from "Correlations.cpp"). This utilises a minimisation algorithm (several to choose from) that determines the optimal fit and lambda value for the data specified. It then outputs these values and returns the optimal lambda value.
   b. Any of the graphing functions. These take a specific lambda value (as well as the other arguements) to return a graph of the data specified. "CorrectedAll" is a function that graphs the data with fit, the ratio of data to fit, and the pull distribution as three graphs in one figure
   c. Some other functions are available, mainly as artefacts of previous attempts:
     i) "Corrections" graphs the corrections factors for each pt bin for a given value of lambda
     ii) "Chi2graph" scans across all lambda values, determines the chi squared for the optimal fit, and plots a graph of this. Useful for visualising the errors in lambda
     iii) The "Omit" functions or "Specific_Corrections" attempts to isolate the correction of the discontinuity by noting a trend in the correction factors, and only changing the data around the discontinuity.

Any issues I wish you good luck and here are some helpful resources:
https://root.cern/ -> documentation for the root framework and all associated objects (e.g. Histograms)
https://cplusplus.com/doc/tutorial/ -> C++ documentation

Alternatively, I'm sure there's a way to message within Github so feel free.
