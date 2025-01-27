#ifndef MKMANGAGRIDPAGE_HPP
#define MKMANGAGRIDPAGE_HPP

#include <nlohmann/json.hpp>

#include <ncpp/NotCurses.hh>
#include <ncpp/Visual.hh>
#include <ncpp/Plane.hh>

#include "mkwidget.hpp"
#include "ncvscrollbar.hpp"

#include "mkmangacoverwidget.hpp"

/* The MKMangaGridPage will feature a few widgets.
 * The manga grid, the scrollbar, and the page selector.
 * The manga grid displays four covers horizontally. It tries to resize dynamically.
 * The scrollbar is next to the grid.
 * The page selector displays a selection of buttons that change pages, as well as a text
 * input that lets you put in a custom page.
 */
class MKMangaGridPage : public MKWidget {
public:
    explicit MKMangaGridPage(ncpp::NotCurses *nc, ncpp::Plane *parent = nullptr)
    : MKWidget{nc, parent}
    {
        uint32_t ph, pw;
        m_parent->get_dim(&ph, &pw);

        m_ph = ph;
        m_pw = pw;
        m_scrollbar = new NCVScrollBar(nc, m_parent, nullptr, m_parent->get_dim_y() - 4);

        temp = new MKMangaCoverWidget(nc, m_parent, nullptr, 0, 0, 32, 32, new ncpp::Visual("data/no_manga.jpg"));
        temp->set_title(L"Manga Title Here テスト寿限無寿限無 ﷽👮🔫🐊apu uihefjjla lewlfcwaefoihjawelfkwhuoihwaeiuhwjnaelkeawhjfoiuhafjljwksef hdslahfeiAUhncoklhhwaenflicuw aehFlkcjWNASHIWLf");
    }

    ~MKMangaGridPage() override {
        delete m_scrollbar;
        delete temp;

        MKWidget::~MKWidget();
    }

    void update() override {
        m_scrollbar->update();
        temp->update();
    }



private:
    uint32_t m_y;
    uint32_t m_x;

    uint32_t m_ph;
    uint32_t m_pw;

    uint32_t m_h;
    uint32_t m_w;

    nlohmann::json *m_json;

    uint64_t m_page;
    NCVScrollBar *m_scrollbar;

    MKMangaCoverWidget *temp;
};

#endif
