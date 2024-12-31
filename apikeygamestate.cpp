#include "apikeygamestate.h"

constexpr int MENU_Y = 20;
constexpr int MENU_X = 90;

ApiKeyGameState::ApiKeyGameState(ncpp::NotCurses *nc, ncpp::Plane *parent, std::mutex *mtx)
    : GameState{nc, parent}
    , m_mtx{mtx}
{
    m_title = new Title(m_nc, m_parent, 5);

    unsigned y;
    unsigned x;
    m_parent->get_dim(&y, &x);

    m_window = new ncpp::Plane(m_parent, MENU_Y, MENU_X, (y / 2) - (MENU_Y / 2),  (x / 2) - (MENU_X / 2));
    m_window->set_fg_default();
    m_window->set_bg_rgb(util::colors::defaults::WINDOW_BG);

    uint64_t channels = 0;
    ncchannels_set_bg_rgb(&channels, util::colors::defaults::WINDOW_BG);
    ncchannels_set_fg_default(&channels);

    for(size_t xpos = 0; xpos < MENU_X; ++xpos) {
        for (size_t ypos = 0; ypos < MENU_Y; ++ypos) {
            m_window->cursor_move(ypos,xpos);

            m_window->putwch(L' ');
        }
    }
    m_window->cursor_move(0,0);

    m_window->rounded_box_sized(0, channels, MENU_Y, MENU_X, 0);

    m_window->set_fg_default();
    m_window->set_bg_default();

    m_usr = new NCLineEdit(m_nc, m_window, 3, 3, 1, 82, true, L"Username", m_mtx);
    m_focused = static_cast<FocusWidget*>(m_usr);

    m_pss = new NCLineEdit(m_nc, m_window, 12, 3, 1, 82, false, L"Password", m_mtx);
}

ApiKeyGameState::~ApiKeyGameState() {
    delete m_title;

    delete m_usr;
    delete m_pss;
}

void ApiKeyGameState::update() {
    if (m_title) m_title->update();
    if (m_usr) m_usr->update();
    if (m_pss) m_pss->update();

    unsigned y, x;
    m_parent->get_dim(&y, &x);

    m_window->move((y / 2) - (MENU_Y / 2),  (x / 2) - (MENU_X / 2));
}
