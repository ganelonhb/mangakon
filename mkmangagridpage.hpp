#ifndef MKMANGAGRIDPAGE_HPP
#define MKMANGAGRIDPAGE_HPP

#include <nlohmann/json.hpp>

#include <ncpp/NotCurses.hh>
#include <ncpp/Visual.hh>
#include <ncpp/Plane.hh>

#include "mkwidget.hpp"
#include "ncvscrollbar.hpp"

#include "mkmangacoverwidget.hpp"

class MKMangaGridPage : public MKWidget {
public:
    explicit MKMangaGridPage(ncpp::NotCurses *nc, ncpp::Plane *parent = nullptr)
    : MKWidget{nc, parent}
    {
        uint32_t ph, pw;
        m_parent->get_dim(&ph, &pw);

        m_ph = ph;
        m_pw = pw;
        m_parent->printf("PH: %d, PW: %d", ph, pw);
        m_scrollbar = new NCVScrollBar(nc, m_parent, nullptr, m_parent->get_dim_y() - 1);

        temp = new MKMangaCoverWidget(nc, m_parent, nullptr, 0, 0, 5, 5, new ncpp::Visual("data/no_manga.jpg"));
        temp->set_title("Test Manga Title");
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
