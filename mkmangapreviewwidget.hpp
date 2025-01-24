#ifndef MKMANGAPREVIEWWIDGET_HPP
#define MKMANGAPREVIEWWIDGET_HPP

#include <nlohmann/json.hpp>
#include <cpp-httplib/httplib.h>

#include <ncpp/NotCurses.hh>
#include <ncpp/Visual.hh>
#include <ncpp/Plane.hh>

#include "mkwidget.hpp"
#include "ncpp/CellStyle.hh"
#include "notcurses/notcurses.h"

class MKMangaPreviewWidget : public MKWidget {
public:
    explicit MKMangaPreviewWidget(ncpp::NotCurses *nc, ncpp::Plane *parent = nullptr)
    : MKWidget{nc, parent}
    {
        uint32_t ph, pw;
        m_parent->get_dim(&ph, &pw);

        m_ph = ph;
        m_pw = pw;

        m_h = ph;
        m_w = 64;
        m_x = pw - m_w - 1;
        m_y = 1;

        m_bgplane = new ncpp::Plane(m_parent, m_h, m_w, m_y, m_x);

        m_coverPlane = new ncpp::Plane(m_bgplane, m_w / 3, m_w / 3, 1, (m_w / 2) - (m_w / 6) + 1);
        m_coverVisual = new ncpp::Visual("data/no_manga.jpg");

        m_vopts = {
            .n = m_coverPlane->to_ncplane(),
            .scaling = NCSCALE_SCALE_HIRES,
            .y = NCALIGN_CENTER,
            .x = NCALIGN_CENTER,
            .blitter = NCBLIT_PIXEL,
            .flags = NCVISUAL_OPTION_CHILDPLANE | NCVISUAL_OPTION_HORALIGNED | NCVISUAL_OPTION_VERALIGNED
        };

        m_coverVisual->blit(&m_vopts);


        m_title = new ncpp::Plane(m_bgplane, 1, m_w - 1, 0, 1);

    }
    ~MKMangaPreviewWidget() override {
        delete m_bgplane;
        delete m_coverPlane;
        delete m_coverVisual;
        delete m_title;
    }

    inline void draw_box() {
        m_bgplane->erase();
        for (uint32_t y = 0; y < m_h; ++y)
            m_bgplane->putwch(y, 0, L'║');

    }

    inline void draw_content() {
        m_title->erase();
        m_title->styles_set(ncpp::CellStyle::Bold);
        m_title->putstr(0, (m_w / 2) - 8, m_manga_json ? L"TODO: set with manga title" : L"No Manga Selected");
        m_title->styles_set(ncpp::CellStyle::None);
    }

    void update() override {

        uint32_t ph, pw;
        m_parent->get_dim(&ph, &pw);
        if (ph != m_ph or pw != m_pw) {
            m_h = ph;
            m_w = 64;
            m_x = pw - m_w - 1;
            m_y = 1;
            m_bgplane->resize(m_h, m_w);
            m_bgplane->move(m_y, m_x);

            m_title->resize(1, m_w - 1);
            m_coverPlane->erase();
            m_coverVisual->blit(&m_vopts);
            m_nc->refresh(nullptr, nullptr);

            m_ph = ph;
            m_pw = pw;
        }

        draw_box();
        draw_content();
    }

    bool handle_event(ncinput &ni, char32_t ch) override {

        return false;
    }

    // void set_manga(std::string id);

private:
    ncpp::Plane *m_bgplane;

    // Cover art planes
    ncpp::Plane *m_coverPlane;
    ncvisual_options m_vopts;
    ncpp::Visual *m_coverVisual;

    ncpp::Plane *m_stats;

    ncpp::Plane *m_desc;

    ncpp::Plane *m_publication;

    uint32_t m_y;
    uint32_t m_x;

    uint32_t m_ph;
    uint32_t m_pw;

    uint32_t m_h;
    uint32_t m_w;

    nlohmann::json *m_manga_json;

    ncpp::Plane *m_title;
    ncpp::Plane *m_banner; // Title, author, and original language title
    ncpp::Plane *m_tags; // Format, Genre, Themes, Demographic
};

#endif
