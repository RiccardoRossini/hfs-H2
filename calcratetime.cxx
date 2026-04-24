{
  TString tag      = "RAL202305";
  UInt_t  repro    = 1;
  UInt_t  batchB   = 6;   // H2
  UInt_t  ndet     = 300; // LaBr-MIB
  TString weight   = "w0*w1";
  UInt_t  tmin     = 700; // tmin starts from 700 in skim1 data files
  UInt_t  tmax     = 2000;
  UInt_t  brmsmax  = 1e7;
  UInt_t  basemax  = 1e7;
  UInt_t  flagmax  = 1e7;
  UInt_t  deltamin = 0;
  UInt_t  nPoints  = 26;

  ndet = 10; // LaBr

  vector<UInt_t> batchVec = GetBatchVec(tag);
  
  for (UInt_t batch=0; batch<batchVec.size(); batch++)
    CalcRateTime(tag, repro, batchVec[batch], batchB, ndet, weight, tmin, tmax, brmsmax, basemax, flagmax, deltamin, nPoints);
}
