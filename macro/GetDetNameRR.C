
TString GetDetName(TString tag  = "RAL202305",		
		   UInt_t  ndet = 0) {

  //From /opt/exp_software/famu/installed-2023/include/FHardConfig-2023-jul1724.h
  TString nameLaBr[ 6] = {"LaBr2","LaBr4","LaBr5","LaBr6","LaBr7","LaBr8"};
  TString nameMIB [30] = {
    "MIB43","MIB44","MIB45","MIB46","MIB49","MIB51","MIB52","MIB53","MIB54","MIB57","MIB62","MIB63",
    "MIB72","MIB76","MIB79","MIB80",
    "MIB81","MIB82","MIB83","MIB84","MIB85","MIB86","MIB71","MIB74","MIB78","MIB87","MIB88","MIB89"};

  if      (tag.Contains("RAL2023")) {}
  else if (tag.Contains("RAL2024")) {

    //From /opt/exp_software/famu/installed-202405/include/FHardConfig-202405.sh
    nameMIB[ 0] = "MIB11";
    nameMIB[ 1] = "MIB12";
    nameMIB[ 2] = "MIB73";
    nameMIB[ 3] = "MIB14";
    nameMIB[ 4] = "MIB16";
    nameMIB[ 5] = "MIB18";
    nameMIB[ 6] = "MIB27";
    nameMIB[ 7] = "MIB28";
    nameMIB[ 8] = "MIB29";
    nameMIB[ 9] = "MIB96";
    nameMIB[10] = "MIB97";
    nameMIB[11] = "MIB98";
    
    nameMIB[12] = "MIB75";
    nameMIB[13] = "MIB77";

    nameMIB[21] = "MIB26";
    nameMIB[22] = "MIB90";
    nameMIB[23] = "MIB91";
    nameMIB[24] = "MIB92";
    nameMIB[25] = "MIB93";
    nameMIB[26] = "MIB94";
    nameMIB[27] = "MIB95";
  }
  else if (tag.Contains("RAL2025")) {

    //From /opt/exp_software/famu/installed-202508/include/FHardConfig-202508.sh
    nameMIB[ 0] = "MIB11";
    nameMIB[ 1] = "MIB12";
    nameMIB[ 2] = "MIB73";
    nameMIB[ 3] = "MIB14";
    nameMIB[ 4] = "MIB16";
    nameMIB[ 5] = "MIB18";
    nameMIB[ 6] = "MIB27";
    nameMIB[ 7] = "MIB28";
    nameMIB[ 8] = "MIB29";
    nameMIB[ 9] = "MIB96";
    nameMIB[10] = "MIB97";
    nameMIB[11] = "MIB98";
    
    nameMIB[12] = "MIB75";
    nameMIB[13] = "MIB77";

    nameMIB[21] = "MIB100";
    nameMIB[22] = "MIB205";
    
    nameMIB[23] = "MIB90";
    nameMIB[24] = "MIB91";
    nameMIB[25] = "MIB92";
    nameMIB[26] = "MIB93";
    nameMIB[27] = "MIB94";
    nameMIB[28] = "MIB95";
  }
  else { cout << " ERROR! Unknown detector names for tag " << tag << endl; exit(0); }
 
  TString detname;
  
  if      (ndet ==  10) detname = "LaBr";
//else if (ndet == 230) detname = "MIB-B";
//else if (ndet == 231) detname = "MIB-A";
//else if (ndet == 232) detname = "MIB-C";
  else if (ndet == 233) detname = "MIB";
  else if (ndet == 300) detname = "LaBr-MIB";
  else if (ndet <    6)               detname = nameLaBr[ndet];
  else if (ndet >= 200 && ndet < 230) detname = nameMIB [ndet%200];
  else { cout << " ERROR! Unknown detector name for tag: " << tag << " ndet: " << ndet << endl; exit(0); }
  
  return detname;
}

map<UInt_t, TString> GetDetMapLaBr(TString tag = "RAL202305") {

  map<UInt_t, TString> detMap;

  for (UInt_t ndet=0; ndet<6; ndet++)
    detMap[ndet] = GetDetName(tag, ndet);

  return detMap;
}

map<UInt_t, TString> GetDetMapMIB(TString tag = "RAL202305") {

  map<UInt_t, TString> detMap;

  for (UInt_t ndet=200; ndet<228; ndet++)
    detMap[ndet] = GetDetName(tag, ndet);
  
  return detMap;
}

map<UInt_t, TString> GetDetMap(TString tag = "RAL202305") {

  map<UInt_t, TString> detMap;
  map<UInt_t, TString> detMap0 = GetDetMapLaBr(tag);
  map<UInt_t, TString> detMap1 = GetDetMapMIB (tag);
  
  map<UInt_t, TString>::iterator detIt;
  
  for (detIt = detMap0.begin(); detIt != detMap0.end(); detIt++)
    detMap[detIt->first] = detIt->second;

  for (detIt = detMap1.begin(); detIt != detMap1.end(); detIt++)
    detMap[detIt->first] = detIt->second;
  
  return detMap;
}
