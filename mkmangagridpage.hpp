#ifndef MKMANGAGRIDPAGE_HPP
#define MKMANGAGRIDPAGE_HPP

#include <vector>

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
    , m_totalManga{0}
    , m_widgets{nullptr}
    {
        uint32_t ph, pw;
        m_parent->get_dim(&ph, &pw);

        m_ph = ph;
        m_pw = pw;
        m_scrollbar = new NCVScrollBar(nc, m_parent, nullptr, m_parent->get_dim_y() - 4);
    }

    ~MKMangaGridPage() override {
        delete m_scrollbar;

        MKWidget::~MKWidget();
    }

    void update() override {
        m_scrollbar->update();
    }

    void clearManga() {
        if (m_widgets) {
            for (size_t i = 0; i < m_widgets->size(); ++i) {
                delete m_widgets->at(i);
            }

            delete m_widgets;
            m_widgets = nullptr;
        }
    }

    void setManga(nlohmann::json json) {
        clearManga();

        m_widgets = new std::vector<MKMangaCoverWidget *>;
        nlohmann::json manga;

        try {
            m_totalManga = json.at("total").get<long>();
            manga = json.at("data");
        }
        catch (std::out_of_range &e) {
            m_totalManga = 0;
            delete m_widgets;
            m_widgets = nullptr;
        }

        for (nlohmann::json &e : manga) {
            MKMangaCoverWidget *page(m_nc, m_parent);
            page->set_info(e);
            //m_widgets->push_back()
        }

    }

    uint32_t getInternalHeight() const {
        return 0;
    }

private:
    uint32_t m_y;
    uint32_t m_x;

    uint32_t m_ph;
    uint32_t m_pw;

    uint32_t m_h;
    uint32_t m_w;

    uint64_t m_totalManga;

    nlohmann::json *m_json;

    uint64_t m_page;
    NCVScrollBar *m_scrollbar;

    std::vector<MKMangaCoverWidget *> *m_widgets;
};

#endif
