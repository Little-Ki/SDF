#include <iostream>

#include "bitmap.h"
#include "vec2.h"
#include "bezier.h"

#include <ft2build.h>
#include <freetype/freetype.h>

#include FT_FREETYPE_H
#include FT_GLYPH_H
#include FT_TYPES_H
#include FT_OUTLINE_H
#include FT_RENDER_H

#define CLAMP(x, y, z) (z < x ? x : (x > y ? y : z))

void drawLine(Bitmap &bm, int x0, int y0, int x1, int y1, float r = 1, float g = 0, float b = 0)
{
    int dx = abs(x1 - x0), sx = x0 < x1 ? 1 : -1;
    int dy = abs(y1 - y0), sy = y0 < y1 ? 1 : -1;
    int erro = (dx > dy ? dx : -dy) / 2;

    int col = 100;
    while (bm.set(x0, y0, {(byte)(CLAMP(0, 255, col) * r) , (byte)(CLAMP(0, 255, col) * g), (byte)(CLAMP(0, 255, col) * b)}), x0 != x1 || y0 != y1)
    {
        int e2 = erro;
        if (e2 > -dx)
        {
            erro -= dy;
            x0 += sx;
        }
        if (e2 < dy)
        {
            erro += dx;
            y0 += sy;
        }
        col += 10;
        if (x0 < 0 || y0 >= bm.width()) {
          break;
        }
        if (y0 < 0 || y0 >= bm.height()) {
          break;
        }
    }
}
  static void render(int y,
                        int count,
                        const FT_Span_ *spans,
                        void *user)
  {
    Bitmap *bmp = (Bitmap *)user;
    y = -y;

    for (int i = 0; i < count; i++)
    {
      const FT_Span span = spans[i];
      auto opacity = float(span.coverage) / 255.0;

      if (span.len > 1)
        drawLine(*bmp, span.x, y, span.x + span.len - 1, y);
      else
        bmp->set(span.x, y, { 255,255,255});
    }
  }

float tofloat(signed long number, int e = 6) {
  auto c = abs(number);
  auto sign = 1;
  if (number < 0) {
    c = number - 1;
    c = ~c;
    sign = -1;
  }
   auto f = (1.0 * c) /  float(1 << e);
    f = f * sign;
    return f;
}

int main()
{

  auto bez = Bezier({
    vec2(10, 100),
    vec2(80, 10),
    vec2(10, 200),
    vec2(200, 10),
    vec2(250, 240)
    });

  auto pp = bez.at(0);
FT_Library library;
FT_Face face;
FT_Init_FreeType( &library );
FT_New_Face( library, "C:\\Windows\\Fonts\\msyh.ttc", 0,&face );
FT_Set_Pixel_Sizes(face,0, 128);

int index = FT_Get_Char_Index(face, L'龍');

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

  auto bmp = Bitmap(256, 256);
    for ( int xx = 0 ; xx < bitmap.width ; ++xx )
  {
    for ( int yy = 0 ; yy < bitmap.rows ; ++yy )
    {
      unsigned char r = bitmap.buffer[(yy*(bitmap.width)+xx)];

      bmp.set(xx, yy, {r,r,r});
    }
  }
// FT_Load_Glyph(face, index, FT_LOAD_DEFAULT);

//  FT_GlyphSlot pGlyphSlot = face->glyph;



// FT_Outline* outline = &pGlyphSlot->outline;

// char*       tags;

// FT_Vector  v_last;

// FT_Vector  v_control;

// FT_Vector  v_start;


//             FT_Pos left = face->glyph->metrics.horiBearingX;
//             FT_Pos right = left + face->glyph->metrics.width;
//             FT_Pos top = face->glyph->metrics.horiBearingY;
//             FT_Pos bottom = top - face->glyph->metrics.height;

//   auto bmp = Bitmap(256, 256, false, true);

// int first = 0;

// for (auto i =0 ; i < outline->n_contours; i++) {
//     int  last =outline->contours[i];
//     auto limit= outline->points + last;
//     auto point = outline->points + first;

//     v_start= outline->points[first];
//     v_last  = outline->points[last];
//     v_control= v_start;
//     tags  = outline->tags  + first;

//     char tag   = FT_CURVE_TAG(tags[0]);

//     float fpriX = tofloat(v_control.x);

//     float fpriY = tofloat(v_control.y);

 

//    float startX = fpriX;

//    float startY = fpriY;

//    while(point < limit)

//    {

//       point++;

//       tags++;

//       tag = FT_CURVE_TAG(tags[0]);

//       if (tag ==  FT_CURVE_TAG_ON) {

//         float fEndX = tofloat(point->x);

//         float fEndY = tofloat(point->y);
//         drawLine(bmp, startX, startY, fEndX, fEndY);
//         startX = fEndX;
//         startY = fEndY;
//       }
//    }
// }
//   for(float i = 0.0; i < 1.0; i += 0.01) {
//     auto p = bez.at(i);
//     drawLine(bmp, pp.point.x, pp.point.y, p.point.x, p.point.y);
//     drawLine(bmp, p.point.x, p.point.y, p.point.x + p.normal.x * 10.f, p.point.y + p.normal.y * 10.f, 0, 1);

//     //bmp.set(p.x, p.y, {255,0,0});
//     pp = p;
//   }

   bmp.save("test.png");
}