#include "maingamestate.h"

#include <ncpp/Visual.hh>

MainGameState::MainGameState(ncpp::NotCurses *nc, ncpp::Plane *parent, std::mutex *mtx, SecureStorePass *user)
    : GameState{nc, parent, mtx, GameStateType::MAINGAMESTATE}
    , m_user{user}
{

    m_prev = new MKMangaPreviewWidget(nc, m_parent);
}

MainGameState::~MainGameState() {

    delete m_prev;


    GameState::~GameState();
}

void MainGameState::update() {
    //TODO: populate
    m_prev->update();
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
