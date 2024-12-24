#ifndef APIKEYGAMESTATE_H
#define APIKEYGAMESTATE_H

#include "gamestate.hpp"
#include "title.h"

class ApiKeyGameState : GameState {
public:
    explicit ApiKeyGameState(ncpp::NotCurses *nc, ncpp::Plane *parent = nullptr);
    ~ApiKeyGameState();

    void update();

private:
    Title *m_title;
};

#endif
