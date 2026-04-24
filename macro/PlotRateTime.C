
void PlotRateTime(TString  tag     = "RAL202305",
		  UInt_t   repro   = 1,
		  UInt_t   ndet    = 300,
		  UInt_t   tmin    = 780,
		  UInt_t   tmax    = 3e3,
		  UInt_t   nPoints = 10,
		  Double_t ymin    = 0,
		  Double_t ymax    = 0,
		  TString  label   = "Internal") {

  vector<UInt_t> batchVec  = GetBatchVec (tag);
  vector<UInt_t> lambdaVec = GetLambdaVec(tag);
  
  UInt_t nBatches = batchVec.size();

  const UInt_t nColors = 9;

  UInt_t gcol[nColors] = {kBlack, kBlue, kRed, kGreen, kViolet, kRed+2, kOrange+1, kCyan, kGray};
  
  if (nBatches > nColors) { cout << " ERROR! more batches than availbale colors... add more colors" << endl; exit(0); }

  vector<TGraphErrors*> gvec;
  
  TString detname = GetDetName(tag, ndet);  
    
  for (UInt_t batchIt=0; batchIt<nBatches; batchIt++) {

    UInt_t batch  = batchVec [batchIt];
    UInt_t lambda = lambdaVec[batchIt];
    
    TString ifilename = Form("plots/%s/repro%i/calcratetime/ndet%03i/calcratetime_batch%02i_ndet%03i_%s_%04ins_%04ins.root", 
 			     tag.Data(), repro, ndet, batch, ndet, detname.Data(), tmin, tmax);
    
    if (gSystem->AccessPathName(ifilename)) { cout << " File not found: " << ifilename << endl; return; }
    
    cout << " ifilename: " << ifilename << endl;

    TFile* ifile = TFile::Open(ifilename, "READ"); 

    gvec.push_back(new TGraphErrors);

    gvec[batchIt] = (TGraphErrors*)ifile->Get(Form("gRate%03i", ndet));

    gvec[batchIt]->SetLineColor  (gcol[batchIt]);
    gvec[batchIt]->SetMarkerColor(gcol[batchIt]);

    Double_t norm = gvec[0]->GetY()[0]/gvec[batchIt]->GetY()[0];
    
    for (UInt_t np=0; np<nPoints; np++) {

      TString dummy = Form("%i", np); // dummy line needed to avoid seg fault!!!!

      gvec[batchIt]->GetY()[np] *= norm;
    }

    TString c0name = Form("plotratetime_ndet%03i_%s_batch%02i_wl%04i",
			  ndet, detname.Data(), batch, lambda); 

    TCanvas* c0 = new TCanvas(c0name, c0name, 1000, 600);
    
    c0->SetRightMargin(0.15);
    
    gvec[batchIt]->Draw("APl");
    
    if (ymin) gvec[batchIt]->SetMinimum(ymin);
    if (ymax) gvec[batchIt]->SetMaximum(ymax);

    TString text = Form("wl %04i", lambda);

    FAMULabel(0.5, 0.86, label);
    myText   (0.5, 0.80, 1, detname);
    myText   (0.5, 0.74, 1, text);

    c0->Print(c0name + ".gif");
    c0->Print(c0name + ".pdf");
  }
  
  TString c0name = Form("plotratetime_ndet%03i_%s", ndet, detname.Data()); 

  TCanvas* c0 = new TCanvas(c0name, c0name, 1000, 600);

  c0->SetRightMargin(0.15);

  for (UInt_t batchIt=0; batchIt<nBatches; batchIt++) {

    if (batchIt == 0) { 

      gvec[batchIt]->Draw("APl");

      if (ymin) gvec[batchIt]->SetMinimum(ymin);
      if (ymax) gvec[batchIt]->SetMaximum(ymax);
    }
    else gvec[batchIt]->Draw("Plsame");

    TString text = Form("wl %04i", lambdaVec[batchIt]);

    myText(0.88, 0.88-0.06*batchIt, gcol[batchIt], text, 0.05);
  }  
  
  FAMULabel(0.5, 0.86, label);
  myText   (0.5, 0.80, 1, detname);

  c0->Print(c0name + ".gif");
  c0->Print(c0name + ".pdf");
}
