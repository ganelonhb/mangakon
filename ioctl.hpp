#ifndef IOCTL_HPP
#define IOCTL_HPP

#include <atomic>
#include <mutex>

#include <ncpp/NotCurses.hh>
#include <ncpp/Plane.hh>

#include "maingamestate.h"
#include "apikeygamestate.h"
#include "utils.hpp"
#include "signal.hpp"

class IOCtl {
public:
    explicit IOCtl(ncpp::NotCurses *nc, std::mutex *mtx, GameState *gs)
    : m_nc{nc}
    , m_mtx{mtx}
    , m_gameover{false}
    , m_statechange{nullptr}
    , m_gs{gs}
    {}

    void loop() {
        forever {
            m_mtx->lock();
            if (m_statechange) {
                // handle statechange (caused by slot usually)
                // statechanges should be happening on this thread,

                delete m_statechange;
                m_statechange = nullptr;
                continue;
            }
            m_mtx->unlock();

            char32_t ch = 0;
            ncinput ni;

            ch = m_nc->get(true, &ni);
            m_mtx->lock();

            if (ch == 'q' and !m_gs->block_fortype()) {
                m_gameover = true;
                m_mtx->unlock();
                return;
            }

            if (ch == 'L' && ni.modifiers & NCKEY_MOD_CTRL) {
                m_nc->refresh(nullptr, nullptr);
                m_mtx->unlock();
                continue;
            }

            gs_info_t *newState = m_gs->handle_event(ni, ch);

            if (newState) {
                // handle transition to new state
                gamestate_t state = newState->state;

                if (state == GameStateType::APIKEYGAMESTATE) [[unlikely]] {
                    // switch to ApiKey menu (no args)
                }
                else if (state == GameStateType::MAINGAMESTATE) {
                    // switch to MainGameState (no args)
                }

                delete newState;
            }
            m_mtx->unlock();
        }
    }

    bool gameover() { return m_gameover; }

private slots:


private:
    ncpp::NotCurses *m_nc;
    std::mutex *m_mtx;

    std::atomic<bool> m_gameover;
    gs_info_t *m_statechange;

    GameState *m_gs;
};

#endif
