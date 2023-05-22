#pragma once
#include <vector>
#include <fstream>

template<size_t C> 
class PNG {
  static_assert(C <= 4 && C >= 1);
public:
  
  PNG(uint32_t w, uint32_t h) : m_width(w), m_height(h) {
    m_map.resize(w * h);
    clear();
  }

  uint32_t width() { return m_width; }

  uint32_t height() { return m_height; }

  unsigned char* raw() { return (unsigned char*) m_map.data(); }

  template<size_t N = 0, typename ...A>
  void get(uint32_t x, uint32_t y, uint8_t& o, A& ...args) {
    static_assert(N < C);
    if (x < m_width && y < m_height) {
      _T& ref = m_map[x + y * m_width];
      o = ref.val[N];
      get<N + 1>(x, y, args...);
    }
  }

  template<size_t N = 0>
  void get(uint32_t x, uint32_t y, uint8_t& o) {
    static_assert(N < C);
    if (x < m_width && y < m_height) {
      _T& ref = m_map[x + y * m_width];
      o = ref.val[N];
    }
  }

  template<size_t N = 0, typename ...A>
  void set(uint32_t x, uint32_t y, uint8_t in, A ...args) {
    static_assert(N < C);
     if (x < m_width && y < m_height) {
      _T& ref = m_map[x + y * m_width];
      ref.val[N] = in;
      set<N + 1>(x, y, args...);
    }
  }


  template<size_t N = 0>
  void set(uint32_t x, uint32_t y, uint8_t in) {
    static_assert(N < C);
    if (x < m_width && y < m_height) {
      _T& ref = m_map[x + y * m_width];
      ref.val[N] = in;
    }
  }


  template<size_t N = 0, typename ...A>
  void clear(uint8_t in, A ...args) {
    static_assert(N < C);
    for(auto i = m_map.begin(); i != m_map.end(); i++) {
      i->val[N] = in;
    }
    clear<N + 1>(args...);
  }

  template<size_t N = 0>
  void clear(uint8_t in) {
    static_assert(N < C);
    for(auto i = m_map.begin(); i != m_map.end(); i++) {
      i->val[N] = in;
    }
  }

  void clear() {
    std::fill(m_map.begin(), m_map.end(), _T { 0 });
  }

  bool save(const std::string& path) {
    #define CRC_U8(_)   { c ^= (_); c = (c >> 4) ^ t[c & 15]; c = (c >> 4) ^ t[c & 15]; }
    #define CRC_U8A(_)  { for(auto i = 0; i < sizeof(_) - 1; i++) CRC_U8(_[i]); }
    
    #define PUT_U8(_, c)  { unsigned char v = _; o.write((const char*) &v, 1); if(c) CRC_U8(_); }
    #define PUT_U32(_, c) { PUT_U8(_ >> 24, c); PUT_U8((_ >> 16) & 0xff, c); PUT_U8((_ >> 8) & 0xff, c); PUT_U8(_ & 0xff, c); }
    #define PUT_U8A(_, c) { o.write(_, sizeof(_) - 1); if(c) CRC_U8A(_); }

    #define CHUNK_BEGIN(name, size) { PUT_U32(size, false); c = ~0U; PUT_U8A(name, true); }
    #define CHUNK_END() { PUT_U32(~c, false); }

    static const unsigned t[] = { 
      0x00000000, 0x1db71064, 0x3b6e20c8, 0x26d930ac, 
      0x76dc4190, 0x6b6b51f4, 0x4db26158, 0x5005713c, 
      0xedb88320, 0xf00f9344, 0xd6d6a3e8, 0xcb61b38c, 
      0x9b64c2b0, 0x86d3d2d4, 0xa00ae278, 0xbdbdf21c 
    }; /* CRC32 Table */ 

    std::ofstream o(path, std::ios::out | std::ios::binary | std::ios::trunc);
    unsigned int c = 0, a = 1, b = 0, p = m_width * C + 1;

    if (!o.good()) {
      std::cout << "Ofstream failed.";
      return false;
    }
    
    PUT_U8A("\x89PNG\r\n\32\n", false);

    CHUNK_BEGIN("IHDR", 13);
    PUT_U32(m_width, true); 
    PUT_U32(m_height, true);
    PUT_U8(8, true);
    char type[] = { 0, 4, 2, 6 };
    PUT_U8(type[C - 1], true);
    PUT_U8A("\0\0\0", true);
    CHUNK_END();

    CHUNK_BEGIN("IDAT", 2 + m_height * (5 + p) + 4);
    PUT_U8A("\x78\1", true);
    auto img = (const unsigned char*) m_map.data();
    for(auto y = 0; y < m_height; y++) {
      PUT_U8(y == m_height - 1, true);
      PUT_U8(p & 0xff, true);
      PUT_U8((p >> 8) & 0xff, true);
      PUT_U8(~p & 0xff, true);
      PUT_U8(((~p) >> 8) & 0xff, true);
      PUT_U8(0, true);
      a = (a + 0) % 65521; 
      b = (b + a) % 65521; 
      for (auto x = 0; x < p - 1; x++, img++) {
        PUT_U8(*img, true);
        a = (a + (*img)) % 65521; 
        b = (b + a) % 65521; 
      }
    }

    PUT_U32(((b << 16) | a), true);
    CHUNK_END();

    CHUNK_BEGIN("IEND", 0);
    CHUNK_END();

    o.close();
    return true;
  }
private:

  struct _T {
    uint8_t val[C];
  };

  uint32_t m_width;

  uint32_t m_height;
  
  bool m_alpha;

  std::vector<_T> m_map;

};