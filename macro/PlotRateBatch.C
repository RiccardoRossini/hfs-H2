
void PlotRateBatch(TString  tag   = "RAL202405",
		   UInt_t   repro = 2,
		   UInt_t   nset  = 0,
		   UInt_t   ndet  = 300,
		   UInt_t   mode  = 0,
		   Double_t ymin  = 0,
		   Double_t ymax  = 0,
		   TString  label = "Internal") {

  TString detname = GetDetName(tag, ndet);  

  TString ifilename = Form("plots/calcratebatch/set%i/calcratebatch_%s_repro%i_ndet%03i_%s.root",
			   nset, tag.Data(), repro, ndet, detname.Data());
  
  if (gSystem->AccessPathName(ifilename)) { cout << " File not found: " << ifilename << endl; return; }
  
  cout << " ifilename: " << ifilename << endl;

  TFile* ifile = TFile::Open(ifilename, "READ"); 

  auto g0 = (TGraphErrors*)ifile->Get(Form("g%i", mode));

  TString c0name = Form("plotratebatch_%s_nset%i_ndet%03i_%s_mode%i",
			tag.Data(), nset, ndet, detname.Data(), mode); 
  
  TCanvas* c0 = new TCanvas(c0name, c0name);

  g0->Draw("AP");
    
  if (ymin) g0->SetMinimum(ymin);
  if (ymax) g0->SetMaximum(ymax);

  TString text0 = GetText0(tag);
  
  FAMULabel(0.22, 0.86, label);
  myText   (0.22, 0.80, 1, text0);
  myText   (0.22, 0.74, 1, detname);
  
  c0->Print(c0name + ".gif");
  c0->Print(c0name + ".pdf");
  c0->Print(c0name + ".png");
}

void PlotRateBatch(UInt_t   repro = 2,
		   UInt_t   nset  = 0,
		   UInt_t   ndet  = 300,
		   UInt_t   mode  = 0,
		   Double_t ymin  = 0,
		   Double_t ymax  = 0,
		   TString  label = "Internal") {

  TString tag0 = "RAL202305";
  TString tag1 = "RAL202405";

  TString detname = GetDetName(tag0, ndet);  

  TString ifilepath = Form("plots/calcratebatch/set%i/calcratebatch", nset);

  TString ifilename0 = Form("%s_%s_repro%i_ndet%03i_%s.root", ifilepath.Data(), tag0.Data(), repro, ndet, detname.Data());
  TString ifilename1 = Form("%s_%s_repro%i_ndet%03i_%s.root", ifilepath.Data(), tag1.Data(), repro, ndet, detname.Data());

  if (gSystem->AccessPathName(ifilename0)) { cout << " File not found: " << ifilename0 << endl; return; }
  if (gSystem->AccessPathName(ifilename1)) { cout << " File not found: " << ifilename1 << endl; return; }
  
  cout << " ifilename0: " << ifilename0 << endl;
  cout << " ifilename1: " << ifilename1 << endl;

  TFile* ifile0 = TFile::Open(ifilename0, "READ"); 
  TFile* ifile1 = TFile::Open(ifilename1, "READ"); 

  auto g0 = (TGraphErrors*)ifile0->Get(Form("g%i", mode));
  auto g1 = (TGraphErrors*)ifile1->Get(Form("g%i", mode));

  g1->SetMarkerStyle(21);

  TMultiGraph* m0 = new TMultiGraph("m0", "m0");

  m0->Add(g0);
  m0->Add(g1);

  TString xtitle = "#lambda [pm] - 6788000";
  TString ytitle = (mode == 2) ? "S/n" : g0->GetYaxis()->GetTitle();
  
  m0->GetXaxis()->SetTitle(xtitle);
  m0->GetYaxis()->SetTitle(ytitle);

  TString c0name = Form("plotratebatch_%s_%s_repro%i_nset%i_ndet%03i_%s_mode%i",
			tag0.Data(), tag1.Data(), repro, nset, ndet, detname.Data(), mode);
  
  TCanvas* c0 = new TCanvas(c0name, c0name);
  
  m0->Draw("AP");

  if (ymin) m0->SetMinimum(ymin);
  if (ymax) m0->SetMaximum(ymax);
  
  FAMULabel   (0.22, 0.86, label);
  myMarkerText(0.72, 0.86, 1, 8,  GetText0(tag0));
  myMarkerText(0.72, 0.80, 1, 21, GetText0(tag1));
  
  if (mode != 1) {
    
    myText(0.22, 0.80, 1, GetDetName(tag0, ndet));
  }
  
  c0->Print(c0name + ".gif");
  c0->Print(c0name + ".pdf");
  c0->Print(c0name + ".png");
}
