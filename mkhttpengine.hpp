#ifndef MKHTTPENGINE_HPP
#define MKHTTPENGINE_HPP

#include <cpp-httplib/httplib.h>
#include <nlohmann/json.hpp>

#include "signal.hpp"

class MKHTTPEngine {
public signals:
    Signal<httplib::Response> recieveResponse;

public:

private:
};

#endif
