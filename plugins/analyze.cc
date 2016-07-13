#include "TTreeReader.h"
#include "TTreeReaderArray.h"

#include "PlotManager.h"
#include "ProtonKinematics.h"

#include "Utils.h"

#include <iostream>
using namespace std;

int
main(int argc, char* argv[])
{
  if (argc<2) { cerr << "Usage: " << argv[0] << " [input file] <output file>" << endl; return -1; }
  const char* output = (argc>2) ? argv[2] : "output.root";

  TFile in(argv[1]);

  // start by booking the values we want to retrieve from tree
  TTreeReader tr("ntp1", &in);
  TTreeReaderValue<int> nHLT(tr, "nHLT");
  //TTreeReaderArray<int> HLT_Accept(tr, "HLT_Accept");
  //TTreeReaderArray<int> HLT_Accept(tr, "HLT_Accept.HLT_Prescl"); //FIXME workaround!!!
  TTreeReaderArray<int> HLT_Prescl(tr, "HLT_Prescl");
  TTreeReaderValue<int> nLocalProtCand(tr, "nLocalProtCand");
  TTreeReaderArray<double> LocalProtCand_x(tr, "LocalProtCand_x");
  TTreeReaderArray<double> LocalProtCand_y(tr, "LocalProtCand_y");
  TTreeReaderArray<int> LocalProtCand_arm(tr, "LocalProtCand_arm");
  TTreeReaderArray<int> LocalProtCand_side(tr, "LocalProtCand_side");

  PlotManager plt(output);
  if (!plt.IsValid()) return -1;

  // define the variables to be plotted
  plt.AddCategory("presel_level");
  plt.AddTH1("num_hlt", "Number of HLT paths fired", 5, 0., 5., "presel_level");
  plt.AddTH1("num_prot_tracks", "Number of tracks reconstructed in strips", 8, 0., 8., "presel_level", "Tracks multiplicity");
  plt.AddTH2("prot_tracks_origin", "Tracks origin hitmap", 250, 0., 25., 300, -15., 15., "presel_level", "x", "y");
  plt.AddTH2("num_prot_tracks_vs_arm", "Number of tracks reconstructed in strips (per arm)", 5, 0., 5., 5, 0., 5., "presel_level", "Tracks in left arm", "Tracks in right arm");

  // left arm
  plt.AddCategory("arm_left", "presel_level");
  plt.AddTH1("num_prot_tracks", "Number of tracks reconstructed in strips (left arm)", 5, 0., 5., "presel_level/arm_left", "Tracks multiplicity");
  plt.AddTH2("prot_tracks_origin", "Tracks origin hitmap (left arm)", 250, 0., 25., 300, -15., 15., "presel_level/arm_left", "x", "y");
  plt.AddTH2("prot_near_far_corr_x", "Near/far hits x-correlation (left arm)", 200, 0., 40., 200, 0., 40., "presel_level/arm_left", "x(near)", "x(far)");
  plt.AddTH2("prot_near_far_corr_y", "Near/far hits y-correlation (left arm)", 200, -20., 20., 200, -20., 20., "presel_level/arm_left", "y(near)", "y(far)");
  plt.AddTH1("prot_near_far_dist_x", "Near/far hits x-correlation (left arm)", 200, -20., 20., "presel_level/arm_left", "x(near)-x(far)");
  plt.AddTH1("prot_near_far_dist_y", "Near/far hits y-correlation (left arm)", 200, -20., 20., "presel_level/arm_left", "y(near)-y(far)");
  plt.AddTH2("prot_near_far_dist_xy", "Near/far hits xy-correlation (left arm)", 200, -20., 20., 200, -20., 20., "presel_level/arm_left", "x(near)-x(far)", "y(near)-y(far)");

  plt.AddCategory("station_near", "presel_level/arm_left");
  plt.AddTH2("prot_tracks_origin", "Tracks origin hitmap", 250, 0., 25., 300, -15., 15., "presel_level/arm_left/station_near", "x", "y");
  plt.AddCategory("station_far", "presel_level/arm_left");
  plt.AddTH2("prot_tracks_origin", "Tracks origin hitmap", 250, 0., 25., 300, -15., 15., "presel_level/arm_left/station_far", "x", "y");

  // right arm
  plt.AddCategory("arm_right", "presel_level");
  plt.AddTH1("num_prot_tracks", "Number of tracks reconstructed in strips (right arm)", 5, 0., 5., "presel_level/arm_right", "Tracks multiplicity");
  plt.AddTH2("prot_tracks_origin", "Tracks origin hitmap (right arm)", 250, 0., 25., 300, -15., 15., "presel_level/arm_right", "x", "y");
  plt.AddTH2("prot_near_far_corr_x", "Near/far hits x-correlation (right arm)", 200, 0., 40., 200, 0., 40., "presel_level/arm_right", "x(near)", "x(far)");
  plt.AddTH2("prot_near_far_corr_y", "Near/far hits y-correlation (right arm)", 200, -20., 20., 200, -20., 20., "presel_level/arm_right", "y(near)", "y(far)");
  plt.AddTH1("prot_near_far_dist_x", "Near/far hits x-correlation (left arm)", 200, -20., 20., "presel_level/arm_right", "x(near)-x(far)");
  plt.AddTH1("prot_near_far_dist_y", "Near/far hits y-correlation (right arm)", 200, -20., 20., "presel_level/arm_right", "y(near)-y(far)");
  plt.AddTH2("prot_near_far_dist_xy", "Near/far hits xy-correlation (right arm)", 200, -20., 20., 200, -20., 20., "presel_level/arm_right", "x(near)-x(far)", "y(near)-y(far)");

  plt.AddCategory("station_near", "presel_level/arm_right");
  plt.AddTH2("prot_tracks_origin", "Tracks origin hitmap", 250, 0., 25., 300, -15., 15., "presel_level/arm_right/station_near", "x", "y");
  plt.AddCategory("station_far", "presel_level/arm_right");
  plt.AddTH2("prot_tracks_origin", "Tracks origin hitmap", 250, 0., 25., 300, -15., 15., "presel_level/arm_right/station_far", "x", "y");

  /*plt.AddCategory("elastic_level");

  plt.AddCategory("elastic_tight_level");*/

  unsigned int i = 0;
  while (tr.Next()) {
    i++;
    //if (i>=50000) break;
    plt.FillTH1(*nHLT, "num_hlt", "presel_level");
    plt.FillTH1(*nLocalProtCand, "num_prot_tracks", "presel_level");
    TH2D* hitmap_lpc = plt.GetTH2("prot_tracks_origin", "presel_level");
    RPHits rph;
    unsigned int num_left = 0, num_right = 0;
    for (unsigned int j=0; j<*nLocalProtCand; j++) {
      //plt.FillTH2(LocalProtCand_x[j], LocalProtCand_y[j], "prot_tracks_origin", "presel_level");
      hitmap_lpc->Fill(LocalProtCand_x[j]*1.e3, LocalProtCand_y[j]*1.e3);
      if (LocalProtCand_side[j]==0) { // left side
        plt.FillTH2(LocalProtCand_x[j]*1.e3, LocalProtCand_y[j]*1.e3, "prot_tracks_origin", "presel_level/arm_left");
        if (LocalProtCand_arm[j]==0) {
          plt.FillTH2(LocalProtCand_x[j]*1.e3, LocalProtCand_y[j]*1.e3, "prot_tracks_origin", "presel_level/arm_left/station_far");
          rph.LF = XYCoord(LocalProtCand_x[j]*1.e3, LocalProtCand_y[j]*1.e3);
        }
        else {
          plt.FillTH2(LocalProtCand_x[j]*1.e3, LocalProtCand_y[j]*1.e3, "prot_tracks_origin", "presel_level/arm_left/station_near");
          rph.LN = XYCoord(LocalProtCand_x[j]*1.e3, LocalProtCand_y[j]*1.e3);
        }
        num_left++;
      }
      else { // right side
        plt.FillTH2(LocalProtCand_x[j]*1.e3, LocalProtCand_y[j]*1.e3, "prot_tracks_origin", "presel_level/arm_right");
        if (LocalProtCand_arm[j]==0) {
          plt.FillTH2(LocalProtCand_x[j]*1.e3, LocalProtCand_y[j]*1.e3, "prot_tracks_origin", "presel_level/arm_right/station_far");
          rph.RF = XYCoord(LocalProtCand_x[j]*1.e3, LocalProtCand_y[j]*1.e3);
        }
        else {
          plt.FillTH2(LocalProtCand_x[j]*1.e3, LocalProtCand_y[j]*1.e3, "prot_tracks_origin", "presel_level/arm_right/station_near");
          rph.RN = XYCoord(LocalProtCand_x[j]*1.e3, LocalProtCand_y[j]*1.e3);
        }
        num_right++;
      }
    }
    plt.FillTH1(num_left, "num_prot_tracks", "presel_level/arm_left");
    plt.FillTH1(num_right, "num_prot_tracks", "presel_level/arm_right");
    plt.FillTH2(num_left, num_right, "num_prot_tracks_vs_arm", "presel_level");

    if (rph.BothArmsLeft()) {
      plt.FillTH2(rph.LN.x, rph.LF.x, "prot_near_far_corr_x", "presel_level/arm_left");
      plt.FillTH2(rph.LN.y, rph.LF.y, "prot_near_far_corr_y", "presel_level/arm_left");
      plt.FillTH1(rph.LN.x-rph.LF.x, "prot_near_far_dist_x", "presel_level/arm_left");
      plt.FillTH1(rph.LN.y-rph.LF.y, "prot_near_far_dist_y", "presel_level/arm_left");
      plt.FillTH2(rph.LN.x-rph.LF.x, rph.LN.y-rph.LF.y, "prot_near_far_dist_xy", "presel_level/arm_left");
    }
    if (rph.BothArmsRight()) {
      plt.FillTH2(rph.RN.x, rph.RF.x, "prot_near_far_corr_x", "presel_level/arm_right");
      plt.FillTH2(rph.RN.y, rph.RF.y, "prot_near_far_corr_y", "presel_level/arm_right");
      plt.FillTH1(rph.RN.x-rph.RF.x, "prot_near_far_dist_x", "presel_level/arm_right");
      plt.FillTH1(rph.RN.y-rph.RF.y, "prot_near_far_dist_y", "presel_level/arm_right");
      plt.FillTH2(rph.RN.x-rph.RF.x, rph.RN.y-rph.RF.y, "prot_near_far_dist_xy", "presel_level/arm_right");
    }
    //if (!HLT_Accept[0] or !HLT_Accept[1]) continue; //FIXME
    //if (HLT_Prescl[0]!=1. or HLT_Prescl[1]!=1.) continue; //FIXME
  }

  


  // 2nd loop after the calibration constants are extracted
  /*while (tr.Next()) {
  }*/

  return 0;
}
