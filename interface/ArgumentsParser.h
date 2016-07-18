#ifndef ArgumentParser_h
#define ArgumentParser_h

#include <vector>
#include <string>
#include <sstream>

/// A simple input arguments parser;
// Shamelessely stolen from [http://stackoverflow.com/users/85381/iain]
class ArgumentsParser{
 public:
  inline ArgumentsParser(int &argc, char **argv) : fExecutable(argv[0]) {
    for (int i=1; i < argc; ++i) fTokens.push_back(std::string(argv[i]));
  }

  inline const char* Get(const std::string &option) const {
    std::vector<std::string>::const_iterator itr;
    itr = std::find(fTokens.begin(), fTokens.end(), option);
    if (itr!=fTokens.end() and ++itr!=fTokens.end()) return itr->c_str();
    return "";
  }

  inline const char* GetInput() const { return Get("--input"); }
  inline const char* GetOutput() const { return Get("--output"); }

  inline bool HasArgument(const std::string &option) const {
    return std::find(fTokens.begin(), fTokens.end(), option)!=fTokens.end();
  }

  inline void PrintUsage() const {
    std::ostringstream os;
    os << "Usage: " << fExecutable << " "
       << "--input [data.root] "
       << "--output [output.root]";
    std::cerr << os.str() << std::endl;
  }

 private:
  const char* fExecutable;
  std::vector<std::string> fTokens;
};

#endif
