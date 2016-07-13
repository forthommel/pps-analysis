#ifndef PlotManager_h
#define PlotManager_h

#include "TFile.h"
#include "TH1.h"
#include "TH2.h"

#include <iostream>

class PlotManager
{
 public:
  PlotManager(const char* out);
  ~PlotManager();

  inline bool IsValid() const {
    if (!fFile->IsOpen() or !fFile->IsWritable()) return false;
    return true;
  }

  void AddCategory(const char* cat_name, const char* parent="/");

  void AddTH1(const char* name, const char* title, unsigned int nbins, double min, double max, const char* cat_path="/", const char* xlabel="", const char* ylabel="");
  TH1D* GetTH1(const char* name, const char* cat_path);
  void FillTH1(double value, const char* name, const char* cat_path, double weight=1.);
  void AddTH2(const char* name, const char* title, unsigned int nbinsx, double minx, double maxx, unsigned int nbinsy, double miny, double maxy, const char* cat_path="/", const char* xlabel="", const char* ylabel="");
  TH2D* GetTH2(const char* name, const char* cat_path);
  void FillTH2(double value_x, double value_y, const char* name, const char* cat_path, double weight=1.);
 
 private:
  TFile* fFile;
};

#endif
