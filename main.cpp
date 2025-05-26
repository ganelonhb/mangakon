#include <argparse/argparse.hpp>
#include <cstdio>
#include <cerrno>
#include <ncpp/NotCurses.hh>
#include <mutex>

#include "frametimer.h"
#include "mousemanager.h"

// CONTANTS
constexpr const char *  PROGRAM_NAME        = "mangakon";
constexpr const char *  PROGRAM_VERSION     = "0.0.1";
constexpr const char *  PROGRAM_DESC        = "read manga directly in the terminal";

// CONSTINITS
constinit double        FRAME_RATE          =  60.0;

int main(int argc, const char **argv) {
    std::mutex ncMutex;

    argparse::ArgumentParser parser(
        PROGRAM_NAME,
        PROGRAM_VERSION,
        argparse::default_arguments::help
    );

    parser.add_description(PROGRAM_DESC);

    parser.add_argument("-v", "--version")
        .help("prints version information and exits")
        .flag();

    parser.add_argument("--framerate")
        .help("set a target framerate")
        .default_value(double{FRAME_RATE})
        .store_into(FRAME_RATE);

    try {
        parser.parse_args(argc, argv);
    } catch (const std::exception &err) {
        std::printf(
            "Error: %s\n\n%s",
                err.what(),
                parser.help().str().c_str()
        );
        return EINVAL;
    }

    if (parser["--version"] == true) {
        std::printf("%s v%s\n", PROGRAM_NAME, PROGRAM_VERSION);
        return EXIT_SUCCESS;
    }

    // Return to home (prep for framebuffer mode)
    std::printf("\033[2J\033[H");
    std::fflush(stdout);

    if (!setlocale(LC_ALL, "")) {
        std::fprintf(stderr, "Error: Could not set the locale.\n");
        return ENOENT;
    }

    // ┉┉┉┉┉┉┉┉┉┉┉┉┉┉┉┉┉┉┉┉┉┉┉┉┉
    // NotCurses domain ahead...
    // ┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┓
    // ┃                                                                            ┃
    // ┃  WARNING: DO NOT USE the std::printf family or std::cout from here on out. ┃
    // ┃                                                                            ┃
    // ┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┛
    // 🕱 YOU HAVE BEEN WARNED 🕱
    // ┉┉┉┉┉┉┉┉┉┉┉┉┉┉┉┉┉┉┉┉┉┉┉┉┉

    ncpp::NotCurses nc({
        .flags  = NCOPTION_INHIBIT_SETLOCALE
                | NCOPTION_SUPPRESS_BANNERS
                | NCOPTION_NO_ALTERNATE_SCREEN
    });

    FrameTimer timer(FRAME_RATE);
    MouseManager *mouseManager = new MouseManager(&nc);

    while(true) {
        timer.clock();
        ncMutex.lock();
        // gamestate update
        nc.render();
        ncMutex.unlock();
        timer.sync();
    }

    delete mouseManager;

    nc.get_stdplane()->putc('\n');

    int EXIT_CODE = nc.stop()
        ? EXIT_SUCCESS
        : EXIT_FAILURE;

    return EXIT_CODE;
}
