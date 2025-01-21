#ifndef MKMANGAGRIDPAGE_HPP
#define MKMANGAGRIDPAGE_HPP

#include <nlohmann/json.hpp>

#include <ncpp/NotCurses.hh>
#include <ncpp/Visual.hh>
#include <ncpp/Plane.hh>

#include "mkwidget.hpp"

class MKMangaGridPage : public MKWidget {
public:
    explicit MKMangaGridPage(ncpp::NotCurses *nc, ncpp::Plane *parent = nullptr)
    : MKWidget{nc, parent}
    {
        uint32_t ph, pw;
        m_parent->get_dim(&ph, &pw);

        m_ph = ph;
        m_pw = pw;


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
};

#endif
