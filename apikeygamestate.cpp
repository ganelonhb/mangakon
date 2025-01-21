#include "apikeygamestate.h"
#include "nclineedit.hpp"
#include "utils.hpp"

constexpr int MENU_Y = 22;
constexpr int MENU_X = 90;

ApiKeyGameState::ApiKeyGameState(ncpp::NotCurses *nc, ncpp::Plane *parent, std::mutex *mtx, SecureStorePass *init)
    : GameState{nc, parent, mtx, GameStateType::APIKEYGAMESTATE}
    , m_init{init}
{
    m_title = new Title(m_nc, m_parent, 5);


    unsigned FG = util::colors::defaults::WINDOW_BG;
    unsigned BG = util::colors::defaults::SELECTED;

    unsigned y;
    unsigned x;
    m_parent->get_dim(&y, &x);
    m_ph = y;
    m_pw = x;

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

    if (m_init)
        m_usr->set_text(util::str2wstr(m_init->user()));

    m_pss = new NCLineEdit(m_nc, m_window, 7, 3, 1, 82, false, L"Password", m_mtx, true);

    if (m_init)
        m_pss->set_text(util::str2wstr(m_init->password()));

    m_key = new NCLineEdit(m_nc, m_window, 11, 3, 1, 82, false, L"API Key", m_mtx);

    if (m_init)
        m_key->set_text(util::str2wstr(m_init->apikey()));

    m_sct = new NCLineEdit(m_nc, m_window, 15, 3, 1, 82, false, L"Secret", m_mtx, true);

    if (m_init)
        m_sct->set_text(util::str2wstr(m_init->secret()));

    m_ok = new NCPushButton(m_nc, m_window, L"Ok", MENU_Y - 4, MENU_X - 7, 1, m_mtx);
    m_skip = new NCPushButton(m_nc, m_window, L"Skip", MENU_Y - 4, MENU_X - 17, 1, m_mtx);

    m_usr->setDown(m_pss);
    m_usr->setUp(m_ok);

    m_pss->setDown(m_key);
    m_pss->setUp(m_usr);

    m_key->setDown(m_sct);
    m_key->setUp(m_pss);

    m_sct->setDown(m_skip);
    m_sct->setUp(m_key);

    m_skip->setUp(m_sct);
    m_skip->setDown(m_usr);
    m_skip->setLeft(m_ok);
    m_skip->setRight(m_ok);

    m_ok->setUp(m_sct);
    m_ok->setDown(m_usr);
    m_ok->setLeft(m_skip);
    m_ok->setRight(m_skip);

    m_focused = static_cast<FocusWidget*>(m_usr);
}

ApiKeyGameState::~ApiKeyGameState() {
    delete m_title;

    delete m_usr;
    delete m_pss;

    delete m_key;
    delete m_sct;

    delete m_ok;
    delete m_skip;

    delete m_window;

    GameState::~GameState();
}

void ApiKeyGameState::update() {
    if (m_title) m_title->update();
    if (m_usr) m_usr->update();
    if (m_pss) m_pss->update();
    if (m_key) m_key->update();
    if (m_sct) m_sct->update();
    if (m_ok) m_ok->update();
    if (m_skip) m_skip->update();

    unsigned y, x;
    m_parent->get_dim(&y, &x);
    if (y != m_ph or x != m_pw) {
        m_window->move((y / 2) - (MENU_Y / 2),  (x / 2) - (MENU_X / 2));
        m_ph = y;
        m_pw = x;
    }
}

bool ApiKeyGameState::block_fortype() {
    return static_cast<bool>(m_focused);
}

