
vector<Double_t> PlotEne(TString tag      = "RAL202304",
			 UInt_t  repro    = 5,
			 UInt_t  batchS   = 13,
			 TString tagB     = "RAL202303",
			 UInt_t  reproB   = 5,
			 UInt_t  batchB   = 6,
			 UInt_t  ndet     = 300,
			 TString weight   = "w0*w1",
			 UInt_t  start0   = 780,
			 UInt_t  start1   = 2000,
			 UInt_t  bsys     = 0,
			 Bool_t  draw     = 1,
			 UInt_t  brmsmax  = 1e7,
			 UInt_t  basemax  = 1e7,
			 UInt_t  flagmax  = 1e7,
			 UInt_t  deltamin = 0,
			 TString label    = "Internal",
			 TString lasercut = "lphd==0",
			 UInt_t  sign0    = 60,
			 UInt_t  sign1    = 200,
			 Bool_t  differ   = 0, // the value returned by PlotEne is: 0 = the ratio, 1 = the difference
			 UInt_t  bin0     = 60,
			 UInt_t  bin1     = 600,
			 UInt_t  binw     = 4,
			 Bool_t  savehist = 1) {
  
  gROOT->SetBatch(!draw);

  vector<Double_t> signVec;
  
  signVec.push_back(0);
  signVec.push_back(0);
  
  TString ifilenameS = gSystem->GetFromPipe(Form("ls data/famu/%s/ROOT/L3-RV%i/batch%i*/*.root | tail -n 1", tag.Data(),  repro,  batchS));
  TString ifilenameB = gSystem->GetFromPipe(Form("ls data/famu/%s/ROOT/L3-RV%i/batch%i*/*.root | tail -n 1", tagB.Data(), reproB, batchB));
  
  if (gSystem->AccessPathName(ifilenameS)) { cout << " File not found: " << ifilenameS << endl; return signVec; }
  if (gSystem->AccessPathName(ifilenameB)) { cout << " File not found: " << ifilenameB << endl; return signVec; }
  
  TFile* tfs = TFile::Open(ifilenameS, "READ"); TTree* t1s = (TTree*)tfs->Get("t1");
  TFile* tfb = TFile::Open(ifilenameB, "READ"); TTree* t1b = (TTree*)tfb->Get("t1");
  
  cout << " ifilenameS: " << ifilenameS << endl;
  cout << " ifilenameB: " << ifilenameB << endl;
  if (!tfs || tfs->IsZombie()) {
    cout << "ERROR opening file S" << endl;
    return signVec;
  }
  if (!tfb || tfb->IsZombie()) {
    cout << "ERROR opening file B" << endl;
    return signVec;
  }
  
  TString h0name = Form("h0_%i_%04i_%04i", bsys, start0, start1);
  TString h1name = Form("h1_%i_%04i_%04i", bsys, start0, start1);
  TString h2name = Form("h2_%i_%04i_%04i", bsys, start0, start1);

  UInt_t bins = (bin1-bin0)/binw;

  TH1F* h0 = new TH1F(h0name, "", bins, bin0, bin1);
  TH1F* h1 = new TH1F(h1name, "", bins, bin0, bin1);
  TH1F* h2 = new TH1F(h2name, "", bins, bin0, bin1);
  h0->Sumw2();
  h1->Sumw2();
  h2->Sumw2();


  TCut tcut = Form("%s*(x.brms<%i&&abs(x.base)<%i&&x.flag<%i&&x.delta>%i&&x.start>%i&&x.start<%i&&%s)",
		   weight.Data(), brmsmax, basemax, flagmax, deltamin, start0, start1, lasercut.Data());
	
	// we always need noLaser data for the background
	TString lasercutB = "lphd==0";
	TCut tcutB = Form("%s*(x.brms<%i&&abs(x.base)<%i&&x.flag<%i&&x.delta>%i&&x.start>%i&&x.start<%i&&%s)",
		   weight.Data(), brmsmax, basemax, flagmax, deltamin, start0, start1, lasercutB.Data());
  
  tcut.Print();
  tcutB.Print();
  
//  TString varexp = "x.ene";
  
  t1s->Project(h0name, "x.ene", tcut); 
  t1b->Project(h1name, "x.ene", tcutB); // changed the cut because we need this to be no-Laser only

//GaussianKernel(t1b->Draw("ene", tcut, "goff"), t1b->GetV1(), h1); 

  cout << " Entries0: " << h0->GetEntries() << endl;
  cout << " Entries1: " << h1->GetEntries() << endl;

  h0->SetStats(0);
  h1->SetStats(0);
  h2->SetStats(0);

  h0->GetXaxis()->SetTitle("Energy [keV]");
  h0->GetYaxis()->SetTitle(Form("# X-rays per trigger / %i keV", binw));
  h2->GetXaxis()->SetTitle(h0->GetXaxis()->GetTitle());
  h2->GetYaxis()->SetTitle(Form("Difference in # X-rays per trigger / %i keV", binw));
  
  h1->SetLineColor(2);
  h1->SetFillColor(2);
  h1->SetFillStyle(3001);
  
  
  Double_t trg0 = GetTriggersRR(tag,  repro,  batchS, lasercut);
  Double_t trg1 = GetTriggersRR(tagB, reproB, batchB, lasercutB);
  cout << "Number of triggers for normalisation --> \tthis batch: " << trg0 << "\tref batch: " << trg1 << endl;
  h0->Scale(1./trg0);
  h1->Scale(1./trg1);;
  
  cout << " region: " << Form("[%i, %i]", sign0, sign1) << endl;
//  cout << " bgnd region: " << Form("[%i, %i]", bgnd0, bgnd1) << endl;
  
  UInt_t sign0bin = (sign0-bin0)/binw;
  UInt_t sign1bin = (sign1-bin0)/binw;
//  UInt_t bgnd0bin = (bgnd0-bin0)/binw;
//  UInt_t bgnd1bin = (bgnd1-bin0)/binw;

  cout << " sign0bin: " << sign0bin << endl;
  cout << " sign1bin: " << sign1bin << endl;
//  cout << " bgnd0bin: " << bgnd0bin << endl;
//  cout << " bgnd1bin: " << bgnd1bin << endl;

  Double_t err0, err1;
  Double_t int0 = h0->IntegralAndError(sign0bin, sign1bin, err0); 
  Double_t int1 = h1->IntegralAndError(sign0bin, sign1bin, err1);
  Double_t norm = int0/int1;
  Double_t errnorm = norm*sqrt(pow(err0/int0,2)+pow(err1/int1,2));
  
  if      (bsys == 1) norm = (int0 + sqrt(int0))/(int1 - sqrt(int1));
  else if (bsys == 2) norm = (int0 - sqrt(int0))/(int1 + sqrt(int1));
  else if (bsys != 0) { cout << Form(" Unknown operation for bsys: %i", bsys) << endl; return signVec; }
  
  cout << "  integral sig: " << int0 << " +/- " << err0 << endl;
  cout << "  integral ref: " << int1 << " +/- " << err1 << endl;
  cout << " ratio sig/ref: " << norm << " +/- " << errnorm << endl;
 
  
  TString detname = GetDetName(tag, ndet);  
  
  TString weightName = weight;
  weightName.ReplaceAll("*", "");
  weightName.ReplaceAll(".", "p");
  
  TString lcut = lasercut;
  lcut.Remove(0, lcut.Last('=') + 1);
  
  TString c0name = Form("plots/plotene/plotene_%s-RV%i-b%02i_lphd%s_det%03i_bsys%i_weight%s_t%04i-%04ins_ene%03i-%03ikeV_ref%s-RV%i-b%02i", 
			tag.Data(), repro, batchS, lcut.Data(), ndet, bsys, weightName.Data(), start0, start1, sign0, sign1, tagB.Data(), reproB, batchB);
  
  TCanvas* c0 = new TCanvas(c0name, c0name);

  if (int0>=int1){
    h0->Draw("HISTO");
    h1->Draw("HISTOsame");
    h0->Draw("HISTOsame");
  } else {
    h1->Draw("HISTO");
    h0->Draw("HISTOsame");
//    h0->Draw("HISTOsame");
  }
  
  TString text0 = GetText0(tag, batchS);
  TString text1 = Form("%s, weight %s", detname.Data(), weight.Data());
  TString text2 = Form("t_{s}: [%i, %i] ns",  start0, start1);

  FAMULabel(0.55, 0.86, label);
  myText   (0.55, 0.80, 1, text0);
  myText   (0.55, 0.74, 1, text1);
  myText   (0.55, 0.68, 1, text2);

  TString textS = text0;
  TString textB = Form("ref (%s)", GetText0(tagB, batchB).Data());
  
  myBoxText(0.55, 0.56, 0.05, 1, 1, 0, textS);
  myBoxText(0.55, 0.50, 0.05, 1, 2, 2, textB);

//  c0->Print(c0name + ".gif");
  c0->Print(c0name + ".pdf");
  c0->Print(c0name + ".png");

  TString c1name = c0name + "_diff";
  
  TCanvas* c1 = new TCanvas(c1name, c1name);

  h2->Add(h0, h1, 1, -1);
  h2->Draw("HISTO");

  FAMULabel(0.55, 0.86, label);
  myText   (0.55, 0.80, 1, text0);
  myText   (0.55, 0.74, 1, text1);
  myText   (0.55, 0.68, 1, text2);

//  c1->Print(c1name + ".gif");
  c1->Print(c1name + ".pdf");
  c1->Print(c1name + ".png");

//  signVec[0] = h2->IntegralAndError(sign0bin, sign1bin, signVec[1]);

  if (differ==0){                                                 // case 0: return the ratio between the signal and background integrals
    signVec[0] = norm;
    signVec[1] = errnorm;
  } 
  else if (differ==1){                                            // case 1: return the difference between the signal and background integrals
    signVec[0] = int0-int1;
    signVec[1] = sqrt(pow(err0,2)+pow(err1,2));
  }
//  cout << Form(" sign_sys%i: %.2f +/- %.2f (%.2f %%)", bsys, signVec[0], signVec[1], 100*signVec[1]/signVec[0]) << endl;

//  if (!draw) {
//    tfs->Close();
//    tfb->Close();
//  }
   
  if (savehist){
    TFile * outfile= new TFile(c0name+".root", "RECREATE");
    h0->Write();
    h1->Write();
    h2->Write();
    outfile->Close();
    cout << "Saving hists in " << c0name << ".root" << endl;
  }
  
  /*
  gSystem->Sleep(5000); // wait 5 s before closing the canvases
  c0->Close(); delete c0;
  c1->Close(); delete c1;
  */
  
  //da qui in poi non cambiare ordine delle cose o crasha tutto... 
  
  cout << "Clearing canvases/histograms/files for the current iteration.";
  h0->SetDirectory(nullptr);
  h1->SetDirectory(nullptr);
  h2->SetDirectory(nullptr);
  c0->Close();  delete c0;
  c1->Close();  delete c1;
  delete h0;  delete h1;  delete h2; 
  tfs->Close();   delete tfs;
  tfb->Close();   delete tfb;

 
  return signVec;
}
