#ifndef FOCUSWIDGET_HPP
#define FOCUSWIDGET_HPP

#include <ncpp/NotCurses.hh>
#include <ncpp/Plane.hh>

using focus_t = uint32_t;

namespace FocusType {
    constexpr focus_t UNKNOWN = -1;
}

struct FocusID {};

class FocusWidget {
public:
    explicit FocusWidget(ncpp::NotCurses *nc, ncpp::Plane *parent = nullptr, focus_t type = FocusType::UNKNOWN)
    : m_nc{nc}
    , m_ownsParent{!static_cast<bool>(parent)}
    , m_type{type}
    {
        m_up = m_down = m_left = m_right = nullptr;
        m_parent = m_ownsParent ? m_nc->get_stdplane() : parent;
    }

    virtual ~FocusWidget() {
        //if (m_ownsParent) delete m_parent;
    }

    virtual void update() {};
    virtual void handle_click(unsigned y, unsigned x) {}

    virtual bool focused() const { return false; }
    virtual void focus() {}
    virtual void unfocus() {}
    virtual void setFocus(bool focus) {}
    virtual focus_t type() { return m_type; }

    virtual void setUp(FocusWidget *up) { m_up = up; }
    virtual void setDown(FocusWidget *down) { m_down = down; }
    virtual void setLeft(FocusWidget *left) { m_left = left; }
    virtual void setRight(FocusWidget *right) { m_right = right; }

    virtual FocusWidget * up() const { return m_up; }
    virtual FocusWidget * down() const { return m_down; }
    virtual FocusWidget * left() const { return m_left; }
    virtual FocusWidget * right() const { return m_right; }


protected:
    ncpp::NotCurses *m_nc;
    ncpp::Plane *m_parent;
    bool m_ownsParent;

    focus_t m_type;

    FocusWidget *m_up;
    FocusWidget *m_down;
    FocusWidget *m_left;
    FocusWidget *m_right;
};

#endif
