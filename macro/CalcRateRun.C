
void CalcRateRun(TString tag      = "RAL202305",
		 UInt_t  repro    = 1,
		 UInt_t  batchS   = 9,
		 UInt_t  batchB   = 6,
		 UInt_t  ndet     = 10,
		 TString weight   = "w0*w1",
		 UInt_t  run0     = 40046,
		 UInt_t  run1     = 40054,
		 UInt_t  brmsmax  = 20,
		 UInt_t  basemax  = 10,
		 UInt_t  flagmax  = 10,
		 UInt_t  deltamin = 30,
		 UInt_t  tmin      = 780,
		 UInt_t  tmax      = 3e3,
		 TString timeUnit  = "m") {

  UInt_t nPoints = run1-run0+1;
  
  cout << " tag:      " << tag      << endl;  
  cout << " repro     " << repro    << endl;  
  cout << " run0:     " << run0     << endl;
  cout << " run1:     " << run1     << endl;
  cout << " ndet:     " << ndet     << endl;
  cout << " brmsmax:  " << brmsmax  << endl;
  cout << " basemax:  " << basemax  << endl;
  cout << " flagmax:  " << flagmax  << endl;
  cout << " deltamin: " << deltamin << endl;
  cout << " tmin:     " << tmin     << endl;
  cout << " tmax:     " << tmax     << endl;  

  Double_t* xVal = new Double_t[nPoints];
  Double_t* yVal = new Double_t[nPoints];
  Double_t* xErr = new Double_t[nPoints];
  Double_t* yErr = new Double_t[nPoints];

  TString timeText;
  
  if      (timeUnit == "s") timeText = "sec.";
  else if (timeUnit == "m") timeText = "min.";
  else if (timeUnit == "h") timeText = "hour";
  
  for (UInt_t np=0; np<nPoints; np++) {

    UInt_t run = run0+np;

    xVal[np] = run;
    xErr[np] = 0;

    vector<Double_t> y0 = PlotEne(tag, repro, batchS, batchB, ndet, weight, tmin, tmax, 0, 1, brmsmax, basemax, flagmax, deltamin);
    vector<Double_t> y1 = PlotEne(tag, repro, batchS, batchB, ndet, weight, tmin, tmax, 1, 0, brmsmax, basemax, flagmax, deltamin);
    vector<Double_t> y2 = PlotEne(tag, repro, batchS, batchB, ndet, weight, tmin, tmax, 2, 0, brmsmax, basemax, flagmax, deltamin);
    
    Double_t dy1 = fabs(y1[0] - y0[0]);
    Double_t dy2 = fabs(y2[0] - y0[0]);
    Double_t sys = TMath::Max(dy1, dy2); 
    
    cout << " dy1: " << dy1 << endl;
    cout << " dy2: " << dy2 << endl;
    cout << " sys: " << sys << endl;

    Double_t dt = GetTimeRun(tag, repro, batchS, run, timeUnit); 
    
    cout << Form(" dt[%s]: %.1f", timeText.Data(), dt) << endl;		    

    yVal[np] = y0[0]/dt; 
    yErr[np] = sqrt(pow(y0[1], 2) + pow(sys, 2))/dt;

    cout << " yval: " << Form("%.2f",          yVal[np]) << endl;
    cout << " yerr: " << Form("%.2f (%.1f%%)", yErr[np], 100*yErr[np]/yVal[np]) << endl;
  }
  
  cout << " Run dependent analysis " << endl;

  TString detname = GetDetName(tag, ndet);  

  TString ofilename = Form("calcraterun_batch%02i_ndet%03i_%s_%04ins_%04ins_%06i_%06i.root", 
			   batchS, ndet, detname.Data(), tmin, tmax, run0, run1);
  
  TFile* ofile = new TFile(ofilename, "UPDATE");

  cout << " ofilename: " << ofilename << endl;
  
  TGraphErrors* gRate = new TGraphErrors(nPoints, xVal, yVal, xErr, yErr);

  TString gName = Form("gRate%03i", ndet);
  
  gRate->GetXaxis()->SetTitle("Run number");
  gRate->GetYaxis()->SetTitle(Form("Number of Oxygen X-rays per %s", timeText.Data()));
  
  gRate->Write(gName);
  
  ofile->Write("", TObject::kOverwrite);
  ofile->Close();
  
  delete [] xVal;
  delete [] yVal;
  delete [] xErr;
  delete [] yErr;
}
