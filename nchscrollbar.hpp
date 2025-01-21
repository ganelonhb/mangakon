#ifndef NCHSCROLLBAR_HPP
#define NCHSCROLLBAR_HPP

#include <cstdint>
#include <ncpp/NotCurses.hh>
#include <ncpp/NotCurses.hh>
#include <mutex>

#include "focuswidget.hpp"
#include "utils.hpp"

namespace FocusType {
    constexpr focus_t NCHSCROLLBAR = util::compile_time_id::get_id("NCHScrollBar");
}

class NCHScrollBar : public FocusWidget {
public:
    explicit NCHScrollBar(ncpp::NotCurses *nc, ncpp::Plane *parent = nullptr, std::mutex *mut = nullptr)
    : FocusWidget{nc, parent, FocusType::NCHSCROLLBAR}
    {

    }

    ~NCHScrollBar() override {

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

    }

    void update() override {
        draw();
    }

    void handle_click(uint32_t mouse_y, uint32_t mouse_x) override {

    }

    uint64_t scroll_offset() { return m_scroll_offset; }
    uint64_t rows() { return m_numrows; }

    void set_scroll(uint64_t offset) { m_scroll_offset = offset; }
    void set_rows(uint64_t rows) { m_numrows = rows; }

    inline bool collides_mouse(unsigned mouse_y, unsigned mouse_x) {
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

    uint64_t m_scroll_offset;
    uint64_t m_numrows;
};

#endif