gs_info_t* ApiKeyGameState::handle_event(ncinput &ni, char32_t ch) {
    NCLineEdit *focused = static_cast<NCLineEdit*>(m_focused);

    if (ni.y > -1 and ni.x > -1 and ch == NCKEY_BUTTON1) { // Process mouse click
        if (focused) focused->unfocus();
        m_focused = nullptr;

        // TODO: Calculate offsets for each cursor click (easy mode, just count how many away)
        if (m_usr->collides_mouse(ni.y, ni.x)) {
            m_usr->focus();
            m_usr->cursor_click(ni.x, 16);
            m_pss->unfocus();
            m_key->unfocus();
            m_sct->unfocus();
            m_ok->unfocus();
            m_skip->unfocus();
            m_focused = static_cast<FocusWidget*>(m_usr);
            return nullptr;
        }

        if (m_pss->collides_mouse(ni.y, ni.x)) {
            m_usr->unfocus();
            m_pss->focus();
            m_pss->cursor_click(ni.x,16);
            m_key->unfocus();
            m_sct->unfocus();
            m_skip->unfocus();
            m_focused = static_cast<FocusWidget*>(m_pss);
            return nullptr;
        }

        if (m_key->collides_mouse(ni.y, ni.x)) {
            m_usr->unfocus();
            m_pss->unfocus();
            m_key->focus();
            m_key->cursor_click(ni.x,16);
            m_sct->unfocus();
            m_ok->unfocus();
            m_skip->unfocus();
            m_focused = static_cast<FocusWidget*>(m_key);
            return nullptr;
        }

        if (m_sct->collides_mouse(ni.y, ni.x)) {
            m_usr->unfocus();
            m_pss->unfocus();
            m_key->unfocus();
            m_sct->focus();
            m_sct->cursor_click(ni.x,16);
            m_ok->unfocus();
            m_skip->unfocus();
            m_focused = static_cast<FocusWidget*>(m_sct);
            return nullptr;
        }

        // TODO: Make skip button skip the apikey login.
        // TODO: Make ok button validate. Red popup if invalid.

        if (m_ok->collides_mouse(ni.y, ni.x) and !m_skip->pressing()) {
            if (ni.evtype == ncpp::EvType::Press) {
                m_ok->focus();
                m_skip->unfocus();
                m_focused = m_ok;
                m_ok->set_pressing(true);
            }
            else if (ni.evtype == ncpp::EvType::Release) {
                m_ok->focus();
                m_skip->unfocus();
                m_focused = m_ok;
                m_ok->handle_release(ni.y, ni.x);
                m_ok->set_pressing(false);
            }
            return nullptr;
        }
        else if (!m_ok->collides_mouse(ni.y, ni.x) and m_ok->pressing()) {
            if (ni.evtype == ncpp::EvType::Release) {
                m_ok->unfocus();
                m_ok->set_pressing(false);
                m_focused = nullptr;
            }
        }

        if ((m_skip->collides_mouse(ni.y, ni.x) and !m_ok->pressing())) {
            if (ni.evtype == ncpp::EvType::Press) {
                m_skip->focus();
                m_ok->unfocus();
                m_focused = m_skip;
                m_skip->set_pressing(true);
            }
            else if (ni.evtype == ncpp::EvType::Release) {
                m_skip->focus();
                m_ok->unfocus();
                m_focused = m_skip;
                m_skip->handle_release(ni.y, ni.x);
                m_skip->set_pressing(false);

                gs_info_t *state = new gs_info_t{ .state=GameStateType::MAINGAMESTATE };

                return state;
            }
            return nullptr;
        }
        else if (!m_skip->collides_mouse(ni.y, ni.x) and m_skip->pressing()) {
            if (ni.evtype == ncpp::EvType::Release) {
                m_skip->unfocus();
                m_skip->set_pressing(false);
                m_focused = nullptr;
            }
        }

        return nullptr;
    }

    bool isSkip = m_focused == m_skip;
    bool isOk = m_focused == m_ok;

    if (isSkip and ch == NCKEY_ENTER) {
        gs_info_t *state = new gs_info_t{ .state=GameStateType::MAINGAMESTATE };

        return state;
    }

    // Verify apikey, then advance if valid.
    if (isOk and ch == NCKEY_ENTER) {
        gs_info_t *state = nullptr;

        return state;
    }

    if (ni.evtype == ncpp::EvType::Release)
        return nullptr;

    if (ch == NCKEY_TAB or ch == NCKEY_ENTER) {
        if (ch == NCKEY_ENTER)
            emit focused->activated();

        if (m_focused == nullptr) {
            m_usr->focus();
            m_focused = m_usr;

            return nullptr;
        }
        if (m_focused == m_ok) {
            m_ok->unfocus();
            m_focused = m_usr;
            m_usr->focus();

            return nullptr;
        }

        FocusWidget *next = m_focused->right();

        if (!next) next = m_focused->down();

        if (next) {
            m_focused->unfocus();
            m_focused = next;
            m_focused->focus();
        }

        return nullptr;
    }

    if (ch == NCKEY_UP or ch == NCKEY_DOWN or ch == NCKEY_LEFT or ch == NCKEY_RIGHT) {
        if (m_focused == nullptr) {
            m_usr->focus();
            m_focused = m_usr;

            return nullptr;
        }

        focus_t type = m_focused->type();

        FocusWidget *next = nullptr;

        switch (ch) {
        case NCKEY_UP:
            next = m_focused->up();
            break;
        case NCKEY_DOWN:
            next = m_focused->down();
            break;
        case NCKEY_LEFT:
            if (type == FocusType::NCPUSHBUTTON)
                next = m_focused->left();
            else if (m_focused->type() == FocusType::NCLINEEDIT)
                static_cast<NCLineEdit*>(m_focused)->cleft();
            break;
        case NCKEY_RIGHT:
            if (type == FocusType::NCPUSHBUTTON)
                next = m_focused->right();
            else if (m_focused->type() == FocusType::NCLINEEDIT)
                static_cast<NCLineEdit*>(m_focused)->cright();
            break;
        default:
            break;
        }

        if (next) {
            m_focused->unfocus();
            m_focused = next;
            m_focused->focus();
        }

        return nullptr;
    }

    if (!focused or focused->type() != FocusType::NCLINEEDIT)
        return nullptr;

    if (ch == NCKEY_BACKSPACE) {
        focused->backspace();
        return nullptr;
    }

    if (ch == NCKEY_DEL) {
        focused->del();
        return nullptr;
    }

    if (ch == NCKEY_ESC or ch == NCKEY_ENTER) {
        focused->esc();
        m_focused = nullptr;
        return nullptr;
    }

    if (ch >= 32 and ch <= 0x10FFFF) {
        focused->putch(ch);
    }

    return nullptr;
}
