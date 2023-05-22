#include <iostream>
#include <fstream>

#include <codecvt> 
#include <locale> 

namespace Utils {
  
  std::u32string utf8Unicode(const std::string& str ) ;

  std::string loadFile(std::string& path);

}