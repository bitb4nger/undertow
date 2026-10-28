// PC stand-in for ArduboyFX: reads fxdata-data.bin and draws from it.
#pragma once
#include "Arduboy2.h"
#include <vector>
#define __uint24 uint32_t
extern Arduboy2Base ab;
constexpr uint8_t dbmNormal = 0, dbmMasked = 16;
struct FX {
  static std::vector<uint8_t>& data() { static std::vector<uint8_t> d; return d; }
  static void begin(uint16_t) {
    const char* p = getenv("FXDATA"); if (!p) p = "../../fxdata/fxdata-data.bin";
    FILE* f = fopen(p, "rb"); if (!f) { fprintf(stderr, "no FX data at %s\n", p); exit(2); }
    auto& d = data(); d.clear(); int c; while ((c = fgetc(f)) != EOF) d.push_back(c); fclose(f);
  }
  static void drawBitmap(int16_t x, int16_t y, uint32_t addr, uint8_t frame, uint8_t mode) {
    auto& d = data();
    int w = d.at(addr) << 8 | d.at(addr + 1), h = d.at(addr + 2) << 8 | d.at(addr + 3);
    int pages = (h + 7) / 8; bool masked = mode & dbmMasked;
    size_t p = addr + 4 + (size_t)frame * w * pages * (masked ? 2 : 1);
    for (int pg = 0; pg < pages; pg++)
      for (int i = 0; i < w; i++) {
        uint8_t b = d.at(p++), m = masked ? d.at(p++) : 0xFF;
        for (int j = 0; j < 8; j++) {
          int yy = pg * 8 + j; if (yy >= h) break;
          if (m & (1 << j)) ab.drawPixel(x + i, y + yy, (b >> j) & 1);
        }
      }
  }
  static uint8_t readIndexedUInt8(uint32_t a, uint8_t i) { return data().at(a + i); }
  static uint16_t readIndexedUInt16(uint32_t a, uint8_t i) { a += i * 2; return data().at(a) << 8 | data().at(a + 1); }
  static uint32_t readIndexedUInt24(uint32_t a, uint8_t i) { a += i * 3; return data().at(a) << 16 | data().at(a + 1) << 8 | data().at(a + 2); }
  static void readDataBytes(uint32_t a, uint8_t* buf, size_t n) { for (size_t k = 0; k < n; k++) buf[k] = data().at(a + k); }
  static void display(bool clear) { memcpy(ab.shown, ab.px, sizeof ab.px); if (clear) ab.clear(); }
};
