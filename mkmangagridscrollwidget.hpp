#ifndef MKMANGAGRIDSCROLLWIDGET_HPP
#define MKMANGAGRIDSCROLLWIDGET_HPP

#include <ncpp/NotCurses.hh>
#include <ncpp/Plane.hh>
#include <mutex>

#include "focuswidget.hpp"
#include "utils.hpp"

namespace FocusType {
    constexpr focus_t MKMANGAGRIDSCROLLWIDGET = util::compile_time_id::get_id("MKMangaGridScrollWidget");
}

class MKMangaGridScrollWidget : public FocusWidget {
public:
    explicit MKMangaGridScrollWidget(ncpp::NotCurses *nc, ncpp::Plane *parent = nullptr, std::mutex *mut = nullptr)
    : FocusWidget{nc, parent, FocusType::MKMANGAGRIDSCROLLWIDGET}
    {}

private:
    ncpp::Plane *m_gridArea;

    uint32_t m_y;
    uint32_t m_x;
    uint32_t m_ph;
    uint32_t m_pw;

    bool m_focused;

    uint32_t m_scroll_offset;
};

#endif
