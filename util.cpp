#include "util.h"

std::string util::string::trim(const std::string &str) {
    const std::string whitespace = " \t\n\r";
    const size_t begin = str.find_first_not_of(whitespace);
    if (begin == std::string::npos) {
        return "";
    }
    const size_t end = str.find_last_not_of(whitespace);
    return str.substr(begin, end - begin + 1);
}

std::string util::string::collapseWhitespace(const std::string& str) {
    return std::regex_replace(str, std::regex("\\s+"), " ");
}

std::wstring util::wstring::str2wstr(const std::string &str) {
    std::wstring wstr(str.size(), L'\0');
    std::mbstowcs(&wstr[0], str.c_str(), str.size());
    return wstr;
}

std::wstring util::wstring::str2wstr(const char *cstr) {
    if (cstr == nullptr)
        return std::wstring();

    std::string str(cstr);
    return util::wstring::str2wstr(str);
}

std::wstring util::wstring::str2wstr(const wchar_t *wcstr) {
    return std::wstring(wcstr);
}

size_t util::wstring::wstring_wclen(const std::wstring &wstr) {
    return wcslen(wstr.c_str());
}

bool util::wstring::isWhitespace(const wchar_t ch) {
    return (
        ch == L' ' or
        ch == L'\t' or
        ch == L'\u00A0' or // Non-breaking space
        ch == L'\u1680' or
        ch == L'\u2000' or
        ch == L'\u2001' or
        ch == L'\u2002' or
        ch == L'\u2003' or
        ch == L'\u2004' or
        ch == L'\u2005' or
        ch == L'\u2006' or
        ch == L'\u2007' or
        ch == L'\u2008' or
        ch == L'\u2009' or
        ch == L'\u200A' or
        ch == L'\u200B' or
        ch == L'\u202F' or
        ch == L'\u205F' or
        ch == L'\u3000' or
        ch == L'\u200C' or
        ch == L'\u200D'
    );
}

bool util::wstring::invalidChar(const wchar_t ch) {
    return ch == L'\r' or ch == L'\v' or ch == L'\f';
}

// Check if a token can fit in a number of columns. Returns -1 if the char is too long.
// Otherwise, it will return the wcwidth of the token.
int64_t util::wstring::wtoken_width(const std::wstring &string, size_t start, size_t remaining) {
    size_t width = 0;

    for(size_t i = start; i < string.size() and !util::wstring::isWhitespace(string[i] and !util::wstring::invalidChar(string[i]) and string[i] != '\n'); ++i) {
        width += wcwidth(string[i]);

        if (width > remaining)
            return -1;
    }

    return width;
}

bool util::wstring::c32_isPrintable(char32_t ch) {
    return (ch >= 0x20 and ch <= 0x7E) or (ch >= 0xA0 and ch <= 0x10FFFF and !std::iscntrl(ch));
}

const std::string util::dirs::get_home() {
    constexpr uint8_t __PLATFORM__ = util::platform::get_platform();

    if constexpr(util::platform::UNIX_LIKE) {
        return std::string(std::getenv("HOME"));
    }
    else if constexpr(__PLATFORM__ == util::platform::PLATFORM_WINDOWS) {
        return std::string(std::getenv("APPDATA"));
    } else {
        static_assert(
            __PLATFORM__ != util::platform::PLATFORM_UNKNOWN,
            "Unknown, unsupported platform. If you want your platform supported, please contribute "
            "or direct your complaints to https://github.com/ganelonhb/deep.donjon."
        );
    }
}

void util::magic::cls() {
    #if defined(_WIN32) or defined(_WIN64) or defined(CYGWIN) or defined(_WINNT)
        HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
        CONSOLE_SCREEN_BUFFER_INFO csbi;
        DWORD written;
        DWORD cells;
        COORD home = { 0, 0 };

        if (hConsole == INVALID_HANDLE_VALUE) return;

        if (GetConsoleScreenBufferInfo(hConsole, &csbi)) {
            cells = csbi.dwSize.X * csbi.dwSize.Y;
            FillConsoleOutputCharacter(hConsole, ' ', cells, home, &written);
            FillConsoleOutputAttribute(hConsole, csbi.wAttributes, cells, home, &written);
            SetConsoleCursorPosition(hConsole, home);
        }
    #else
        std::cout << "\033[2J\033[H" << std::flush;
    #endif
}

