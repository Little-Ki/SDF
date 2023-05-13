#pragma once

#include <freetype/freetype.h>
#include "string"

class FTProxy {
  
public:

  static bool initialize(FT_Error* out = nullptr);

  bool load(const std::string& font);

  void setSize(unsigned int width,unsigned int height);
  
  int charIndex(wchar_t symbol);

  bool getGlyph(int index);
  
  bool getGlyph(wchar_t symbol);

  std::string lastError();

private:

  static FT_Library ft_library;

  FT_Error m_error;

  FT_Face m_face;
};