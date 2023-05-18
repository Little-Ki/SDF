#pragma once

#include <vector>
#include <string>

using byte = unsigned char;

struct Color
{
    byte r, g, b;
};

class Bitmap
{
public:
    Bitmap(uint32_t width, uint32_t height, bool flip_x  = false, bool flip_y = false);

    const Color &get(uint32_t x, uint32_t y);

    void set(uint32_t x, uint32_t y, Color c);

    void save(std::string path);

    uint32_t width() { return m_width; }

    uint32_t height() { return m_height; }

private:
    std::vector<Color> m_map;

    Color m_pad;

    uint32_t m_width, m_height;

    bool m_flip_x = 1;

    bool m_flip_y = 1;
};