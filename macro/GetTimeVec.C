
vector<Double_t> GetTimeVec(Double_t tmin    = 1100, 
			    Double_t tmax    = 6000,
			    UInt_t   nPoints = 10,
			    Bool_t   logbins = 0) {

  vector<Double_t> tvec; // time vector
  
  if (logbins) {

    Double_t lmin = log(tmin);
    Double_t lmax = log(tmax);
    Double_t bins = (lmax - lmin)/nPoints;
    
    vector<Double_t> lvec; // logarithmic vector
    
    lvec.push_back(lmin); 
    
    for (UInt_t i=1; i<nPoints; i++) 
      lvec.push_back(lvec[i-1] + bins);
    
    lvec.push_back(lmax);
    
    for (UInt_t i=0; i<nPoints+1; i++)
      tvec.push_back(TMath::Nint(exp(lvec[i])));
  }
  else {
    
    Double_t dt = (tmax - tmin)/nPoints;

    for (UInt_t np=0; np<nPoints+1; np++) 
      tvec.push_back(tmin + np*dt);
  }
  
  return tvec;
}
