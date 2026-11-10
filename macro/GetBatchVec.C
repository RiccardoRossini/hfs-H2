
vector<UInt_t> GetBatchVec(TString period = "RAL202305") {

  vector<UInt_t> batchVec;

  if      (period == "RAL202303") return batchVec;
  else if (period == "RAL202304") {

    batchVec.push_back(11);
    batchVec.push_back(13);
    batchVec.push_back(19);
    batchVec.push_back(21);
    batchVec.push_back(23);
    batchVec.push_back(29);
  }
  else if (period == "RAL202305") {
    
    batchVec.push_back( 9);
    batchVec.push_back(11);
    batchVec.push_back(12);
    batchVec.push_back(14);
    batchVec.push_back(18);
    batchVec.push_back(20);
    batchVec.push_back(22);
    batchVec.push_back(24);
    batchVec.push_back(26);
  }
  else if (period == "RAL202404") {
    
    batchVec.push_back( 1);
    batchVec.push_back( 3);
    batchVec.push_back( 7);
    batchVec.push_back( 9);
    batchVec.push_back(11);
    batchVec.push_back(13);
    batchVec.push_back(15);
  }
  else if (period == "RAL202405") {
    
    batchVec.push_back( 3);
    batchVec.push_back( 5);
    batchVec.push_back( 7);
    batchVec.push_back(13);
    batchVec.push_back(15);
    batchVec.push_back(17);
    batchVec.push_back(19);
    batchVec.push_back(21);
  }  
  else if (period == "RAL202508") {
    
    batchVec.push_back( 7);
    batchVec.push_back(11);
    batchVec.push_back(13);
    batchVec.push_back(17);
    batchVec.push_back(19);
    batchVec.push_back(21);
    batchVec.push_back(23);
    batchVec.push_back(25);
    batchVec.push_back(27);
  }  
  else if (period == "RAL202509") {
    
    batchVec.push_back( 3);
    batchVec.push_back( 5);
    batchVec.push_back( 7);
    batchVec.push_back( 9);
    batchVec.push_back(11);
    batchVec.push_back(13);
    batchVec.push_back(17);
    batchVec.push_back(21);
    batchVec.push_back(23);
    batchVec.push_back(25);
    batchVec.push_back(27);
    batchVec.push_back(29);
    batchVec.push_back(31);
  }  
  else if (period == "hydrogen") {
    
    batchVec.push_back( 6); // in RAL202303 H2 - for 2023
    batchVec.push_back(25); // in RAL202405 H2 - for 2024
    batchVec.push_back(33); // in RAL202509 H2 - for 2025
  } 
  else { cout << " ERROR! Unknown batch vector for period " << period << endl; exit(0); }
 
  return batchVec;
}
