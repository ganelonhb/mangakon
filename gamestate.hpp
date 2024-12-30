#ifndef GAMESTATE_HPP
#define GAMESTATE_HPP

#include <ncpp/NotCurses.hh>
#include <ncpp/Plane.hh>

class GameState {
public:
    explicit GameState(ncpp::NotCurses *nc, ncpp::Plane *parent = nullptr)
        : m_nc{nc}
        , m_ownsParent{!bool(parent)}
    {
        m_parent = m_ownsParent ? m_nc->get_stdplane() : parent;
    }

    ~GameState() {
        if (m_ownsParent)
            delete m_parent;
    }

protected:
    ncpp::NotCurses *m_nc;
    ncpp::Plane *m_parent;

    bool m_ownsParent;
};

#endif
