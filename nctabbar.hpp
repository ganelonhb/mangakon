#ifndef NCTABBAR_HPP
#define NCTABBAR_HPP

#include <mutex>
#include <vector>

#include "focuswidget.hpp"
#include "signal.hpp"
#include "utils.hpp"

namespace FocusType {
    constexpr focus_t NCTABBAR = util::compile_time_id::get_id("NCTABBAR");
}

class NCTabBar : public FocusWidget {
public:
    explicit NCTabBar(ncpp::NotCurses *nc, ncpp::Plane *parent = nullptr, std::mutex *mut = nullptr)
        :FocusWidget(nc, parent, FocusType::NCTABBAR)
        , clicked{mut}
    {

    }

    ~NCTabBar() override {
        FocusWidget::~FocusWidget();
    }

    inline void draw_bar() {

    }

    inline void draw_text() {

    }

    void update() override {
        draw_bar();
        draw_text();
    }

    void focus() override {
        m_focused = true;
    }

    void unfocus() override {
        m_focused = false;
    }

public signals:
    Signal<> clicked;
private:
    std::unique_ptr<ncpp::Plane> m_tabBar;

    int m_internal_width;
    int m_scroll_offset;

    bool m_focused;

    std::vector<std::wstring> m_tabs;
};

#endif
