#include "PlotManager.h"

PlotManager::PlotManager(const char* out):
  fFile(new TFile(out, "recreate"))
{
  fFile->cd(); // start at "/"
}

PlotManager::~PlotManager()
{
  if (!fFile) return;
  //fFile->Write();
  fFile->Write(0, TObject::kOverwrite);
  delete fFile;
}

void
PlotManager::AddCategory(const char* cat_name, const char* parent)
{
  if (!fFile) return;
  TDirectory* dir = fFile->GetDirectory(parent);
  if (!dir) { std::cerr << "ERROR failed to retrieve the parent category " << (char*)parent << " for new category " << (char*)cat_name << std::endl; return; }
  if (!dir->mkdir(cat_name)) { std::cerr << "ERROR failed to create a category " << (char*)cat_name << " with parent " << (char*)parent << std::endl; return; }
}

void
PlotManager::AddTH1(const char* name, const char* title, unsigned int nbins, double min, double max, const char* cat_path, const char* xlabel, const char* ylabel)
{
  TDirectory* dir = fFile->GetDirectory(cat_path);
  if (!dir) { std::cerr << "ERROR failed to retrieve the parent category " << cat_path << " to build 1D histogram " << name << std::endl; return; }
  TH1D h_tmp(name, title, nbins, min, max);
  h_tmp.GetXaxis()->SetTitle(xlabel);
  h_tmp.GetYaxis()->SetTitle(ylabel);
  dir->WriteTObject(&h_tmp);
}

TH1D*
PlotManager::GetTH1(const char* name, const char* cat_path)
{
  TDirectory* dir = fFile->GetDirectory(cat_path);
  if (!dir) { std::cerr << "ERROR failed to retrieve the parent category " << cat_path << " to retrieve 1D histogram " << name << std::endl; return 0; }
  //TH1D* h_tmp = (TH1D*)dir->FindObject(name);
  TH1D* h_tmp = 0; dir->GetObject(Form("%s", name), h_tmp);
  return h_tmp;
}

void
PlotManager::FillTH1(double value, const char* name, const char* cat_path, double weight)
{
  TH1D* h_tmp = GetTH1(name, cat_path);
  if (!h_tmp) { std::cerr << "ERROR failed to retrieve the 1D histogram " << name << " in category " << cat_path << std::endl; return; }
  h_tmp->Fill(value, weight);
}

void
PlotManager::AddTH2(const char* name, const char* title, unsigned int nbinsx, double minx, double maxx, unsigned int nbinsy, double miny, double maxy, const char* cat_path, const char* xlabel, const char* ylabel)
{
  TDirectory* dir = fFile->GetDirectory(cat_path);
  if (!dir) { std::cerr << "ERROR failed to retrieve the parent category " << cat_path << " to build 1D histogram " << name << std::endl; return; }
  TH2D h_tmp(name, title, nbinsx, minx, maxx, nbinsy, miny, maxy);
  h_tmp.GetXaxis()->SetTitle(xlabel);
  h_tmp.GetYaxis()->SetTitle(ylabel);
  dir->WriteTObject(&h_tmp);
}

TH2D*
PlotManager::GetTH2(const char* name, const char* cat_path)
{
  TDirectory* dir = fFile->GetDirectory(cat_path);
  if (!dir) { std::cerr << "ERROR failed to retrieve the parent category " << cat_path << " to retrieve 1D histogram " << name << std::endl; return 0; }
  TH2D* h_tmp = 0; dir->GetObject(Form("%s", name), h_tmp);
  return h_tmp;
}

void
PlotManager::FillTH2(double value_x, double value_y, const char* name, const char* cat_path, double weight)
{
  TH2D* h_tmp = GetTH2(name, cat_path);
  if (!h_tmp) { std::cerr << "ERROR failed to retrieve the 1D histogram " << name << " in category " << cat_path << std::endl; return; }
  h_tmp->Fill(value_x, value_y, weight);
}
