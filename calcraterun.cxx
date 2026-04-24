{
  TString tag      = "RAL202305";
  UInt_t  repro    = 0;
  UInt_t  skim     = 1;
  UInt_t  batchS   = 9; // O2
  UInt_t  batchB   = 6; // H2
  UInt_t  ndet     = 10;
  TString weight   = "w0*w1";
  UInt_t  run0     = 40046;
  UInt_t  run1     = 40054;
  UInt_t  brmsmax  = 1e7;
  UInt_t  basemax  = 1e7;
  UInt_t  flagmax  = 1e7;
  UInt_t  deltamin = 0;
  UInt_t  tmin     = 780;
  UInt_t  tmax     = 3e3;

  Double_t timeshift = 0; // with respect to digitizer 4

//ndet = 230; timeshift = 60;  // digitizers 2-3
//ndet = 231; timeshift = 130; // digitizers 0-1
//ndet = 232; timeshift = 130; // digitizers 0-1

  if (tag = "RAL202305" && repro == 0) {
    
    tmin += timeshift;
    tmax += timeshift;
  }
  
  CalcRateRun(tag, repro, batchS, batchB, ndet, weight, run0, run1, brmsmax, basemax, flagmax, deltamin, tmin, tmax);
}
