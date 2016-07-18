#include "TTreeReader.h"
#include "TTreeReaderArray.h"

#include "PlotManager.h"
#include "ProtonKinematics.h"

#include "FillNumberLUTHandler.h"
#include "Utils.h"

#include <iostream>
#include <map>

using namespace std;

#define MAX_RUNS 1000

int
main(int argc, char* argv[])
{
  if (argc<2) { cerr << "Usage: " << argv[0] << " [input file] <output file>" << endl; return -1; }
  const char* output = (argc>2) ? argv[2] : "output_AlCa.root";

  TFile in(argv[1]);

  // start by booking the values we want to retrieve from tree
  TTreeReader tr("ntp1", &in);
  TTreeReaderValue<int> Run(tr, "Run");
  TTreeReaderValue<int> nLocalProtCand(tr, "nLocalProtCand");
  TTreeReaderArray<double> LocalProtCand_x(tr, "LocalProtCand_x");
  TTreeReaderArray<double> LocalProtCand_y(tr, "LocalProtCand_y");
  TTreeReaderArray<int> LocalProtCand_arm(tr, "LocalProtCand_arm");
  TTreeReaderArray<int> LocalProtCand_side(tr, "LocalProtCand_side");

  FillNumberLUTHandler lut;

  PlotManager plt(output);
  if (!plt.IsValid()) return -1;

  TH1D* l_nf_x_run, *l_nf_y_run, *r_nf_x_run, *r_nf_y_run;
  TH1D* l_nf_x_fill, *l_nf_y_fill, *r_nf_x_fill, *r_nf_y_fill;

  // define the variables to be plotted
  plt.AddCategory("alca");

  // left arm
  plt.AddCategory("arm_left", "alca");
  plt.AddTH2("prot_near_far_corr_x", "Near/far hits x-correlation (left arm)", 200, 0., 40., 200, 0., 40., "alca/arm_left", "x(near)", "x(far)");
  plt.AddTH2("prot_near_far_corr_y", "Near/far hits y-correlation (left arm)", 200, -20., 20., 200, -20., 20., "alca/arm_left", "y(near)", "y(far)");
  plt.AddTH1("prot_near_far_dist_x", "Near/far hits x-correlation (left arm)", 200, -20., 20., "alca/arm_left", "x(near)-x(far)");
  plt.AddTH1("prot_near_far_dist_y", "Near/far hits y-correlation (left arm)", 200, -20., 20., "alca/arm_left", "y(near)-y(far)");
  plt.AddTH2("prot_near_far_dist_xy", "Near/far hits xy-correlation (left arm)", 200, -20., 20., 200, -20., 20., "alca/arm_left", "x(near)-x(far)", "y(near)-y(far)");
  plt.AddCategory("runs", "alca/arm_left");
  plt.AddCategory("fills", "alca/arm_left");

  // right arm
  plt.AddCategory("arm_right", "alca");
  plt.AddTH2("prot_near_far_corr_x", "Near/far hits x-correlation (right arm)", 200, 0., 40., 200, 0., 40., "alca/arm_right", "x(near)", "x(far)");
  plt.AddTH2("prot_near_far_corr_y", "Near/far hits y-correlation (right arm)", 200, -20., 20., 200, -20., 20., "alca/arm_right", "y(near)", "y(far)");
  plt.AddTH1("prot_near_far_dist_x", "Near/far hits x-correlation (right arm)", 200, -20., 20., "alca/arm_right", "x(near)-x(far)");
  plt.AddTH1("prot_near_far_dist_y", "Near/far hits y-correlation (right arm)", 200, -20., 20., "alca/arm_right", "y(near)-y(far)");
  plt.AddTH2("prot_near_far_dist_xy", "Near/far hits xy-correlation (right arm)", 200, -20., 20., 200, -20., 20., "alca/arm_right", "x(near)-x(far)", "y(near)-y(far)");
  plt.AddCategory("runs", "alca/arm_right");
  plt.AddCategory("fills", "alca/arm_right");

  plt.AddTTree("alca_parameters_byrun");
  TTree* t_r = plt.GetTTree("alca_parameters_byrun");
  int run_id; t_r->Branch("run_id", &run_id, "run_id/I");
  double mean_near_far_x_left_byrun; t_r->Branch("mean_near_far_x_left_byrun", &mean_near_far_x_left_byrun, "mean_near_far_x_left_byrun/D");
  double sigma_near_far_x_left_byrun; t_r->Branch("sigma_near_far_x_left_byrun", &sigma_near_far_x_left_byrun, "sigma_near_far_x_left_byrun/D");
  double mean_near_far_y_left_byrun; t_r->Branch("mean_near_far_y_left_byrun", &mean_near_far_y_left_byrun, "mean_near_far_y_left_byrun/D");
  double sigma_near_far_y_left_byrun; t_r->Branch("sigma_near_far_y_left_byrun", &sigma_near_far_y_left_byrun, "sigma_near_far_y_left_byrun/D");
  double mean_near_far_x_right_byrun; t_r->Branch("mean_near_far_x_right_byrun", &mean_near_far_x_right_byrun, "mean_near_far_x_right_byrun/D");
  double sigma_near_far_x_right_byrun; t_r->Branch("sigma_near_far_x_right_byrun", &sigma_near_far_x_right_byrun, "sigma_near_far_x_right_byrun/D");
  double mean_near_far_y_right_byrun; t_r->Branch("mean_near_far_y_right_byrun", &mean_near_far_y_right_byrun, "mean_near_far_y_right_byrun/D");
  double sigma_near_far_y_right_byrun; t_r->Branch("sigma_near_far_y_right_byrun", &sigma_near_far_y_right_byrun, "sigma_near_far_y_right_byrun/D");

  plt.AddTTree("alca_parameters_byfill");
  TTree* t_f = plt.GetTTree("alca_parameters_byfill");
  int fill_num; t_f->Branch("fill_num", &fill_num, "fill_num/I");
  double mean_near_far_x_left_byfill; t_f->Branch("mean_near_far_x_left_byfill", &mean_near_far_x_left_byfill, "mean_near_far_x_left_byfill/D");
  double sigma_near_far_x_left_byfill; t_f->Branch("sigma_near_far_x_left_byfill", &sigma_near_far_x_left_byfill, "sigma_near_far_x_left_byfill/D");
  double mean_near_far_y_left_byfill; t_f->Branch("mean_near_far_y_left_byfill", &mean_near_far_y_left_byfill, "mean_near_far_y_left_byfill/D");
  double sigma_near_far_y_left_byfill; t_f->Branch("sigma_near_far_y_left_byfill", &sigma_near_far_y_left_byfill, "sigma_near_far_y_left_byfill/D");
  double mean_near_far_x_right_byfill; t_f->Branch("mean_near_far_x_right_byfill", &mean_near_far_x_right_byfill, "mean_near_far_x_right_byfill/D");
  double sigma_near_far_x_right_byfill; t_f->Branch("sigma_near_far_x_right_byfill", &sigma_near_far_x_right_byfill, "sigma_near_far_x_right_byfill/D");
  double mean_near_far_y_right_byfill; t_f->Branch("mean_near_far_y_right_byfill", &mean_near_far_y_right_byfill, "mean_near_far_y_right_byfill/D");
  double sigma_near_far_y_right_byfill; t_f->Branch("sigma_near_far_y_right_byfill", &sigma_near_far_y_right_byfill, "sigma_near_far_y_right_byfill/D");

  map<unsigned int,unsigned int> run_ids_map, fill_num_map;
  unsigned int run = 0, fill = 0;
  unsigned int i = 0;
  while (tr.Next()) {
    i++;
    //if (i>=50000) break;
    RPHits rph;
    for (unsigned int j=0; j<*nLocalProtCand; j++) {
      if (LocalProtCand_side[j]==0) { // left side
        if (LocalProtCand_arm[j]==0) { rph.LF = XYCoord(LocalProtCand_x[j]*1.e3, LocalProtCand_y[j]*1.e3); }
        else { rph.LN = XYCoord(LocalProtCand_x[j]*1.e3, LocalProtCand_y[j]*1.e3); }
      }
      else { // right side
        if (LocalProtCand_arm[j]==0) { rph.RF = XYCoord(LocalProtCand_x[j]*1.e3, LocalProtCand_y[j]*1.e3); }
        else { rph.RN = XYCoord(LocalProtCand_x[j]*1.e3, LocalProtCand_y[j]*1.e3); }
      }
    }

    // 1st batch of histograms indexed by the CMS run id
    map<unsigned int,unsigned int>::const_iterator map_it = run_ids_map.find(*Run);
    if (map_it==run_ids_map.end()) {
      run_ids_map.insert(pair<unsigned int,unsigned int>(*Run, run));
      plt.AddCategory(Form("%i", *Run), "alca/arm_left/runs");
      plt.AddTH1("near_minus_far_x", "", 200, -20., 20., Form("alca/arm_left/runs/%i", *Run)); l_nf_x_run = plt.GetTH1("near_minus_far_x", Form("alca/arm_left/runs/%i", *Run));
      plt.AddTH1("near_minus_far_y", "", 200, -20., 20., Form("alca/arm_left/runs/%i", *Run)); l_nf_y_run = plt.GetTH1("near_minus_far_y", Form("alca/arm_left/runs/%i", *Run));
      plt.AddCategory(Form("%i", *Run), "alca/arm_right/runs");
      plt.AddTH1("near_minus_far_x", "", 200, -20., 20., Form("alca/arm_right/runs/%i", *Run)); r_nf_x_run = plt.GetTH1("near_minus_far_x", Form("alca/arm_right/runs/%i", *Run));
      plt.AddTH1("near_minus_far_y", "", 200, -20., 20., Form("alca/arm_right/runs/%i", *Run)); r_nf_y_run = plt.GetTH1("near_minus_far_y", Form("alca/arm_right/runs/%i", *Run));
      run++;
    }

    // 2nd batch of histograms indexed by the LHC fill number
    const unsigned int fill_num = lut.GetFillNumber(*Run);
    map<unsigned int,unsigned int>::const_iterator map_f_it = fill_num_map.find(fill_num);
    if (map_f_it==fill_num_map.end()) {
      fill_num_map.insert(pair<unsigned int,unsigned int>(fill_num, fill));
      plt.AddCategory(Form("%i", fill_num), "alca/arm_left/fills");
      plt.AddTH1("near_minus_far_x", "", 200, -20., 20., Form("alca/arm_left/fills/%i", fill_num)); l_nf_x_fill = plt.GetTH1("near_minus_far_x", Form("alca/arm_left/fills/%i", fill_num));
      plt.AddTH1("near_minus_far_y", "", 200, -20., 20., Form("alca/arm_left/fills/%i", fill_num)); l_nf_y_fill = plt.GetTH1("near_minus_far_y", Form("alca/arm_left/fills/%i", fill_num));
      plt.AddCategory(Form("%i", fill_num), "alca/arm_right/fills");
      plt.AddTH1("near_minus_far_x", "", 200, -20., 20., Form("alca/arm_right/fills/%i", fill_num)); r_nf_x_fill = plt.GetTH1("near_minus_far_x", Form("alca/arm_right/fills/%i", fill_num));
      plt.AddTH1("near_minus_far_y", "", 200, -20., 20., Form("alca/arm_right/fills/%i", fill_num)); r_nf_y_fill = plt.GetTH1("near_minus_far_y", Form("alca/arm_right/fills/%i", fill_num));
      fill++;
    }

    if (rph.BothArmsLeft()) {
      plt.FillTH2(rph.LN.x, rph.LF.x, "prot_near_far_corr_x", "alca/arm_left");
      plt.FillTH2(rph.LN.y, rph.LF.y, "prot_near_far_corr_y", "alca/arm_left");
      plt.FillTH1(rph.LN.x-rph.LF.x, "prot_near_far_dist_x", "alca/arm_left");
      plt.FillTH1(rph.LN.y-rph.LF.y, "prot_near_far_dist_y", "alca/arm_left");
      plt.FillTH2(rph.LN.x-rph.LF.x, rph.LN.y-rph.LF.y, "prot_near_far_dist_xy", "alca/arm_left");
      l_nf_x_run->Fill(rph.LN.x-rph.LF.x);
      l_nf_y_run->Fill(rph.LN.y-rph.LF.y);
      l_nf_x_fill->Fill(rph.LN.x-rph.LF.x);
      l_nf_y_fill->Fill(rph.LN.y-rph.LF.y);
    }
    if (rph.BothArmsRight()) {
      plt.FillTH2(rph.RN.x, rph.RF.x, "prot_near_far_corr_x", "alca/arm_right");
      plt.FillTH2(rph.RN.y, rph.RF.y, "prot_near_far_corr_y", "alca/arm_right");
      plt.FillTH1(rph.RN.x-rph.RF.x, "prot_near_far_dist_x", "alca/arm_right");
      plt.FillTH1(rph.RN.y-rph.RF.y, "prot_near_far_dist_y", "alca/arm_right");
      plt.FillTH2(rph.RN.x-rph.RF.x, rph.RN.y-rph.RF.y, "prot_near_far_dist_xy", "alca/arm_right");
      r_nf_x_run->Fill(rph.RN.x-rph.RF.x);
      r_nf_y_run->Fill(rph.RN.y-rph.RF.y);
      r_nf_x_fill->Fill(rph.RN.x-rph.RF.x);
      r_nf_y_fill->Fill(rph.RN.y-rph.RF.y);
    }
  }
  for (map<unsigned int,unsigned int>::const_iterator map_it=run_ids_map.begin(); map_it!=run_ids_map.end(); map_it++) {
    run_id = map_it->first;
    mean_near_far_x_left_byrun = plt.GetTH1("near_minus_far_x", Form("alca/arm_left/runs/%i", run_id))->GetMean();
    sigma_near_far_x_left_byrun = plt.GetTH1("near_minus_far_x", Form("alca/arm_left/runs/%i", run_id))->GetStdDev();
    mean_near_far_y_left_byrun = plt.GetTH1("near_minus_far_y", Form("alca/arm_left/runs/%i", run_id))->GetMean();
    sigma_near_far_y_left_byrun = plt.GetTH1("near_minus_far_y", Form("alca/arm_left/runs/%i", run_id))->GetStdDev();
    mean_near_far_x_right_byrun = plt.GetTH1("near_minus_far_x", Form("alca/arm_right/runs/%i", run_id))->GetMean();
    sigma_near_far_x_right_byrun = plt.GetTH1("near_minus_far_x", Form("alca/arm_right/runs/%i", run_id))->GetStdDev();
    mean_near_far_y_right_byrun = plt.GetTH1("near_minus_far_y", Form("alca/arm_right/runs/%i", run_id))->GetMean();
    sigma_near_far_y_right_byrun = plt.GetTH1("near_minus_far_y", Form("alca/arm_right/runs/%i", run_id))->GetStdDev();
    plt.FillTTree("alca_parameters_byrun");
  }
  for (map<unsigned int,unsigned int>::const_iterator map_it=fill_num_map.begin(); map_it!=fill_num_map.end(); map_it++) {
    fill_num = map_it->first;
    mean_near_far_x_left_byfill = plt.GetTH1("near_minus_far_x", Form("alca/arm_left/fills/%i", fill_num))->GetMean();
    sigma_near_far_x_left_byfill = plt.GetTH1("near_minus_far_x", Form("alca/arm_left/fills/%i", fill_num))->GetStdDev();
    mean_near_far_y_left_byfill = plt.GetTH1("near_minus_far_y", Form("alca/arm_left/fills/%i", fill_num))->GetMean();
    sigma_near_far_y_left_byfill = plt.GetTH1("near_minus_far_y", Form("alca/arm_left/fills/%i", fill_num))->GetStdDev();
    mean_near_far_x_right_byfill = plt.GetTH1("near_minus_far_x", Form("alca/arm_right/fills/%i", fill_num))->GetMean();
    sigma_near_far_x_right_byfill = plt.GetTH1("near_minus_far_x", Form("alca/arm_right/fills/%i", fill_num))->GetStdDev();
    mean_near_far_y_right_byfill = plt.GetTH1("near_minus_far_y", Form("alca/arm_right/fills/%i", fill_num))->GetMean();
    sigma_near_far_y_right_byfill = plt.GetTH1("near_minus_far_y", Form("alca/arm_right/fills/%i", fill_num))->GetStdDev();
    plt.FillTTree("alca_parameters_byfill");
  }

  return 0;
}
