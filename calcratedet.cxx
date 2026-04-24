{
  TString tag      = "RAL202305";
  UInt_t  repro    = 1;
  UInt_t  batchS   = 9; // O2
  UInt_t  batchB   = 6; // H2
  TString weight   = "w0*w1";
  UInt_t  tmin     = 1000;
  UInt_t  tmax     = 1600;
  UInt_t  brmsmax  = 1e7;
  UInt_t  basemax  = 1e7;
  UInt_t  flagmax  = 1e7;
  UInt_t  deltamin = 0;

  tag    = "RAL202405";
  repro  = 0;
  batchB = 25; // H2

  CalcRateDet(tag, repro, batchS, batchB, weight, tmin, tmax,
	      brmsmax, basemax, flagmax, deltamin);
}
