
void CalcRateBatch(TString tag      = "RAL202405",
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
		   TString label    = "Internal",
		   UInt_t  sign0    = 60,
			 UInt_t  sign1    = 200,
			 Bool_t  differ   = 0) { // differ:  the value returned by PlotEne is: 0 = the ratio, 1 = the difference
  
  vector<UInt_t> batchVec  = GetBatchVec (tag);
  vector<UInt_t> lambdaVec = GetLambdaVec(tag);
  
  UInt_t nPoints = batchVec.size();

  Double_t* xVal  = new Double_t[nPoints];
  Double_t* yVal0 = new Double_t[nPoints];
  Double_t* yVal1 = new Double_t[nPoints];
  Double_t* yVal2 = new Double_t[nPoints];

  Double_t* yErr0 = new Double_t[nPoints];
  Double_t* yErr1 = new Double_t[nPoints];
  Double_t* yErr2 = new Double_t[nPoints];
  
//  vector<Double_t> lambdaTrueVec[2];
  vector<Double_t> lambdaTrueVec = {0,0};     
  
  TString lasercutB = "lphd==0";
  
  // Setting up string variables
  
  TString detname = GetDetName(tag, ndet);

  TString weightName = weight;
  weightName.ReplaceAll("*", "");
  weightName.ReplaceAll(".", "p");

  TString lcut = lasercut;
  lcut.Remove(0, lcut.Last('=') + 1); //just a string for the file name, not a cut! 
  
  // setting up and opening the output file
  TString ofilename = Form("plots/calcratebatch/calcratebatch_%s-RV%i_lphd%s_ndet%03i_bsys%i_weight%s_t%04i-%04ins_ene%03i-%03ikeV_ref%s-RV%i-b%02i.root",
			   tag.Data(), repro, lcut.Data(), ndet, bsys, weightName.Data(), tmin, tmax, sign0, sign1, tagB.Data(), reproB, batchB);
  TFile* ofile = new TFile(ofilename, "RECREATE");
  cout << " ofilename: " << ofilename << endl;
  
  // setting up the output file tree
  TTree* tree = new TTree("tRate", "Rate batch results");
  UInt_t   t_batch;
  Double_t t_lambdaAim;
  Double_t t_lambdaTrue;
  Double_t t_lambdaTrueErr;
  Double_t t_s0;
  Double_t t_s0Err;
  Double_t t_trigRatio;
  Double_t t_yield;
  Double_t t_yieldErr;
  tree->Branch("batch",             &t_batch,         "batch/i");
  tree->Branch("lambdaAim",         &t_lambdaAim,     "lambdaAim/D");
  tree->Branch("lambdaTrue",        &t_lambdaTrue,    "lambdaTrue/D");
  tree->Branch("lambdaTrueErr",     &t_lambdaTrueErr, "lambdaTrueErr/D");
  tree->Branch("s0",                &t_s0,            "s0/D");
  tree->Branch("s0err",             &t_s0Err,         "s0err/D");
  tree->Branch("trigRatio",         &t_trigRatio,     "trigRatio/D");
  tree->Branch("yield",             &t_yield,         "yield/D");
  tree->Branch("yieldErr",          &t_yieldErr,      "yieldErr/D");
  
  
  
//  for (UInt_t npp=0; npp<nPoints; npp++)     cout << batchVec[npp] << endl; //debug
  
  ////////// MAIN LOOP /////////////
  for (UInt_t np=0; np<nPoints; np++) {

//    if (!np) continue;
    
    UInt_t   batchS = batchVec [np];
    Double_t lambda = lambdaVec[np];
    cout << "---\nProcessing batch " << batchS << " - wl " << lambda << " (" << np+1 << " of " << nPoints << ")" << endl;
    
    xVal[np] = lambda;

    vector<Double_t> y0vec = PlotEne(tag, repro, batchS, tagB, reproB, batchB, ndet, weight, tmin, tmax, bsys, draw, brmsmax, basemax, flagmax, deltamin, label, lasercut, sign0, sign1, differ);
    
    Double_t s0    = y0vec[0]; // Integral ratio of current batch wrt reference batch 
    Double_t s0err = y0vec[1];
    
//    Double_t s1 = y1vec[0];
//    Double_t s2 = y2vec[0];

//    Double_t ds1 = fabs(s1 - s0);
//    Double_t ds2 = fabs(s2 - s0);    
//    Double_t sys = TMath::Max(ds1, ds2);

    yVal0[np] = s0;
    yErr0[np] = s0err;
//    yErr0[np] = sqrt(pow(s0err, 2) + pow(sys, 2)); // Add systematic effect due to background fluctuations

    Double_t trgS = GetTriggersRR(tag,  repro,  batchS, lasercut);
    Double_t trgB = GetTriggersRR(tagB, reproB, batchB, lasercutB);
    if (trgB==0){ // check denominator is not zero! 
       cerr << "ERROR: zero triggers in reference sample\n" << "tagB=" << tagB << " reproB=" << reproB << " batchB=" << batchB << endl;
       throw std::runtime_error("Zero reference triggers in background data");
    }
    yVal1[np] = trgS / trgB;
    yErr1[np] = yVal1[np] * sqrt(1.0/trgS + 1.0/trgB);
    
    yVal2[np] = yVal0[np]/yVal1[np]; // Oxygen X-rays per muon trigger
    yErr2[np] = yErr0[np]/yVal1[np];

//		auto lambdaTrueVec = GetLambdaTrueRR(tag, repro, batchS);
    
    cout << Form(" ndet %03i %8s batch %02i wl %04.0f signal: %9.2f +/- %9.2f (%5.1f%%) triggers: %.0f", 
		  ndet, detname.Data(), batchS, lambda,
		  yVal0[np], yErr0[np], 100*yErr0[np]/yVal0[np],
		  yVal1[np]) << endl;

		 
		// filling the tree
		t_batch         = batchS;
    t_lambdaAim     = lambda;
    t_lambdaTrue    = lambdaTrueVec[0];
    t_lambdaTrueErr = lambdaTrueVec[1];
    t_s0            = yVal0[np];
    t_s0Err         = yErr0[np];
    t_trigRatio     = yVal1[np];
    t_yield         = yVal2[np];
    t_yieldErr      = yErr2[np];
    tree->Fill();

  } ////////// END OF MAIN LOOP /////////////

  // setting up the output file graphs
  TGraphErrors* g0 = new TGraphErrors(nPoints, xVal, yVal0, NULL, yErr0);
  TGraphErrors* g1 = new TGraphErrors(nPoints, xVal, yVal1, NULL, yErr1);
  TString xtitle = "Wavelength [pm] - 6788000";
  g0->GetXaxis()->SetTitle(xtitle);
  g1->GetXaxis()->SetTitle(xtitle);
  g0->GetYaxis()->SetTitle(Form("Normalised integral ratio in [%i,%i] keV wrt. reference", sign0, sign1));
  g1->GetYaxis()->SetTitle("Ratio of #mu triggers wrt. reference");

  ofile->cd();
  g0->Write("g0");
  g1->Write("g1");
  tree->Write("", TObject::kOverwrite);
  ofile->Write("", TObject::kOverwrite);
  ofile->Close();

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
}
