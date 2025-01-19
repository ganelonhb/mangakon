#ifndef MKWIDGET_HPP
#define MKWIDGET_HPP

#include <vector>

#include <ncpp/NotCurses.hh>
#include <ncpp/Plane.hh>

#include "focuswidget.hpp"

class MKWidget {
public:
    MKWidget(ncpp::NotCurses *nc, ncpp::Plane *parent = nullptr)
    : m_nc{nc}
    , m_parent{parent}
    , m_ownsParent{!static_cast<bool>(parent)}
    {}

    virtual ~MKWidget() {
        for(FocusWidget *widget :m_widgets)
            delete widget;
    }

    virtual void update() {}
    virtual bool handle_event(ncinput &ni, char32_t ch) { return false; }

protected:
    ncpp::NotCurses *m_nc;
    ncpp::Plane *m_parent;

    bool m_ownsParent;

    std::vector<FocusWidget *> m_widgets;

};


#endif
