
void PlotRateRun(TString  tag   = "RAL202303",
		 UInt_t   repro = 1,
		 UInt_t   batch = 9,
		 UInt_t   nset  = 0,
		 UInt_t   ndet  = 10,
		 UInt_t   tmin  = 780,
		 UInt_t   tmax  = 3e3,
		 UInt_t   run0  = 40046,
		 UInt_t   run1  = 40054,
		 Double_t ymin  = 1,
		 Double_t ymax  = 0,
		 TString  label = "Internal") {

  TString detname = GetDetName(tag, ndet);  

  TString ifilename = Form("plots/%s/repro%i/calcraterun/batch%02i/set%i/calcraterun_ndet%03i_%s_%04ins_%04ins_%06i_%06i.root",
			   tag.Data(), repro, batch, nset, ndet, detname.Data(), tmin, tmax, run0, run1);
  
  if (gSystem->AccessPathName(ifilename)) { cout << " File not found: " << ifilename << endl; return; }

  cout << " ifilename: " << ifilename << endl;

  TFile* ifile = TFile::Open(ifilename); 

  gStyle->SetOptFit(1);

  TString g0name = Form("gRate%03i", ndet);
  
  TGraphErrors* g0 = (TGraphErrors*)ifile->Get(g0name);
  
  if (!g0) { cout << " ERROR! TGraphErrors not found: " << g0name << endl; exit(0); }

  TGaxis::SetMaxDigits(6);

  g0->GetXaxis()->SetNdivisions(505);
  
  TString c0name = gSystem->BaseName(ifilename);

  c0name.Replace(c0name.Index("calc"), 4, "plot");  
  c0name.Remove (c0name.Index(".root"));
  
  TCanvas* c0 = new TCanvas(c0name, c0name);
  
  g0->Fit("pol0");
  g0->Draw("AP");

  if (ymin) g0->SetMinimum(ymin);
  if (ymax) g0->SetMinimum(ymax);

  SetStatPos(c0, g0, 0.52, 0.71, 0.88, 0.87);  

  TString text0 = GetText0(tag, batch);
  TString text1 = Form("91K, %s", detname.Data());
  
  FAMULabel(0.25, 0.86, label);
  myText   (0.25, 0.80, 1, text0);
  myText   (0.25, 0.74, 1, text1);

  c0->Print(c0name + ".gif");
  c0->Print(c0name + ".pdf");
}
