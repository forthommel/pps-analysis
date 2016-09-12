#include "TTreeReader.h"
#include "TTreeReaderArray.h"
#include "TLorentzVector.h"

#include "PlotManager.h"
#include "ArgumentsParser.h"
#include "AlCaReader.h"

#include "ProtonKinematics.h"
#include "Utils.h"

#include <iostream>
using namespace std;

int
main(int argc, char* argv[])
{
  ArgumentsParser arg(argc, argv);
  if (!arg.HasArgument("--input")) { arg.PrintUsage(); return -1; }
  const char* output = (arg.HasArgument("--output")) ? arg.GetOutput() : "output.root";
  if (arg.HasArgument("--alca-file")) {;}
  const unsigned int num_sigma = 2;

  TFile in(arg.GetInput());

  // start by booking the values we want to retrieve from tree
  TTreeReader tr("ntp1", &in);
  TTreeReaderValue<int> Run(tr, "Run");
  TTreeReaderValue<int> nHLT(tr, "nHLT");
  TTreeReaderArray<int> HLT_Prescl(tr, "HLT_Prescl");

  TTreeReaderValue<int> nPrimVertexCand(tr, "nPrimVertexCand");
  TTreeReaderArray<int> PrimVertexCand_tracks(tr, "PrimVertexCand_tracks");
  TTreeReaderArray<int> PrimVertexCand_hasdil(tr, "PrimVertexCand_hasdil");
  TTreeReaderArray<double> Pair_mass(tr, "Pair_mass");
  TTreeReaderArray<int> Pair_lepton1(tr, "Pair_lepton1");
  TTreeReaderArray<int> Pair_lepton2(tr, "Pair_lepton2");
  TTreeReaderArray<double> MuonCand_pt(tr, "MuonCand_pt");
  TTreeReaderArray<double> MuonCand_px(tr, "MuonCand_px");
  TTreeReaderArray<double> MuonCand_py(tr, "MuonCand_py");
  TTreeReaderArray<double> MuonCand_pz(tr, "MuonCand_pz");
  TTreeReaderArray<double> MuonCand_eta(tr, "MuonCand_eta");
  TTreeReaderArray<double> MuonCand_phi(tr, "MuonCand_phi");

  TTreeReaderValue<int> nProtonCand(tr, "nProtonCand");
  TTreeReaderArray<int> ProtonCand_side(tr, "ProtonCand_side");
  TTreeReaderArray<double> ProtonCand_xi(tr, "ProtonCand_xi");

  PlotManager plt(output);
  if (!plt.IsValid()) return -1;

  AlCaReader acr;
  double mean_x, sigma_x, mean_y, sigma_y;

  // define the variables to be plotted
  plt.AddCategory("presel_level");
  plt.AddTH1("num_hlt", "Number of HLT paths fired", 5, 0., 5., "presel_level");
  plt.AddTH1("num_protons", "Number of protons reconstructed in event", 5, 0., 5., "presel_level");

  plt.AddTH1("xi1_reco", "#xi (reconstructed from dilepton)", 250, 0., 0.25, "presel_level", "#xi");
  plt.AddTH1("xi2_reco", "#xi (reconstructed from dilepton)", 250, 0., 0.25, "presel_level", "#xi");
  plt.AddTH2("xi1_recomatch", "#xi (reconstructed from dilepton/RP)", 250, 0., 0.25, 250, 0., 0.25, "presel_level", "#xi_{dilepton}", "#xi_{RP}");
  plt.AddTH2("xi2_recomatch", "#xi (reconstructed from dilepton/RP)", 250, 0., 0.25, 250, 0., 0.25, "presel_level", "#xi_{dilepton}", "#xi_{RP}");
  plt.AddTH1("dilepton_mass", "Dilepton mass", 500, 0., 1000., "presel_level", "m_{#mu#mu} (GeV)");

  // left arm
  plt.AddCategory("arm_left", "presel_level");
  //plt.AddTH2("prot_tracks_origin", "Tracks origin hitmap (left arm)", 250, 0., 25., 300, -15., 15., "presel_level/arm_left/station_near", "x", "y");
  plt.AddTH1("prot_xi", "Proton #xi (left arm)", 250, 0., 0.25, "presel_level/arm_left", "#xi");
  // right arm
  plt.AddCategory("arm_right", "presel_level");
  //plt.AddTH2("prot_tracks_origin", "Tracks origin hitmap (right arm)", 250, 0., 25., 300, -15., 15., "presel_level/arm_right/station_near", "x", "y");
  plt.AddTH1("prot_xi", "Proton #xi (right arm)", 250, 0., 0.25, "presel_level/arm_right", "#xi");


  unsigned int i = 0;
  TLorentzVector l1, l2;
  const float sqrt_s = 13.e3, muon_mass = 0.1057;
  float xi_reco1, xi_reco2;

  while (tr.Next()) {
    i++;
if (i%1000==0) cerr << "event " << i << endl;
    //if (i>=50000) break;
    plt.FillTH1(*nHLT, "num_hlt", "presel_level");
    plt.FillTH1(*nProtonCand, "num_protons", "presel_level");

    xi_reco1 = xi_reco2 = -1.;

    //if (!HLT_Accept[0] or !HLT_Accept[1]) continue; //FIXME
    //if (HLT_Prescl[0]!=1. or HLT_Prescl[1]!=1.) continue; //FIXME
    //
    for (unsigned j=0; j<*nPrimVertexCand; j++) {
      //plt.FillTH1(PrimVertexCand_tracks[j], "num_tracks_all", "presel_level");
      if (!PrimVertexCand_hasdil[j]) continue;

      const unsigned int l1_id = Pair_lepton1[j], l2_id = Pair_lepton2[j];
if (l1_id>100 or l2_id>100) continue;
      l1.SetPtEtaPhiM(MuonCand_pt[l1_id], MuonCand_eta[l1_id], MuonCand_phi[l1_id], muon_mass);
      l2.SetPtEtaPhiM(MuonCand_pt[l2_id], MuonCand_eta[l2_id], MuonCand_phi[l2_id], muon_mass);

      if (l1.Pt()<50. or fabs(l1.Eta())>2.4) continue;
      if (l2.Pt()<50. or fabs(l2.Eta())>2.4) continue;
      if ((l1+l2).M()<110.) continue;

      plt.FillTH1(Pair_mass[j], "dilepton_mass", "presel_level");
      /*cout << "vertex "<< j << ":reco muon 1 " << " has pt=" << MuonCand_pt[l1_id] << ", eta=" << MuonCand_eta[l1_id] << ", phi=" << MuonCand_phi[l1_id] << endl;
 *       cout << "                   2 " << " has pt=" << MuonCand_pt[l2_id] << ", eta=" << MuonCand_eta[l2_id] << ", phi=" << MuonCand_phi[l2_id] << endl;*/
      xi_reco1 = (l1.Pt()*exp( l1.Eta())+l2.Pt()*exp( l2.Eta()))/sqrt_s;
      xi_reco2 = (l1.Pt()*exp(-l1.Eta())+l2.Pt()*exp(-l2.Eta()))/sqrt_s;
      plt.FillTH1(xi_reco1, "xi1_reco", "presel_level");
      plt.FillTH1(xi_reco2, "xi2_reco", "presel_level");
      // if (PrimVertexCand_tracks[j]==2) { // exclusive level
    }
    //
    for (int j=0; j<*nProtonCand; j++) {
      if (ProtonCand_side[j]==0) {
        if (xi_reco1>=0.) {
          plt.FillTH2(xi_reco1, ProtonCand_xi[j], "xi1_recomatch", "presel_level");
cout << "xi1: " << xi_reco1 << "/" << ProtonCand_xi[j] << endl;
        }
        plt.FillTH1(ProtonCand_xi[j], "prot_xi", "presel_level/arm_left");
      }
      if (ProtonCand_side[j]==1) {
        if (xi_reco2>=0.) {
          plt.FillTH2(xi_reco2, ProtonCand_xi[j], "xi2_recomatch", "presel_level");
cout << "xi2: " << xi_reco2 << "/" << ProtonCand_xi[j] << endl;
        }
        plt.FillTH1(ProtonCand_xi[j], "prot_xi", "presel_level/arm_right");
      }
    }
  }

  


  // 2nd loop after the calibration constants are extracted
  /*while (tr.Next()) {
  }*/

  return 0;
}
