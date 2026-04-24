
void CalcRateTime(TString tag      = "RAL202305",
		  UInt_t  repro    = 1,
		  UInt_t  batchS   = 9,
		  UInt_t  batchB   = 6,
		  UInt_t  ndet     = 1,
		  TString weight   = "w0*w1",
		  UInt_t  tmin     = 780,
		  UInt_t  tmax     = 3e3,
		  UInt_t  brmsmax  = 20,
		  UInt_t  basemax  = 10,
		  UInt_t  flagmax  = 10,
		  UInt_t  deltamin = 30,
		  UInt_t  nPoints  = 1) {

  vector<Double_t> tvec = GetTimeVec(tmin, tmax, nPoints);
  
  for (UInt_t i=0; i<tvec.size(); i++)
    cout << " i: " << i << " tvec: " << tvec[i] << endl;
  
  Double_t* xVal = new Double_t[nPoints];
  Double_t* yVal = new Double_t[nPoints];
  Double_t* xErr = new Double_t[nPoints];
  Double_t* yErr = new Double_t[nPoints];

  TString detname = GetDetName(tag, ndet);  

  for (UInt_t np=0; np<nPoints; np++) {

    Double_t dt = tvec[np+1] - tvec[np]; 

    cout << " np: " << np << " tvec: " << tvec[np] << " dt: " << dt << endl;
    
    xErr[np] = 0.5*dt;
    xVal[np] = tvec[np] + xErr[np];

    vector<Double_t> y0 = PlotEne(tag, repro, batchS, batchB, ndet, weight, tvec[np], tvec[np+1], 0, np<6, brmsmax, basemax, flagmax, deltamin);
    vector<Double_t> y1 = PlotEne(tag, repro, batchS, batchB, ndet, weight, tvec[np], tvec[np+1], 1,    0, brmsmax, basemax, flagmax, deltamin);
    vector<Double_t> y2 = PlotEne(tag, repro, batchS, batchB, ndet, weight, tvec[np], tvec[np+1], 2,    0, brmsmax, basemax, flagmax, deltamin);
    
    Double_t dy1 = fabs(y1[0] - y0[0]);
    Double_t dy2 = fabs(y2[0] - y0[0]);
    Double_t sys = TMath::Max(dy1, dy2); 
    
    cout << " dy1: " << dy1 << endl;
    cout << " dy2: " << dy2 << endl;
    cout << " sys: " << sys << endl;

    yVal[np] = y0[0]/dt; 
    yErr[np] = sqrt(pow(y0[1], 2) + pow(sys, 2))/dt;

    cout << Form(" signal: ndet %03i %8s %9.2f %9.2f (%5.1f%%)", 
		 ndet, detname.Data(), yVal[np]*dt, yErr[np]*dt, 100*yErr[np]/yVal[np]) << endl;
  }
  
  if (nPoints == 1) return;
  
  cout << " Time dependent analysis " << endl;

  TString ofilename = Form("calcratetime_batch%02i_ndet%03i_%s_%04ins_%04ins.root", 
			   batchS, ndet, detname.Data(), tmin, tmax);

  TFile* ofile = new TFile(ofilename, "UPDATE");

  cout << " ofilename: " << ofilename << endl;
  
  TGraphErrors* gRate = new TGraphErrors(nPoints, xVal, yVal, xErr, yErr);
  
  TString gName = Form("gRate%03i", ndet);

  gRate->GetXaxis()->SetTitle("t_{s} [ns] = t_{signal} - t_{trigger} ");
  gRate->GetYaxis()->SetTitle("Number of Oxygen X-rays per ns");
  
  gRate->Write(gName);
  
  ofile->Write("", TObject::kOverwrite);
  ofile->Close();
  
  delete [] xVal;
  delete [] yVal;
  delete [] xErr;
  delete [] yErr;
}
