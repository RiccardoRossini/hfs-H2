
void CalcRateBatch(TString period      = "RAL202405",
		   UInt_t  repro    = 4,
		   TString tagB     = "RAL202304",
		   UInt_t  reproB   = 4,
		   UInt_t  batchB   = 11,
		   UInt_t  ndet     = 10,
		   TString weight   = "w0*w1",
		   UInt_t  tmin     = 780,
		   UInt_t  tmax     = 3e3,
		   UInt_t  bsys     = 0,
		   Bool_t  draw     = 1,
		   UInt_t  brmsmax  = 1e7,
		   UInt_t  basemax  = 1e7,
		   UInt_t  flagmax  = 1e7,
		   UInt_t  deltamin = 0,
		   TString lasercut = "lphd==0",
       TString lasercutB= "lphd==0",  // we always need noLaser data for the background
		   TString label    = "Internal",
		   UInt_t  sign0    = 60,
			 UInt_t  sign1    = 200,
			 Bool_t  differ   = 0) { // differ:  the value returned by PlotEne is: 0 = the ratio, 1 = the difference
  

  vector<UInt_t> batchVec  = GetBatchVec(period);
  vector<UInt_t> lambdaVec = GetLambdaVec(period); 
  vector<TString> tagVec;

  if (period == "hydrogen") {
    tagVec   = GetHydrogenTags(); 
  } else {
    tagVec   = {period};
  }
  
  UInt_t nPoints = batchVec.size();

  Double_t* xVal  = new Double_t[nPoints];
  Double_t* yVal0 = new Double_t[nPoints];
  Double_t* yVal1 = new Double_t[nPoints];
//  Double_t* yVal2 = new Double_t[nPoints];

  Double_t* yErr0 = new Double_t[nPoints];
  Double_t* yErr1 = new Double_t[nPoints];
//  Double_t* yErr2 = new Double_t[nPoints];
  
//  vector<Double_t> lambdaTrueVec[2];
  vector<Double_t> lambdaTrueVec = {0,0};     
  
  
  
  // Setting up string variables

  TString weightName = weight;
  weightName.ReplaceAll("*", "");
  weightName.ReplaceAll(".", "p");

  TString lcut = lasercut; //just a string for the file name, not a cut! 
  lcut.ReplaceAll("=", "");     // Equal
  lcut.ReplaceAll(">=", "GE");  // Greater or Equal
  lcut.ReplaceAll("<=", "LE");  // Less or Equal
  lcut.ReplaceAll(">",  "G");   // Greater
  lcut.ReplaceAll("<",  "L");   // Less
  
  // setting up and opening the output file
  TString ofilename = Form("plots/calcratebatch/calcratebatch_%s-RV%i_%s_ndet%03i_bsys%i_weight%s_t%04i-%04ins_ene%03i-%03ikeV_ref%s-RV%i-b%02i.root",
			   period.Data(), repro, lcut.Data(), ndet, bsys, weightName.Data(), tmin, tmax, sign0, sign1, tagB.Data(), reproB, batchB);
  TFile* ofile = new TFile(ofilename, "RECREATE");
  cout << " ofilename: " << ofilename << endl;
  
  // setting up the output file tree
  TTree* tree = new TTree("tRate", "Rate batch results");
  UInt_t   t_batch;
  Double_t t_lambdaExp;
  Double_t t_lambdaTrue, t_lambdaTrueErr;
  Double_t t_s0, t_s0Err;
  Double_t t_trigRatio, t_trigRatioErr;
  Double_t t_trigS, t_trigSErr;
  Double_t t_trigB, t_trigBErr;
  Double_t t_intS, t_intSErr;
  Double_t t_intB, t_intBErr;
  Double_t t_pres, t_presErr;
  Double_t t_temp, t_tempErr;

  tree->Branch("batch",             &t_batch,         "batch/i");
  tree->Branch("lambdaExp",         &t_lambdaExp,     "lambdaExp/D");
  tree->Branch("lambdaTrue",        &t_lambdaTrue,    "lambdaTrue/D");
  tree->Branch("lambdaTrueErr",     &t_lambdaTrueErr, "lambdaTrueErr/D");
  tree->Branch("s0",                &t_s0,            "s0/D");
  tree->Branch("s0Err",             &t_s0Err,         "s0Err/D");
  tree->Branch("trigRatio",         &t_trigRatio,     "trigRatio/D");
  tree->Branch("trigRatioErr",      &t_trigRatioErr,  "trigRatioErr/D");
  tree->Branch("trigS",             &t_trigS,         "trigS/D");
  tree->Branch("trigSErr",          &t_trigSErr,      "trigSErr/D");
  tree->Branch("trigB",             &t_trigB,         "trigB/D");
  tree->Branch("trigBErr",          &t_trigBErr,      "trigBErr/D");
  tree->Branch("intS",              &t_intS,          "intS/D");
  tree->Branch("intSErr",           &t_intSErr,       "intSErr/D");
  tree->Branch("intB",              &t_intB,          "intB/D");
  tree->Branch("intBErr",           &t_intBErr,       "intBErr/D");
  tree->Branch("pres",              &t_pres,          "pres/D");
  tree->Branch("presErr",           &t_presErr,       "presErr/D");
  tree->Branch("temp",              &t_temp,          "temp/D");
  tree->Branch("tempErr",           &t_tempErr,       "tempErr/D");

  TCut cutS, cutB; 
  
  UInt_t j = 0;
  
  
  ////////// MAIN LOOP /////////////
  for (UInt_t np=0; np<nPoints; np++) {

//    if (!np) continue;
    
    UInt_t   batchS = batchVec [np];
    Double_t lambda = lambdaVec[np];
    cout << "---\nProcessing batch " << batchS << " - wl " << lambda << " (" << np+1 << " of " << nPoints << ")" << endl;
    
    xVal[np] = lambda;


    vector<Double_t> y0vec = PlotEne(tagVec[j], repro, batchS, tagB, reproB, batchB, ndet, weight, tmin, tmax, bsys, draw, brmsmax, basemax, flagmax, deltamin, label, lasercut, lasercutB, sign0, sign1, differ, &cutS, &cutB);
    if (y0vec.size() < 14) throw std::runtime_error("PlotEne returned fewer than 14 values");
      
    Double_t s0    = y0vec[0]; // Integral ratio of current batch wrt reference batch 
    Double_t s0err = y0vec[1];

    yVal0[np] = s0;
    yErr0[np] = s0err;


    Double_t trgS = y0vec[2], eTrgS = y0vec[3];
    Double_t trgB = y0vec[4], eTrgB = y0vec[5];
    if (trgS == 0 || trgB == 0) {
      cerr << "ERROR: zero triggers, batch " << batchS << endl;
      throw std::runtime_error("Zero triggers");
    }
    yVal1[np] = trgS / trgB;
    yErr1[np] = yVal1[np] * sqrt(pow(eTrgS/trgS, 2) + pow(eTrgB/trgB, 2));

    TString detname = GetDetName(tagVec[j], ndet);

    cout << Form(" ndet %03i %8s batch %02i wl %04.0f signal: %9.2f +/- %9.2f (%5.1f%%) triggers: %.1f +/- %.1f",
             ndet, detname.Data(), batchS, lambda,
             yVal0[np], yErr0[np], 100*yErr0[np]/yVal0[np],
             trgS, eTrgS) << endl;



		 
		// filling the tree
		t_batch         = batchS;
    t_lambdaExp     = lambda;
    t_lambdaTrue    = lambdaTrueVec[0];
    t_lambdaTrueErr = lambdaTrueVec[1];
    t_s0            = yVal0[np];
    t_s0Err         = yErr0[np];
    t_trigRatio     = yVal1[np];
    t_trigRatioErr  = yErr1[np];
    t_trigS         = trgS;   
    t_trigSErr      = eTrgS;
    t_trigB         = trgB;   
    t_trigBErr      = eTrgB;
    t_intS          = y0vec[6];
    t_intSErr       = y0vec[7];
    t_intB          = y0vec[8];
    t_intBErr       = y0vec[9];
    t_pres          = y0vec[10];
    t_presErr       = y0vec[11];
    t_temp          = y0vec[12];
    t_tempErr       = y0vec[13];

    tree->Fill();

    if (period == "hydrogen") j++;

  } ////////// END OF MAIN LOOP /////////////

  // setting up the output file graphs
  TGraphErrors* g0 = new TGraphErrors(nPoints, xVal, yVal0, NULL, yErr0);
  TGraphErrors* g1 = new TGraphErrors(nPoints, xVal, yVal1, NULL, yErr1);
  TString xtitle = "Wavelength [pm] - 6788000 pm";
  g0->GetXaxis()->SetTitle(xtitle);
  g1->GetXaxis()->SetTitle(xtitle);
  g0->GetYaxis()->SetTitle(Form("Normalised integral ratio in [%i,%i] keV wrt. reference", sign0, sign1));
  g1->GetYaxis()->SetTitle("Ratio of #mu triggers wrt. reference");

  ofile->cd();
  g0->Write("g0");
  g1->Write("g1");
  cutS.Write("tcutS");
  cutB.Write("tcutB");
  tree->Write("", TObject::kOverwrite);
  ofile->Write("", TObject::kOverwrite);
  ofile->Close();

  /*
  delete[] xVal;
  delete[] yVal0;
  delete[] yVal1;
  delete[] yVal2;
  delete[] yErr0;
  delete[] yErr1;
  delete[] yErr2;
  delete g0;
  delete g1;
  delete tree;
  delete ofile;  
  */
}
