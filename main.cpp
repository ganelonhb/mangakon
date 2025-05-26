#include <argparse/argparse.hpp>
#include <cstdio>
#include <cerrno>
#include <ncpp/NotCurses.hh>
#include <mutex>

#include <cinttypes>

#include "frametimer.h"
#include "mousemanager.h"
#include "util.h"

// CONTANTS
constexpr const char *  PROGRAM_NAME        = "mangakon";
constexpr const char *  PROGRAM_VERSION     = "0.0.1";
constexpr const char *  PROGRAM_DESC        = "read manga directly in the terminal";

// CONSTINITS
constinit double        FRAME_RATE          = 60.0;

int main(int argc, const char **argv) {
    const bool isKittyTerminal = util::kitty::isKittyTerminal();
    const util::smart_colors::TerminalColors terminalColors = util::smart_colors::getDefaultTermColors();

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

    parser.add_argument("-f", "--framerate")
        .help("set a target framerate for screen updates per second (fps)")
        .default_value(double{FRAME_RATE})
        .store_into(FRAME_RATE);

    parser.add_argument("-m", "--mascot")
        .help("enable the mascot")
        .flag();

    parser.add_argument("--terminfo")
        .help("print terminal compatibility information and exit")
        .flag();

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
    } else if (parser["--terminfo"] == true){
        // TODO: Add colors
        std::printf(
                "+-----------------------+\n"
                "|      Information      |\n"
                "+-----------------------+\n"
                "---General-Information---\n"
                "Program name:      %s\n"
                "Version:           v%s\n"
                "NotCurses Version: %s\n"
                "Multimedia Stack:  OpenImageIO\n"
                "-------------------------\n"
                "------Terminal-Type------\n"
                "Platform:          %s\n"
                "isKitty:           %s\n"
                "-----Terminal-Colors-----\n"
                "Black:             0x%" PRIx32 "\n"
                "Bright Black:      0x%" PRIx32 "\n"
                "Red:               0x%" PRIx32 "\n"
                "Bright Red:        0x%" PRIx32 "\n"
                "Green:             0x%" PRIx32 "\n"
                "Bright Green:      0x%" PRIx32 "\n"
                "Yellow:            0x%" PRIx32 "\n"
                "Bright Yellow:     0x%" PRIx32 "\n"
                "Blue:              0x%" PRIx32 "\n"
                "Bright Blue:       0x%" PRIx32 "\n"
                "Magenta:           0x%" PRIx32 "\n"
                "Bright Magenta:    0x%" PRIx32 "\n"
                "Cyan:              0x%" PRIx32 "\n"
                "Bright Cyan:       0x%" PRIx32 "\n"
                "White:             0x%" PRIx32 "\n"
                "Bright White:      0x%" PRIx32 "\n"
            , PROGRAM_NAME
            , PROGRAM_VERSION
            , notcurses_version()
            , util::platform::get_platform_string()
            , isKittyTerminal ? "yes" : "no"
            , terminalColors.black
            , terminalColors.bblack
            , terminalColors.red
            , terminalColors.bred
            , terminalColors.green
            , terminalColors.bgreen
            , terminalColors.yellow
            , terminalColors.byellow
            , terminalColors.blue
            , terminalColors.bblue
            , terminalColors.magenta
            , terminalColors.bmagenta
            , terminalColors.cyan
            , terminalColors.bcyan
            , terminalColors.white
            , terminalColors.bwhite
        );

        if (isKittyTerminal) {
            std::printf(
                    "Selection FG:      0x%" PRIx32 "\n"
                    "Selection BG:      0x%" PRIx32 "\n"
                    "Cursor FG:         0x%" PRIx32 "\n"
                    "Cursor BG:         0x%" PRIx32 "\n"
                , util::kitty::kittyColor(18)
                , util::kitty::kittyColor(19)
                , util::kitty::kittyColor(21)
                , util::kitty::kittyColor(20)
            );
        }

        std::printf("-------------------------\n");

        return EXIT_SUCCESS;
    }

    // Return to home (prep for framebuffer mode)
    std::printf("\033[2J\033[H");
    std::fflush(stdout);

    // Attempt to set the locale.
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
        // state.update();
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
