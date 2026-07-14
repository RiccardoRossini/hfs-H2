{ UInt_t  repro    = 6;
  UInt_t  ndet     = 300; // 300 = LaBr+MIB, 299 = never-moved, 230 = MIB, 10 = LaBr
  TString weight   = "w0*w1";
  UInt_t  tmin     = 780; // -1 = ltshot (laser injection)
  UInt_t  tmax     = 2000; 
  UInt_t  bsys     = 0;
  Bool_t  draw     = 0; // set 1 to see plots (might crash...)
  UInt_t  brmsmax  = 1e7;
  UInt_t  basemax  = 1e7;
  UInt_t  flagmax  = 1e7;
  UInt_t  deltamin = 0;
//  TString label    = "Internal";
  UInt_t  nset     = 0;
  TString lasercut = "lphd==0";
  label = "";
  UInt_t  sign0    = 100;
  UInt_t  sign1    = 200;
  Bool_t  differ   = 0; // differ:  the value returned by PlotEne is: 0 = the ratio, 1 = the difference

  if (nset == 1) weight = "1";
  
  // set the period to analyse
  TString   tag      = "RAL202509";
  
  // set the background batch 0=REF, 1=H2, other=custom
  UInt_t    bckgrsel = 1;
  TString   tagB;
  UInt_t    batchB;
  UInt_t    reproB = repro;
  switch (bckgrsel) {
    case 0:
        tagB   = "RAL202304";
        batchB = 11;
        break;
    case 1:
        tagB   = "RAL202509";
        batchB = 33;
        break;
    default:
        tagB   = "insert_tag";
        batchB = 0; // <-- insert batch 
        break;
}
  
//ndet = 1; // LaBr

  CalcRateBatch(tag, repro, tagB, reproB, batchB, ndet, weight, tmin, tmax, bsys, draw, brmsmax, basemax, flagmax, deltamin, lasercut, "Internal", sign0, sign1, differ);

}
