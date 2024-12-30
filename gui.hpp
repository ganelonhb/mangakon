#ifndef GUI_HPP
#define GUI_HPP

#include <cwchar>

#include "utils.hpp"

#define STRBUFFER 1024

constexpr const wchar_t *MANGAKON_TITLE[STRBUFFER] = {
    L"███╗   ███╗ █████╗ ███╗   ██╗ ██████╗  █████╗ ██╗  ██╗ ██████╗ ███╗   ██╗\n",
    L"████╗ ████║██╔══██╗████╗  ██║██╔════╝ ██╔══██╗██║ ██╔╝██╔═══██╗████╗  ██║\n",
    L"██╔████╔██║███████║██╔██╗ ██║██║  ███╗███████║█████╔╝ ██║   ██║██╔██╗ ██║\n",
    L"██║╚██╔╝██║██╔══██║██║╚██╗██║██║   ██║██╔══██║██╔═██╗ ██║   ██║██║╚██╗██║\n",
    L"██║ ╚═╝ ██║██║  ██║██║ ╚████║╚██████╔╝██║  ██║██║  ██╗╚██████╔╝██║ ╚████║\n",
    L"╚═╝     ╚═╝╚═╝  ╚═╝╚═╝  ╚═══╝ ╚═════╝ ╚═╝  ╚═╝╚═╝  ╚═╝ ╚═════╝ ╚═╝  ╚═══╝\n",
    nullptr
};

constexpr const int MANGAKON_TITLE_ROWS = util::cwstr::cwsalen(MANGAKON_TITLE);
constexpr const int MANGAKON_TITLE_COLS = util::cwstr::cmaxwsalen(MANGAKON_TITLE);

constexpr const wchar_t *MANGAKON_JP[STRBUFFER] = {
    L"█████████╗ ██═╗         ██╗  █╗█╗            ██═╗      \n",
    L"       ██║ ╚██║       █████████╔╝ █████████╗ ╚██║      \n",
    L"  ██  ██╔╝  ╚═╝   ██╗ ╚═██╔══██║  ╚══════██║  ╚═╝   ██╗\n",
    L"  ╚████═╝        ██╔╝   ██║  ██║         ██║       ██╔╝\n",
    L"   ╚██╚╗       ███╔╝   ██╔╝  ██║         ██║     ███╔╝ \n",
    L"    ╚██║   █████╔═╝   ██╔╝ ██╔═╝  █████████║ █████╔═╝  \n",
    L"     ╚═╝   ╚════╝     ╚═╝  ╚═╝    ╚════════╝ ╚════╝    \n",
    nullptr
};

constexpr const int MANGAKON_JP_ROWS = util::cwstr::cwsalen(MANGAKON_JP);
constexpr const int MANGAKON_JP_COLS = util::cwstr::cmaxwsalen(MANGAKON_JP);
#endif
