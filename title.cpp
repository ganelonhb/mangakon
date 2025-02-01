#include "title.h"

Title::Title(ncpp::NotCurses *nc, ncpp::Plane *parent, uint32_t y, uint32_t x)
    : m_nc{nc}
    , m_ownsParent{!bool(parent)}
    , m_ypos{y}
    , m_xpos{x}
{
    m_parent = m_ownsParent ? m_nc->get_stdplane() : parent;

    m_parent->get_dim(&m_parenth, &m_parentw);

    uint32_t true_y = y;
    uint32_t true_x = x;

    if (true_y == static_cast<uint32_t>(-1))
        true_y = (m_parenth / 2) - (MANGAKON_JP_ROWS / 2);
    else if (true_y == static_cast<uint32_t>(-2))
        true_y = 0;
    else if (true_y == static_cast<uint32_t>(-3))
        true_y = m_parenth - MANGAKON_JP_ROWS;

    if (true_x == static_cast<uint32_t>(-1)) [[likely]]
        true_x = (m_parentw / 2) - (MANGAKON_JP_COLS / 2);
    else if (true_x == static_cast<uint32_t>(-2))
        true_x = 0;
    else if (true_x == static_cast<uint32_t>(-3))
        true_x = m_parentw - MANGAKON_JP_COLS;

    m_title = new ncpp::Plane(m_parent, MANGAKON_JP_ROWS, MANGAKON_JP_COLS, true_y, true_x);

    for (int i = 0; i < MANGAKON_JP_ROWS; ++i)
        m_title->putstr(i, 0, MANGAKON_JP[i]);
}

Title::~Title() {
    if (m_ownsParent)
        delete m_parent;

    delete m_title;
}

uint32_t Title::ypos() const {
    return m_ypos;
}

uint32_t Title::xpos() const {
    return m_xpos;
}

uint32_t Title::h() const {
    uint32_t y, x;
    m_title->get_dim(y, x);

    return y;
}

uint32_t Title::w() const {
    uint32_t y, x;
    m_title->get_dim(y, x);

    return x;
}

void Title::update() {
    uint32_t y, x;
    m_parent->get_dim(&y, &x);

    if (y != m_parenth or x != m_parentw) {

        uint32_t true_y = m_ypos;
        uint32_t true_x = m_xpos;

        if (true_y == static_cast<uint32_t>(-1))
            true_y = (y / 2) - (MANGAKON_JP_ROWS / 2);
        else if (true_y == static_cast<uint32_t>(-2))
            true_y = 0;
        else if (true_y == static_cast<uint32_t>(-3))
            true_y = y - MANGAKON_JP_ROWS;

        if (true_x == static_cast<uint32_t>(-1)) [[likely]]
            true_x = (x / 2) - (MANGAKON_JP_COLS / 2);
        else if (true_x == static_cast<uint32_t>(-2))
            true_x = 0;
        else if (true_x == static_cast<uint32_t>(-3))
            true_x = x - MANGAKON_JP_COLS;

        m_title->move(true_y, true_x);

        m_parenth = y;
        m_parentw = x;
    }
}
