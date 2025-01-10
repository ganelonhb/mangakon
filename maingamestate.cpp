#include "maingamestate.h"

MainGameState::MainGameState(ncpp::NotCurses *nc, ncpp::Plane *parent, std::mutex *mtx, SecureStorePass *user)
    : GameState{nc, parent, mtx, GameStateType::MAINGAMESTATE}
    , m_user{user}
{

}

MainGameState::~MainGameState() {

}

void MainGameState::update() {
    //TODO: populate
}

gs_info_t *MainGameState::handle_event(ncinput &ni, char32_t ch) {
    return {}; //TODO: populate
}

bool MainGameState::block_fortype() {
    return false; //TODO: populate
}
