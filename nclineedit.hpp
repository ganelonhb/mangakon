#ifndef NCLINEEDIT_HPP
#define NCLINEEDIT_HPP

#include "ncpp/Root.hh"
#include <ncpp/NotCurses.hh>
#include <ncpp/Cell.hh>
#include <ncpp/NCBox.hh>

class NCLineEdit {
public:
    explicit NCLineEdit(ncpp::NotCurses *nc, ncpp::Plane *parent)
        : m_nc{nc}
        , m_parent{parent}
        , m_boxChannels{0}
    {
        m_lineEdit = new ncpp::Plane(m_parent, 5,5,0,0);

        ncchannels_set_fg_rgb(&m_boxChannels, 0x00b040);
        m_lineEdit->rounded_box(0, m_boxChannels, 0, 5, 0);
    }

private:
    ncpp::NotCurses *m_nc;
    ncpp::Plane *m_parent;

    ncpp::Plane *m_lineEdit;
    uint64_t m_boxChannels;
};

#endif
