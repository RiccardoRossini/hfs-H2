
Double_t GetTriggersRR(TString tag   = "RAL202305",
		     UInt_t  repro = 2,
		     UInt_t  batch = 9,
		     TString lasercut = "lphd==0",
		     TString type  = "",
		     UInt_t  mode  = 0,
		     Bool_t  debug = 0) {
  
  TString ifilename = gSystem->GetFromPipe(Form("ls data/famu/%s/ROOT/L3-RV%i/batch%i*/*.root | tail -n 1",
						tag.Data(), repro, batch));
  
  if (gSystem->AccessPathName(ifilename)) { cout << " File not found: " << ifilename << endl; return 0; }
  
  TFile* ifile = TFile::Open(ifilename, "READ");

  TString treename = "t1";

  TTree* t0 = (TTree*)ifile->Get(treename);
  
  //TString lcut = type.Length() ? GetLaserCut(type, mode) : "1";
  
  Double_t nTriggers = t0->GetEntries(lasercut); 
  
  ifile->Close();

  if (debug) cout << " nTriggers: " << nTriggers << endl;    
  
  return nTriggers;
}
