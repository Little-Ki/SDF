#include <iostream>

#include "bitmap.h"
#include "vec2.h"
#include "bezier.h"

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

int main()
{
  auto bmp = Bitmap(256, 256);
  auto bez = Bezier({
    vec2(10, 100),
    vec2(80, 10),
    vec2(10, 200),
    vec2(200, 10),
    vec2(250, 240)
    });

  auto pp = bez.at(0);
  for(float i = 0.0; i < 1.0; i += 0.01) {
    auto p = bez.at(i);
    drawLine(bmp, pp.point.x, pp.point.y, p.point.x, p.point.y);
    drawLine(bmp, p.point.x, p.point.y, p.point.x + p.normal.x * 10.f, p.point.y + p.normal.y * 10.f, 0, 1);

    //bmp.set(p.x, p.y, {255,0,0});
    pp = p;
  }

  bmp.save("test.png");
}