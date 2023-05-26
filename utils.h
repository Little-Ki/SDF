#include <iostream>
#include <fstream>

#include <codecvt> 
#include <locale> 
#include <vector>

namespace Utils {

  std::vector<std::pair<std::string, std::u32string>> utf8Unicode(const std::string& str) ;
  
  std::vector<std::string> splitUtf8(const std::string& str);

  std::string loadFile(std::string& path);

}