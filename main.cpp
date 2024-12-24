#define NCPP_EXCEPTIONS_PLEASE

#include <thread>
#include <chrono>
#include <mutex>

#include <ncpp/NotCurses.hh>

#include "utils.hpp"
#include "apikeygamestate.h"

#include "nclineedit.hpp"

#define FRAME_TARGET 60.

std::mutex ncmtx;

int main() {
	const std::chrono::milliseconds frameTarget(static_cast<int>(1000.0/FRAME_TARGET));

	if (!setlocale(LC_ALL, ""))
		return 1;

	notcurses_options ncopts{};
    ncopts.flags = NCOPTION_INHIBIT_SETLOCALE; // | NCOPTION_NO_ALTERNATE_SCREEN;
    ncpp::NotCurses nc(ncopts);

	ApiKeyGameState gs(&nc, nullptr);
    NCLineEdit l(&nc);

    std::atomic<bool> gameover = false;
    // io loop (gets its own thread)
    auto ioctl = [&] {
        forever {
            char32_t ch = 0;
            ncinput ni;
            ch = nc.get(true, &ni);

            if (ch == 'q' and !l.focused()) {
                gameover = true;
                return;
            }

            if (ni.evtype == ncpp::EvType::Release)
                continue;

            if (ch == 'L' && ni.ctrl)
                nc.refresh(nullptr, nullptr);

            if (l.focused()) {
                ncmtx.lock();

                if (ch == 0x7F or ch == 0x08 or ch == NCKEY_BACKSPACE) {
                    l.backspace();
                }
                if (ch >= 0x20 and ch <= 0x10FFFF) {
                    l.putch(ch);
                }

                ncmtx.unlock();
            }
        }
    };

    std::thread listen(ioctl);

    // main loop
	forever
    {
        if (gameover)
            break;
        std::chrono::time_point<std::chrono::high_resolution_clock> frameStart = std::chrono::high_resolution_clock::now();

        l.update();
		gs.update();
        nc.render();

        std::chrono::time_point<std::chrono::high_resolution_clock> frameEnd = std::chrono::high_resolution_clock::now();
        std::chrono::milliseconds elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(frameEnd - frameStart);

        if (elapsed < frameTarget) {
            std::this_thread::sleep_for(frameTarget - elapsed);
        }
    }

    listen.join();


	return nc.stop() ? 0 : -1;
}
