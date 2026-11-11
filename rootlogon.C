#include "/storage/gpfs_data/famu/riccardo/rootStyle/atlasstyle-00-04-02/AtlasStyle.C"
#include "/storage/gpfs_data/famu/riccardo/rootStyle/atlasstyle-00-04-02/AtlasUtils.C"
#include "/storage/gpfs_data/famu/riccardo/rootStyle/atlasstyle-00-04-02/AtlasLabels.C"
#include "/storage/gpfs_data/famu/riccardo/rootStyle/atlasstyle-00-04-02/FAMULabels.C"

void rootlogon() {

  SetAtlasStyle();

  TGaxis::SetMaxDigits(4);

  gStyle->SetOptStat(0);

  gStyle->SetPadRightMargin(0.07);
  gStyle->SetPadTopMargin  (0.07);

  // Load utility macros
  gROOT->LoadMacro("macro/GetHydrogenTags.C");
  gROOT->LoadMacro("macro/GetBatchVec.C");
  gROOT->LoadMacro("macro/GetLambdaVec.C");
  gROOT->LoadMacro("macro/GetDetName.C");
  gROOT->LoadMacro("macro/GetTriggers.C");
  gROOT->LoadMacro("macro/GetText.C");
  gROOT->LoadMacro("macro/GetLambdaTrue.C");
  gROOT->LoadMacro("macro/NeighbourCut.C");
  gROOT->LoadMacro("macro/GetDetCut.C");
  gROOT->LoadMacro("macro/GaussianKernel.C");

  // Load main macro calculating the ratio/difference of integrals
  gROOT->LoadMacro("macro/PlotEne.C");

  // Load main macro working on a batch and building the .root output
  gROOT->LoadMacro("macro/CalcRateBatch.C");

  // Load plotting utilities
  gROOT->LoadMacro("plots/PlotRateBatch.C");
  gROOT->LoadMacro("plots/RatioFromCFiles.C");

}
