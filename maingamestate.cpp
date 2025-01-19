#include "maingamestate.h"

MainGameState::MainGameState(ncpp::NotCurses *nc, ncpp::Plane *parent, std::mutex *mtx, SecureStorePass *user)
    : GameState{nc, parent, mtx, GameStateType::MAINGAMESTATE}
    , m_user{user}
{
    m_tabBar = new NCTabBar(m_nc, m_parent, m_mtx);
    m_tabBar->add_tab(L"Popular");
    m_tabBar->add_tab(L"Latest Updates");
    m_tabBar->add_tab(L"Search");
    m_tabBar->add_tab(L"Lists");
    m_tabBar->add_tab(L"Settings");

    m_prev = new MKMangaPreviewWidget(nc, m_parent);
}

MainGameState::~MainGameState() {

    delete m_tabBar;
    delete m_prev;

    GameState::~GameState();
}

void MainGameState::update() {
    //TODO: populate
    m_tabBar->update();
    m_prev->update();
}

gs_info_t *MainGameState::handle_event(ncinput &ni, char32_t ch) {
    return {}; //TODO: populate
}

bool MainGameState::block_fortype() {
    return false; //TODO: populate
}
