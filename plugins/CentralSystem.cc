#include "TTreeReader.h"
#include "TTreeReaderArray.h"
#include "TLorentzVector.h"

#include "PlotManager.h"
#include "ProtonKinematics.h"

#include "Utils.h"

//#define muon_mass 0.10566
#define muon_mass 0.1057
#define sqrt_s 13.e3

#include <iostream>
using namespace std;

int
main(int argc, char* argv[])
{
  if (argc<2) { cerr << "Usage: " << argv[0] << " [input file] <output file>" << endl; return -1; }
  const char* output = (argc>2) ? argv[2] : "output.root";

  const double dr_cut = 0.18;

  TFile in(argv[1]);

  // start by booking the values we want to retrieve from tree
  TTreeReader tr("ntp1", &in);
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
  TTreeReaderValue<int> nGenProtCand(tr, "nGenProtCand");
  TTreeReaderArray<double> GenProtCand_pz(tr, "GenProtCand_pz");
  TTreeReaderValue<int> nGenMuonCand(tr, "nGenMuonCand");
  TTreeReaderArray<double> GenMuonCand_pt(tr, "GenMuonCand_pt");
  TTreeReaderArray<double> GenMuonCand_px(tr, "GenMuonCand_px");
  TTreeReaderArray<double> GenMuonCand_py(tr, "GenMuonCand_py");
  TTreeReaderArray<double> GenMuonCand_pz(tr, "GenMuonCand_pz");
  TTreeReaderArray<double> GenMuonCand_eta(tr, "GenMuonCand_eta");
  TTreeReaderArray<double> GenMuonCand_phi(tr, "GenMuonCand_phi");

  PlotManager plt(output);
  if (!plt.IsValid()) return -1;

  // define the variables to be plotted
  plt.AddCategory("presel_level");

  plt.AddTH1("num_vertices_all", "Vertices multiplicity", 200, 0., 200., "presel_level", "Vertices multiplicity)");
  plt.AddTH1("num_tracks_all", "Tracks multiplicity", 200, 0., 200., "presel_level", "Tracks multiplicity)");
  //plt.AddTH2("prot_near_far_dist_xy", "Near/far hits xy-correlation (left arm)", 200, -20., 20., 200, -20., 20., "alca/arm_left", "x(near)-x(far)", "y(near)-y(far)");
  plt.AddTH2("xi1_recomatching", "#xi (proton 1)", 200, 0., 0.2, 200, 0., 0.2, "presel_level", "GEN-level #xi(p_{1})", "RECO-level #xi(p_{1}) from #mu#mu");
  plt.AddTH2("xi2_recomatching", "#xi (proton 2)", 200, 0., 0.2, 200, 0., 0.2, "presel_level", "GEN-level #xi(p_{2})", "RECO-level #xi(p_{2}) from #mu#mu");

  plt.AddCategory("vtxing_level");
  plt.AddTH2("dilepton_mass_ctl", "", 200, 0., 200., 200, 0., 200., "vtxing_level");
  plt.AddTH1("dilepton_mass", "Dilepton mass", 200, 0., 200., "vtxing_level");
  plt.AddTH1("dilepton_xi", "Dilepton #xi", 200, 0., 1., "vtxing_level");
  plt.AddTH1("xi_reco_1", "#xi (proton 1)", 200, 0., 0.2, "vtxing_level", "#xi(p_{1}) (from dilepton system)");
  plt.AddTH1("xi_reco_2", "#xi (proton 2)", 200, 0., 0.2, "vtxing_level", "#xi(p_{2}) (from dilepton system)");

  plt.AddCategory("excl_level");
  plt.AddTH1("dilepton_mass", "Dilepton mass", 200, 0., 200., "excl_level");
  plt.AddTH1("dilepton_pt", "Dilepton p_{T}", 200, 0., 200., "excl_level");
  plt.AddTH1("dilepton_xi", "Dilepton #xi", 200, 0., 1., "excl_level");
  

  /*plt.AddCategory("elastic_level");

  plt.AddCategory("elastic_tight_level");*/

  TLorentzVector l1, l2, gen_tmp, l1_gen, l2_gen;
  unsigned int i = 0;
  while (tr.Next()) {
    i++;
    //if (i>=25000) break;
    //cout << ">>>>>>>>>>>>>>>> NEW EVENT <<<<<<<<<<<<<<<" << endl;
    plt.FillTH1(*nPrimVertexCand, "num_vertices_all", "presel_level");

    bool has_gen_match_l1 = false, has_gen_match_l2 = false;
    for (unsigned j=0; j<*nPrimVertexCand; j++) {
      plt.FillTH1(PrimVertexCand_tracks[j], "num_tracks_all", "presel_level");
      if (!PrimVertexCand_hasdil[j]) continue;
      plt.FillTH1(Pair_mass[j], "dilepton_mass", "vtxing_level");
      const unsigned int l1_id = Pair_lepton1[j], l2_id = Pair_lepton2[j];
      l1.SetPtEtaPhiM(MuonCand_pt[l1_id], MuonCand_eta[l1_id], MuonCand_phi[l1_id], muon_mass);
      l2.SetPtEtaPhiM(MuonCand_pt[l2_id], MuonCand_eta[l2_id], MuonCand_phi[l2_id], muon_mass);
      plt.FillTH2((l1+l2).M(), Pair_mass[j], "dilepton_mass_ctl", "vtxing_level");
      ////////////
      // reconstruct xi from the central system using relation in [http://journals.aps.org/prl/abstract/10.1103/PhysRevLett.102.222002]
      const double xi_reco_1 = (l1.Pt()*exp( l1.Eta())+l2.Pt()*exp( l2.Eta()))/sqrt_s,
                   xi_reco_2 = (l1.Pt()*exp(-l1.Eta())+l2.Pt()*exp(-l2.Eta()))/sqrt_s;
      ////////////
      plt.FillTH1(xi_reco_1, "xi_reco_1", "vtxing_level");
      plt.FillTH1(xi_reco_2, "xi_reco_2", "vtxing_level");
      if (PrimVertexCand_tracks[j]==2) { // exclusive level
        plt.FillTH1((l1+l2).M(), "dilepton_mass", "excl_level");
        plt.FillTH1((l1+l2).Pt(), "dilepton_pt", "excl_level");
      }
    }
    cout << "gen matching? " << has_gen_match_l1 << " / " << has_gen_match_l2 << endl;
  }


  return 0;
}
