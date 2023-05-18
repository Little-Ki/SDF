#include "ft_proxy.h"

FT_Library FTProxy::ft_library;

bool FTProxy::initialize(FT_Error* out) {
  FT_Error error =FT_Init_FreeType( &ft_library );
  if (out != nullptr) {
    *out = error;
  }
  return error == 0;
}

bool FTProxy::load(const std::string& font) {
  m_error = FT_New_Face( ft_library, font.c_str(), 0, &m_face );
  return m_error == 0;
}

std::string FTProxy::lastError() {
  return std::string(FT_Error_String(m_error));
}

void FTProxy::setSize(unsigned int width,unsigned int height) {
  FT_Set_Pixel_Sizes(m_face, width, height);
}

int FTProxy::charIndex(wchar_t symbol) {
  return FT_Get_Char_Index(m_face, symbol);
}

bool FTProxy::getGlyph(int index) {
  m_error = FT_Load_Glyph(m_face, index, FT_LOAD_DEFAULT|FT_LOAD_NO_BITMAP);
  return m_error == 0;
}

bool FTProxy::getGlyph(wchar_t symbol) {
  auto idx = charIndex(symbol);
  return getGlyph(idx);
}

void FTProxy::renderSDF() {
  FT_Render_Glyph( m_face->glyph , FT_RENDER_MODE_SDF );
}