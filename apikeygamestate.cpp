#include "apikeygamestate.h"


ApiKeyGameState::ApiKeyGameState(ncpp::NotCurses *nc, ncpp::Plane *parent)
    : GameState(nc, parent)
{
     m_title = new Title(m_nc, m_parent, 5);


}

ApiKeyGameState::~ApiKeyGameState() {
    delete m_title;
}

void ApiKeyGameState::update() {
    if (m_title) m_title->update();
}
