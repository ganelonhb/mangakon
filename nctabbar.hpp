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
        , m_scroll_offset{0}
        , m_focused{false}
        , m_selected_tab{0}
    {
        m_tabBar = std::make_unique<ncpp::Plane>(m_parent, 1, m_parent->get_dim_x(), 0, 0);
        m_internal_width = m_parent->get_dim_x() - 2;
    }

    ~NCTabBar() override {
        FocusWidget::~FocusWidget();
    }

    inline void draw_bar() {
        m_tabBar->erase();

        m_tabBar->set_bg_rgb(util::colors::defaults::BUTTON);
        m_tabBar->set_bg_alpha(0xFF);
        m_tabBar->putwch(0,0,m_scroll_offset > 0 ? L'J' : L' ');
        m_tabBar->putwch(0, m_internal_width + 1, get_tab_width() >= m_internal_width ? L'L' : L' ');
        m_tabBar->set_bg_default();
        m_tabBar->set_bg_alpha(0x00);

        m_tabBar->set_bg_rgb(util::colors::defaults::TABBAR_BG);
        m_tabBar->set_bg_alpha(0xFF);
        for (size_t i = 1; i < m_internal_width + 1; ++i)
            m_tabBar->putc(0,i, ' ');
        m_tabBar->set_bg_default();
        m_tabBar->set_bg_alpha(0x00);

    }

    inline void draw_text() {
        m_tabBar->cursor_move(0,1);
        m_tabBar->set_bg_alpha(0xFF);

        for (size_t i = 0; i < m_tabs.size(); ++i) {
            const std::wstring tab = m_tabs[i];

            std::wstring display_tab = L"|" + tab + L"|";
            size_t tab_width = display_tab.length();

            if (tab_width >m_internal_width) break;

            m_tabBar->set_bg_rgb(i == m_selected_tab ? util::colors::defaults::SELECTED : util::colors::defaults::TABBAR_TAB);

            m_tabBar->putstr(display_tab.c_str());
        }
        m_tabBar->set_bg_alpha(0x00);
        m_tabBar->set_bg_default();
    }

    void update() override {

        m_internal_width = m_parent->get_dim_x() - 2;
        m_tabBar->resize(1, m_parent->get_dim_x());

        draw_bar();
        draw_text();
    }

    void focus() override {
        m_focused = true;
    }

    void unfocus() override {
        m_focused = false;
    }

    void add_tab(const std::wstring &tab) {
        m_tabs.push_back(tab);
    }

    void select_tab(size_t index) {
        if (index < m_tabs.size() and index >= 0) {
            m_selected_tab = index;
        }
    }

public signals:
    Signal<> clicked;
private:
    std::unique_ptr<ncpp::Plane> m_tabBar;

    size_t m_internal_width;
    int m_scroll_offset;
    bool m_focused;
    size_t m_selected_tab;

    std::vector<std::wstring> m_tabs;

    uint32_t get_tab_width() const {
        int width = 0;
        for (std::wstring tab : m_tabs) {
            width += tab.length() + 2;
        }
        return width;
    }
};

#endif
