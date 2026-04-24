{
  TString  tag   = "RAL202305";
  UInt_t   repro = 1;
  UInt_t   batch = 9; // O2
  Double_t y0min = 0;
  Double_t y0max = 0;
  Double_t y1min = 0;
  Double_t y1max = 0;
  TString  label = "Internal";

  tag   = "RAL202405";
  repro = 0;
  batch = 3; // O2

  PlotRateDet(tag, repro, batch, 0, y0min, y0max, label); // S
  PlotRateDet(tag, repro, batch, 1, y1min, y1max, label); // S/B
}
