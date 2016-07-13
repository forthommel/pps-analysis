#include "TreeLoader.h"

#include <iostream>

TreeLoader::TreeLoader(const char* file, const char* tree_name):
  fFile(0), fTree(0)
{
  fFile = new TFile(file);
  if (!fFile->IsOpen()) {
    std::cerr << "ERROR while retrieving input file " << file << std::endl;
    return;
  }

  //fTree = (TTree*)(fFile->Get(tree_name)->Clone());
  (fFile->Get(tree_name))->Copy((TObject&)(*fTree));
  delete fFile;
  
  fTree->SetBranchStatus("*", 0);

  std::cout << ">> Loaded a ntuple with " << GetEntries() << " entries" << std::endl;
}

TreeLoader::~TreeLoader()
{
  if (fTree) delete fTree;
  if (fFile) delete fFile;
}
