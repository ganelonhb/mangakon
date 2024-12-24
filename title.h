#ifndef TITLE_H
#define TITLE_H

#include <cwchar>

#include <memory>

#include <ncpp/NotCurses.hh>
#include <ncpp/Plane.hh>

#include "gui.hpp"

/* Title
 * Renders a NotCurses Plane at the specified location. Handles resizes automatically.
 */
class Title {
    static const uint32_t CENTER = util::constants::align::CENTER;

public:
    explicit Title(ncpp::NotCurses *nc, ncpp::Plane *parent = nullptr,  uint32_t y = CENTER, uint32_t x = CENTER);
    ~Title();

    uint32_t ypos() const;
    uint32_t xpos() const;

    uint32_t h() const;
    uint32_t w() const;

    void update();
    void listen();

private:
    ncpp::NotCurses *m_nc;
    ncpp::Plane *m_parent;
    bool m_ownsParent;

    ncpp::Plane *m_title;

    uint32_t m_ypos;
    uint32_t m_xpos;

    uint32_t m_parenth;
    uint32_t m_parentw;
};

#endif
