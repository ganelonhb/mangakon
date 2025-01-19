#ifndef MKMANGAPREVIEWWIDGET_HPP
#define MKMANGAPREVIEWWIDGET_HPP

#include <nlohmann/json.hpp>

#include <ncpp/NotCurses.hh>
#include <ncpp/Visual.hh>
#include <ncpp/Plane.hh>

#include "focuswidget.hpp"
#include "ncpushbutton.hpp"

class MKMangaPreviewWidget {
public focus_widgets:

    NCPushButton *m_chapters;   // see chapter list. Set preferred TL Group
    NCPushButton *m_read;       // Read from first chapter or pick up from where left off

public:
    explicit MKMangaPreviewWidget(ncpp::NotCurses *nc, ncpp::Plane *parent)
    : m_nc{nc}
    , m_parent{parent}
    , m_ownsParent{!static_cast<bool>(parent)}
    {
        unsigned ph, pw;
        m_parent->get_dim(&ph, &pw);

        m_h = ph;
        m_w = pw / 3;
        m_x = pw - m_w - 1;
        m_y = 1;

        m_bgplane = new ncpp::Plane(m_parent, m_w, m_h, m_y, m_x);

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
    ~MKMangaPreviewWidget() {
        delete m_bgplane;
        delete m_coverPlane;
        delete m_coverVisual;
    }

    inline void draw_box() {
        m_bgplane->erase();
        for (unsigned y = 0; y < m_h; ++y)
            m_bgplane->putwch(y, 0, L'║');

    }

    inline void draw_content() {
        m_title->set_base("", NCSTYLE_BOLD, 0);
        m_title->putstr(0, (m_w / 2) - 8, m_manga_json ? L"TODO: set with manga title" : L"No Manga Selected");
        m_title->set_base("", 0, 0);
    }

    void update() {
        unsigned ph, pw;
        m_parent->get_dim(&ph, &pw);

        m_h = ph;
        m_w = pw / 3;
        m_x = pw - m_w - 1;
        m_y = 1;

        m_bgplane->resize(m_y, m_x);
        m_bgplane->move(m_w, m_h);

        draw_box();
        draw_content();
    }

    // void set_manga(std::string id);

private:
    ncpp::NotCurses *m_nc;
    ncpp::Plane *m_parent;

    ncpp::Plane *m_bgplane;

    // Cover art planes
    ncpp::Plane *m_coverPlane;
    ncvisual_options m_vopts;
    ncpp::Visual *m_coverVisual;

    ncpp::Plane *m_stats;

    ncpp::Plane *m_desc;

    ncpp::Plane *m_publication;

    unsigned m_y;
    unsigned m_x;

    unsigned m_h;
    unsigned m_w;

    bool m_ownsParent;

    nlohmann::json *m_manga_json;

    ncpp::Plane *m_title;
    ncpp::Plane *m_banner; // Title, author, and original language title
    ncpp::Plane *m_tags; // Format, Genre, Themes, Demographic
};

#endif
