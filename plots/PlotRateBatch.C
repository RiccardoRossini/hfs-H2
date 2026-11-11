 
/* 
This macro contains two functions: 
  - PlotRateBatch to plot variables from calcratebatch files as a function of the batch, giving those hists as output in a plotratebatch root file
  - DrawFileInfo, utility to put the text on the plots
*/ 

void DrawFileInfo(const TString& filename, Double_t x, Double_t y);

void PlotRateBatch( TString PATH        = "calcratebatch/RV6_lphd0_det300_bsys0_weightw0w1_t0780-2000ns_ene060-200keV_bckgr-H2/",
                    Bool_t  makeNorm    = kFALSE,
                    TString plotPATH    = "plotratebatch/",
                    TString normFile    = "norm.root") {

    /////////////////////////////////////////////////////////////
    /////////////////// PLOT SETUP PARAMETERS ///////////////////
    ///////////////////////////////////////////////////////////////

    const UInt_t nYvarPerFile = 3;
    const UInt_t nXvarPerFile = 2;
    const UInt_t nGraphsPerFile = nYvarPerFile*nXvarPerFile;
    vector<TCanvas*> c(nGraphsPerFile);

    const TString yExpr[nYvarPerFile] = {"s0",    "trigRatio",    "trigS"};
    const TString eExpr[nYvarPerFile] = {"s0Err", "trigRatioErr", "trigSErr"};
    const TString xExpr[nXvarPerFile] = {"lambdaExp", "batch"};
    const TString xTitles[nYvarPerFile] = {   "Wavelength [pm] - 6788000",
                                                "Batch number"};
    const TString yTitles[nYvarPerFile] = {   "Normalised integral ratio wrt. reference",
                                                "Ratio of #mu triggers wrt. reference",
                                                "Triggers of the signal batch (weighted)"};
    for (UInt_t j = 0; j < nGraphsPerFile; ++j) c[j] = new TCanvas(Form("c%d", j), "", 800, 600);

    std::vector<int> markerStyles = {24, 20, 25, 21, 28, 34};
    std::vector<int> colors       = {kBlack, kBlack, kBlack, kBlack, kBlack, kBlack};
    std::vector<TString> periods  = {"RAL202304", "RAL202305", "RAL202404", "RAL202405", "RAL202508", "RAL202509"};
    std::vector<TString> labels   = {"OCT 2023", "DEC 2023", "JUL 2024", "SEP 2024", "OCT 2025", "NOV 2025"};

    ///////////////////////////////////////////////////////////////
    /////////////////// END OF SETUP PARAMETERS ///////////////////
    ///////////////////////////////////////////////////////////////

    if (!PATH.EndsWith("/"))        PATH     += "/";
    if (!plotPATH.EndsWith("/"))    plotPATH += "/";
    
    std::vector<TString> fileNamesVec;
    TSystemDirectory dir("mydir", PATH);
    TList *files = dir.GetListOfFiles();

    if (files) {
        TIter next(files);
        TSystemFile *file;
        cout << "Suitable files found in " << PATH << ":" << endl;
        UInt_t fileCount = 0;

        while ((file = (TSystemFile*)next())) {
            TString fname = file->GetName();

            // skip the norm file and hydrogen batches
            if (fname == normFile || fname.Contains("hydrogen")) continue; 

            if (!file->IsDirectory() && fname.EndsWith(".root")) {
                fileNamesVec.push_back(PATH + fname);
                cout << "\t" << fname << "for period" << periods[fileCount++] << endl;
            }
        }
        if (fileNamesVec.empty()) {
            cout << "No ROOT files found in " << PATH << endl;
        return;
        }
    } else { 
        cout << "Directory " << PATH << " is empty or it does not exist. " << endl; 
        return;
    }

    std::sort(fileNamesVec.begin(), fileNamesVec.end()); // alphabetical order

    const UInt_t nFiles = fileNamesVec.size();

    // Shared legend
    TLegend *leg = new TLegend(0.7, 0.7, 0.9, 0.9);
    leg->SetBorderSize(1);
    leg->SetFillStyle(0);
    leg->SetTextSize(0.03);

    // Setup of normalisation file
    TFile* fNorm = nullptr; 
    TTree* tNorm = nullptr;
    TString  t_Period;
    UInt_t   t_batch;
    Double_t t_norm;
    if(makeNorm){
        fNorm = new TFile(PATH + normFile, "RECREATE");
        tNorm = new TTree("tNorm", "Normalisation coefficients");
        tNorm->Branch("period", &t_Period);
        tNorm->Branch("batch",  &t_batch, "batch/i");
        tNorm->Branch("norm",   &t_norm,  "norm/D");
    } 

    // MAIN LOOP
    UInt_t idxcanvas=0;
    for (UInt_t h = 0; h < nYvarPerFile; ++h) {
        for (UInt_t k = 0; k < nXvarPerFile; ++k){

            for (UInt_t i = 0; i < nFiles; ++i) {

                TFile *f = TFile::Open(fileNamesVec[i]);
                if (!f || f->IsZombie()) {
                    printf("Cannot open %s\n", fileNamesVec[i].Data());
                    continue;
                }

                // Data retrieval (using Draw() with opt. "goff" to avoid plotting)
                TTree *t = (TTree*)f->Get("tRate");
                TGraphErrors *gr = nullptr;
                if (t && t->Draw(Form("%s:%s:%s", xExpr[k].Data(), yExpr[h].Data(), eExpr[h].Data()), "", "goff")){
                    gr = new TGraphErrors(t->GetSelectedRows(), t->GetV1(), t->GetV2(), nullptr, t->GetV3());
                }
                if (!gr) {
                    printf("Missing %s in %s\n", yExpr[h].Data(), fileNamesVec[i].Data());
                    f->Close();
                    continue;
                }
                if (!gr) {
                    printf("Missing %s in %s\n", yExpr[h].Data(), fileNamesVec[i].Data());
                    f->Close();
                    continue;
                }

                gr->SetTitle("");
                gr->SetMarkerStyle(markerStyles[i]);
                gr->SetMarkerColor(colors[i]);
                gr->SetLineColor(colors[i]);
                gr->SetMarkerSize(1.2);
                gr->GetXaxis()->SetTitle(xTitles[k]);
                gr->GetYaxis()->SetTitle(yTitles[h]);
                
                cout << "File " << fileNamesVec[i] << " g" << h << "\t" << gr->GetN() << " data points" << endl; 

                c[idxcanvas]->cd();
                c[idxcanvas]->SetGrid();
                
                
                if (i == 0) { 
                    gr->Draw("AP");
                } else {
                    gr->Draw("P SAME");
                }
                
                gr->GetYaxis()->SetTitleSize(0.04);
                // move the plot up-right
                c[idxcanvas]->SetLeftMargin(0.15);
                c[idxcanvas]->SetRightMargin(0.05);
                c[idxcanvas]->SetTopMargin(0.05);
                c[idxcanvas]->SetBottomMargin(0.15);
                // move the legend accordingly 
                leg->SetX1NDC(0.75);
                leg->SetY1NDC(0.75);
                leg->SetX2NDC(0.95);
                leg->SetY2NDC(0.95);
            
                if (h == 0){ 
                    if (k == 0) leg->AddEntry(gr, labels[i], "p");
                    gr->GetYaxis()->SetRangeUser(0.5, 1.6);     // Y range  
                } else if (h == 1) {
                    gr->GetYaxis()->SetRangeUser(0., 2.);       // Y range  
                } else if (h == 2) {
                    gr->GetYaxis()->SetRangeUser(0., 3.5E6);    // Y range
                }

                if (k == 0){
                    gr->GetXaxis()->SetLimits(-50, 1200);       // X range
                } else if (k == 1){
                    gr->GetXaxis()->SetLimits(-1, 40);       // X range
                }

                // save norm factors if makeNorm=True, y=s0, and x=batch
                if (makeNorm && h==0 && k==1){
                    // put x and y in the norm vectors! 
                    vector<UInt_t> batchVec = GetBatchVec(periods[i]);
                    for (UInt_t ii=0; ii<batchVec.size(); ++ii) {
                        if (batchVec[ii] != (UInt_t)gr->GetX()[ii]) {
                            cerr << "ERROR: batch mismatch, batchVec[" << ii << "] = " << batchVec[ii] << ", gr->GetX()[" << ii << "] = " << gr->GetX()[ii] << endl;
                            throw std::runtime_error("Batch mismatch");
                        }
                        if (gr->GetY()[ii] == 0) {
                            cerr << "ERROR: Null normalisation factor, gr->GetY()[" << ii << "] = 0" << endl;
                            throw std::runtime_error("Null normalisation factor");
                        }
                        t_Period = periods[i];
                        t_batch  = batchVec[ii];
                        t_norm   = 1/gr->GetY()[ii];
                        tNorm->Fill();
                    }
                }

                f->Close();
            }


            c[idxcanvas]->cd();
            leg->Draw();
            c[idxcanvas]->Update();

            idxcanvas++;
        }
    }

    // 
    if (makeNorm){
        fNorm->cd();
        tNorm->Write("", TObject::kOverwrite);
        fNorm->Write("", TObject::kOverwrite);
        fNorm->Close();
    }

    // Draw text and horizontal lines at y=1 on all canvases
    idxcanvas=0;
    for (UInt_t h = 0; h < nYvarPerFile; ++h) {
        for (UInt_t k = 0; k < nXvarPerFile; ++k){

            c[idxcanvas]->cd();
            DrawFileInfo(fileNamesVec[0], 0.18, 0.92);
            c[idxcanvas]->Update(); // ensure axes are set
                
                        
            TLine* line = new TLine(c[idxcanvas]->GetUxmin(), 1, c[idxcanvas]->GetUxmax(), 1);
            line->SetLineColor(kRed);
            line->SetLineStyle(2); // dashed
            line->SetLineWidth(2);
            if (h!=2) line->Draw("same"); // no line for trigS
            c[idxcanvas]->Update();

            idxcanvas++;
        }
    }
    
    // === Save 
    TString baseName = gSystem->BaseName(fileNamesVec[0]);
    baseName.ReplaceAll("calcratebatch_RAL202304-", "");    // remove prefix
    baseName.ReplaceAll(".root", "");             // remove extension
    TString outName, outFullName;
    idxcanvas=0;
    for (UInt_t h = 0; h < nYvarPerFile; ++h) {
        for (UInt_t k = 0; k < nXvarPerFile; ++k){

            outName = Form("plotratebatch_%s-%s_%s", baseName.Data(), xExpr[k].Data(), yExpr[h].Data());
            outFullName = plotPATH + outName + ".png";
            c[idxcanvas]->SaveAs(outFullName);

            outName.ReplaceAll("-", "_"); // remove "-" in the .C version (or it does not work)
            outFullName = plotPATH + outName + ".C";
            c[idxcanvas]->SaveAs(outFullName);

            idxcanvas++;
        }
    }

}

