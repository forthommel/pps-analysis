#ifndef TreeLoader_h
#define TreeLoader_h

#include "TFile.h"
#include "TTree.h"

#include <iostream>

class TreeLoader
{
 public:
  TreeLoader(const char* file, const char* tree_name="ntp1");
  ~TreeLoader();

  inline bool IsValid() const {
    if (!fFile or !fFile->IsOpen()) return false;
    if (!fTree) return false;
    return true;
  }

  inline unsigned int GetEntries() const {
    if (!fTree) return 0;
    return fTree->GetEntries();
  }
  inline void GetEntry(unsigned long i) {
    if (!fTree) return;
    fTree->GetEntry(i);
  }
  inline void SetBranchAddress(const char* bname, void* add) {
    if (!fTree) return;
    fTree->SetBranchStatus(bname, 1);
    int ret =fTree->SetBranchAddress(bname, add);
    if (ret!=0) { std::cerr << "ERROR failed to set address for branch " << bname << ": return value=" << ret << std::endl; return; }
  }

 private:
  TFile* fFile;
  TTree* fTree;
};

#endif
