
void GaussianKernel(UInt_t    nEntries,
		    Double_t* eneVec,
		    TH1F*     eneHis,
		    TString   formula = "97.1/sqrt(x*511./390.)",
		    UInt_t    nSteps  = 100) { 
  
  if (!eneHis) { cout << " Histrogram for GaussianKernel is not defined!" << endl; exit(0); }

  cout << " GaussianKernel, nSteps: " << nSteps << endl;

  TF1*      reso = new TF1("reso", formula, 1, 5000); // Resolution[%] Vs Energy[keV]
  TRandom3* rnd3 = new TRandom3();
  
  rnd3->SetSeed(1);
  
  for (UInt_t entry=0; entry<nEntries; entry++) {

    Double_t ene = eneVec[entry];
    
    for (UInt_t step=0; step<nSteps; step++) 
      eneHis->Fill(rnd3->Gaus(ene, ene*reso->Eval(ene)));
  }
}
