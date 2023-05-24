#include <iostream>
#include <fstream>

#include <codecvt> 
#include <locale> 

namespace Utils {
  
  std::u32string utf8Unicode(const std::string& str ) ;

  std::string unicodeUtf8(const std::u32string& str );

  template<typename T>
  std::string unicodeUtf8(const T& in) {
    std::wstring_convert<std::codecvt_utf8<char32_t>, char32_t> converter;
    return converter.to_bytes(in);
  }

  std::string loadFile(std::string& path);

}