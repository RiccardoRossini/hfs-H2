
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
			 UInt_t  sign1    = 200) {
  
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
  
  TString detname = GetDetName(tag, ndet);  
  
//  for (UInt_t npp=0; npp<nPoints; npp++)     cout << batchVec[npp] << endl; //debug
  
  for (UInt_t np=0; np<nPoints; np++) {

//    if (!np) continue;
    
    UInt_t   batchS = batchVec [np];
    Double_t lambda = lambdaVec[np];
    cout << "---\nProcessing batch " << batchS << " - wl " << lambda << " (" << np+1 << " of " << nPoints << ")" << endl;
    
    xVal[np] = lambda;

    vector<Double_t> y0vec = PlotEne(tag, repro, batchS, tagB, reproB, batchB, ndet, weight, tmin, tmax, bsys, draw, brmsmax, basemax, flagmax, deltamin, label, lasercut, sign0, sign1);
//    vector<Double_t> y1vec = PlotEne(tag, repro, batchS, tagB, reproB, batchB, ndet, weight, tmin, tmax, 1, 0, brmsmax, basemax, flagmax, deltamin, label, lasercut);
//    vector<Double_t> y2vec = PlotEne(tag, repro, batchS, tagB, reproB, batchB, ndet, weight, tmin, tmax, 2, 0, brmsmax, basemax, flagmax, deltamin, label, lasercut);
    
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

    yVal1[np] = GetTriggersRR(tag, repro, batchS, lasercut) / GetTriggersRR(tagB, reproB, batchB, lasercut);
    yErr1[np] = 0;
    
    yVal2[np] = yVal0[np]/yVal1[np]; // Oxygen X-rays per muon trigger
    yErr2[np] = yErr0[np]/yVal1[np];
    
    cout << Form(" ndet %03i %8s batch %02i wl %04.0f signal: %9.2f +/- %9.2f (%5.1f%%) triggers: %.0f", 
		 ndet, detname.Data(), batchS, lambda,
		 yVal0[np], yErr0[np], 100*yErr0[np]/yVal0[np],
		 yVal1[np]) << endl;
  }
  
  TString weightName = weight;
  weightName.ReplaceAll("*", "");
  weightName.ReplaceAll(".", "p");
  
  TString lcut = lasercut;
  lcut.Remove(0, lcut.Last('=') + 1);
  
  TString ofilename = Form("plots/calcratebatch/calcratebatch_%s-RV%i_lphd%s_ndet%03i_bsys%i_weight%s_t%04i-%04ins_ene%03i-%03ikeV_ref%s-RV%i-b%02i.root",
			   tag.Data(), repro, lcut.Data(), ndet, bsys, weightName.Data(), tmin, tmax, sign0, sign1, tagB.Data(), reproB, batchB);
  
  TFile* ofile = new TFile(ofilename, "UPDATE");

  cout << " ofilename: " << ofilename << endl;

  TGraphErrors* g0 = new TGraphErrors(nPoints, xVal, yVal0, NULL, yErr0);
  TGraphErrors* g1 = new TGraphErrors(nPoints, xVal, yVal1, NULL, yErr1);
//  TGraphErrors* g2 = new TGraphErrors(nPoints, xVal, yVal2, NULL, yErr2);

  TString xtitle = "Wavelength [pm] - 6788000";

  g0->GetXaxis()->SetTitle(xtitle);
  g1->GetXaxis()->SetTitle(xtitle);
//  g2->GetXaxis()->SetTitle(xtitle);

  g0->GetYaxis()->SetTitle(Form("Normalised integral ratio in [%i,%i] keV wrt. reference", sign0, sign1));
  g1->GetYaxis()->SetTitle("Ratio of #mu triggers wrt. reference");
//  g2->GetYaxis()->SetTitle(Form("Normalised integral ratio in [%i,%i] keV wrt. reference", sign0, sign1));
  
  g0->Write("g0");
  g1->Write("g1");
//  g2->Write("g2");
  
  ofile->Write("", TObject::kOverwrite);
  ofile->Close();
  delete ofile;
  
}
