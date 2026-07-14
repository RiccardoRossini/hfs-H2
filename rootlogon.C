#include "/storage/gpfs_data/famu/riccardo/rootStyle/atlasstyle-00-04-02/AtlasStyle.C"
#include "/storage/gpfs_data/famu/riccardo/rootStyle/atlasstyle-00-04-02/AtlasUtils.C"
#include "/storage/gpfs_data/famu/riccardo/rootStyle/atlasstyle-00-04-02/AtlasLabels.C"
#include "/storage/gpfs_data/famu/riccardo/rootStyle/atlasstyle-00-04-02/FAMULabels.C"

void rootlogon() {

//gErrorIgnoreLevel = kError;

  SetAtlasStyle();

  TGaxis::SetMaxDigits(4);

  gStyle->SetOptStat(0);

  gStyle->SetPadRightMargin(0.07);
  gStyle->SetPadTopMargin  (0.07);

// If needed, ask Antonello
//  gROOT->LoadMacro("/home/FAMU/sbrizzifamu/asbrizzi/tools/macro/SetStatPos.C");                         // commented on 30/3/26
//  gROOT->LoadMacro("/home/FAMU/sbrizzifamu/asbrizzi/tools/macro/SetPalette.C");                         // commented on 30/3/26

//  gROOT->LoadMacro("/home/FAMU/sbrizzifamu/asbrizzi/2023/ana/hfs/macro/utils/GetBatchVec.C");
//  gROOT->LoadMacro("/home/FAMU/sbrizzifamu/asbrizzi/2023/ana/hfs/macro/utils/GetDetName.C");
//  gROOT->LoadMacro("/home/FAMU/sbrizzifamu/asbrizzi/2023/ana/hfs/macro/utils/GetLaserCut.C");           // commented on 30/3/26
//  gROOT->LoadMacro("/home/FAMU/sbrizzifamu/asbrizzi/2023/ana/hfs/macro/utils/GetLambdaVec.C");
//  gROOT->LoadMacro("/home/FAMU/sbrizzifamu/asbrizzi/2023/ana/hfs/macro/utils/GetText.C");
//  gROOT->LoadMacro("/home/FAMU/sbrizzifamu/asbrizzi/2023/ana/hfs/macro/utils/GetTriggers.C");

  gROOT->LoadMacro("macro/GetBatchVecRR.C");
  gROOT->LoadMacro("macro/GetLambdaVecRR.C");
  gROOT->LoadMacro("macro/GetDetNameRR.C");
  gROOT->LoadMacro("macro/GetTriggersRR.C");
  gROOT->LoadMacro("macro/GetTextRR.C");
  gROOT->LoadMacro("macro/GetLambdaTrueRR.C");
  gROOT->LoadMacro("macro/NeighbourCutRR.C");
  gROOT->LoadMacro("macro/GetDetCutRR.C");

  gROOT->LoadMacro("macro/PlotEne.C");

  gROOT->LoadMacro("macro/GetTimeVec.C");
  gROOT->LoadMacro("macro/GetTimeRun.C");

  gROOT->LoadMacro("macro/CalcRateBatch.C");
//  gROOT->LoadMacro("macro/CalcRateDet.C");
//  gROOT->LoadMacro("macro/CalcRateRun.C");
//  gROOT->LoadMacro("macro/CalcRateTime.C");

  gROOT->LoadMacro("macro/GaussianKernel.C");

//  gROOT->LoadMacro("macro/PlotRateBatch.C");
//  gROOT->LoadMacro("macro/PlotRateDet.C");
//  gROOT->LoadMacro("macro/PlotRateRun.C");
//  gROOT->LoadMacro("macro/PlotRateTime.C");

//gSystem->Load("lib/TransferFit_C.so");

//gROOT->LoadMacro("macro/GetTransferTemp0.C");
//gROOT->LoadMacro("macro/GetTransferTemp1.C");
//gROOT->LoadMacro("macro/PlotTransferSys.C");
//gROOT->LoadMacro("macro/PlotTransferScale.C");
//gROOT->LoadMacro("macro/PlotTransferTemp0.C");
//gROOT->LoadMacro("macro/PlotTransferTemp1.C");

//gROOT->LoadMacro("macro/paper/plot2018.C");
}
