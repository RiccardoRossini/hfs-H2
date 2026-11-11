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
  gROOT->LoadMacro("../macro/GetBatchVec.C");
  gROOT->LoadMacro("../macro/GetDetName.C");

  // Load plotting utilities
  gROOT->LoadMacro("PlotRateBatch.C");
  gROOT->LoadMacro("RatioFromCFiles.C");

}
