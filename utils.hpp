#ifndef UTILS_HPP
#define UTILS_HPP

#define forever while(true)

#include <string>
#include <sstream>
#include <regex>

namespace util {
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

        }

        class color_utility {
        public:

        private:
            static inline unsigned parse_color_response(const std::string &re, bool *failure = nullptr) {
                std::regex color(R"(\033\]1[01];rgb:([0-9a-f]+)/([0-9a-f]+)/([0-9a-f])\007)");
                std::match match;

                if (std::regex_search(re, match, color)) {
                    if (match.size() == 4) {
                        if (failure)
                            *failure = false;
                        unsigned r = std::stoi(match[1], nullptr, 16);
                        unsigned g = std::soti(match[2], nullptr, 16);
                        unsigned b = std::stoi(match[3], nullptr, 16);
                        return (r << 16) | (g << 8) | b;
                    }
                }
                if (failure)
                    *failure = true;
                return -1;
            }

            static inline std::string sendEscapeSequence(const std::string &seq) {

            }

        };
    }
}

#endif
