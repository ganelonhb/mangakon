#ifndef MKMANGACOVERWIDGET_HPP
#define MKMANGACOVERWIDGET_HPP

#include <mutex>
#include <ncpp/NotCurses.hh>
#include <ncpp/Visual.hh>
#include <ncpp/Plane.hh>

#include "focuswidget.hpp"
#include "utils.hpp"

namespace FocusType {
    constexpr focus_t MKMANGACOVERWIDGET = util::compile_time_id::get_id("MKMangaCoverWidget");
}

class MKMangaCoverWidget : public FocusWidget {
public:
    explicit MKMangaCoverWidget(ncpp::NotCurses *nc, ncpp::Plane *parent = nullptr, std::mutex *mut = nullptr, uint32_t y = 0, uint32_t x = 0, uint32_t h = 5, uint32_t w = 5, ncpp::Visual *vis = nullptr, const std::wstring &title=L"")
    : FocusWidget{nc, parent, FocusType::MKMANGACOVERWIDGET}
    , m_coverVisual{vis}
    , m_titleStr{title}
    , m_y{y}
    , m_x{x}
    , m_ph{m_parent->get_dim_y()}
    , m_pw{m_parent->get_dim_x()}
    , m_h{h}
    , m_w{w}
    , m_focused{false}
    {
        m_cover = new ncpp::Plane(m_parent, y, x, h, w);

        m_coverPlane = new ncpp::Plane(m_cover, 0, 0, m_h - 4, m_w);
        m_vopts = {
            .n = m_coverPlane->to_ncplane(),
            .scaling = NCSCALE_SCALE_HIRES,
            .y = NCALIGN_CENTER,
            .x = NCALIGN_CENTER,
            .blitter = NCBLIT_PIXEL,
            .flags = NCVISUAL_OPTION_CHILDPLANE | NCVISUAL_OPTION_HORALIGNED | NCVISUAL_OPTION_VERALIGNED
        };

        if (m_coverVisual)
            m_coverVisual->blit(&m_vopts);

        m_title = new ncpp::Plane(m_cover, m_h - 5, 0, 4, m_w);
    }

    ~MKMangaCoverWidget() override {
        delete m_cover;
        delete m_coverPlane;
        delete m_coverVisual;
        delete m_title;

        FocusWidget::~FocusWidget();
    }

    inline void putTitle(const std::wstring *title = nullptr) {
    }

    void resize(uint32_t h, uint32_t w) {
        if (m_h != h or m_w != w) {
            m_h = h;
            m_w = w;

            // TODO: reposition all subwidgets, as well as resize them. Refresh.
        }
    }

    void move(uint32_t y, uint32_t x) {
        if (m_y != y or m_x != x) {
            m_y = y;
            m_x = x;

            m_cover->move(y, x);
        }
    }

    void set_visual(ncpp::Visual *vis) {
        delete m_coverVisual;

        m_coverVisual = vis;
        m_coverPlane->erase();
        m_coverVisual->blit(&m_vopts);
        m_nc->refresh(nullptr, nullptr);
    }

    void update() override {
        drawTitle();
    }

    inline void drawTitle() {
        if (!m_title || m_titleStr.empty())
            return;

        m_title->erase();

        const uint32_t rows = 4;
        const uint32_t cols = m_w;

        std::wstringstream stream(m_titleStr);
        std::vector<std::wstring> words;
        std::wstring word;

        while (stream >> word)
            words.push_back(word);

        uint32_t currentRow = 0;
        uint32_t currentCol = 0;

        for (size_t i = 0; i < words.size() && currentRow < rows; ++currentRow) {

            std::wstring rowText;

            while (i < words.size() && (currentCol + words[i].length() <= cols || rowText.empty()))
            {
                if (!rowText.empty())
                    rowText += L" ";

                rowText += words[i];
                currentCol = rowText.length();
                ++i;
            }

            uint32_t x = (cols - rowText.length()) / 2;

            for (wchar_t ch : rowText)
                m_title->putwch(currentRow, x++, ch);

            currentCol = 0;

            if (currentRow == rows - 1 && i < words.size()) {
                m_title->putwch(currentRow, cols - 2, L'…');
                break;
            }
        }

    }

    void handle_click(uint32_t y, uint32_t x) override {

    }

    bool focused() const override {
        return m_focused;
    }

    void focus() override {
        m_focused = true;
    }

    void unfocus() override {
        m_focused = false;
    }

    void setFocus(bool focus) override {
        m_focused = focus;
    }

    void set_title(std::wstring title) {
        m_titleStr = title;
    }

    void set_title(std::string title) {
        m_titleStr = util::str2wstr(title);
    }

private:
    ncpp::Plane *m_cover;
    ncpp::Plane *m_coverPlane;
    ncvisual_options m_vopts;
    ncpp::Visual *m_coverVisual;

    ncpp::Plane *m_title;

    std::wstring m_titleStr;

    uint32_t m_y;
    uint32_t m_x;

    uint32_t m_ph;
    uint32_t m_pw;

    uint32_t m_h;
    uint32_t m_w;

    bool m_focused;
};

#endif
