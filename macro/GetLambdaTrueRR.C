
vector<Double_t> GetLambdaTrueRR( TString tag   = "RAL202305", // this function calculates the true mean and sigma of measured wl 
    UInt_t  repro = 2,
    UInt_t  batch = 9,
    TString lasercut = "lphd>0",
    TString type  = "",
    UInt_t  mode  = 0,
    Bool_t  debug = 0){
    
    
    vector<Double_t> out(2,0);

    TString ifilename = gSystem->GetFromPipe(Form("ls data/famu/%s/ROOT/L3-RV%i/batch%i*/*.root | tail -n 1",
						tag.Data(), repro, batch));
    if (gSystem->AccessPathName(ifilename)) { cout << " File not found: " << ifilename << endl; return out; }

    TFile* ifile = TFile::Open(ifilename,"READ");
    if (!ifile || ifile->IsZombie()) return out;

    TTree* t0 = (TTree*)ifile->Get("t1");
    if (!t0) {  ifile->Close(); delete ifile; return out; }

    TH1D* htmp = new TH1D("htmp","",20000,6787.,6790.);
    htmp->SetDirectory(nullptr);
    t0->Project("htmp","lwave",lasercut);
    Double_t lambda = htmp->GetMean();
    Double_t lambdaSigma = htmp->GetStdDev();
    Double_t n     = htmp->GetEntries();
    Double_t lambdaErr = lambdaSigma/sqrt(n);

    out[0] = lambda;
    out[1] = lambdaErr;

    if (debug)
        cout << "lambda = " << lambda
             << " +/- " << lambdaErr << endl;

    delete htmp;
    ifile->Close();
    delete ifile;

    return out;
}
