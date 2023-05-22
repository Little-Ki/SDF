#include "utils.h"

std::u32string Utils::utf8Unicode(const std::string& str ) {
    std::wstring_convert<std::codecvt_utf8<char32_t>, char32_t> conv_utf8_utf32;
    auto unicode_codepoints = conv_utf8_utf32.from_bytes(str);
    return unicode_codepoints;
  }

  
  std::string Utils::loadFile(std::string& path) {
    std::ifstream stream(path);
    std::string str((std::istreambuf_iterator<char>(stream)), std::istreambuf_iterator<char>());
    return str;
  }