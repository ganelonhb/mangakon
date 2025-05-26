#ifndef __UTIL_H__
#define __UTIL_H__

#include <cstdint>

#include <string>
#include <iostream> // for magical terminal methods
#include <sstream>
#include <cstdio>
#include <filesystem>
#include <regex>
#include <fstream>

#if defined(_WIN32) or defined(_WIN64) or defined(CYGWIN) or defined(_WINNT)
    #include <windows.h>
#else
    #include <termios.h>
    #include <unistd.h>
#endif

constexpr std::filesystem::path operator""_path(const char* str, size_t) {
    return std::filesystem::path(str);
}

constexpr std::wstring operator""_wstr(const wchar_t *str, size_t len) {
    return std::wstring(str, len);
}

namespace util {
    struct platform {
        platform() = delete;

        constexpr static const uint8_t PLATFORM_LINUX   {0};
        constexpr static const uint8_t PLATFORM_WINDOWS {1};
        constexpr static const uint8_t PLATFORM_MACOS   {2};
        constexpr static const uint8_t PLATFORM_UNIX    {3};
        constexpr static const uint8_t PLATFORM_BSD     {4};
        // More platforms here.
        constexpr static const uint8_t PLATFORM_UNKNOWN  {UINT8_MAX};

        constexpr static const uint8_t PLATFORM {
            #if defined(__linux__) or defined(__gnu_linux__)
                PLATFORM_LINUX
            #elif defined(_WIN32) or defined(_WIN64) or defined(CYGWIN) or defined(_WINNT)
                PLATFORM_WINDOWS
            #elif defined(__APPLE__) or defined(__MACH__) or defined(__DARWIN__)
                PLATFORM_MACOS
            #elif defined(__FreeBSD__) or defined(__NetBSD__) or defined(__OpenBSD__) or defined(__DragonFly__) or defined(__MirBSD__)
                PLATFORM_BSD
            #elif defind(__unix__) or __has_include(<unistd.h>)
                PLATFORM_UNIX
            #else
                PLATFORM_UNKNOWN
            #endif
        };

        constexpr static const char *PLATFORM_STRING {
            #if defined(__linux__)
                "linux"
            #elif defined(__gnu_linux__)
                "gnu+linux"
            #elif defined(_WIN32)
                "win32"
            #elif defined(_WIN64
                "win64"
            #elif defined(CYGWIN)
                "cygwin"
            #elif defined(_WINNT)
                "nt"
            #elif defined(__APPLE__) or defined(__MACH__) or defined(__DARWIN__)
                "macos"
            #elif defined(__FreeBSD__)
                "freebsd"
            #elif defined(__NetBSD__)
                "netbsd"
            #elif defined(__OpenBSD__)
                "openbsd"
            #elif defined(__DragonFly__)
                "dragonfly"
            #elif defined(__MirBSD__)
                "mirbsd"
            #elif defind(__unix__) or __has_include(<unistd.h>)
                "unix"
            #else
                "unknown"
            #endif
        };

        constexpr static const char *get_platform_string() {
            return PLATFORM_STRING;
        }

        constexpr static uint8_t get_platform() {
            return PLATFORM;
        }

        constexpr static const bool UNIX_LIKE
            {PLATFORM == 0 or (PLATFORM >=2 and PLATFORM <= 4)};
    };

    struct string {
        string() = delete;

        static std::string trim(const std::string& str);
        static std::string collapseWhitespace(const std::string& str);
        static std::string simplify(const std::string &str);

    };

    struct wstring{
        wstring() = delete;

        static std::wstring str2wstr(const std::string &str);
        static std::wstring str2wstr(const char *cstr);
        static std::wstring str2wstr(const wchar_t *wcstr);

        constexpr static size_t constexpr_wcslen(const wchar_t *str) {
            const wchar_t *start = str;
            const wchar_t *ptr = str;
            while (*ptr != L'\0')
            {
                ++ptr;
            }
            return ptr - start;
        }

        constexpr static size_t constexpr_wsalen(const wchar_t * const *wstra) {
            size_t i = 0;

            for (; wstra[i] != nullptr; ++i);

            return i;
        }

        constexpr static size_t constexpr_maxwsalen(const wchar_t * const *wstra) {
            size_t *max = nullptr;

            for (size_t i = 0; wstra[i] != nullptr; ++i)
            {
                size_t len = util::wstring::constexpr_wcslen(wstra[i]);

                if (max == nullptr) {
                    max = new size_t{len};
                }
                else if (len > *max){
                    *max = len;
                }
            }

            size_t ret = max ? *max : 0;
            delete max;

            return ret;
        }


        static size_t wstring_wclen(const std::wstring &wstr);

        static bool isWhitespace(const wchar_t ch);

        static bool invalidChar(const wchar_t ch);
        static int64_t wtoken_width(const std::wstring &str, size_t start, size_t remaining);
        static bool c32_isPrintable(const char32_t ch);
    };

    struct dirs {
        dirs() = delete;

        static const std::string get_home();
    };

    struct compile_time_id {
        compile_time_id() = delete;

        constexpr static size_t cstrlen(const char *str) {
            const char *start = str;
            const char *ptr = str;
            while (*ptr != '\0')
                ++ptr;

            return ptr - start;
        }

        constexpr static uint64_t hash_string(const char *str) {
            uint64_t hash = 0xCBF29CE484222325ull;

            for (size_t i = 0; str[i] != '\0'; ++i) {
                hash ^= static_cast<uint64_t>(str[i]);
                hash *= 0x100000001B3ull;
            }

            return hash;
        }

