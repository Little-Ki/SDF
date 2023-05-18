#include "bitmap.h"
#include "svpng.inc"

Bitmap::Bitmap(uint32_t width, uint32_t height, bool flip_x, bool flip_y) : m_flip_x(flip_x), m_flip_y(flip_y)
{
    m_width = width;
    m_height = height;
    m_map.resize(width * height);
}

const Color &Bitmap::get(uint32_t x, uint32_t y)
{
    if (m_flip_x) x = m_width - x - 1;
    if (m_flip_y) y = m_height - y - 1;

    if (x < m_width && y < m_height)
    {
        return m_map[y * m_width + x];
    }
    return m_pad;
}

void Bitmap::set(uint32_t x, uint32_t y, Color c)
{
    if (m_flip_x) x = m_width - x - 1;
    if (m_flip_y) y = m_height - y - 1;

    if (x < m_width && y < m_height)
    {
        m_map[y * m_width + x] = c;
    }
}

void Bitmap::save(std::string path)
{
    FILE *fp = fopen(path.c_str(), "wb");

    svpng(fp, m_width, m_height, reinterpret_cast<byte *>(m_map.data()), 0);

    fclose(fp);
}