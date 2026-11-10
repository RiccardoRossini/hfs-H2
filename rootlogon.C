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
  gROOT->LoadMacro("macro/GetBatchVecRR.C");
  gROOT->LoadMacro("macro/GetLambdaVecRR.C");
  gROOT->LoadMacro("macro/GetDetNameRR.C");
  gROOT->LoadMacro("macro/GetTriggersRR.C");
  gROOT->LoadMacro("macro/GetTextRR.C");
  gROOT->LoadMacro("macro/GetLambdaTrueRR.C");
  gROOT->LoadMacro("macro/NeighbourCutRR.C");
  gROOT->LoadMacro("macro/GetDetCutRR.C");

  gROOT->LoadMacro("macro/PlotEne.C");

  gROOT->LoadMacro("macro/CalcRateBatch.C");

  gROOT->LoadMacro("macro/GaussianKernel.C");

}