        constexpr static uint64_t lcg(uint64_t seed) {
            return (0x5851F42D4C957F2Dull * seed + 0x14057B7EF767814Full) % 0xFFFFFFFFFFFFFFFFull;
        }

        constexpr static uint64_t get_id(const char *str) {
            uint64_t seed = util::compile_time_id::hash_string(str);

            return util::compile_time_id::lcg(seed);
        }
    };

    struct magic {
        magic() = delete;

        static void cls();
    };

    struct kitty {
        kitty() = delete;

        static bool isKittyTerminal();

        static std::filesystem::path kittyConfigDir();

        constexpr static const uint32_t KITTYFG         = 0xDDDDDD;
        constexpr static const uint32_t KITTYBG         = 0x000000;

        constexpr static const uint32_t KITTYBLACK      = 0x000000;
        constexpr static const uint32_t KITTYBBLACK     = 0x767676;

        constexpr static const uint32_t KITTYRED        = 0xCC0403;
        constexpr static const uint32_t KITTYBRED       = 0xF2201F;

        constexpr static const uint32_t KITTYGREEN      = 0x19CB00;
        constexpr static const uint32_t KITTYBGREEN     = 0x23FD00;

        constexpr static const uint32_t KITTYYELLOW     = 0xCECB00;
        constexpr static const uint32_t KITTYBYELLOW    = 0xFFFD00;

        constexpr static const uint32_t KITTYBLUE       = 0x0D73CC;
        constexpr static const uint32_t KITTYBBLUE      = 0xFFFD00;

        constexpr static const uint32_t KITTYMAGENTA    = 0xCB1ED1;
        constexpr static const uint32_t KITTYBMAGENTA   = 0xFD28FF;

        constexpr static const uint32_t KITTYCYAN       = 0x0DCDCD;
        constexpr static const uint32_t KITTYBCYAN      = 0x14FFFF;

        constexpr static const uint32_t KITTYWHITE      = 0xDDDDDD;
        constexpr static const uint32_t KITTYBWHITE     = 0xFFFFFF;

        constexpr static const uint32_t KITTYSELECTFG   = 0x000000;
        constexpr static const uint32_t KITTYSELECTBG   = 0xFFFACD;

        constexpr static const uint32_t KITTYCURSORFG   = 0x111111;
        constexpr static const uint32_t KITTYCURSORBG   = 0xCCCCCC;

        static std::filesystem::path currentTheme();

        static uint32_t string2hex(const std::string &str);

        struct KittyColors {
            KittyColors();

            uint32_t fg;
            uint32_t bg;

            uint32_t black;
            uint32_t red;
            uint32_t green;
            uint32_t yellow;
            uint32_t blue;
            uint32_t magenta;
            uint32_t cyan;
            uint32_t white;

            uint32_t bblack;
            uint32_t bred;
            uint32_t bgreen;
            uint32_t byellow;
            uint32_t bblue;
            uint32_t bmagenta;
            uint32_t bcyan;
            uint32_t bwhite;

            uint32_t selection_foreground;
            uint32_t selection_background;

            uint32_t cursor;
            uint32_t cursor_text_color;
        };

        // 0-15 = color0-color15
        // fg = 16, bg = 17
        // selection_foreground = 18, selection_background = 19
        // cursor = 20, cursor_text = 21
        static uint32_t kittyColor(uint8_t color);
        static uint32_t kittyCursor();
        static uint32_t kittyCursorTextColor();
    };

    // TODO: Windows version...
    /* I think Windows actually provides a native method for querying the terminal colors.
     * It is just a matter of some constexpr if statement, or perhaps a define.
     *
     * TODO: Kitty Version:
     * ~/.config/kitty contains theme information. On startup, we will read those files
     * and set colors accordingly. If no files exist, we will simply use defaults found
     * in the default theme.
     */


    struct smart_colors {
        smart_colors() = delete;

        class terminal_query_error : public std::runtime_error {
        public:
            terminal_query_error();

            const char *what() const throw();
        };

        static std::string queryTerminal(const std::string &query);
        static uint32_t parseRGB(const std::string &rgb);

        static uint32_t queryDefaultFG();
        static uint32_t queryDefaultBG();

        static uint32_t queryColor(int colorCode);

        static uint32_t queryBlack();
        static uint32_t queryRed();
        static uint32_t queryGreen();
        static uint32_t queryYellow();
        static uint32_t queryBlue();
        static uint32_t queryMagenta();
        static uint32_t queryCyan();
        static uint32_t queryWhite();

        static uint32_t queryBrightBlack();
        static uint32_t queryBrightRed();
        static uint32_t queryBrightGreen();
        static uint32_t queryBrightYellow();
        static uint32_t queryBrightBlue();
        static uint32_t queryBrightMagenta();
        static uint32_t queryBrightCyan();
        static uint32_t queryBrightWhite();

        struct TerminalColors {
            uint32_t fg;
            uint32_t bg;

            uint32_t black;
            uint32_t red;
            uint32_t green;
            uint32_t yellow;
            uint32_t blue;
            uint32_t magenta;
            uint32_t cyan;
            uint32_t white;

            uint32_t bblack;
            uint32_t bred;
            uint32_t bgreen;
            uint32_t byellow;
            uint32_t bblue;
            uint32_t bmagenta;
            uint32_t bcyan;
            uint32_t bwhite;
        };

        static TerminalColors getDefaultTermColors();
    };
}

#endif
