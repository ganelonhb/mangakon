#include "maingamestate.h"
#include "mkmangagridpage.hpp"

#include <ncpp/Visual.hh>

MainGameState::MainGameState(ncpp::NotCurses *nc, ncpp::Plane *parent, std::mutex *mtx, SecureStorePass *user)
    : GameState{nc, parent, mtx, GameStateType::MAINGAMESTATE}
    , m_user{user}
{
    m_tabBar = new NCTabBar(m_nc, m_parent, m_mtx);
    m_tabBar->add_tab(L"Latest Updates"); // Latest Update
    m_tabBar->add_tab(L"Popular"); // Latest Update + Most Follows
    m_tabBar->add_tab(L"Highest Rated"); // Rating
    m_tabBar->add_tab(L"Recently Added"); // Recently added.
    m_tabBar->add_tab(L"Search");
    m_tabBar->add_tab(L"Saved"); // Local
    m_tabBar->add_tab(L"Settings");

    m_prev = new MKMangaPreviewWidget(nc, m_parent);
    m_grid = new ncpp::Plane(m_parent, m_parent->get_dim_y(), m_parent->get_dim_x() - 65, 1, 0);

    m_gridPage = new MKMangaGridPage(nc, m_grid);
    m_gridPage->setManga(m_http.get_latest());
}

MainGameState::~MainGameState() {

    delete m_tabBar;
    delete m_prev;
    delete m_gridPage;
    delete m_grid;

    GameState::~GameState();
}

void MainGameState::update() {
    //TODO: populate
    m_tabBar->update();
    m_prev->update();
    m_gridPage->update();
}

gs_info_t *MainGameState::handle_event(ncinput &ni, char32_t ch) {
    if (ni.id == NCKEY_RESIZE) {

        m_prev->handle_event(ni, ch);

        return nullptr;
    }

    return {}; //TODO: populate
}

bool MainGameState::block_fortype() {
    return false; //TODO: populate
}
