#include <iostream>

#include <ft2build.h>
#include <freetype/freetype.h>

#include "png.hpp"

#include FT_FREETYPE_H
#include FT_GLYPH_H
#include FT_TYPES_H
#include FT_OUTLINE_H
#include FT_RENDER_H


int main()
{

FT_Library library;
FT_Face face;
FT_Init_FreeType( &library );
FT_New_Face( library, "C:\\Windows\\Fonts\\msyh.ttc", 0,&face );
FT_Set_Pixel_Sizes(face,0, 256);

int index = FT_Get_Char_Index(face, 0x4f60);

auto error = FT_Load_Glyph(face, index, FT_LOAD_DEFAULT);
  if ( error )
  {
    std::cout << "BitmapFontGenerator > failed to load glyph, error: " << FT_Error_String(error) << std::endl;
    return 0;
  }

const FT_Glyph_Metrics& metrics = face->glyph->metrics;
int glyphHang = (metrics.horiBearingY - metrics.height) >> 6;

FT_Load_Glyph( face , index , FT_LOAD_DEFAULT );
FT_Render_Glyph( face->glyph , FT_RENDER_MODE_SDF );

  auto width = face->glyph->metrics.width >> 6;

  const FT_Bitmap& bitmap = face->glyph->bitmap;

  auto png = PNG<1>(300, 300);
    for ( int xx = 0 ; xx < bitmap.width ; ++xx )
  {
    for ( int yy = 0 ; yy < bitmap.rows ; ++yy )
    {
      unsigned char r = bitmap.buffer[(yy*(bitmap.width)+xx)];

      png.set(xx, yy, r);
    }
  }
   png.save("test.png");
}