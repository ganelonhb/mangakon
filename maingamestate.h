#ifndef MAINGAMESTATE_H
#define MAINGAMESTATE_H

#include <ncpp/NotCurses.hh>
#include <ncpp/Plane.hh>

#include "utils.hpp"
#include "gamestate.hpp"
#include "securestorepass.hpp"
#include "mkmangapreviewwidget.hpp"
#include "mkhttpengine.hpp"

namespace GameStateType {
    constexpr gamestate_t MAINGAMESTATE = util::compile_time_id::get_id("MainGameState");
}

class MainGameState : public GameState {
public:
    explicit MainGameState(ncpp::NotCurses *nc, ncpp::Plane *parent, std::mutex *mtx = nullptr, SecureStorePass *user = nullptr);

    ~MainGameState() override;

    void update() override;
    gs_info_t *handle_event(ncinput &ni, char32_t ch) override;

    bool block_fortype() override;
private:
    SecureStorePass *m_user;

    MKMangaPreviewWidget *m_prev;

    MKHTTPEngine m_http;
};

#endif
