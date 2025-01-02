#ifndef GAMESTATE_HPP
#define GAMESTATE_HPP

#include <ncpp/NotCurses.hh>
#include <ncpp/Plane.hh>

#include <mutex>

class GameState {
public:
    explicit GameState(ncpp::NotCurses *nc, ncpp::Plane *parent = nullptr, std::mutex *mtx = nullptr)
        : m_nc{nc}
        , m_mtx{mtx}
        , m_ownsParent{!bool(parent)}
    {
        m_parent = m_ownsParent ? m_nc->get_stdplane() : parent;
    }

    ~GameState() {
        if (m_ownsParent)
            delete m_parent;
    }


    virtual void update() {}
    virtual void handle_event(ncinput &ni, char32_t ch) {}
    virtual bool block_fortype() { return false; }

protected:
    ncpp::NotCurses *m_nc;
    ncpp::Plane *m_parent;

    std::mutex *m_mtx;

    bool m_ownsParent;
};

#endif
