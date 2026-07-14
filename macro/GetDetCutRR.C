
TString GetDetCut(UInt_t ndet) {

    cout << "Building detector cut for ndet: " << ndet << endl;
    TString detcut;

    // Particular cases
    if (ndet == 10)         detcut = "x.det<6";                             // ALL LaBrs
    else if (ndet == 230)   detcut = "x.det>=200&&x.det<230";               // ALL MIBs
    else if (ndet == 300)   detcut = "";                                    // ALL DETECTORS
    else if (ndet == 299)   detcut = "(x.det<6||(x.det>=214&&x.det<=220))"; // ALL detectors never moved in 2023-25

    // Single detectors
    else if(ndet < 6 || (ndet >=200)&&(ndet<230))  detcut = Form("x.det==%i", ndet);

    // Error
    else {
        cout << " GetDetCut ERROR: GetDetCut() --> ndet: " << ndet << " is not valid!" << endl;
        detcut = "";
    }
    
    cout << "Detector cut: " << detcut << endl;
    return detcut;
}