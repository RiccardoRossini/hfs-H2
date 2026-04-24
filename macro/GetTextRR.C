
TString GetText0(TString tag = "RAL202305", UInt_t batch = 0) {

  TString text0;

  if      (tag == "RAL202303") text0 = "JUL 2023";
  else if (tag == "RAL202304") text0 = "OCT 2023";
  else if (tag == "RAL202305") text0 = "DEC 2023";
  else if (tag == "RAL202404") text0 = "JUL 2024";
  else if (tag == "RAL202405") text0 = "SEP 2024";
  else if (tag == "RAL202508") text0 = "OCT 2025";
  else if (tag == "RAL202509") text0 = "NOV 2025";
  else { cout << " Unknown operation for tag: " << tag << endl; exit(0); } 

  if (batch) text0 += Form(" Batch %i", batch);
  
  return text0;
}

TString GetText1(TString tag = "RAL202305", UInt_t batch = 3, UInt_t temp = 91, Double_t cO = 1.5) {

  TString text1;

  if      (tag == "RAL202303" && batch ==  6) text1 = "H_{2}";
  else if (tag == "RAL202405" && batch == 25) text1 = "H_{2}";
  else if (tag == "RAL202509" && batch == 33) text1 = "H_{2}";
  else                                        text1 = Form("H_{2} + %0.1f%% O_{2}", cO);
  
  text1 += Form(", %iK", temp);
  
  return text1;
}
