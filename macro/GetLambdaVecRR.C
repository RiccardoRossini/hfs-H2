
vector<UInt_t> GetLambdaVec(TString tag = "RAL202305") {

  vector<UInt_t> lambdaVec;
  
  if (tag == "RAL202304") {

    lambdaVec.push_back( 550);
    lambdaVec.push_back( 650);
    lambdaVec.push_back( 850);
    lambdaVec.push_back( 600);
    lambdaVec.push_back( 750);
    lambdaVec.push_back( 650);
  }
  else if (tag == "RAL202305") {
    
    lambdaVec.push_back( 800);
    lambdaVec.push_back( 700);
    lambdaVec.push_back( 575);
    lambdaVec.push_back( 675);
    lambdaVec.push_back( 775);
    lambdaVec.push_back( 625);
    lambdaVec.push_back( 725);
    lambdaVec.push_back( 825);
    lambdaVec.push_back( 875);
  }
  else if (tag == "RAL202404") {
    
    lambdaVec.push_back( 525);
    lambdaVec.push_back( 500);
    lambdaVec.push_back( 475);
    lambdaVec.push_back( 450);
    lambdaVec.push_back( 425);
    lambdaVec.push_back( 900);
    lambdaVec.push_back( 925);
  }
  else if (tag == "RAL202405") {
    
    lambdaVec.push_back( 925);
    lambdaVec.push_back( 950);
    lambdaVec.push_back( 900);
    lambdaVec.push_back( 975);
    lambdaVec.push_back( 550);
    lambdaVec.push_back(1000);
    lambdaVec.push_back(1025);
    lambdaVec.push_back(1050);
  }  
  else if (tag == "RAL202508") {
    
    lambdaVec.push_back( 400);
    lambdaVec.push_back( 375);
    lambdaVec.push_back( 350);
    lambdaVec.push_back( 325);
    lambdaVec.push_back( 300);
    lambdaVec.push_back( 275);
    lambdaVec.push_back( 250);
    lambdaVec.push_back( 225);
    lambdaVec.push_back( 200);
  }  
  else if (tag == "RAL202509") {
    
    lambdaVec.push_back( 525);
    lambdaVec.push_back( 555);
    lambdaVec.push_back( 585);
    lambdaVec.push_back( 615);
    lambdaVec.push_back( 645);
    lambdaVec.push_back( 675);
    lambdaVec.push_back(1050);
    lambdaVec.push_back(1090);
    lambdaVec.push_back(1140);
    lambdaVec.push_back( 160);
    lambdaVec.push_back( 120);
    lambdaVec.push_back(  80);
    lambdaVec.push_back(  40);
  }  
  else { cout << " ERROR! Unknown lambda vector for tag " << tag << endl; exit(0); }
 
  return lambdaVec;
}
