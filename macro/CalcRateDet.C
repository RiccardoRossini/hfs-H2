
void CalcRateDet(TString tag      = "RAL202305",
		 UInt_t  repro    = 1,
		 UInt_t  batchS   = 9,
		 UInt_t  batchB   = 6,
		 TString weight   = "w0*w1",
		 UInt_t  tmin     = 1000,
		 UInt_t  tmax     = 1600,
		 UInt_t  brmsmax  = 1e7,
		 UInt_t  basemax  = 1e7,
		 UInt_t  flagmax  = 1e7,
		 UInt_t  deltamin = 0) {

  auto detMap = GetDetMap(tag);

  UInt_t nBins = detMap.size();

  TH1F* h0 = new TH1F("h0", "h0", nBins, -0.5, nBins+0.5);
  TH1F* h1 = new TH1F("h1", "h1", nBins, -0.5, nBins+0.5);

  TString ytitle = "Number of X-rays from Oxygen";
  
  h0->GetYaxis()->SetTitle(ytitle);
  h0->GetYaxis()->SetTitleOffset(0.7);  
  h0->GetXaxis()->SetLabelSize(0.06);
  
  h1->GetYaxis()->SetTitle(ytitle);
  h1->GetYaxis()->SetTitleOffset(0.7);  
  h1->GetXaxis()->SetLabelSize(0.06);
  
  map<UInt_t, TString>::iterator detIt;
  
  UInt_t bin=0;
  
  for (detIt = detMap.begin(); detIt != detMap.end(); detIt++) {

    UInt_t  ndet    = detIt->first;
    TString detname = detIt->second;
    
    cout << Form(" ndet: %03i detname: %s", ndet, detname.Data()) << endl;

    h0->GetXaxis()->SetBinLabel(bin+1, detname);
    h1->GetXaxis()->SetBinLabel(bin+1, detname);

    vector<Double_t> y0 = PlotEne(tag, repro, batchS, batchB, ndet, weight, tmin, tmax, 0, 1, brmsmax, basemax, flagmax, deltamin);
    vector<Double_t> y1 = PlotEne(tag, repro, batchS, batchB, ndet, weight, tmin, tmax, 1, 0, brmsmax, basemax, flagmax, deltamin);
    vector<Double_t> y2 = PlotEne(tag, repro, batchS, batchB, ndet, weight, tmin, tmax, 2, 0, brmsmax, basemax, flagmax, deltamin);
    
    Double_t dy1 = fabs(y1[0] - y0[0]);
    Double_t dy2 = fabs(y2[0] - y0[0]);
    Double_t sys = TMath::Max(dy1, dy2); 
    
    cout << " dy1: " << dy1 << endl;
    cout << " dy2: " << dy2 << endl;
    cout << " sys: " << sys << endl;

    Double_t yVal0 = y0[0]; // Oxygen X-rays
    Double_t yErr0 = sqrt(pow(y0[1], 2) + pow(sys, 2));

    Double_t yVal1 = y0[0]/y0[2]; // Signal/Background
    Double_t yErr1 = y0[0]/y0[2]*sqrt(pow(y0[1]/y0[0], 2) + pow(y0[3]/y0[2], 2));

    h0->SetBinContent(bin+1, yVal0);
    h0->SetBinError  (bin+1, yErr0);

    h1->SetBinContent(bin+1, yVal1);
    h1->SetBinError  (bin+1, yErr1);
    
    cout << Form(" ndet %03i %8s batch %02i signal: %9.2f +/- %9.2f (%5.1f%%) S/N: %.0f", 
		 ndet, detname.Data(), batchS, yVal0, yErr0, 100*yErr0/yVal0, yVal1) << endl;
    
    bin++;
  }
  
  cout << " Detector dependent analysis " << endl;

  TString ofilename = "calcratedet.root";
  
  TFile* ofile = new TFile(ofilename, "UPDATE");

  cout << " ofilename: " << ofilename << endl;
  
  h0->Write();
  h1->Write();
  
  ofile->Write("", TObject::kOverwrite);
  ofile->Close();
}
