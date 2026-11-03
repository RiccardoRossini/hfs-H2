#include "/storage/gpfs_data/famu/riccardo/rootStyle/atlasstyle-00-04-02/AtlasStyle.C"
#include "/storage/gpfs_data/famu/riccardo/rootStyle/atlasstyle-00-04-02/AtlasUtils.C"
#include "/storage/gpfs_data/famu/riccardo/rootStyle/atlasstyle-00-04-02/AtlasLabels.C"
#include "/storage/gpfs_data/famu/riccardo/rootStyle/atlasstyle-00-04-02/FAMULabels.C"
#include "/storage/gpfs_data/famu/riccardo/analysis/hfs-H2/macro/GetDetNameRR.C"
 
/* this macro contains two functions: 
  - PlotRateBatch to plot variables from calcratebatch files as a function of the batch, giving those hists as output in a plotratebatch root file
  - c0c1_PlotRateBatch, old version which extracts plots from the canvases and not from the tree [DEPRECATED but to be used for vers <= 20261020]
  - ImportPlotRatio to plot the ratio between two histograms from different plotratebatch root files

  - DrawFileInfo, utility to put the text on the plots
*/ 

void DrawFileInfo(const TString& filename, double x, double y);

void PlotRateBatch(TString PATH = "calcratebatch/RV6_lphd0_det300_bsys0_weightw0w1_t0780-2000ns_ene060-200keV_bckgr-H2/") {

    const UInt_t nYvarPerFile = 3;
    const UInt_t nXvarPerFile = 2;
    const UInt_t nGraphsPerFile = nYvarPerFile*nXvarPerFile;
    vector<TCanvas*> c(nGraphsPerFile);

//    const TString names[nGraphsPerFile] = {"s0",    "trigRatio",    "trigS"};
    const TString yExpr[nYvarPerFile] = {"s0",    "trigRatio",    "trigS"};
    const TString eExpr[nYvarPerFile] = {"s0Err", "trigRatioErr", "trigSErr"};
    const TString xExpr[nXvarPerFile] = {"lambdaExp", "batch"};
    const TString xTitles[nYvarPerFile] = {   "Wavelength [pm] - 6788000",
                                                "Batch number"};
    const TString yTitles[nYvarPerFile] = {   "Normalised integral ratio wrt. reference",
                                                "Ratio of #mu triggers wrt. reference",
                                                "Triggers of the signal batch (weighted)"};
    for (int j = 0; j < nGraphsPerFile; ++j) c[j] = new TCanvas(Form("c%d", j), "", 800, 600);

    if (!PATH.EndsWith("/")) PATH += "/";
    
    std::vector<TString> fileNamesVec;
    TSystemDirectory dir("mydir", PATH);
    TList *files = dir.GetListOfFiles();

    if (files) {
        TIter next(files);
        TSystemFile *file;

        while ((file = (TSystemFile*)next())) {
            TString fname = file->GetName();

            if (!file->IsDirectory() && fname.EndsWith(".root")) {
                fileNamesVec.push_back(PATH + fname);
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

    const int nFiles = fileNamesVec.size();
    std::vector<int> markerStyles = {24, 20, 25, 21, 28, 34};
    std::vector<int> colors       = {kBlack, kBlack, kBlack, kBlack, kBlack, kBlack};
    std::vector<TString> labels   = {"OCT 2023", "DEC 2023", "JUL 2024", "SEP 2024", "OCT 2025", "NOV 2025"};

    // Shared legend
    TLegend *leg = new TLegend(0.7, 0.7, 0.9, 0.9);
    leg->SetBorderSize(1);
    leg->SetFillStyle(0);
    leg->SetTextSize(0.03);

    // === Loop over graph types (g0, g1, g2) ===
    UInt_t idxcanvas=0;
    for (int h = 0; h < nYvarPerFile; ++h) {
        for (int k = 0; k < nXvarPerFile; ++k){

            for (int i = 0; i < nFiles; ++i) {

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

                f->Close();
            }


            c[idxcanvas]->cd();
            leg->Draw();
            c[idxcanvas]->Update();

            idxcanvas++;
        }
    }

        // Draw text and horizontal lines at y=1 on all canvases
idxcanvas=0;
    for (int h = 0; h < nYvarPerFile; ++h) {
        for (int k = 0; k < nXvarPerFile; ++k){

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
    
    // === Save canvases and histograms in the same path as the files ===
    TString baseName = gSystem->BaseName(fileNamesVec[0]);
    baseName.ReplaceAll("calcratebatch_RAL202304-", "");    // remove prefix
    baseName.ReplaceAll(".root", "");             // remove extension

    TString outName;

    idxcanvas=0;
    for (int h = 0; h < nYvarPerFile; ++h) {
        for (int k = 0; k < nXvarPerFile; ++k){
            outName = Form("plotratebatch/plotratebatch_%s-%s_%s.C", baseName.Data(), xExpr[k].Data(), yExpr[h].Data());
            outName.ReplaceAll("-", "_"); // remove dashes in the .C version (otherwise the function has a bad name)
            c[idxcanvas]->SaveAs(outName);
            cout << "Saved " << outName << endl;
            outName = Form("plotratebatch/plotratebatch_%s-%s_%s.png", baseName.Data(), xExpr[k].Data(), yExpr[h].Data());
            c[idxcanvas]->SaveAs(outName);
            cout << "Saved " << outName << endl;
            idxcanvas++;
        }
    }

}



// old version (kept as it has to be used for files produced before 20/10/2026)
void c0c1_PlotRateBatch(TString PATH = "calcratebatch/RV6_lphd0_det300_bsys0_weightw0w1_t0780-2000ns_ene060-200keV_bckgr-H2/") {

    if (!PATH.EndsWith("/")) PATH += "/";
    
    std::vector<TString> fileNamesVec;
    TSystemDirectory dir("mydir", PATH);
    TList *files = dir.GetListOfFiles();

    if (files) {
        TIter next(files);
        TSystemFile *file;

        while ((file = (TSystemFile*)next())) {
            TString fname = file->GetName();

            if (!file->IsDirectory() && fname.EndsWith(".root")) {
                fileNamesVec.push_back(PATH + fname);
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

    const int nFiles = fileNamesVec.size();
    const int nGraphsPerFile = 2;
    std::vector<int> markerStyles = {24, 20, 25, 21, 28, 34};
    std::vector<int> colors       = {kBlack, kBlack, kBlack, kBlack, kBlack, kBlack};
    std::vector<TString> labels   = {"OCT 2023", "DEC 2023", "JUL 2024", "SEP 2024", "OCT 2025", "NOV 2025"};

    // Create one canvas per graph type (each with unique name!)
    std::vector<TCanvas*> c(nGraphsPerFile);
    for (int j = 0; j < nGraphsPerFile; ++j)
        c[j] = new TCanvas(Form("c%d", j), Form("Graph g%d", j), 800, 600);

    // Shared legend
    TLegend *leg = new TLegend(0.7, 0.7, 0.9, 0.9);
    leg->SetBorderSize(1);
    leg->SetFillStyle(0);
    leg->SetTextSize(0.03);

    // === Loop over graph types (g0, g1, g2) ===
    for (int j = 0; j < nGraphsPerFile; ++j) {

        for (int i = 0; i < nFiles; ++i) {

            TFile *f = TFile::Open(fileNamesVec[i]);
            if (!f || f->IsZombie()) {
                printf("Cannot open %s\n", fileNamesVec[i].Data());
                continue;
            }

            TString graphName = Form("g%d", j);
            TGraphErrors *gr = (TGraphErrors*)f->Get(graphName)->Clone();

            if (!gr) {
                printf("Missing %s in %s\n", graphName.Data(), fileNamesVec[i].Data());
                f->Close();
                continue;
            }

            gr->SetTitle("");
            gr->SetMarkerStyle(markerStyles[i]);
            gr->SetMarkerColor(colors[i]);
            gr->SetLineColor(colors[i]);
            gr->SetMarkerSize(1.2);
            
            cout << "File " << fileNamesVec[i] << " g" << j << "\t" << gr->GetN() << " data points" << endl; 

            c[j]->cd();
            c[j]->SetGrid();
            
            
            if (i == 0) { 
                gr->Draw("AP");
            } else {
                gr->Draw("P SAME");
            }
            
            gr->GetXaxis()->SetLimits(-50, 1200);     // X range
            gr->GetYaxis()->SetTitleSize(0.04);
            // move the plot up-right
            c[j]->SetLeftMargin(0.15);
            c[j]->SetRightMargin(0.05);
            c[j]->SetTopMargin(0.05);
            c[j]->SetBottomMargin(0.15);
            // move the legend accordingly 
            leg->SetX1NDC(0.75);
            leg->SetY1NDC(0.75);
            leg->SetX2NDC(0.95);
            leg->SetY2NDC(0.95);
           
            if (j == 0){ 
                leg->AddEntry(gr, labels[i], "p");
                gr->GetYaxis()->SetRangeUser(0.5, 1.6);   // Y range  
            } else {
                gr->GetYaxis()->SetRangeUser(0., 2.);   // Y range  
            }

            f->Close();
        }

        c[j]->cd();
        leg->Draw();
        c[j]->Update();
    }

        // Draw horizontal lines at y=1 on all canvases after all graphs are drawn
    for (int j = 0; j < nGraphsPerFile; j++) {
        c[j]->cd();
        DrawFileInfo(fileNamesVec[0], 0.18, 0.92);
        c[j]->Update(); // ensure axes are set
            
                    
        TLine* line = new TLine(c[j]->GetUxmin(), 1, c[j]->GetUxmax(), 1);
        line->SetLineColor(kRed);
        line->SetLineStyle(2); // dashed
        line->SetLineWidth(2);
        line->Draw("same");
        c[j]->Update();
    }
    
    // === Save canvases and histograms in the same path as the files ===
    TString baseName = gSystem->BaseName(fileNamesVec[0]);
    baseName.ReplaceAll("calcratebatch_", "");    // remove prefix
    baseName.ReplaceAll(".root", "");             // remove extension

    TString outName;

    for (int j = 0; j < nGraphsPerFile; ++j) {
        outName = Form("plotratebatch/plotratebatch_%s-c%d.C", baseName.Data(), j);
        c[j]->SaveAs(outName);
        cout << "Saved " << outName << endl;
        outName = Form("plotratebatch/plotratebatch_%s-c%d.png", baseName.Data(), j);
        c[j]->SaveAs(outName);
        cout << "Saved " << outName << endl;
    }

}

/////////////////////////////////////////

void PlotRatioFromCanvases(TCanvas* cNum, TCanvas* cDen, TString canvasName="cRatio") 
{
    if (!cNum || !cDen) return;

    // --- Recupera tutti i TGraphErrors da ciascun canvas ---
    std::vector<TGraphErrors*> graphsNum;
    std::vector<TGraphErrors*> graphsDen;

    TObject* obj;
    TIter next(cNum->GetListOfPrimitives());
    while ((obj = (TObject*)next())) {
        TGraphErrors* g = dynamic_cast<TGraphErrors*>(obj);
        if (g) graphsNum.push_back((TGraphErrors*)g->Clone());
    }

    next = TIter(cDen->GetListOfPrimitives());
    while ((obj = (TObject*)next())) {
        TGraphErrors* g = dynamic_cast<TGraphErrors*>(obj);
        if (g) graphsDen.push_back((TGraphErrors*)g->Clone());
    }

    int nGraphs = std::min(graphsNum.size(), graphsDen.size());
    if (nGraphs == 0) {
        printf("No TGraphErrors found in canvases.\n");
        return;
    }

    // --- Crea un nuovo canvas per i rapporti ---
    TCanvas* cRatio = new TCanvas(canvasName, "Ratio Num/Den", 800, 600);
    cRatio->SetGrid();
    cRatio->SetLeftMargin(0.15);
    cRatio->SetRightMargin(0.05);
    cRatio->SetTopMargin(0.05);
    cRatio->SetBottomMargin(0.15);

    // --- Colori e marker (modifica come vuoi) ---
    std::vector<int> colors = {kBlue, kRed, kGreen+2, kMagenta, kOrange, kCyan};
    std::vector<int> markers = {20, 21, 22, 23, 24, 25};

    TLegend* leg = new TLegend(0.7,0.7,0.9,0.9);
    leg->SetBorderSize(1);
    leg->SetFillStyle(0);
    leg->SetTextSize(0.03);

    // --- Loop sui grafici e costruzione del rapporto ---
    for (int i=0; i<nGraphs; ++i) {
        TGraphErrors* gNum = graphsNum[i];
        TGraphErrors* gDen = graphsDen[i];

        TGraphErrors* gRatio = new TGraphErrors();
        int nPoints = std::min(gNum->GetN(), gDen->GetN());

        for (int j=0; j<nPoints; ++j) {
            double xNum, yNum, xDen, yDen;
            gNum->GetPoint(j, xNum, yNum);
            gDen->GetPoint(j, xDen, yDen);

            // --- Salta mismatch x (o interpolare) ---
            if (TMath::Abs(xNum - xDen) > 1e-6) continue;

            double ex = gNum->GetErrorX(j);
            double ey = (yDen != 0) ? (yNum/yDen)*sqrt(pow(gNum->GetErrorY(j)/yNum,2.)+pow(gDen->GetErrorY(j)/yDen,2.)) : 0;

            gRatio->SetPoint(j, xNum, (yDen!=0)?yNum/yDen:0);
            gRatio->SetPointError(j, ex, ey);
        }

        gRatio->SetMarkerStyle(markers[i%markers.size()]);
        gRatio->SetMarkerColor(colors[i%colors.size()]);
        gRatio->SetLineColor(colors[i%colors.size()]);
        gRatio->SetMarkerSize(1.2);

        gRatio->Draw(i==0 ? "AP" : "P SAME");

        leg->AddEntry(gRatio, Form("Ratio %d", i+1), "p");
    }

    leg->Draw();

    // --- Linea orizzontale y=1 ---
    TLine* line = new TLine(cRatio->GetUxmin(),1,cRatio->GetUxmax(),1);
    line->SetLineColor(kRed);
    line->SetLineStyle(2);
    line->SetLineWidth(2);
    line->Draw("same");

    cRatio->Update();
}

/////////////////////////////////////////



void DrawFileInfo(const TString& filename, double x = 0.12, double y = 0.88)
{
    // --- Extract weight ---
    TString weight = "";
    Ssiz_t posW = filename.Index("weight");
    if (posW != kNPOS) {
        Ssiz_t end = filename.Index("_", posW);
        if (end == kNPOS) end = filename.Length();
        weight = filename(posW + 6, end - (posW + 6));   // salta "weight" (6 caratteri)
    }
/*    TString weight = "";
    Ssiz_t posW = filename.Index("weight ");
    if (posW != kNPOS) {
        Ssiz_t end = filename.Index("_", posW);
        weight = filename(posW, end - posW);   // weightw0
        weight.ReplaceAll("weight", "weight ");
    }*/

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

            int tmin = tmin_str.Atoi();                  // converte 0780 → 780
            int tmax = tmax_str.Atoi();
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
                int e0 = TString(e(0, dash)).Atoi();
                int e1 = TString(e(dash + 1, e.Length() - dash - 1)).Atoi();
                erange = Form("[%d, %d] keV", e0, e1);
            }
        }
    }

/*    TString trange = "";
    Ssiz_t posT = filename.Index("_t");
    if (posT != kNPOS) {
        Ssiz_t end = filename.Index("_", posT + 2);
        if (end == kNPOS) end = filename.Length();
        trange = filename(posT + 2, end - (posT + 2)); // es. 0780-2000ns
        trange.ReplaceAll("ns","");
        Ssiz_t dash = trange.Index("-");
        if (dash != kNPOS) {
            //TString tmin = trange(0, dash);
            //TString tmax = trange(dash + 1, trange.Length() - dash - 1);
            int tmin = trange(0, dash).Atoi();  // converte 0780 → 780
            int tmax = trange(dash + 1, trange.Length() - dash - 1).Atoi();
            trange = Form(", t \\in [%s, %s] ns", tmin.Data(), tmax.Data());
        } else trange = "";
    }
*/    
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


/* void plot_calcratebatch() {


    const int nFiles = 5;
    const int nGraphsPerFile = 3;

    TString fileNames[nFiles] = {
        "calcratebatch/calcratebatch_RAL202304_repro4_ndet300_LaBr-MIB.root",
        "calcratebatch/calcratebatch_RAL202305_repro4_ndet300_LaBr-MIB.root",
        "calcratebatch/calcratebatch_RAL202404_repro4_ndet300_LaBr-MIB.root",
        "calcratebatch/calcratebatch_RAL202405_repro4_ndet300_LaBr-MIB.root",
        "calcratebatch/calcratebatch_RAL202508_repro4_ndet300_LaBr-MIB.root"
    };

    // Marker styles
    int markerStyles[nFiles] = {24, 20, 25, 21, 28};
    int colors[nFiles]       = {kBlack, kBlack, kBlack, kBlack, kBlack};
    TString labels[nFiles]   = {"OCT 2023", "DEC 2023", "JUL 2024", "SEP 2024", "OCT 2025"};

    // Canvas
    TCanvas *c0 = new TCanvas("c0", "c0", 1000, 800);
    TCanvas *c1 = new TCanvas("c0", "c0", 1000, 800);
    TCanvas *c2 = new TCanvas("c0", "c0", 1000, 800);

    // Legend 
    TLegend *leg = new TLegend(0.7, 0.7, 0.9, 0.9);
    leg->SetBorderSize(0);
    leg->SetFillStyle(0);
    leg->SetTextSize(0.03);

     // Flags for first draw
    bool first_g0 = true, first_g1 = true, first_g2 = true;

    // === Loop over files ===
    for (int i = 0; i < nFiles; ++i) {

        TFile *f = TFile::Open(fileNames[i]);
        if (!f || f->IsZombie()) {
            printf("Error: could not open %s\n", fileNames[i].Data());
            continue;
        }

        // --- g0 ---
        {
            TGraphErrors *gr = (TGraphErrors*)f->Get("g0");
            if (gr) {
                gr->SetMarkerStyle(markerStyles[i]);
                gr->SetMarkerColor(colors[i]);
                gr->SetLineColor(colors[i]);
                gr->SetMarkerSize(1.2);

                c0->cd();
                if (first_g0) {
                    gr->Draw("AP");
                    gr->GetXaxis()->SetTitle("X axis");
                    gr->GetYaxis()->SetTitle("Y axis");
                    first_g0 = false;
                } else {
                    gr->Draw("P SAME");
                }

                // Add to legend only once (from g0)
                leg->AddEntry(gr, labels[i], "p");
            } else {
                printf("Warning: g0 not found in %s\n", fileNames[i].Data());
            }
        }

        // --- g1 ---
        {
            TGraphErrors *gr = (TGraphErrors*)f->Get("g1");
            if (gr) {
                gr->SetMarkerStyle(markerStyles[i]);
                gr->SetMarkerColor(colors[i]);
                gr->SetLineColor(colors[i]);
                gr->SetMarkerSize(1.2);

                c1->cd();
                if (first_g1) {
                    gr->Draw("AP");
                    gr->GetXaxis()->SetTitle("X axis");
                    gr->GetYaxis()->SetTitle("Y axis");
                    first_g1 = false;
                } else {
                    gr->Draw("P SAME");
                }
            }
        }

        // --- g2 ---
        {
            TGraphErrors *gr = (TGraphErrors*)f->Get("g2");
            if (gr) {
                gr->SetMarkerStyle(markerStyles[i]);
                gr->SetMarkerColor(colors[i]);
                gr->SetLineColor(colors[i]);
                gr->SetMarkerSize(1.2);

                c2->cd();
                if (first_g2) {
                    gr->Draw("AP");
                    gr->GetXaxis()->SetTitle("X axis");
                    gr->GetYaxis()->SetTitle("Y axis");
                    first_g2 = false;
                } else {
                    gr->Draw("P SAME");
                }
            }
        }

        f->Close();
    }

    // Draw the same legend on all canvases (or only on the first if you prefer)
    c0->cd(); leg->Draw();
    c1->cd(); leg->Draw();
    c2->cd(); leg->Draw();

    // Update canvases
    c0->Update();
    c1->Update();
    c2->Update();
}
    
    
    ---
    bool first = true;

    // Loop sui file
    for (int i = 0; i < nFiles; ++i) {
        TFile *f = TFile::Open(fileNames[i]);
        if (!f || f->IsZombie()) {
            printf("Errore: non posso aprire %s\n", fileNames[i].Data());
            continue;
        }

        // Loop sui 3 grafici (g0, g1, g2)
        for (int j = 0; j < nGraphsPerFile; ++j) {
            TString graphName = Form("g%d", j);
            TGraphErrors *gr = (TGraphErrors*)f->Get(graphName);

            if (!gr) {
                printf("Attenzione: %s non trovato in %s\n", graphName.Data(), fileNames[i].Data());
                continue;
            }

            // Stile
            gr->SetMarkerStyle(markerStyles[i]);
            gr->SetMarkerColor(colors[i]);
            gr->SetLineColor(colors[i]);
            gr->SetMarkerSize(1.2);

            // Disegno
            if (first) {
                gr->Draw("AP");
                gr->GetXaxis()->SetTitle("X axis");
                gr->GetYaxis()->SetTitle("Y axis");
                first = false;
            } else {
                gr->Draw("P SAME");
            }

            // Aggiungo alla legenda solo il primo grafico del file
            if (j == 0) {
                leg->AddEntry(gr, fileNames[i].Data(), "p");
            }
        }

        f->Close();
    }

    leg->Draw();
    c->Update();
}
*/
