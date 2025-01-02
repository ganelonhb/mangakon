#include "apikeygamestate.h"
#include "nclineedit.hpp"

constexpr int MENU_Y = 20;
constexpr int MENU_X = 90;

ApiKeyGameState::ApiKeyGameState(ncpp::NotCurses *nc, ncpp::Plane *parent, std::mutex *mtx)
    : GameState{nc, parent, mtx}
{
    m_title = new Title(m_nc, m_parent, 5);

    unsigned FG = util::colors::defaults::WINDOW_BG;
    unsigned BG = util::colors::defaults::SELECTED;

    unsigned y;
    unsigned x;
    m_parent->get_dim(&y, &x);

    m_window = new ncpp::Plane(m_parent, MENU_Y, MENU_X, (y / 2) - (MENU_Y / 2),  (x / 2) - (MENU_X / 2));
    m_window->set_fg_rgb(BG);
    m_window->set_bg_rgb(FG);

    uint64_t channels = 0;
    ncchannels_set_bg_rgb(&channels, FG);
    ncchannels_set_fg_rgb(&channels, BG);

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

    m_pss = new NCLineEdit(m_nc, m_window, 12, 3, 1, 82, false, L"Password", m_mtx, true);

    m_ok = new NCPushButton(m_nc, m_window, L"Ok", 1, 1, 1, m_mtx);
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
    if (m_ok) m_ok->update();

    unsigned y, x;
    m_parent->get_dim(&y, &x);

    m_window->move((y / 2) - (MENU_Y / 2),  (x / 2) - (MENU_X / 2));
}

bool ApiKeyGameState::block_fortype() {
    return static_cast<bool>(m_focused);
}

void ApiKeyGameState::handle_event(ncinput &ni, char32_t ch) {
    NCLineEdit *focused = static_cast<NCLineEdit*>(m_focused);

    if (ni.y > -1 and ni.x > -1 and ch == NCKEY_BUTTON1) { // Process mouse click
        if (focused) focused->unfocus();
        m_focused = nullptr;

        if (m_usr->collides_mouse(ni.y, ni.x)) {
            m_usr->focus();
            m_focused = static_cast<FocusWidget*>(m_usr);
            return;
        }

        if (m_pss->collides_mouse(ni.y, ni.x)) {
            m_pss->focus();
            m_focused = static_cast<FocusWidget*>(m_pss);
        }
        return;
    }

    if (ni.evtype == ncpp::EvType::Release)
        return;

    if (ch == NCKEY_DOWN) {
        if (m_focused == m_usr) {
            m_usr->unfocus();
            m_focused = m_pss;
            m_pss->focus();
        }
        else if(m_focused == m_pss or m_focused == nullptr) {
            if (m_focused) m_pss->unfocus();
            m_focused = m_usr;
            m_usr->focus();
        }

        return;
    }

    if (ch == NCKEY_TAB) {
        if (m_focused == nullptr) {
            m_usr->focus();
            m_focused = m_usr;
        }
        else if (m_focused == m_usr) {
            m_usr->unfocus();
            m_pss->focus();
            m_focused = m_pss;
        }
        else if (m_focused == m_pss) {
            m_pss->unfocus();
            m_focused = nullptr;
        }

        return;
    }

    if (ch == NCKEY_UP) {
        if (m_focused == m_usr or m_focused == nullptr) {
            if (m_focused) m_usr->unfocus();
            m_focused = m_pss;
            m_pss->focus();
        }
        else if (m_focused == m_pss) {
            m_pss->unfocus();
            m_focused = m_usr;
            m_usr->focus();
        }

        return;
    }

    if (!focused)
        return;

    if (ch == NCKEY_BACKSPACE) {
        focused->backspace();
        return;
    }

    if (ch == NCKEY_DEL) {
        focused->del();
        return;
    }

    if (ch == NCKEY_LEFT) {
        focused->left();
        return;
    }

    if (ch == NCKEY_RIGHT) {
        focused->right();
        return;
    }

    if (ch == NCKEY_ESC or ch == NCKEY_ENTER) {
        focused->esc();
        if (ch == NCKEY_ENTER)
            emit focused->activated();
        m_focused = nullptr;
        return;
    }

    if (ch >= 32 && ch <= 0x10FFFF) {
        focused->putch(ch);
    }
}