bool util::kitty::isKittyTerminal() {
    const char* kittyEnv = std::getenv("KITTY_WINDOW_ID");
    return kittyEnv != nullptr;
}

std::filesystem::path util::kitty::kittyConfigDir() {
    return std::filesystem::path(util::dirs::get_home()) / ".config"_path / "kitty"_path;
}

std::filesystem::path util::kitty::currentTheme() {
    std::filesystem::path conf = kittyConfigDir() / "current-theme.conf"_path;

    if (std::filesystem::exists(conf) and std::filesystem::is_regular_file(conf)) {
        return conf;
    }

    return std::filesystem::path();
}

uint32_t util::kitty::string2hex(const std::string &str) {
    std::string hex = str.substr(1, str.size() - 1);

    uint32_t num = 0;

    for (size_t i = 0; i < hex.size(); ++i) {
        char8_t ch = hex[i];
        uint32_t chInt = 0;

        if (ch >= '0' and ch <= '9') {
            chInt = ch - '0';
        } else if (ch >= 'A' and ch <= 'F') {
            chInt = ch - 'A' + 10;
        } else if (ch >= 'a' and ch <= 'f') {
            chInt = ch - 'a' + 10;
        } else {
            throw std::invalid_argument("Please supply a valid hexadecimal number (" + str + " could not  be converted).");
        }

        num = (num << 4) | chInt;
    }

    return num;
}

util::kitty::KittyColors::KittyColors() {
    std::filesystem::path theme = currentTheme();

    fg = KITTYFG;
    bg = KITTYBG;

    black = KITTYBLACK;
    red = KITTYRED;
    green = KITTYGREEN;
    yellow = KITTYYELLOW;
    blue = KITTYBLUE;
    magenta = KITTYMAGENTA;
    cyan = KITTYCYAN;
    white = KITTYWHITE;

    bblack = KITTYBBLACK;
    bred = KITTYBRED;
    bgreen = KITTYBGREEN;
    byellow = KITTYBYELLOW;
    bblue = KITTYBBLUE;
    bmagenta = KITTYBMAGENTA;
    bcyan = KITTYBCYAN;
    bwhite = KITTYBWHITE;

    selection_foreground = KITTYSELECTFG;
    selection_background = KITTYSELECTBG;

    cursor = KITTYCURSORBG;
    cursor_text_color = KITTYCURSORFG;

    if (std::filesystem::exists(theme) and std::filesystem::is_regular_file(theme)) {
        if (std::ifstream f(theme); f) {
            std::string line;
            while (std::getline(f, line)) {
                std::string processed = string::trim(string::collapseWhitespace(line));

                if (!(processed.empty() or processed.starts_with('#'))) {
                    std::istringstream stream(processed);
                    std::string key;
                    std::string value;

                    if (std::getline(stream, key, ' ') and std::getline(stream, value)) {
                        // Handle foreground, background, or color
                        if (key == "foreground") {
                            fg = string2hex(value);
                        } else if (key == "background") {
                            bg = string2hex(value);
                        } else if (key == "selection_foreground") {
                            selection_foreground = string2hex(value);
                        } else if (key == "selection_background") {
                            selection_background = string2hex(value);
                        } else if (key == "cursor") {
                            cursor = string2hex(value);
                        } else if (key == "cursor_text_color") {
                            cursor_text_color = string2hex(value);
                        } else if (key.starts_with("color")) {
                            uint8_t color_index = static_cast<uint8_t>(std::stoi(key.substr(5)));
                            uint32_t color_value = string2hex(value);
                            switch (color_index) {
                                case 0:
                                    black = color_value;
                                    break;
                                case 1:
                                    red = color_value;
                                    break;
                                case 2:
                                    green = color_value;
                                    break;
                                case 3:
                                    yellow = color_value;
                                    break;
                                case 4:
                                    blue = color_value;
                                    break;
                                case 5:
                                    magenta = color_value;
                                    break;
                                case 6:
                                    cyan = color_value;
                                    break;
                                case 7:
                                    white = color_value;
                                    break;
                                case 8:
                                    bblack = color_value;
                                    break;
                                case 9:
                                    bred = color_value;
                                    break;
                                case 10:
                                    bgreen = color_value;
                                    break;
                                case 11:
                                    byellow = color_value;
                                    break;
                                case 12:
                                    bblue = color_value;
                                    break;
                                case 13:
                                    bmagenta = color_value;
                                    break;
                                case 14:
                                    bcyan = color_value;
                                    break;
                                case 15:
                                    bwhite = color_value;
                                    break;
                                default:
                                    break;
                            }
                        }
                    }
                }
            }
        }
    }
}

