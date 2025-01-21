#ifndef APIKEYGAMESTATE_H
#define APIKEYGAMESTATE_H

#include <ncpp/Plane.hh>

#include <mutex>

#include "utils.hpp"
#include "gamestate.hpp"
#include "maingamestate.h"
#include "title.h"
#include "nclineedit.hpp"
#include "ncpushbutton.hpp"
#include "securestorepass.hpp"

namespace GameStateType {
    constexpr gamestate_t APIKEYGAMESTATE = util::compile_time_id::get_id("ApiKeyGameState");
}

class ApiKeyGameState : public GameState {
public:
    explicit ApiKeyGameState(ncpp::NotCurses *nc, ncpp::Plane *parent = nullptr, std::mutex *mtx = nullptr, SecureStorePass *init = nullptr);
    ~ApiKeyGameState() override;

    void update() override;
    gs_info_t* handle_event(ncinput &ni, char32_t ch) override;

    bool block_fortype() override;

private:
    Title *m_title;

    uint32_t m_ph;
    uint32_t m_pw;

    ncpp::Plane *m_window;

    NCLineEdit *m_usr;
    NCLineEdit *m_pss;
    NCLineEdit *m_key;
    NCLineEdit *m_sct;

    NCPushButton *m_ok;
    NCPushButton *m_skip;

    FocusWidget *m_focused;

    SecureStorePass *m_init;
};

#endif
