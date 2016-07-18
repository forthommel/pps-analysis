#ifndef AlCaReader_h
#define AlCaReader_h

#include "TFile.h"
#include "TTree.h"

#include <map>

struct AlcaParameters
{
  double mean_near_minus_far_x_left, mean_near_minus_far_x_right;
  double sigma_near_minus_far_x_left, sigma_near_minus_far_x_right;
  double mean_near_minus_far_y_left, mean_near_minus_far_y_right;
  double sigma_near_minus_far_y_left, sigma_near_minus_far_y_right;
};

class AlCaReader
{
 public:
  inline AlCaReader(const char* file="output_AlCa.root"):
    fGood(false)
  {
    TFile f(file);
    if (!f.IsOpen()) return;
    TTree* param = (TTree*)f.Get("alca_parameters_byrun");
    int run_id; param->SetBranchAddress("run_id", &run_id);
    AlcaParameters p;
    param->SetBranchAddress("mean_near_far_x_left_byrun", &(p.mean_near_minus_far_x_left));
    param->SetBranchAddress("sigma_near_far_x_left_byrun", &(p.sigma_near_minus_far_x_left));
    param->SetBranchAddress("mean_near_far_y_left_byrun", &(p.mean_near_minus_far_y_left));
    param->SetBranchAddress("sigma_near_far_y_left_byrun", &(p.sigma_near_minus_far_y_left));
    param->SetBranchAddress("mean_near_far_x_right_byrun", &(p.mean_near_minus_far_x_right));
    param->SetBranchAddress("sigma_near_far_x_right_byrun", &(p.sigma_near_minus_far_x_right));
    param->SetBranchAddress("mean_near_far_y_right_byrun", &(p.mean_near_minus_far_y_right));
    param->SetBranchAddress("sigma_near_far_y_right_byrun", &(p.sigma_near_minus_far_y_right));
    for (unsigned int i=0; i<param->GetEntries(); i++) {
      param->GetEntry(i);
      fMap.insert(std::pair<unsigned int,AlcaParameters>(run_id, p));
    }
    fGood = true;
  }

  inline void GetNFLeftCorrX(unsigned int run_id, double* mean, double* sigma) const {
    AlcaParameters p = GetParameters(run_id);
    *mean = p.mean_near_minus_far_x_left;
    *sigma = p.sigma_near_minus_far_x_left;
  }
  inline void GetNFLeftCorrY(unsigned int run_id, double* mean, double* sigma) const {
    AlcaParameters p = GetParameters(run_id);
    *mean = p.mean_near_minus_far_y_left;
    *sigma = p.sigma_near_minus_far_y_left;
  }
  inline void GetNFRightCorrX(unsigned int run_id, double* mean, double* sigma) const {
    AlcaParameters p = GetParameters(run_id);
    *mean = p.mean_near_minus_far_x_right;
    *sigma = p.sigma_near_minus_far_x_right;
  }
  inline void GetNFRightCorrY(unsigned int run_id, double* mean, double* sigma) const {
    AlcaParameters p = GetParameters(run_id);
    *mean = p.mean_near_minus_far_y_right;
    *sigma = p.sigma_near_minus_far_y_right;
  }
 
 private:
  inline AlcaParameters GetParameters(unsigned int run_id) const {
    std::map<unsigned int,AlcaParameters>::const_iterator it = fMap.find(run_id);
    if (it==fMap.end()) return AlcaParameters();
    return it->second;
  }

  bool fGood;
  std::map<unsigned int,AlcaParameters> fMap;
};

#endif
