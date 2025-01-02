#define NCPP_EXCEPTIONS_PLEASE

#include <thread>
#include <chrono>
#include <mutex>
#include <iostream>

#include <ncpp/NotCurses.hh>

#include "utils.hpp"
#include "apikeygamestate.h"

#include "nclineedit.hpp"

#define FRAME_TARGET 60.

std::mutex ncmtx;
bool mouse_supported = true;

int main() {
	const std::chrono::milliseconds frameTarget(static_cast<int>(1000.0/FRAME_TARGET));

    // clear the screen with ANSI magic
    util::magic::cls();

	if (!setlocale(LC_ALL, ""))
		return 1;

	notcurses_options ncopts{};
    ncopts.flags = NCOPTION_INHIBIT_SETLOCALE | NCOPTION_SUPPRESS_BANNERS  | NCOPTION_NO_ALTERNATE_SCREEN;
    ncpp::NotCurses nc(ncopts);

    // Attempt to enable mouse. If not, thats alright!
    try {
        nc.mouse_enable(NCMICE_ALL_EVENTS);
    }
    catch (ncpp::call_error &e) {
        mouse_supported = false;
    }

	GameState *gs = new ApiKeyGameState(&nc, nullptr, &ncmtx);

    std::atomic<bool> gameover = false;
    // io loop (gets its own thread). TODO: Migrate this to its own class.
    auto ioctl = [&] {
        forever {
            char32_t ch = 0;
            ncinput ni;

            ch = nc.get(true, &ni);
            ncmtx.lock();

            if (ch == 'q' and !gs->block_fortype()) {
                gameover = true;
                return;
            }

            if (ch == 'L' && ni.ctrl) {
                nc.refresh(nullptr, nullptr);
                return;
            }

            gs->handle_event(ni, ch);
            ncmtx.unlock();
        }
    };

    std::thread listen(ioctl);

    // main loop
	forever
    {
        if (gameover)
            break;
        std::chrono::time_point<std::chrono::high_resolution_clock> frameStart = std::chrono::high_resolution_clock::now();

        ncmtx.lock();
		gs->update();
        nc.render();
        ncmtx.unlock();

        std::chrono::time_point<std::chrono::high_resolution_clock> frameEnd = std::chrono::high_resolution_clock::now();
        std::chrono::milliseconds elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(frameEnd - frameStart);

        if (elapsed < frameTarget) {
            std::this_thread::sleep_for(frameTarget - elapsed);
        }
    }

    listen.join();

    if (mouse_supported)
        nc.mouse_disable();
    int EXIT_CODE = nc.stop() ? 0 : -1;
    delete gs;
	return EXIT_CODE;
}
