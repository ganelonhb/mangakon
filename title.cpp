#include "title.h"

Title::Title(ncpp::NotCurses *nc, ncpp::Plane *parent, uint32_t y, uint32_t x)
    : m_nc{nc}
    , m_ypos{y}
    , m_xpos{x}
    , m_ownsParent{!bool(parent)}
{
    m_parent = m_ownsParent ? m_nc->get_stdplane() : parent;

    m_parent->get_dim(m_parenth, m_parentw);

    uint32_t true_y = y;
    uint32_t true_x = x;

    if (true_y == -1)
        true_y = (m_parenth / 2) - (MANGAKON_TITLE_ROWS / 2);
    else if (true_y == -2)
        true_y = 0;
    else if (true_y == -3)
        true_y = m_parenth - MANGAKON_TITLE_ROWS;

    if (true_x == -1) [[likely]]
        true_x = (m_parentw / 2) - (MANGAKON_TITLE_COLS / 2);
    else if (true_x == -2)
        true_x = 0;
    else if (true_x == -3)
        true_x = m_parentw - MANGAKON_TITLE_COLS;

    m_title = new ncpp::Plane(m_parent, MANGAKON_TITLE_ROWS, MANGAKON_TITLE_COLS, true_y, true_x);

    for (int i = 0; i < MANGAKON_TITLE_ROWS; ++i)
        m_title->putstr(i, 0, MANGAKON_TITLE[i]);
}

Title::~Title() {
    if (m_ownsParent)
        delete m_parent;
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

    m_parenth = y;
    m_parentw = x;

    uint32_t true_y = m_ypos;
    uint32_t true_x = m_xpos;

    if (true_y == -1)
        true_y = (m_parenth / 2) - (MANGAKON_TITLE_ROWS / 2);
    else if (true_y == -2)
        true_y = 0;
    else if (true_y == -3)
        true_y = m_parenth - MANGAKON_TITLE_ROWS;

    if (true_x == -1) [[likely]]
        true_x = (m_parentw / 2) - (MANGAKON_TITLE_COLS / 2);
    else if (true_x == -2)
        true_x = 0;
    else if (true_x == -3)
        true_x = m_parentw - MANGAKON_TITLE_COLS;

    m_title->move(true_y, true_x);
}
