#ifndef UTILS_HPP
#define UTILS_HPP

#define forever while(true)


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
    }
}

#endif