/////////////////////////////////////////

void DrawFileInfo(const TString& filename, Double_t x = 0.12, Double_t y = 0.88)
{
    // --- Extract weight ---
    TString weight = "";
    Ssiz_t posW = filename.Index("weight");
    if (posW != kNPOS) {
        Ssiz_t end = filename.Index("_", posW);
        if (end == kNPOS) end = filename.Length();
        weight = filename(posW + 6, end - (posW + 6));   // salta "weight" (6 caratteri)
    }

    // --- Extract reference ---
    TString ref = "";
    Ssiz_t posR = filename.Index("ref");
    if (posR != kNPOS)
        ref = filename(posR + 3, 9);  // RAL202304

    // --- Extract batch ---
    TString batch = "";
    Ssiz_t posB = filename.Index("-b");
    if (posB != kNPOS) {
        Ssiz_t end = filename.Index(".root", posB);
        batch = filename(posB + 2, end - (posB + 2)); // 11
    }
    
    // --- Extract t-range ---
    TString trange = "";
    Ssiz_t posT = filename.Index("_t");
    if (posT != kNPOS) {
        Ssiz_t end = filename.Index("_", posT + 2);
        if (end == kNPOS) end = filename.Length();
        trange = filename(posT + 2, end - (posT + 2)); // es. 0780-2000ns
        trange.ReplaceAll("ns", "");                    // rimuove "ns"

        Ssiz_t dash = trange.Index("-");
        if (dash != kNPOS) {
            TString tmin_str = trange(0, dash);          // TSubString → TString
            TString tmax_str = trange(dash + 1, trange.Length() - dash - 1);

            UInt_t tmin = tmin_str.Atoi();                  // converte 0780 → 780
            UInt_t tmax = tmax_str.Atoi();
            //trange = Form(", t #in [%d, %d] ns", tmin, tmax);
            trange = Form("[%d, %d] ns", tmin, tmax);
        } else trange = "";
    }

    // --- Extract E-range ---
    TString erange = "";
    Ssiz_t posE = filename.Index("_ene");
    if (posE != kNPOS) {
        Ssiz_t end = filename.Index("keV", posE);
        if (end != kNPOS) {
            TString e = filename(posE + 4, end - (posE + 4));          // "100-200"
            Ssiz_t dash = e.Index("-");
            if (dash != kNPOS) {
                Int_t e0 = TString(e(0, dash)).Atoi();
                Int_t e1 = TString(e(dash + 1, e.Length() - dash - 1)).Atoi();
                erange = Form("[%d, %d] keV", e0, e1);
            }
        }
    }

    // --- Extract number of data processing ---
    TString rv = "";
    Ssiz_t posRV = filename.Index("-RV");
    if (posRV != kNPOS) {
        Ssiz_t end = filename.Index("_", posRV);
        if (end == kNPOS) end = filename.Length();
        rv = filename(posRV + 1, end - posRV -1);
    }

    // --- Extract detector number ---
    UInt_t det = 0;
    Ssiz_t posDet = filename.Index("_ndet");
    if (posDet != kNPOS) {
        Ssiz_t end = filename.Index("_", posDet + 5);
        if (end == kNPOS) end = filename.Length();
        det = (UInt_t)TString(filename(posDet + 5, end - (posDet + 5))).Atoi();
    }
    

    // --- Final text ---
    TString line0 = "#bf{FAMU} Internal";
    //TString line2 = weight + trange;
    TString line1 = GetDetName("RAL202304", det);
    TString line2 = "weights: " + weight + "," + trange + " #times " + erange;
    TString line3;
    if (ref == "RAL202303" && batch == "06") line3 = "Reference H_{2}";
    else line3 = Form("Reference %s batch %s", ref.Data(), batch.Data());
    line3 = line3 + " - " + rv;

    // --- Draw ---
    TLatex latex;
    latex.SetNDC();
    latex.SetTextAlign(13); // left-top
//    latex.SetTextSize(textSize);
    latex.SetTextFont(43);  // Helvetica, pixel-precise
    latex.SetTextSize(22);  // in pixel

    latex.DrawLatex(x, y,        line0);
    latex.DrawLatex(x, y - 0.05, line1);
    latex.DrawLatex(x, y - 0.10, line2);
    latex.DrawLatex(x, y - 0.15, line3);
}
