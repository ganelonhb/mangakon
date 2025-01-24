#ifndef NCVSCROLLBAR_HPP
#define NCVSCROLLBAR_HPP

#include <cstdint>
#include <ncpp/NotCurses.hh>
#include <ncpp/Plane.hh>
#include <mutex>

#include "focuswidget.hpp"
#include "utils.hpp"

namespace FocusType {
    constexpr focus_t NCVSCROLLBAR = util::compile_time_id::get_id("NCVScrollBar");
}

class NCVScrollBar : public FocusWidget {
public:
    explicit NCVScrollBar(ncpp::NotCurses *nc, ncpp::Plane *parent = nullptr, std::mutex *mut = nullptr, uint32_t numrows = 1)
    : FocusWidget{nc, parent, FocusType::NCVSCROLLBAR}
    , m_ph{parent->get_dim_y()}
    , m_pw{parent->get_dim_x()}
    , m_scroll_offset{0}
    , m_numrows{numrows}
    {
        m_scrollbar = new ncpp::Plane(m_parent, m_parent->get_dim_y(), 1, 0, m_parent->get_dim_x() - 1);
    }

    ~NCVScrollBar() override {
        delete m_scrollbar;
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

    inline void draw() {
        m_scrollbar->erase();
        m_scrollbar->cursor_move(0, 0);

        m_scrollbar->set_bg_rgb(util::colors::defaults::BUTTON);
        m_scrollbar->set_bg_alpha(0xFF);
        m_scrollbar->putwch(0,0, L'▲');
        m_scrollbar->putwch(m_ph - 2,0, L'▼');

        m_scrollbar->set_bg_alpha(util::colors::defaults::TABBAR_BG);
        for(uint32_t i = 1; i < m_ph - 2; ++i)
            m_scrollbar->putwch(i, 0, L' ');

        m_scrollbar->set_bg_default();
        m_scrollbar->set_bg_alpha(0x00);
    }

    inline void draw_bar() {
        if (m_numrows <= m_ph) {
            for (uint32_t i = 1; i < m_ph - 2; ++i)
                m_scrollbar->putwch(i, 0, L'█');
        }
        else {
            uint32_t bar_size = std::min(std::max(1u, (m_ph - 2) * m_ph /m_numrows), m_ph - 2);

            uint32_t bar_start = 1 + (m_scroll_offset * (m_ph - 2) / m_numrows);

            if (bar_start + bar_size > m_ph - 3)
                bar_start = m_ph - 1 - bar_size;

            for (uint32_t i = 0; i < bar_size; ++i)
                m_scrollbar->putwch(bar_start + i, 0, L'█');
        }
    }

    void update() override {
        draw();
        draw_bar();
    }

    void handle_click(uint32_t mouse_y, uint32_t mouse_x) override {

    }

    uint32_t scroll_offset() { return m_scroll_offset; }
    uint32_t rows() { return m_numrows; }

    void set_scroll(uint32_t offset) { m_scroll_offset = offset; }
    void set_rows(uint32_t rows) { m_numrows = rows; }

    inline bool collides_mouse(uint32_t mouse_y, uint32_t mouse_x) {
        int p_y = m_parent->get_y();
        int p_x = m_parent->get_x();


        return mouse_x >= p_x + m_x and mouse_x <= p_x + m_x + m_pw + 2 and mouse_y >= p_y + m_y and mouse_y <= p_y + m_y + m_ph + 2;
    }

private:
    ncpp::Plane *m_scrollbar;

    uint32_t m_y;
    uint32_t m_x;
    uint32_t m_ph;
    uint32_t m_pw;

    bool m_focused;

    uint32_t m_scroll_offset;
    uint32_t m_numrows;
};

#endif
