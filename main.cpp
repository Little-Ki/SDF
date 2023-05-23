#include <iostream>

#include <ft2build.h>
#include <freetype/freetype.h>

#include "utils.h"
#include "png.hpp"
#include "arg_parser.h"

#include FT_FREETYPE_H
#include FT_GLYPH_H
#include FT_TYPES_H
#include FT_OUTLINE_H
#include FT_RENDER_H


template<typename T0, typename T1>
T0 convert(const T1& in) {
  std::stringstream ss;
  T0 val;
  ss << in;
  ss >> val;
  return val;
}

int main(int argc, char* argv[])
{
#define HANDLE_ERR(s) { auto error = (s); if(error != FT_Err_Ok) { std::cerr << FT_Error_String(error); return 1; } }

  ArgParser program;

  program
  .add("font", { "-f", "-font" }, true, true, "Path of font file.")
  .add("fontsize", { "-fs", "-fontsize" }, true, false, "Pixel size of font.")
  .add("imgsize", { "-s", "-size" }, true, false, "Output image size.")
  .add("input", { "-i", "-input" }, true, true, "Input words text.")
  .add("output", { "-o", "-output" }, true, false, "Output folder.")
  .add("help", { "-h", "-help", "-?" }, false, false, "Get argument info.");

  try {
    program.parse(argc, argv);
  }
  catch (const std::runtime_error& err) {
    std::cerr << err.what() << std::endl;
    return 1;
  }

  if (program.exists("help")) {
    std::cout << program.help();
    return 0;
  }

  auto str = Utils::loadFile(program.get<std::string>("input", ""));

  if (str.empty()) {
    std::cerr << "Empty input words.\n";
    return 1;
  }

  auto ucode = Utils::utf8Unicode(str);


  auto size = program.get<int>("imgsize", 2048);
  auto fsize = program.get<int>("fontsize", 64);
  auto outpath = program.get<std::string>("output", "");
  auto png = PNG<1>(size, size);
  long baseline = 0;

  auto baseX = 3U, baseY = 3U;
  auto nextY = 3U;

  FT_Library library;
  FT_Face face;
  HANDLE_ERR(FT_Init_FreeType(&library));
  HANDLE_ERR(FT_New_Face(library, program.get<std::string>("font", "").c_str(), 0, &face));
  HANDLE_ERR(FT_Set_Pixel_Sizes(face, 0, fsize));

  for(auto& u : ucode) {
    FT_UInt index;
    index = FT_Get_Char_Index(face, u );
    HANDLE_ERR(FT_Load_Glyph(face, index , FT_LOAD_DEFAULT));
    const auto& metrics = face->glyph->metrics;
    auto hang = (metrics.height - metrics.horiBearingY) / 64;
    baseline = std::max(baseline, hang);
  }

  auto outIndex = 0;
  
  for(auto& u : ucode) {
    FT_UInt index;
    index = FT_Get_Char_Index(face , u);
    HANDLE_ERR(FT_Load_Glyph(face, index , FT_LOAD_DEFAULT));
    HANDLE_ERR(FT_Render_Glyph(face->glyph , FT_RENDER_MODE_SDF));
    const FT_Bitmap& bitmap = face->glyph->bitmap;

    auto img_width = bitmap.width;
    auto img_height = bitmap.rows;

    if (baseX + img_width > size) {
      baseX = 1;
      baseY = nextY + 1;
    }

    if (baseY + img_height + fsize - face->glyph->bitmap_top > size) {
      png.save(outpath + convert<std::string>(outIndex) + ".png");
      png.clear();
      outIndex++;
      baseX = 1;
      baseY = 1;
      nextY = 1;
    }

    auto x = baseX, y = baseY;
    y += fsize - face->glyph->bitmap_top;
    
    for ( int xx = 0 ; xx < bitmap.width ; ++xx )
    {
      for ( int yy = 0 ; yy < bitmap.rows ; ++yy )
      {
        unsigned char r = bitmap.buffer[(yy*(bitmap.width)+xx)];
        png.set(x + xx, y + yy, r);
      }
    }

    baseX += img_width + 1;
    nextY = std::max(nextY, y + img_height);
  }

  png.save(outpath + convert<std::string>(outIndex) + ".png");

  return 0;
}