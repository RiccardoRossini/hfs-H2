// Calculates number of triggers if weight ==1, otherwise the weighted number of triggers

// version 1: passing the tree already set with the TEntryList from NeighbourCut
Double_t GetTriggersRR(TTree*    t,  // t: tree with the entry list (neighbour cut) already set by the caller
         TString   lasercut = "lphd==0", 
         TString   weight = "1",
         Double_t* err = nullptr, 
         Bool_t    debug = 0) {

    if (err) *err = 0;

    if (!t) {
        if (debug) cout << " GetTriggersRR: null tree" << endl;
        return 0;
    }

    t->SetEstimate(t->GetEntries() + 1);

    Long64_t n = t->Draw(
        "1",
        Form("(%s)*(%s)", weight.Data(), lasercut.Data()),
        "goff"
    );

    if (n < 0) {
        if (debug) cout << " GetTriggersRR: Draw failed" << endl;
        return 0;
    }

    Double_t* wv = t->GetW();
    Double_t sum = 0;
    Double_t sum2 = 0;

    for (Long64_t i = 0; i < n; ++i) {
        sum  += wv[i];
        sum2 += wv[i] * wv[i];
    }

    Double_t uncertainty = std::sqrt(sum2);

    if (err) *err = uncertainty;

    if (debug) {
        cout << " nTriggers: " << sum
             << " +/- " << uncertainty << endl;
    }

    return sum;
}


// version 2: passing info and extracting the tree from the file (can't do it if TEntryList from NeighbourCut has been applied)
Double_t GetTriggersRR(TString tag   = "RAL202305",
		     UInt_t  repro = 2,
		     UInt_t  batch = 9,
		     TString lasercut = "lphd==0",
         TString weight   = "1",
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