// 0-15 = color0-color15, fg = 16, bg = 17
uint32_t util::kitty::kittyColor(uint8_t color) {
    static const KittyColors k;

    switch (color) {
        case 0:
            return k.black;
        case 1:
            return k.red;
        case 2:
            return k.green;
        case 3:
            return k.yellow;
        case 4:
            return k.blue;
        case 5:
            return k.magenta;
        case 6:
            return k.cyan;
        case 7:
            return k.white;
        case 8:
            return k.bblack;
        case 9:
            return k.bred;
        case 10:
            return k.bgreen;
        case 11:
            return k.byellow;
        case 12:
            return k.bblue;
        case 13:
            return k.bmagenta;
        case 14:
            return k.bcyan;
        case 15:
            return k.bwhite;
        case 16:
            return k.fg;
        case 17:
            return k.bg;
        case 18:
            return k.selection_foreground;
        case 19:
            return k.selection_background;
        case 20:
            return k.cursor;
        case 21:
            return k.cursor_text_color;
        default:
            break;
    }

    return 0;
}

uint32_t util::kitty::kittyCursor() {
    return kittyColor(20);
}

uint32_t util::kitty::kittyCursorTextColor() {
    return kittyColor(21);
}

util::smart_colors::terminal_query_error::terminal_query_error()
    : std::runtime_error("Failed to retrieve terminal colors.") {}

const char *util::smart_colors::terminal_query_error::what() const throw() {
    return "Failed to retrieve terminal color.";
}

std::string util::smart_colors::queryTerminal(const std::string &query) {
    if (util::kitty::isKittyTerminal()) {
        return "Querying the kitty terminal is not supported.";
    }

    std::cout << query;
    std::cout.flush();

    struct termios originalTermios, rawTermios;
    tcgetattr(STDIN_FILENO, &originalTermios);
    rawTermios = originalTermios;
    cfmakeraw(&rawTermios);
    tcsetattr(STDIN_FILENO, TCSANOW, &rawTermios);

    std::string response;
    char ch;
    while (read(STDIN_FILENO, &ch, 1) == 1) {
        response += ch;
        if (ch == '\a') {
            break;
        }
    }

    tcsetattr(STDIN_FILENO, TCSANOW, &originalTermios);

    return response;
}

uint32_t util::smart_colors::parseRGB(const std::string& response) {
    size_t start = response.find("rgb:");
    if (start == std::string::npos) {
        return 0;
    }

    std::string rgbStr = response.substr(start + 4);
    int r, g, b;

    std::istringstream ss(rgbStr);
    std::string rStr, gStr, bStr;
    std::getline(ss, rStr, '/');
    std::getline(ss, gStr, '/');
    std::getline(ss, bStr, '\a');

    if (rStr.size() > 2) rStr = rStr.substr(0, 2);
    if (gStr.size() > 2) gStr = gStr.substr(0, 2);
    if (bStr.size() > 2) bStr = bStr.substr(0, 2);

    std::stringstream rSS(rStr), gSS(gStr), bSS(bStr);
    rSS >> std::hex >> r;
    gSS >> std::hex >> g;
    bSS >> std::hex >> b;

    return (static_cast<uint32_t>(r) << 16) | (static_cast<uint32_t>(g) << 8) | static_cast<uint32_t>(b);
}

uint32_t util::smart_colors::queryDefaultFG() {

    if (kitty::isKittyTerminal()) {
        if (std::filesystem::path path = kitty::kittyConfigDir(); std::filesystem::exists(path) and std::filesystem::is_directory(path) and !path.empty()) {
            return kitty::kittyColor(16);
        } else {
            return kitty::KITTYFG;
        }
    }

    std::string response = queryTerminal("\033]10;?\007");
    return parseRGB(response);
}

uint32_t util::smart_colors::queryDefaultBG() {
    if (kitty::isKittyTerminal()) {
        if (std::filesystem::path path = kitty::kittyConfigDir(); std::filesystem::exists(path) and std::filesystem::is_directory(path) and !path.empty()) {
            return kitty::kittyColor(17);
        } else {
            return kitty::KITTYBG;
        }
    }

    std::string response = queryTerminal("\033]11;?\007");
    return parseRGB(response);
}

