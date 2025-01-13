#include "maingamestate.h"

MainGameState::MainGameState(ncpp::NotCurses *nc, ncpp::Plane *parent, std::mutex *mtx, SecureStorePass *user)
    : GameState{nc, parent, mtx, GameStateType::MAINGAMESTATE}
    , m_user{user}
{
    m_tabBar = new NCTabBar(m_nc, m_parent, m_mtx);
    m_tabBar->add_tab(L"Test 1");
    m_tabBar->add_tab(L"Test 2");
}

MainGameState::~MainGameState() {

    delete m_tabBar;

    GameState::~GameState();
}

void MainGameState::update() {
    //TODO: populate
    m_tabBar->update();
}

gs_info_t *MainGameState::handle_event(ncinput &ni, char32_t ch) {
    return {}; //TODO: populate
}

bool MainGameState::block_fortype() {
    return false; //TODO: populate
}
