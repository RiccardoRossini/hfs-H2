#include <TCanvas.h>
#include <TGraphErrors.h>
#include <TLegend.h>
#include <TLine.h>
#include <TROOT.h>
#include <TSystem.h>
#include <TList.h>
#include <TObject.h>

#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm>

/*
This macro takes two canvases saved in .C (outputs of PlotRateBatch), and returns the ratio
of the values on the y axis, graph by graph (graph i of file1 / graph i of file2).
Both files must be of the same kind (both "lambda" or both "batch").
*/

TCanvas* RatioFromCFiles(
    TString file1 = "plotratebatch/file1.C",
    TString file2 = "plotratebatch/file2.C",
    TString saveName = "ratio",
    Double_t tolerance = 1e-9
) {
    // ---- full paths ----
    TString fullPath = gSystem->WorkingDirectory();
    fullPath += "/";
    TString file1path = fullPath + file1;
    TString file2path = fullPath + file2;

    if (gROOT->LoadMacro(file1path) < 0) {
        std::cerr << "ERROR: Cannot load " << file1path << std::endl;
        return nullptr;
    }
    if (gROOT->LoadMacro(file2path) < 0) {
        std::cerr << "ERROR: Cannot load " << file2path << std::endl;
        return nullptr;
    }

    // names of the functions defined by the saved macros
    TString func1 = gSystem->BaseName(file1);
    TString func2 = gSystem->BaseName(file2);
    func1.ReplaceAll(".C", "");
    func2.ReplaceAll(".C", "");

    // ---- clones the graphs of a canvas, so they survive when the canvas is closed ----
    auto GetGraphs = [](TCanvas* c) {
        std::vector<TGraphErrors*> graphs;
        TIter next(c->GetListOfPrimitives());
        TObject* obj;
        while ((obj = next()))
            if (obj->InheritsFrom(TGraphErrors::Class()))
                graphs.push_back((TGraphErrors*)obj->Clone());
        return graphs;
    };

    // ---- run the two macros, copy the graphs, close each canvas right away ----
    gROOT->ProcessLine(func1 + "();");
    TCanvas* c1 = gPad ? gPad->GetCanvas() : nullptr;
    if (!c1) { std::cerr << "ERROR: no canvas from " << file1 << std::endl; return nullptr; }
    std::vector<TGraphErrors*> graphs1 = GetGraphs(c1);
    delete c1;                                   // frees the canvas name before the second macro runs

    gROOT->ProcessLine(func2 + "();");
    TCanvas* c2 = gPad ? gPad->GetCanvas() : nullptr;
    if (!c2) { std::cerr << "ERROR: no canvas from " << file2 << std::endl; return nullptr; }
    std::vector<TGraphErrors*> graphs2 = GetGraphs(c2);
    delete c2;

    std::cout << "Graphs found: " << graphs1.size() << " and " << graphs2.size() << std::endl;

    // ---- checks: same number of graphs, same number of points, same x ----
    if (graphs1.empty() || graphs1.size() != graphs2.size()) {
        std::cerr << "ERROR: different (or zero) number of graphs: "
                  << graphs1.size() << " vs " << graphs2.size() << std::endl;
        return nullptr;
    }
    for (size_t ig = 0; ig < graphs1.size(); ++ig) {
        TGraphErrors* g1 = graphs1[ig];
        TGraphErrors* g2 = graphs2[ig];
        if (g1->GetN() != g2->GetN()) {
            std::cerr << "ERROR: different number of points in graph " << ig << std::endl;
            return nullptr;
        }
        for (int i = 0; i < g1->GetN(); ++i) {
            Double_t x1, y1, x2, y2;
            g1->GetPoint(i, x1, y1);
            g2->GetPoint(i, x2, y2);
            if (std::abs(x1 - x2) > tolerance * std::max(1.0, std::abs(x1))) {
                std::cerr << "ERROR: X mismatch in graph " << ig << ", point " << i
                          << ": " << x1 << " vs " << x2 << std::endl;
                return nullptr;
            }
        }
    }

    // ---- ratios ----
    std::vector<TGraphErrors*> ratioGraphs;
    Double_t ymin = 1e9, ymax = -1e9;

    for (size_t ig = 0; ig < graphs1.size(); ++ig) {
        TGraphErrors* g1 = graphs1[ig];
        TGraphErrors* g2 = graphs2[ig];
        int n = g1->GetN();

        TGraphErrors* gr = new TGraphErrors(n);
        gr->SetName(Form("RatioGraph%zu", ig));
        gr->SetTitle("");
        gr->SetMarkerStyle(g1->GetMarkerStyle());
        gr->SetMarkerColor(g1->GetMarkerColor());
        gr->SetMarkerSize(g1->GetMarkerSize());
        gr->SetLineColor(g1->GetLineColor());

        for (int i = 0; i < n; ++i) {
            Double_t x1, y1, x2, y2;
            g1->GetPoint(i, x1, y1);
            g2->GetPoint(i, x2, y2);

            if (y2 == 0) {
                std::cerr << "ERROR: zero denominator in graph " << ig << ", point " << i << std::endl;
                return nullptr;
            }

            Double_t e1 = g1->GetErrorY(i);
            Double_t e2 = g2->GetErrorY(i);
            Double_t ratio = y1 / y2;
            Double_t error = (y1 != 0)
                ? std::abs(ratio) * std::sqrt(std::pow(e1 / y1, 2.) + std::pow(e2 / y2, 2.))
                : 0.;

            gr->SetPoint(i, x1, ratio);
            gr->SetPointError(i, 0, error);

            ymin = std::min(ymin, ratio - error);
            ymax = std::max(ymax, ratio + error);
        }
        ratioGraphs.push_back(gr);
    }

    // ---- output canvas ----
    TCanvas* cR = new TCanvas("cR", "Ratio", 800, 600);
    cR->SetGrid();
    cR->SetLeftMargin(0.15);
    cR->SetRightMargin(0.05);
    cR->SetTopMargin(0.05);
    cR->SetBottomMargin(0.15);

    TLegend* leg = new TLegend(0.75, 0.75, 0.95, 0.95);
    leg->SetBorderSize(1);
    leg->SetFillStyle(0);
    leg->SetTextSize(0.03);

    std::vector<TString> labels = {"OCT 2023", "DEC 2023", "JUL 2024", "SEP 2024", "OCT 2025", "NOV 2025"};

    for (size_t ig = 0; ig < ratioGraphs.size(); ++ig) {
        ratioGraphs[ig]->Draw(ig == 0 ? "AP" : "P SAME");
        leg->AddEntry(ratioGraphs[ig], ig < labels.size() ? labels[ig] : Form("Graph %zu", ig), "p");
    }

    // ---- axes: limits and titles from the type of plot (the first graph drawn defines the axes) ----
    TString fname1 = gSystem->BaseName(file1);
    TGraphErrors* g0 = ratioGraphs[0];
    if (fname1.Contains("lambda")) {
        cout << "Filename " << fname1.Data() << " contains lambda" << endl;
        g0->GetXaxis()->SetLimits(-50, 1200);
        g0->GetXaxis()->SetTitle("Wavelength [pm] - 6788000");
    } else if (fname1.Contains("batch")) {
        cout << "Filename " << fname1.Data() << " contains batch" << endl;
        g0->GetXaxis()->SetLimits(-1, 40);
        g0->GetXaxis()->SetTitle("Batch number");
    } else {
        std::cerr << "WARNING: cannot tell the x variable from " << fname1 << std::endl;
    }
    g0->GetYaxis()->SetTitle("Ratio");
    g0->GetYaxis()->SetRangeUser(0.9, 1.4);
    g0->GetYaxis()->SetTitleSize(0.04);

    leg->Draw();
    cR->Update();

    // reference line at 1
    TLine* line = new TLine(cR->GetUxmin(), 1, cR->GetUxmax(), 1);
    line->SetLineColor(kRed);
    line->SetLineStyle(2);
    line->SetLineWidth(2);
    line->Draw("same");

    cR->Modified();
    cR->Update();

    cR->SaveAs(saveName + ".C");
    cR->SaveAs(saveName + ".png");

    return cR;
}