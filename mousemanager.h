#ifndef __MOUSEMANAGER_H__
#define __MOUSEMANAGER_H__

#include <ncpp/NotCurses.hh>

class MouseManager {
public:
    explicit MouseManager(const ncpp::NotCurses * const nc);
    ~MouseManager();

private:
    const ncpp::NotCurses *const m_nc;
    bool m_mouseSupported;
};

#endif