uint32_t util::smart_colors::queryColor(int colorCode) {
    std::string response = queryTerminal("\033]4;" + std::to_string(colorCode) + ";?\007");
    return parseRGB(response);
}

uint32_t util::smart_colors::queryBlack() {
    if (kitty::isKittyTerminal()) {
        if (std::filesystem::path path = kitty::kittyConfigDir(); std::filesystem::exists(path) and std::filesystem::is_directory(path) and !path.empty()) {
            return kitty::kittyColor(0);
        } else {
            return kitty::KITTYBLACK;
        }
    }

    return queryColor(0);
}

uint32_t util::smart_colors::queryRed() {
    if (kitty::isKittyTerminal()) {
        if (std::filesystem::path path = kitty::kittyConfigDir(); std::filesystem::exists(path) and std::filesystem::is_directory(path) and !path.empty()) {
            return kitty::kittyColor(1);
        } else {
            return kitty::KITTYRED;
        }
    }

    return queryColor(1);
}

uint32_t util::smart_colors::queryGreen() {
    if (kitty::isKittyTerminal()) {
        if (std::filesystem::path path = kitty::kittyConfigDir(); std::filesystem::exists(path) and std::filesystem::is_directory(path) and !path.empty()) {
            return kitty::kittyColor(2);
        } else {
            return kitty::KITTYGREEN;
        }
    }

    return queryColor(2);
}

uint32_t util::smart_colors::queryYellow() {
    if (kitty::isKittyTerminal()) {
        if (std::filesystem::path path = kitty::kittyConfigDir(); std::filesystem::exists(path) and std::filesystem::is_directory(path) and !path.empty()) {
            return kitty::kittyColor(3);
        } else {
            return kitty::KITTYYELLOW;
        }
    }

    return queryColor(3);
}

uint32_t util::smart_colors::queryBlue() {
    if (kitty::isKittyTerminal()) {
        if (std::filesystem::path path = kitty::kittyConfigDir(); std::filesystem::exists(path) and std::filesystem::is_directory(path) and !path.empty()) {
            return kitty::kittyColor(4);
        } else {
            return kitty::KITTYBLUE;
        }
    }

    return queryColor(4);
}

uint32_t util::smart_colors::queryMagenta() {
    if (kitty::isKittyTerminal()) {
        if (std::filesystem::path path = kitty::kittyConfigDir(); std::filesystem::exists(path) and std::filesystem::is_directory(path) and !path.empty()) {
            return kitty::kittyColor(5);
        } else {
            return kitty::KITTYMAGENTA;
        }
    }

    return queryColor(5);
}

uint32_t util::smart_colors::queryCyan() {
    if (kitty::isKittyTerminal()) {
        if (std::filesystem::path path = kitty::kittyConfigDir(); std::filesystem::exists(path) and std::filesystem::is_directory(path) and !path.empty()) {
            return kitty::kittyColor(6);
        } else {
            return kitty::KITTYCYAN;
        }
    }

    return queryColor(6);
}

uint32_t util::smart_colors::queryWhite() {
    if (kitty::isKittyTerminal()) {
        if (std::filesystem::path path = kitty::kittyConfigDir(); std::filesystem::exists(path) and std::filesystem::is_directory(path) and !path.empty()) {
            return kitty::kittyColor(7);
        } else {
            return kitty::KITTYWHITE;
        }
    }

    return queryColor(7);
}


uint32_t util::smart_colors::queryBrightBlack() {
    if (kitty::isKittyTerminal()) {
        if (std::filesystem::path path = kitty::kittyConfigDir(); std::filesystem::exists(path) and std::filesystem::is_directory(path) and !path.empty()) {
            return kitty::kittyColor(8);
        } else {
            return kitty::KITTYBBLACK;
        }
    }

    return queryColor(8);
}

uint32_t util::smart_colors::queryBrightRed() {
    if (kitty::isKittyTerminal()) {
        if (std::filesystem::path path = kitty::kittyConfigDir(); std::filesystem::exists(path) and std::filesystem::is_directory(path) and !path.empty()) {
            return kitty::kittyColor(9);
        } else {
            return kitty::KITTYBRED;
        }
    }

    return queryColor(9);
}

