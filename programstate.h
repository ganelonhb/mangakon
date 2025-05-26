#ifndef __PROGRAMSTATE_H__
#define __PROGRAMSTATE_H__

#include <mutex>

#include <string>
#include <map>
#include <set>
#include <ncpp/NotCurses.hh>
#include <ncpp/Plane.hh>

#include "util.h" // replace with smartcolors class

using programstate_t = uint64_t;

namespace ProgramStateType {
    constexpr programstate_t UNKNOWN = -1;
}

struct programstate_info_t {
    programstate_t state;
    std::map<std::string, std::wstring> kwargs;
};

class ProgramState {
public:
    bool gameover = false;

    explicit ProgramState(ncpp::NotCurses *nc, ncpp::Plane *parent = nullptr, std::mutex *mtx = nullptr, const util::smart_colors::TerminalColors *const tc = nullptr, programstate_t type = ProgramStateType::UNKNOWN);

    virtual ~ProgramState();

    virtual void update();
    virtual programstate_info_t *handle_event(ncinput &ni, char32_t ch);
    virtual bool block_fortype();

    bool maybeLock();
    bool maybeUnlock();

protected:
    ncpp::NotCurses *m_nc;
    ncpp::Plane *m_parent;

    std::mutex *m_mtx;

    bool m_ownsParent;

    programstate_t m_type;

    const util::smart_colors::TerminalColors * const m_tc;
};

#endif
