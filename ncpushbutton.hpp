#ifndef NCPUSHBUTTON_HPP
#define NCPUSHBUTTON_HPP

#include <atomic>
#include <mutex>

#include "focuswidget.hpp"
#include "signal.hpp"
#include "utils.hpp"

class NCPushButton : public FocusWidget {
public:
  explicit NCPushButton(ncpp::NotCurses *nc, ncpp::Plane *parent = nullptr, std::wstring text = L"PushButton", uint32_t y = 0, uint32_t x = 0, uint32_t h = 1, std::mutex *mut = nullptr)
        : FocusWidget{nc, parent}
        , m_text{text}
        , m_y{y}
        , m_x{x}
        , m_h{h + 2}
        , clicked{mut}
        , pressed{mut}
        , released{mut}
        , m_focused{false}
        , m_pressing{false}
    {
        m_w = text.size() + 4;
        m_pushButton = new ncpp::Plane(m_parent, m_h, m_w, m_y, m_x);
    }

    bool focused() const override {
        return m_focused;
    }

    inline void draw_box() {
        m_pushButton->erase();
        m_pushButton->set_fg_default();

        if (m_pressing) {
            m_pushButton->set_bg_rgb(util::colors::defaults::BUTTONPRESS);
            m_pushButton->set_bg_alpha(0xFF);
        }
        else {
            m_pushButton->set_bg_rgb(m_focused ? util::colors::defaults::SELECTED : util::colors::defaults::BUTTON);
            m_pushButton->set_bg_alpha(0x00);
        }

        uint64_t channels = 0;

        m_pushButton->cursor_move(1,1);
        for(size_t i = 0; i < m_w; ++i)
            m_pushButton->putwch(L' ');
        m_pushButton->cursor_move(0,0);

        if (m_pressing) {
            ncchannels_set_bg_rgb(&channels, util::colors::defaults::BUTTONPRESS);
            ncchannels_set_bg_alpha(&channels, 0xFF);
        }
        else {
            ncchannels_set_bg_rgb(&channels, m_focused ? util::colors::defaults::SELECTED : util::colors::defaults::BUTTON);
            ncchannels_set_bg_alpha(&channels, 0x00);
        }

        m_pushButton->rounded_box_sized(0, channels, m_h, m_w, 0);

        m_pushButton->set_bg_default();
        m_pushButton->set_fg_default();
        m_pushButton->set_bg_alpha(0xFF);
    }

    inline void draw_text() {
        m_pushButton->set_fg_default();
        if (m_pressing) {
            m_pushButton->set_bg_rgb(util::colors::defaults::BUTTONPRESS);
            m_pushButton->set_bg_alpha(0xFF);
        }
        else {
            m_pushButton->set_bg_rgb(m_focused ? util::colors::defaults::SELECTED : util::colors::defaults::BUTTON);
            m_pushButton->set_bg_alpha(0x00);
        }

        m_pushButton->cursor_move(1, 2);

        m_pushButton->putstr(m_text.c_str());

        m_pushButton->set_bg_default();
        m_pushButton->set_bg_alpha(0xFF);
    }

    void update() override {
        draw_box();
        draw_text();
    }

    void handle_click(unsigned mouse_y, unsigned mouse_x) override {}

public signals:
    Signal<> clicked;
    Signal<> pressed;
    Signal<> released;

private:
    ncpp::Plane *m_pushButton;

    std::wstring m_text;

    uint32_t m_y;
    uint32_t m_x;
    uint32_t m_h;
    uint32_t m_w;

    bool m_focused;
    bool m_pressing;
};

#endif
