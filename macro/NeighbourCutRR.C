
TEntryList* NeighbourCut(TTree* t, TString lasercut){
    /*
     * NeighbourCut builds a TEntryList selecting events based on laser-neighbour logics.
     *
     * Selection rules:
     *  - all laser events (lphd > 0) are always kept
     *  - no-laser events (lphd == 0) are kept only if at least one neighbouring event (i ± 1) is a "good laser"
     *
     * A "good laser" is defined as:
     *  - lphd > 0
     *  - satisfies the user-defined condition given in 'lasercut'
     *
     * The function does NOT apply any physics cuts (energy, timing, etc.), it returns a mask vector. 
     */

    Long64_t n = t->GetEntries();
    auto list = new TEntryList();
    UInt_t lphd;
    t->SetBranchAddress("lphd", &lphd);
    TTreeFormula cutFormula("cut", lasercut, t);

    // mapping laser events (1 = goodLaser, 0 = noLaser and badLaser)
    std::vector<char> isGood(n, 0);
    for (Long64_t i = 0; i < n; ++i) {
        t->GetEntry(i);
        if (lphd > 0 && cutFormula.EvalInstance()) isGood[i] = 1;
    }

    // mark events i with lphd==0 AND a goodLaser in neighbouring event (i +/- 1)
    for (Long64_t i = 0; i < n; ++i) {

        t->GetEntry(i);

        // keep all laser events
        if (lasercut) {
            list->Enter(i);
            continue;
        }

        // keep only no-laser events neighbouring a good laser
        if ((i > 0     && isGood[i - 1]) ||
            (i < n - 1 && isGood[i + 1])) {
            list->Enter(i);
        }
    }
    return list;
}
