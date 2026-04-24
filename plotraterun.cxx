{
  TString  tag   = "RAL202303";
  UInt_t   repro = 1;
  UInt_t   batch = 9;
  UInt_t   nset  = 0;
  UInt_t   ndet  = 10;
  UInt_t   tmin  = 780;
  UInt_t   tmax  = 3e3;
  UInt_t   run0  = 40046;
  UInt_t   run1  = 40054;
  Double_t ymin  = 1;
  Double_t ymax  = 349;
  
  //ndet = 230; ymax = 549;
  //ndet = 231; ymax = 199;
  //ndet = 232; ymax =  99;

  PlotRateRun(tag, repro, batch, nset, ndet, tmin, tmax, run0, run1, ymin, ymax);
}
