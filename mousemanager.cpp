#include "mousemanager.h"

#ifndef NCPP_EXCEPTIONS_PLEASE
#define NCPP_EXCEPTIONS_PLEASE
#endif

MouseManager::MouseManager(const ncpp::NotCurses * const nc)
    : m_nc{nc}
{
    m_mouseSupported = true;
    try {
        m_nc->mouse_enable(NCMICE_ALL_EVENTS);
    }
    catch (ncpp::call_error &e) {
        m_mouseSupported = false;
    }
}

MouseManager::~MouseManager() {
    if (m_mouseSupported) {
        m_nc->mouse_disable();
    }
}
