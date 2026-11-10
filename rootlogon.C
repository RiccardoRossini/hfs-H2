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

  gROOT->LoadMacro("macro/GetHydrogenTags.C");
  gROOT->LoadMacro("macro/GetBatchVec.C");
  gROOT->LoadMacro("macro/GetLambdaVec.C");
  gROOT->LoadMacro("macro/GetDetName.C");
  gROOT->LoadMacro("macro/GetTriggers.C");
  gROOT->LoadMacro("macro/GetText.C");
  gROOT->LoadMacro("macro/GetLambdaTrue.C");
  gROOT->LoadMacro("macro/NeighbourCut.C");
  gROOT->LoadMacro("macro/GetDetCut.C");

  gROOT->LoadMacro("macro/PlotEne.C");

  gROOT->LoadMacro("macro/CalcRateBatch.C");

  gROOT->LoadMacro("macro/GaussianKernel.C");

}
