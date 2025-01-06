#ifndef NCLINEEDIT_HPP
#define NCLINEEDIT_HPP

#include <notcurses/notcurses.h>
#include <ncpp/NotCurses.hh>
#include <ncpp/Plane.hh>
#include <atomic>
#include <string>
#include <mutex>

#include <memory>

#include "focuswidget.hpp"
#include "signal.hpp"
#include "utils.hpp"

namespace FocusType {
    constexpr focus_t NCLINEEDIT = util::compile_time_id::get_id("NCLineEdit");
}

class NCLineEdit : public FocusWidget {
public:
    explicit NCLineEdit(ncpp::NotCurses *nc, ncpp::Plane *parent = nullptr, uint32_t y = 0, uint32_t x = 0, uint32_t h = 1, uint32_t w = 10, bool isDefault = false, std::wstring title = L"", std::mutex *mut = nullptr, bool secure = false, wchar_t secure_char = L'*')
    : FocusWidget{nc, parent, FocusType::NCLINEEDIT}
    , m_y{y}
    , m_x{x}
    , m_h{h}
    , m_w{w}
    , m_cursor_pos{0}
    , m_scroll_offset{0}
    , m_focused{isDefault}
    , m_frame{0u}
    , clicked{mut}
    , activated{mut}
    , m_title{title}
    , m_secure{secure}
    , m_secure_char{secure_char}
    {
        m_lineEdit = std::make_unique<ncpp::Plane>(m_parent, m_h + 2, m_w + 2,m_y,m_x);
    }

    ~NCLineEdit() override {
        FocusWidget::~FocusWidget();
    }

    bool focused() const override {
        return m_focused;
    }

    void focus() override {
        m_focused = true;
    }

    void unfocus() override {
        m_focused = false;
    }

    void setFocus(bool focus) override {
        m_focused = focus;
    }

    void move(uint32_t y, uint32_t x) {
        m_y = y;
        m_x = x;
        m_lineEdit->move(y, x);
    }

    void resize(uint32_t h, uint32_t w) {
        m_h = h;
        m_w = w;
        m_lineEdit->resize(h, w);
    }

    inline void draw_box() {
        m_lineEdit->erase();
        m_lineEdit->set_fg_default();

        if (m_focused) {
            m_lineEdit->set_bg_rgb(util::colors::defaults::SELECTED);
            m_lineEdit->set_bg_alpha(0xFF);
        }
        else {
            m_lineEdit->set_bg_rgb(m_parent->get_bg_rgb());
            m_lineEdit->set_bg_alpha(0x00);
        }

        uint64_t channels = 0;

        m_lineEdit->cursor_move(1,1);
        for(size_t i = 0; i < m_w; ++i)
            m_lineEdit->putwch(L' ');
        m_lineEdit->cursor_move(0,0);

        if (m_focused) {
            ncchannels_set_bg_rgb(&channels, util::colors::defaults::SELECTED);
            ncchannels_set_bg_alpha(&channels, 0xFF);
        }
        else {
            ncchannels_set_bg_rgb(&channels, m_parent->get_bg_rgb());
            ncchannels_set_bg_alpha(&channels, 0x00);
        }

        m_lineEdit->rounded_box_sized(0, channels, m_h + 2, m_w + 2, 0);

        if (m_title != L"") {
                m_lineEdit->cursor_move(0,1);
                std::wstring newtitle = m_title;
                for (wchar_t &ch : newtitle) {
                    if (ch == L' ')
                        ch = L'─';
                }
                m_lineEdit->putstr(newtitle.c_str());
                m_lineEdit->cursor_move(0,0);

        }

        m_lineEdit->set_bg_default();
        m_lineEdit->set_fg_default();
        m_lineEdit->set_bg_alpha(0xFF);
    }

    void set_title(std::wstring title) {
        m_title = title;
    }

    inline void draw_text() {
        const uint32_t usable_width = m_w - 1;
        uint32_t screen_width = 0;

        std::wstring visible_text;
        for (size_t i = m_scroll_offset; i < m_text.size(); ++i) {
            int char_width = wcwidth(m_text[i]);
            if (char_width < 0) char_width = 1;

            if (screen_width + char_width > usable_width) break;

            visible_text += m_text[i];
            screen_width += char_width;
        }

        m_lineEdit->set_fg_default();

        if (m_focused) {
            m_lineEdit->set_bg_rgb(util::colors::defaults::SELECTED);
            m_lineEdit->set_bg_alpha(0xFF);
        }
        else {
            m_lineEdit->set_bg_rgb(m_parent->get_bg_rgb());
            m_lineEdit->set_bg_alpha(0x00);
        }

        m_lineEdit->cursor_move(1, 1);
        if (m_secure) {
            for(size_t i = 0; i <visible_text.size(); ++i)
                m_lineEdit->putwch(m_secure_char);
        }
        else {
            m_lineEdit->putstr(visible_text.c_str());
        }

        if (m_focused) {
            uint32_t cursor_screen_pos = 1;
            screen_width = 0;

            for (size_t i = m_scroll_offset; i < m_cursor_pos; ++i) {
                int char_width = wcwidth(m_text[i]);
                if (char_width < 0) char_width = 1;
                screen_width += char_width;
            }
            cursor_screen_pos += screen_width;

            if (cursor_screen_pos >= 1 and cursor_screen_pos < m_w) {
                if (m_frame >= 0u and m_frame < 30) {
                    m_lineEdit->cursor_move(1, cursor_screen_pos);
                    m_lineEdit->styles_set(ncpp::CellStyle(NCSTYLE_UNDERLINE));
                    wchar_t blinker = m_secure ? m_secure_char : m_text[m_cursor_pos];
                    m_lineEdit->putwch((m_cursor_pos < m_text.size()) ? blinker : L' ');

                    m_lineEdit->styles_off(ncpp::CellStyle(NCSTYLE_UNDERLINE));
                }
            }
        }

        m_lineEdit->set_bg_default();
        m_lineEdit->set_fg_default();
        m_lineEdit->set_bg_alpha(0xFF);
    }

