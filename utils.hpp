#ifndef UTILS_HPP
#define UTILS_HPP

#define forever while(true)
#define pass (void)(0)

#include <iostream> // use sparsely
#include <fstream>
#include <string>
#include <functional>

#include <openssl/sha.h>

#include <termios.h>
#include <fcntl.h>

namespace util {
    namespace hash {
        inline size_t hash_file_contents(const std::string &filePath) {
            std::ifstream file(filePath, std::ios::binary);

            if (!file)
                throw std::runtime_error("Failed to open file: " + filePath);

            std::string contents((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());

            std::hash<std::string> hasher;

            return hasher(contents);
        }
    }

    namespace platform {
        constexpr uint16_t PLATFORM_LINUX = 0;
        constexpr uint16_t PLATFORM_WINDOWS = 1;
        constexpr uint16_t PLATFORM_MACOS = 2;
        constexpr uint16_t PLATFORM_UNIX = 3;
        constexpr uint16_t PLATFORM_BSD = 4;
        constexpr uint16_t PLATFORM_UNKNOWN = 65535;

        constexpr uint16_t get_platform() {
            #if defined(__linux__) or defined(__gnu_linux__)
                return PLATFORM_LINUX;
            #elif defined(_WIN32) or defined(_WIN64) or defined(CYGWIN) or defined(_WINNT)
                return PLATFORM_WINDOWS;
            #elif defined(__APPLE__) or defined(__MACH__) or defined(__DARWIN__)
                return PLATFORM_MACOS;
            #elif defined(__FreeBSD__) or defined(__NetBSD__) or defined(__OpenBSD__) or defined(__DragonFly__) or defined(__MirBSD__)
                return PLATFORM_BSD;
            #elif defind(__unix__)
                return PLATFORM_UNIX;
            #else
                return PLATFORM_UNKNOWN;
            #endif
        }

        constexpr uint16_t PLATFORM = get_platform();
        constexpr bool UNIX_LIKE = PLATFORM == 0 or PLATFORM == 2 or PLATFORM == 3 or PLATFORM == 4;
        constexpr bool NT_LIKE = PLATFORM == 1;
        constexpr bool OSX_LIKE = PLATFORM == 2;
    }

    namespace dirs {
        inline std::string get_home() {
            using namespace util::platform;
            if constexpr(get_platform() == PLATFORM_LINUX or get_platform() == PLATFORM_UNIX or get_platform() == PLATFORM_MACOS) {
                return std::string(std::getenv("HOME"));
            }
            else if constexpr(get_platform() == PLATFORM_WINDOWS) {
                return std::string(std::getenv("APPDATA"));
            }
            else {
                throw std::runtime_error("Unknown or unsupported platform.");
            }
        }
    }

    namespace compile_time_id {
        constexpr size_t cstrlen(const char *str) {
            const char *start = str;
            const char *ptr = str;
            while (*ptr != '\0')
                ++ptr;

            return ptr - start;
        }

        constexpr uint32_t hash_string(const char* str) {
            uint32_t hash = 0x811C9DC5u;

            for (size_t i = 0; i < cstrlen(str); ++i) {
                hash ^= static_cast<uint32_t>(str[i]);
                hash *= 0x1000193u;
            }

            return hash;
        }

        constexpr uint32_t lcg(uint32_t seed) {
            return (0x19660D * seed + 0x3C6EF35F) % 0xFFFFFFFF;
        }

        constexpr int get_id(const char* str) {
            uint32_t seed = hash_string(str);

            return lcg(seed);
        }
    }

    namespace cwstr {
        constexpr int cwcslen(const wchar_t * str) {
            const wchar_t *start = str;
            const wchar_t *ptr = str;
            while (*ptr != L'\0')
            {
                ++ptr;
            }
            return ptr - start;
        }

        constexpr int cwsalen(const wchar_t * const*wstra) {
            int i = 0;

            for (; wstra[i] != nullptr; ++i);

            return i;
        }

        constexpr int cmaxwsalen(const wchar_t* const* wstra) {
            int max = -1;

            for (int i = 0; wstra[i] != nullptr; ++i)
            {
                int len = cwcslen(wstra[i]);
                if (len > max) max = len;
            }

            return max;
        }

    }

    namespace constants {
        namespace align {
            constexpr int CENTER = -1;
            constexpr int LEFT = -2;
            constexpr int RIGHT = -3;
        }
    }

    namespace colors {
        namespace defaults {
            constexpr int SELECTED = 0xE68A00;
            constexpr int WINDOW_BG = 0xFF5E00;
            constexpr int BUTTON = 0xFF7E33;

            constexpr int BUTTONPRESS = 0xE65400;
        }
    }

    namespace magic {
        inline void cls() {
            std::cout << "\033[2J\033[H" << std::flush;
        }
    }
}

#endif
