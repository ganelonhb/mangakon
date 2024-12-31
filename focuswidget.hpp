#ifndef FOCUSWIDGET_HPP
#define FOCUSWIDGET_HPP

#include <ncpp/NotCurses.hh>
#include <ncpp/Plane.hh>

class FocusWidget {
public:
    explicit FocusWidget(ncpp::NotCurses *nc, ncpp::Plane *parent = nullptr)
    : m_nc{nc}
    {
        m_parent = static_cast<bool>(parent) ? parent : m_nc->get_stdplane();
    }

    virtual void update() {};
    virtual void handle_click(unsigned y, unsigned x) {};

    virtual bool focused() const { return false; };



protected:
    ncpp::NotCurses *m_nc;
    ncpp::Plane *m_parent;
};

#endif