    void update() override {
        if (m_frame == 60u)
            m_frame = 0u;

        draw_box();
        draw_text();
        ++m_frame;
    }

    void handle_click(unsigned mouse_y, unsigned mouse_x) override {
        if (collides_mouse(mouse_y, mouse_x))
        {
            m_focused = true; // TODO: Delete me.
            emit clicked();
        }
        else {
            m_focused = false;
        }
    }

    void backspace() {
        if (m_cursor_pos > 0) {
            int char_width = wcwidth(m_text[m_cursor_pos - 1]);
            if (char_width < 0) char_width = 1;

            m_text.erase(m_cursor_pos - 1, 1);
            --m_cursor_pos;

            uint32_t new_width = 0;
            for (size_t i = m_scroll_offset; i < m_cursor_pos; ++i) {
                int width = wcwidth(m_text[i]);
                if (width < 0) width = 1;
                new_width += width;
            }

            if (new_width < static_cast<uint32_t>(m_w - 1)) {
                m_scroll_offset = 0;
            } else if (m_cursor_pos < m_scroll_offset) {
                m_scroll_offset -= char_width;
            }
        }
    }

    void del() {
        if (m_cursor_pos < m_text.size()) {
            int char_width = wcwidth(m_text[m_cursor_pos]);
            if (char_width < 0) char_width = 1;

            m_text.erase(m_cursor_pos, 1);
        }
    }

    void esc() {
        m_focused = false;
    }

    void putch(char32_t ch) {
        if (ch == '\n' or ch == '\r' or ch == '\t')
            return;

        m_text.insert(m_cursor_pos, 1, static_cast<wchar_t>(ch));
        int char_width = wcwidth(ch);
        if (char_width < 0) char_width = 1;
        ++m_cursor_pos;

        uint32_t new_width = 0;
        for (size_t i = m_scroll_offset; i < m_cursor_pos; ++i) {
            int width = wcwidth(m_text[i]);
            if (width < 0) width = 1;
            new_width += width;
        }

        if (new_width >= static_cast<uint32_t>(m_w - 1)) {
            m_scroll_offset += char_width;
        }
    }

    void left() {
        if (m_cursor_pos > 0) {
            --m_cursor_pos;
            int char_width = wcwidth(m_text[m_cursor_pos]);
            if (char_width < 0) char_width = 1;

            if (m_scroll_offset > 0 and m_cursor_pos < m_scroll_offset) {
                m_scroll_offset -= char_width;
            }
        }
    }

    void right() {
        if (m_cursor_pos < m_text.size()) {
            int char_width = wcwidth(m_text[m_cursor_pos]);
            if (char_width < 0) char_width = 1;

            ++m_cursor_pos;

            uint32_t new_width = 0;
            for (size_t i = m_scroll_offset; i <= m_cursor_pos; ++i) {
                int width = wcwidth(m_text[i]);
                if (width < 0) width = 1;
                new_width += width;
            }

            if (new_width >= static_cast<uint32_t>(m_w - 1)) {
                m_scroll_offset += char_width;
            }
        }
    }

    std::wstring text()  const{
        return m_text;
    }

    size_t cursor_pos() const {
        return m_cursor_pos;
    }

    size_t scroll() {
        return m_scroll_offset;
    }

    inline bool collides_mouse(unsigned mouse_y, unsigned mouse_x) {
        int p_y = m_parent->get_y();
        int p_x = m_parent->get_x();


        return mouse_x >= p_x + m_x and mouse_x <= p_x + m_x + m_w + 2 and mouse_y >= p_y + m_y and mouse_y <= p_y + m_y + m_h + 2;
    }

public signals:
    Signal<> clicked;
    Signal<> activated;


private:

    std::unique_ptr<ncpp::Plane> m_lineEdit;

    uint32_t m_y;
    uint32_t m_x;
    uint32_t m_h;
    uint32_t m_w;
    std::wstring m_text;
    std::wstring m_title;
    std::atomic<size_t> m_cursor_pos;
    std::atomic<size_t> m_scroll_offset;
    bool m_focused;

    uint8_t m_frame;

    bool m_secure;
    wchar_t m_secure_char;
};

#endif

