#ifndef NCLINEEDIT_HPP
#define NCLINEEDIT_HPP

#include "notcurses/notcurses.h"
#include <ncpp/NotCurses.hh>
#include <ncpp/Plane.hh>
#include <atomic>
#include <string>


class NCLineEdit {
public:
    explicit NCLineEdit(ncpp::NotCurses *nc, ncpp::Plane *parent = nullptr, uint32_t y = 0, uint32_t x = 0, uint32_t h = 1, uint32_t w = 10, bool isDefault = false)
    : m_nc{nc}
    , m_y{y}
    , m_x{x}
    , m_h{h}
    , m_w{w}
    , m_cursor_pos{0}
    , m_scroll_offset{0}
    , m_focused{true}
    , m_frame{0u}
    {
        m_parent = static_cast<bool>(parent) ? parent : m_nc->get_stdplane();
        m_lineEdit = new ncpp::Plane(m_parent, m_h + 2,m_w + 2,m_y,m_x);
    }

    ~NCLineEdit() {
        delete m_lineEdit;
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
        m_lineEdit->set_bg_alpha(0x00);
        m_lineEdit->rounded_box_sized(0, 0, m_h + 2, m_w + 2, 0);
    }

    inline void draw_text() {
        const uint32_t usable_width = m_w - 1;

        std::wstring visible_text = m_text.substr(m_scroll_offset, usable_width);
        m_lineEdit->set_fg_default();
        m_lineEdit->set_bg_alpha(0x00);
        m_lineEdit->cursor_move(1, 1);
        m_lineEdit->putstr(visible_text.c_str());

        if (m_focused) {
            int cursor_x = m_cursor_pos - m_scroll_offset + 1;

            if (m_frame >= 0u and m_frame < 30) {
                if (cursor_x >= 1 && cursor_x < m_w - 1) {
                    m_lineEdit->cursor_move(1, cursor_x);
                    m_lineEdit->styles_set(ncpp::CellStyle(NCSTYLE_UNDERLINE));
                    m_lineEdit->putwch((m_cursor_pos) < m_text.size() ? m_text[m_cursor_pos] : L' ');
                    m_lineEdit->styles_off(ncpp::CellStyle(NCSTYLE_UNDERLINE));
                }
            }
        }
    }

    void update() {
        if (m_frame == 60u)
            m_frame = 0u;

        draw_box();
        draw_text();
        ++m_frame;
    }

    void backspace() {
        if (m_cursor_pos > 0) {
            m_text.erase(m_cursor_pos - 1, 1);
            --m_cursor_pos;
            if (m_scroll_offset > 0 && m_cursor_pos <m_scroll_offset)
                --m_scroll_offset;
        }
    }

    void putch(char32_t ch) {
        m_text.insert(m_cursor_pos, 1, static_cast<wchar_t>(ch));
        ++m_cursor_pos;
        if (m_cursor_pos - m_scroll_offset >= static_cast<size_t>(m_w - 1)) {
            ++m_scroll_offset;
        }
    }

    void left() {

    }

    void right() {

    }

    bool focused() {
        return m_focused;
    }

    std::wstring text() {
        return m_text;
    }

    size_t cursor_pos() {
        return m_cursor_pos;
    }

    size_t scroll() {
        return m_scroll_offset;
    }


private:
    ncpp::NotCurses *m_nc;
    ncpp::Plane *m_parent;

    ncpp::Plane *m_lineEdit;

    uint32_t m_y;
    uint32_t m_x;
    uint32_t m_h;
    uint32_t m_w;
    std::wstring m_text;
    std::atomic<size_t> m_cursor_pos;
    std::atomic<size_t> m_scroll_offset;
    bool m_focused;

    uint8_t m_frame;
};

#endif