uint32_t util::smart_colors::queryBrightGreen() {
    if (kitty::isKittyTerminal()) {
        if (std::filesystem::path path = kitty::kittyConfigDir(); std::filesystem::exists(path) and std::filesystem::is_directory(path) and !path.empty()) {
            return kitty::kittyColor(10);
        } else {
            return kitty::KITTYBGREEN;
        }
    }

    return queryColor(10);
}

uint32_t util::smart_colors::queryBrightYellow() {
    if (kitty::isKittyTerminal()) {
        if (std::filesystem::path path = kitty::kittyConfigDir(); std::filesystem::exists(path) and std::filesystem::is_directory(path) and !path.empty()) {
            return kitty::kittyColor(11);
        } else {
            return kitty::KITTYBYELLOW;
        }
    }

    return queryColor(11);
}

uint32_t util::smart_colors::queryBrightBlue() {
    if (kitty::isKittyTerminal()) {
        if (std::filesystem::path path = kitty::kittyConfigDir(); std::filesystem::exists(path) and std::filesystem::is_directory(path) and !path.empty()) {
            return kitty::kittyColor(12);
        } else {
            return kitty::KITTYBBLUE;
        }
    }

    return queryColor(12);
}

uint32_t util::smart_colors::queryBrightMagenta() {
    if (kitty::isKittyTerminal()) {
        if (std::filesystem::path path = kitty::kittyConfigDir(); std::filesystem::exists(path) and std::filesystem::is_directory(path) and !path.empty()) {
            return kitty::kittyColor(13);
        } else {
            return kitty::KITTYBMAGENTA;
        }
    }

    return queryColor(13);
}

uint32_t util::smart_colors::queryBrightCyan() {
    if (kitty::isKittyTerminal()) {
        if (std::filesystem::path path = kitty::kittyConfigDir(); std::filesystem::exists(path) and std::filesystem::is_directory(path) and !path.empty()) {
            return kitty::kittyColor(14);
        } else {
            return kitty::KITTYBCYAN;
        }
    }

    return queryColor(14);
}

uint32_t util::smart_colors::queryBrightWhite() {
    if (kitty::isKittyTerminal()) {
        if (std::filesystem::path path = kitty::kittyConfigDir(); std::filesystem::exists(path) and std::filesystem::is_directory(path) and !path.empty()) {
            return kitty::kittyColor(15);
        } else {
            return kitty::KITTYBWHITE;
        }
    }

    return queryColor(15);
}

util::smart_colors::TerminalColors util::smart_colors::getDefaultTermColors() {
    TerminalColors tc;

    try {
        tc = TerminalColors{
            .fg         = queryDefaultFG(),
            .bg         = queryDefaultBG(),
            .black      = queryBlack(),
            .red        = queryRed(),
            .green      = queryGreen(),
            .yellow     = queryYellow(),
            .blue       = queryBlue(),
            .magenta    = queryMagenta(),
            .cyan       = queryCyan(),
            .white      = queryWhite(),
            .bblack     = queryBrightBlack(),
            .bred       = queryBrightRed(),
            .bgreen     = queryBrightGreen(),
            .byellow    = queryBrightYellow(),
            .bblue      = queryBrightBlue(),
            .bmagenta   = queryBrightMagenta(),
            .bcyan      = queryBrightCyan(),
            .bwhite     = queryBrightWhite()
        };
    }
    catch (terminal_query_error &e) {
        tc = TerminalColors{
            .fg         = kitty::KITTYFG,
            .bg         = kitty::KITTYBG,
            .black      = kitty::KITTYBLACK,
            .red        = kitty::KITTYRED,
            .green      = kitty::KITTYGREEN,
            .yellow     = kitty::KITTYYELLOW,
            .blue       = kitty::KITTYBLUE,
            .magenta    = kitty::KITTYMAGENTA,
            .cyan       = kitty::KITTYCYAN,
            .white      = kitty::KITTYWHITE,
            .bblack     = kitty::KITTYBBLACK,
            .bred       = kitty::KITTYBRED,
            .bgreen     = kitty::KITTYBGREEN,
            .byellow    = kitty::KITTYBYELLOW,
            .bblue      = kitty::KITTYBBLUE,
            .bmagenta   = kitty::KITTYBMAGENTA,
            .bcyan      = kitty::KITTYBCYAN,
            .bwhite     = kitty::KITTYBWHITE,
        };
    }

    return tc;
}

std::string util::string::simplify(const std::string &str) {
    return trim(collapseWhitespace(str));
}
