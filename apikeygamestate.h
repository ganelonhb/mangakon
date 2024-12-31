#ifndef APIKEYGAMESTATE_H
#define APIKEYGAMESTATE_H

#include <ncpp/Plane.hh>

#include <mutex>

#include "utils.hpp"
#include "gamestate.hpp"
#include "title.h"
#include "nclineedit.hpp"

class ApiKeyGameState : GameState {
public:
    explicit ApiKeyGameState(ncpp::NotCurses *nc, ncpp::Plane *parent = nullptr, std::mutex *mtx = nullptr);
    ~ApiKeyGameState();

    void update() override;
    void handle_event(ncinput &ni) override {} // TODO: fixme

    bool block_fortype() override{return false;} // TODO: fixme

private:
    std::mutex *m_mtx;
    Title *m_title;

    ncpp::Plane *m_window;

    NCLineEdit *m_usr;
    NCLineEdit *m_pss;

    FocusWidget *m_focused;
};

#endif
