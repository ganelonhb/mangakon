#ifndef GAMESTATE_HPP
#define GAMESTATE_HPP

#include <mutex>
#include <string>
#include <map>

#include <ncpp/NotCurses.hh>
#include <ncpp/Plane.hh>

using gamestate_t = uint64_t;

namespace GameStateType {
    constexpr gamestate_t UNKNOWN = -1;
}

struct gs_info_t {
    gamestate_t state;
    std::map<std::string, std::wstring> kwargs;
};

class GameState {
public:
    explicit GameState(ncpp::NotCurses *nc, ncpp::Plane *parent = nullptr, std::mutex *mtx = nullptr, gamestate_t type = GameStateType::UNKNOWN)
        : m_nc{nc}
        , m_mtx{mtx}
        , m_ownsParent{!bool(parent)}
        , m_type{type}
    {
        m_parent = m_ownsParent ? m_nc->get_stdplane() : parent;
    }

    virtual ~GameState() {
        /*if (m_ownsParent)
            delete m_parent;*/
    }


    virtual void update() {}
    virtual gs_info_t* handle_event(ncinput &ni, char32_t ch) { return nullptr; }
    virtual bool block_fortype() { return false; }

protected:
    ncpp::NotCurses *m_nc;
    ncpp::Plane *m_parent;

    std::mutex *m_mtx;

    bool m_ownsParent;

    gamestate_t m_type;
};

#endif
