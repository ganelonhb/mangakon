#include "programstate.h"

ProgramState::ProgramState(ncpp::NotCurses *nc, ncpp::Plane *parent, std::mutex *mtx,const util::smart_colors::TerminalColors *const tc, programstate_t type)
    : m_nc{nc}
    , m_mtx{mtx}
    , m_ownsParent{!bool(parent)}
    , m_type{type}
    , m_tc{tc}
{
    m_parent = m_ownsParent ? m_nc->get_stdplane() : parent;
}

ProgramState::~ProgramState() {}

void ProgramState::update() {}

programstate_info_t *ProgramState::handle_event(ncinput &ni, char32_t ch) {
    return nullptr;
}

// Block certain signals for typing.
bool ProgramState::block_fortype() {
    return false;
}

bool ProgramState::maybeLock() {
    bool mut = m_mtx != nullptr;

    if (mut) {
        m_mtx->lock();
    }

    return mut;
}

bool ProgramState::maybeUnlock() {
    bool mut = m_mtx != nullptr;

    if (mut) {
        m_mtx->unlock();
    }

    return mut;
}
