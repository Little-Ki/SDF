#include "utils.h"

std::vector<std::string> Utils::splitUtf8(const std::string& str) {
  std::vector<std::string> result;
  auto data = (const unsigned char*)str.data();
  auto size = str.size();
  for(auto i = 0; i < size;) {
    auto c = data[i];
    auto len = 0;
    if ((c & 0xF8) == 0xF0) {
      len = 4;
    } else if ((c & 0xF0) == 0xE0) {
      len = 3;
    } else if ((c & 0xE0) == 0xC0) {
      len = 2;
    } else if ((c & 0x80) == 0x00) {
      len = 1;
    }
    if (len == 0) {
      i += 1;
    } else {
      result.push_back(str.substr(i, len));
      i += len;
    }
  }
  return result;
}

std::vector<std::pair<std::string, std::u32string>> Utils::utf8Unicode(const std::string& str) {
  auto list = splitUtf8(str);
  std::vector<std::pair<std::string, std::u32string>> result;
   std::wstring_convert<std::codecvt_utf8<char32_t>, char32_t> converter;

  for(auto &i : list) {
    result.push_back(
      std::make_pair(
        i,
        converter.from_bytes(i)
      )
    );
  }
  
 // return converter.from_bytes(str);
  return result;
}


std::string Utils::loadFile(std::string& path) {
  std::ifstream stream(path);
  std::string str((std::istreambuf_iterator<char>(stream)), std::istreambuf_iterator<char>());
  return str;
}