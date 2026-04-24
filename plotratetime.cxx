{
  TString  tag     = "RAL202305";
  UInt_t   repro   = 1;
  UInt_t   ndet    = 300; // LaBr + MIB
  UInt_t   tmin    = 700; // tmin starts from 700 in slim data files
  UInt_t   tmax    = 2e3;
  UInt_t   nPoints = 26;
  Double_t ymin    = 0;
  Double_t ymax    = 1600;
  Bool_t   prelim  = 1;

  ndet = 10; // LaBr
  ymax = 500;

  PlotRateTime(tag, repro, ndet, tmin, tmax, nPoints, ymin, ymax, prelim);
}
