#define NCPP_EXCEPTIONS_PLEASE

#include <thread>
#include <chrono>

#include <ncpp/NotCurses.hh>

#include "utils.hpp"
#include "apikeygamestate.h"

#define FRAME_TARGET 60.0

int main() {
	const std::chrono::milliseconds frameTarget(static_cast<int>(1000.0/FRAME_TARGET));

	if (!setlocale(LC_ALL, ""))
		return 1;

	notcurses_options ncopts{};
    ncopts.flags = NCOPTION_INHIBIT_SETLOCALE; // | NCOPTION_NO_ALTERNATE_SCREEN;
    ncpp::NotCurses nc(ncopts);

	ApiKeyGameState gs(&nc, nullptr);
	forever
    {
        std::chrono::time_point<std::chrono::high_resolution_clock> frameStart = std::chrono::high_resolution_clock::now();

		gs.update();
        nc.render();

        std::chrono::time_point<std::chrono::high_resolution_clock> frameEnd = std::chrono::high_resolution_clock::now();
        std::chrono::milliseconds elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(frameEnd - frameStart);

        if (elapsed < frameTarget) {
            std::this_thread::sleep_for(frameTarget - elapsed);
        }
    }

	return nc.stop() ? 0 : -1;
}
