#ifndef FillNumberLUTHandler_h
#define FillNumberLUTHandler_h

#include <fstream>

class FillNumberLUTHandler
{
 public:
  inline FillNumberLUTHandler(const char* file="test/fill_run_lut.dat"):
    fInvalid(true) {
    std::ifstream f(file);
    if (!f.is_open()) return;
    std::string ss;
    while (f >> ss) {
      size_t pos = ss.find(":");
      if (pos==std::string::npos) continue;
      unsigned int run = atoi(ss.substr(0, pos).c_str()), fill = atoi(ss.substr(pos+1, ss.size()).c_str());
      fFillInRun[run] = fill;
    }
    fInvalid = false;
  }

  inline unsigned int GetFillNumber(unsigned int run) const {
    if (fFillInRun.count(run)==0) return 0;
    return fFillInRun.at(run);
  }

  inline bool IsValid() const { return !fInvalid; }

 private:
  bool fInvalid;
  std::map<unsigned int,unsigned int> fFillInRun;
};

#endif
