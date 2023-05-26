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


  auto imageSize = program.get<int>("imgsize", 2048);
  auto fontSize = program.get<int>("fontsize", 64);
  auto outFolder = program.get<std::string>("output", "");
  auto png = PNG<1>(imageSize, imageSize);
  long baseline = 0;

  auto baseX = 0U, baseY = 0U;
  auto nextY = 0U;

  FT_Library library;
  FT_Face face;
  HANDLE_ERR(FT_Init_FreeType(&library));
  HANDLE_ERR(FT_New_Face(library, program.get<std::string>("font", "").c_str(), 0, &face));
  HANDLE_ERR(FT_Set_Pixel_Sizes(face, 0, fontSize));

  for(auto& u : ucode) {
    FT_UInt index;
    index = FT_Get_Char_Index(face, u.second[0]);
    HANDLE_ERR(FT_Load_Glyph(face, index , FT_LOAD_DEFAULT));
    const auto& metrics = face->glyph->metrics;
    auto hang = (metrics.height - metrics.horiBearingY) / 64;
    baseline = std::max(baseline, hang);
  }

  unsigned int outIndex = 0;
  unsigned int baseHeight = fontSize + 10;
  unsigned int cellHeight = fontSize + 16 + baseline;
  std::ofstream outtext(outFolder + "index.txt", std::ios::out | std::ios::trunc);
  
  for(auto& u : ucode) {
    FT_UInt index;
    index = FT_Get_Char_Index(face , u.second[0]);
    HANDLE_ERR(FT_Load_Glyph(face, index , FT_LOAD_DEFAULT));
    HANDLE_ERR(FT_Render_Glyph(face->glyph , FT_RENDER_MODE_SDF));
    const auto& bitmap = face->glyph->bitmap;
    const auto& metrics = face->glyph->metrics;
    auto hang = (metrics.height - metrics.horiBearingY) / 64;

    auto img_width = bitmap.width;
    auto img_height = bitmap.rows;

    if (baseX + img_width > imageSize) {
      baseX = 0;
      baseY += cellHeight;
    }

    if (baseY + cellHeight > imageSize) {
      png.save(outFolder + convert<std::string>(outIndex) + ".png");
      png.clear();
      outIndex++;
      baseX = 0;
      baseY = 0;
    }

    auto x = baseX, y = baseY;

    if (outtext.good()) {
      outtext << outIndex << '\t' << u.first << '\t' << u.second[0] << '\t' << x << '\t' << y << '\t' << img_width <<  '\t' << cellHeight << '\n';
    }

    y += baseHeight - img_height + hang;
    
    for ( int xx = 0 ; xx < bitmap.width ; ++xx )
    {
      for ( int yy = 0 ; yy < bitmap.rows ; ++yy )
      {
        unsigned char r = bitmap.buffer[(yy*(bitmap.width)+xx)];
        png.set(x + xx, y + yy, r);
      }
    }

    baseX += img_width;
  }

  png.save(outFolder + convert<std::string>(outIndex) + ".png");
  
  HANDLE_ERR(FT_Done_FreeType(library));
  HANDLE_ERR(FT_Done_Face(face));
  
  std::cout << "Done\n";

  std::getchar();

  return 0;
}