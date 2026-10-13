
TEntryList* NeighbourCut(TTree* t, TString lasercut)
{
    /*
     * Keep:
     *   - all events satisfying lasercut (good laser)
     *   - all no-laser events neighbouring a good-laser event
     */

    Long64_t n = t->GetEntries();

    auto list = new TEntryList();

    TTreeFormula goodLaser("goodLaser", lasercut, t);
    TTreeFormula lphdFormula("lphdFormula", "lphd", t);

    std::vector<char> isGood(n, 0);

    Long64_t g = 0;

    // First pass: identify good laser events
    for (Long64_t i = 0; i < n; ++i) {

        t->LoadTree(i);

        goodLaser.UpdateFormulaLeaves();
        lphdFormula.UpdateFormulaLeaves();

        t->GetEntry(i);

        UInt_t lphd = (UInt_t)lphdFormula.EvalInstance();

        if (lphd > 0 && goodLaser.EvalInstance()) {
            isGood[i] = 1;
            ++g;
        }
    }

    cout << "NeighbourCut: total = " << n
         << ", good laser = " << g << endl;

    Long64_t L = 0;
    Long64_t nL = 0;

    // Second pass
    for (Long64_t i = 0; i < n; ++i) {

        t->LoadTree(i);
        t->GetEntry(i);

        if (isGood[i]) {
            list->Enter(i);
            ++L;
        }
        else if ((i > 0 && isGood[i-1]) ||
                 (i < n-1 && isGood[i+1])) {
            list->Enter(i);
            ++nL;
        }
    }

    cout << "NeighbourCut: selected = " << list->GetN() << endl;
    cout << "NeighbourCut: L = " << L << " (" << L/(double)list->GetN()*100 << "%), nL = " << nL << " (" << nL/(double)list->GetN()*100 << "%)" << endl;

    return list;
}


/*
TEntryList* NeighbourCut(TTree* t, TString lasercut){
    
    // * NeighbourCut builds a TEntryList selecting events based on laser-neighbour logics.
    // * Selection rules:
    // *  - all "good laser" events are always kept
    // *  - no-laser events (lphd == 0) are kept only if at least one neighbouring event (i ± 1) is a "good laser"
    // * Good laser means that "lasercut" is True.
     

    Long64_t n = t->GetEntries();
    auto list = new TEntryList();
    UInt_t lphd;
    t->SetBranchAddress("lphd", &lphd);
    TTreeFormula cutFormula("cut", lasercut, t);

    Long64_t g=0, L=0, nL=0; // counters for good, good-laser, and good-no-laser events

    // mapping laser events (1 = goodLaser, 0 = noLaser and badLaser)
    std::vector<char> isGood(n, 0);
    for (Long64_t i = 0; i < n; ++i) {
        t->GetEntry(i);
        if (lphd > 0 && cutFormula.EvalInstance()){ 
            isGood[i] = 1;
            g++;
        }
    }
    cout << " NeighbourCut: Total events: " << n << ", good laser events: " << g << endl;

    // DEBUG: Print the first 200 values of the isGood vector
    cout << " NeighbourCut check: first 200 values of the isGood vector: ";
    for (Long64_t i = 0; i < std::min(n, 200LL); ++i) {
        cout << static_cast<int>(isGood[i]) << " ";
    }
    cout << endl;
    // END OF DEBUG

    // mark events i with lphd==0 AND a goodLaser in neighbouring event (i +/- 1)
    for (Long64_t i = 0; i < n; ++i) {

        t->GetEntry(i);

        // keep all good laser events
        if (isGood[i]) {
            list->Enter(i);
            L++;
        }

        // keep only no-laser events neighbouring a good laser
        else if ((i>0 && isGood[i-1]) || (i<n-1 && isGood[i+1])) {
            list->Enter(i);
            nL++;
        }
    }

    cout << " NeighbourCut: Total events: " << n << ", selected events: " << list->GetN() << endl;
    cout << " NeighbourCut: L = " << L << ", nL = " << nL << endl;

    t->SetBranchAddress("lphd", nullptr);
//    t->ResetBranchAddresses();

    return list;
}
*/