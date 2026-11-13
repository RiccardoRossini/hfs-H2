
// given a tree, this function extracts average and uncertainty values of target pressure and temperature

void GetPresTemp (  TTree*    t,  // t: tree with the entry list (neighbour cut) already set by the caller
                    Double_t* pres = nullptr,
                    Double_t* errPres = nullptr,
                    Double_t* temp = nullptr,
                    Double_t* errTemp = nullptr) {

    t->Draw("pres:temp", "", "goff");

    Double_t* presArr = t->GetV1();
    Double_t* tempArr = t->GetV2();
    Long64_t N = t->GetSelectedRows();
    
    if (N<2) {
        cout << "GetPresTemp: not enough entries to compute average P, T (N=" << N << ")" << endl;
        if (pres) *pres = 0;
        if (errPres) *errPres = 0;
        if (temp) *temp = 0;
        if (errTemp) *errTemp = 0;
        return;
    }

    *pres = TMath::Mean(N, presArr);
    *errPres = TMath::RMS(N, presArr)/sqrt(N);
    *temp = TMath::Mean(N, tempArr);
    *errTemp = TMath::RMS(N, tempArr)/sqrt(N);
}