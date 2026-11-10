
TString GetDetCut(UInt_t ndet) {

    cout << "Building detector cut for ndet: " << ndet << endl;
    TString detcut;

    // Particular cases
    if (ndet == 10)         detcut = "x.ndet<6";                                // ALL LaBrs
    else if (ndet == 230)   detcut = "x.ndet>=200&&x.ndet<230";                 // ALL MIBs
    else if (ndet == 300)   detcut = "1";                                       // ALL DETECTORS
    else if (ndet == 299)   detcut = "(x.ndet<6||(x.ndet>=214&&x.ndet<=220))";  // ALL detectors never moved in 2023-25

    // Single detectors
    else if(ndet < 6 || (ndet >=200)&&(ndet<230))  detcut = Form("x.ndet==%i", ndet);

    // Error
    else {
        cout << " GetDetCut ERROR: GetDetCut() --> ndet: " << ndet << " is not valid!" << endl;
        detcut = "";
    }
    
    cout << "Detector cut: " << detcut << endl;
    return detcut;
}