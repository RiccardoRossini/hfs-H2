
void PlotRateDet(TString  tag   = "RAL202305",
		 UInt_t   repro = 1,
		 UInt_t   batch = 9,
		 UInt_t   histo = 0,
		 Double_t ymin  = 0,
		 Double_t ymax  = 0,
		 TString  label = "Internal") {
  
  TString ifilename = Form("plots/%s/repro%i/calcratedet/batch%02i/calcratedet.root",
			   tag.Data(), repro, batch);

  if (gSystem->AccessPathName(ifilename)) { cout << " File not found: " << ifilename << endl; return; }

  cout << " ifilename: " << ifilename << endl;

  TFile* ifile = TFile::Open(ifilename, "READ"); 

  auto h0 = (TGraphErrors*)ifile->Get(Form("h%i", histo));
  
  TString c0name = Form("plotratedet_batch%02i_h%i",
			batch, histo); 
  
  TCanvas* c0 = new TCanvas(c0name, c0name);

  h0->Draw("");
    
  if (ymin) h0->SetMinimum(ymin);
  if (ymax) h0->SetMaximum(ymax);

  TString text0 = GetText0(tag, batch);
  
  FAMULabel(0.25, 0.86, label, 0.1);
  myText   (0.25, 0.80, 1, text0);
  
  c0->Print(c0name + ".gif");
  c0->Print(c0name + ".pdf");
}
