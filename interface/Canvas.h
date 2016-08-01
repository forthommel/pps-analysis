#ifndef Canvas_h
#define Canvas_h

#include "TCanvas.h"
#include "TLegend.h"
#include "TPaveText.h"

#include <string.h>

class Canvas : public TCanvas
{
 public:
  inline Canvas(const char* name, const char* title="") :
    TCanvas(name, "", 450, 450),
    fTitle(title), fTopLabel(0),
    fLeg(0), fLegX1(0.5), fLegY1(0.65) {
    Build();
  }
  inline ~Canvas() {
    if (fLeg) delete fLeg;
    if (fTopLabel) delete fTopLabel;
  }

  inline void Prettify(const TH1* obj) {
    TAxis* x = (TAxis*)obj->GetXaxis(), *y = (TAxis*)obj->GetYaxis();
    x->SetLabelSize(0.035);
    x->SetTitleSize(0.05);
    y->SetLabelSize(0.035);
    y->SetTitleSize(0.05);
    y->SetTitleOffset(1.2);
  }

  inline void SetTopLabel(const char* lab="") {
    if (strcmp(lab, "")!=0) fTitle = lab;
    if (!fTopLabel) {
      fTopLabel = new TPaveText(0.5, 0.95, 0.925, 0.96, "NB NDC");
      fTopLabel->SetFillStyle(0);
      fTopLabel->SetFillColor(0);
      fTopLabel->SetLineColor(0);
      fTopLabel->SetLineStyle(0);
      fTopLabel->SetTextFont(42);
      fTopLabel->SetTextSize(0.03);
      fTopLabel->SetTextAlign(kHAlignRight+kVAlignBottom);
    }
    else fTopLabel->Clear();
    fTopLabel->AddText(fTitle);
    fTopLabel->Draw();
  }

  inline void SetLegendX1(double x) { fLegX1 = x; }
  inline void SetLegendY1(double y) { fLegY1 = y; }
  inline void AddLegendEntry(const TObject* obj, const char* title, Option_t* option="lpf") {
    if (!fLeg) {
      fLeg = new TLegend(fLegX1, fLegY1, fLegX1+0.3, fLegY1+0.15);
      fLeg->SetLineColor(kWhite);
      fLeg->SetLineWidth(0);
      fLeg->SetTextSize(0.04);
    }
    fLeg->AddEntry(obj, title, option);
  }

  inline void Save(const char* ext) {
    if (strstr(ext, "pdf")==NULL) return;
    if (fLeg) fLeg->Draw();
    if (fTopLabel) fTopLabel->Draw();
    TCanvas::SaveAs(Form("%s.%s", TCanvas::GetName(), ext));
  }

 private:
  inline void Build() {
    TCanvas::SetLeftMargin(0.14);
    TCanvas::SetTopMargin(0.06);
    TCanvas::SetRightMargin(0.1);
    TCanvas::SetBottomMargin(0.12);
    SetTopLabel();
  }

  TString fTitle;
  TPaveText* fTopLabel;
  TLegend* fLeg;
  double fLegX1, fLegY1;
};

#endif
