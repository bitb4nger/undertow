// Headless Ardens driver: load a .hex + FX .bin, run a button script, dump PBM screenshots.
// Usage: drive <hex> <fxbin> <fxport d1|e2> <script>
//   script tokens:  w<ms> wait   A/B/U/D/L/R tap (100 ms down, 100 ms up)   s<name> screenshot
#include "absim.hpp"
#include <cstdio>
#include <fstream>
#include <memory>
#include <string>
#include <sstream>
using namespace absim;
static std::unique_ptr<arduboy_t> E;
static uint8_t pinb_, pine_, pinf_;
static void setPins() {
  E->core_state.cpu.data[reg::addr::PINB] = pinb_;
  E->core_state.cpu.data[reg::addr::PINE] = pine_;
  E->core_state.cpu.data[reg::addr::PINF] = pinf_;
}
static void run(uint64_t ms) {
  uint64_t end = E->core_state.cpu.cycle_count + ms * 16000ULL;
  while (E->core_state.cpu.cycle_count < end) { setPins(); E->cycle(); }
}
static void release() {
  pinb_ = reg::bit::PINB::PINB4; pine_ = reg::bit::PINE::PINE6;
  pinf_ = reg::bit::PINF::PINF7 | reg::bit::PINF::PINF6 | reg::bit::PINF::PINF5 | reg::bit::PINF::PINF4;
}
static void shot(const std::string& name) {
  auto& D = E->peripherals.display;                    // last fully drawn frame, row-major as shown
  auto& px = D.pixels[D.enable_filter ? (D.pixel_history_index + display_t::MAX_PIXEL_HISTORY - 1) % display_t::MAX_PIXEL_HISTORY : 0];
  FILE* f = fopen((name + ".pbm").c_str(), "w");
  fprintf(f, "P1\n128 64\n");
  for (int y = 0; y < 64; y++) { for (int x = 0; x < 128; x++) fputc(px[y * 128 + x] > 127 ? '1' : '0', f); fputc('\n', f); }
  fclose(f);
  printf("shot %s  pc=%04x sp=%04x cycles=%llu\n", name.c_str(), E->core_state.cpu.pc * 2,
         E->core_state.cpu.sp(), (unsigned long long)E->core_state.cpu.cycle_count);
}
int main(int argc, char** argv) {
  if (argc < 5) { fprintf(stderr, "usage\n"); return 2; }
  E = std::make_unique<arduboy_t>();
  std::string port = argv[3];
  if (port == "e2") { E->program_state.cfg.fxport_reg = reg::addr::PORTE; E->program_state.cfg.fxport_mask = reg::bit::PORTE::PORTE2; }
  for (int i = 1; i <= 2; i++) {
    std::ifstream f(argv[i], std::ios::binary);
    std::string err = E->load_file(argv[i], f);
    if (!err.empty()) { fprintf(stderr, "%s: %s\n", argv[i], err.c_str()); return 1; }
  }
  if (port == "e2") { E->program_state.cfg.fxport_reg = reg::addr::PORTE; E->program_state.cfg.fxport_mask = reg::bit::PORTE::PORTE2; E->reset(); }
  E->core_state.cpu.enabled_autobreaks.reset();
  E->debugger_state.paused = false;
  release();
  std::istringstream sc(argv[4]); std::string t;
  while (sc >> t) {
    char c = t[0];
    if (c == 'w') run(std::stoull(t.substr(1)));
    else if (c == 's') shot(t.substr(1));
    else {
      release();
      if (c == 'A') pine_ &= ~reg::bit::PINE::PINE6;
      if (c == 'B') pinb_ &= ~reg::bit::PINB::PINB4;
      if (c == 'U') pinf_ &= ~reg::bit::PINF::PINF7;
      if (c == 'R') pinf_ &= ~reg::bit::PINF::PINF6;
      if (c == 'L') pinf_ &= ~reg::bit::PINF::PINF5;
      if (c == 'D') pinf_ &= ~reg::bit::PINF::PINF4;
      run(100); release(); run(100);
    }
  }
  return 0;
}
