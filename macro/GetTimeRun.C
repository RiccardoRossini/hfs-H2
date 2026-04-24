 
Double_t GetTimeRun(TString tag      = "RAL202303",
		    UInt_t  repro    = 0,
		    UInt_t  batch    = 5,
		    UInt_t  run      = 40046,
		    TString timeUnit = "s") {
 
  TString ifilename = Form("data/famu/%s/ROOT/pseudo-L3/repro%i/header/batch%02i/run0%i_header.root",
			   tag.Data(), repro, batch, run);
  
  if (gSystem->AccessPathName(ifilename)) { cout << " File not found: " << ifilename << endl; return 0; }

  TFile* ifile = TFile::Open(ifilename); 

  TTree* t0 = (TTree*)ifile->Get("t0");
  
  Double_t dt = (t0->GetMaximum("time")-t0->GetMinimum("time"));  
  Double_t norm;

  if      (timeUnit == "s") norm = 1;     // seconds
  else if (timeUnit == "m") norm = 60;    // minutes
  else if (timeUnit == "h") norm = 60*60; // hour;
  else { cout << " Unkown operation for timeUnit: " << timeUnit << endl; exit(0); }
  
  if (norm > 1) dt /= norm;
  
  return dt;
}
