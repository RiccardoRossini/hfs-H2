{
  UInt_t  repro = 2;  
  UInt_t  nset  = 0;   // weight="w0*w1"
  UInt_t  ndet  = 300; // LaBr+MIB
  TString label = "Internal";

//nset = 1; // weight="1"
  
  PlotRateBatch(repro, nset, ndet, 0, -5e4, +8.0e5, label); // Oxygen X-rays
  PlotRateBatch(repro, nset, ndet, 1, 1e-3, +5.5e6, label); // mu-triggers
  PlotRateBatch(repro, nset, ndet, 2, 1e-3, +0.290, label); // Oxygen X-rays/mu-triggers
}
