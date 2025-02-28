#define NCPP_EXCEPTIONS_PLEASE

#include <thread>
#include <chrono>
#include <mutex>

#include <ncpp/NotCurses.hh>

#include "maingamestate.h"
#include "apikeygamestate.h"
#include "ioctl.hpp"
#include "utils.hpp"
#include "securestorepass.hpp"

#define FRAME_TARGET 60.

std::mutex ncmtx;
bool mouse_supported = true;

int main() {
	const std::chrono::milliseconds frameTarget(static_cast<int>(1000.0/FRAME_TARGET));

    // clear the screen with ANSI magic

    util::magic::cls();

	if (!setlocale(LC_ALL, ""))
		return 1;

    // Define Instance of notcurses.
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

    // Load apikey for user.
    SecureStorePass *user_profile = new SecureStorePass();

	GameState *gs = user_profile->valid() ?
          static_cast<GameState*>(new MainGameState(&nc, nullptr, &ncmtx, user_profile))
        : static_cast<GameState*>(new ApiKeyGameState(&nc, nullptr, &ncmtx, user_profile));


    IOCtl *ioctl = new IOCtl(&nc, &ncmtx, &gs, user_profile);
    std::thread listen(&IOCtl::loop, ioctl);

    // main loop
	forever
    {
        std::chrono::time_point<std::chrono::high_resolution_clock> frameStart = std::chrono::high_resolution_clock::now();

        ncmtx.lock();

        if (ioctl->gameover()) {
            ncmtx.unlock();
            break;
        }

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

    delete gs;
    delete ioctl;
    delete user_profile;
    int EXIT_CODE = nc.stop() ? 0 : -1;

	return EXIT_CODE;
}
