#ifndef MKHTTPENGINE_HPP
#define MKHTTPENGINE_HPP

#include <cpp-httplib/httplib.h>
#include <nlohmann/json.hpp>

#include "mangadextag.hpp"

class MKHTTPEngine {
public:
    std::vector<mangadextag_t> get_tags() {
        return mangadextag_t::get_tags();
    }

    nlohmann::json get_latest(uint64_t page = 0, uint8_t per_page = 10) {
        nlohmann::json json = {
            {"limit", std::to_string(per_page)},
            {"offset", std::to_string(page)}
        };

        return get_manga(json);
    }

    nlohmann::json get_popular(uint64_t page = 0, uint8_t per_page = 10) {
        nlohmann::json json = {
            {"limit", std::to_string(per_page)},
            {"offset", std::to_string(page)},
            {"order[latestUploadedChapter]", "desc"},
            {"order[followedCount]", "desc"}
        };

        return get_manga(json);
    }

    nlohmann::json get_rated(uint64_t page = 0, uint8_t per_page = 10) {
        nlohmann::json json= {
            {"limit", std::to_string(per_page)},
            {"offset", std::to_string(page)},
            {"order[rating]", "desc"}
        };

        return get_manga(json);
    }

    nlohmann::json get_recent(uint64_t page = 0, uint8_t per_page = 10) {
        nlohmann::json json= {
            {"limit", std::to_string(per_page)},
            {"offset", std::to_string(page)},
            {"order[createdAt]", "desc"}
        };

        return get_manga(json);
    }

    // nlohmann::json advanced_search() {} // Custom API provided in app

private:
    // generic method for getting a LIST of manga. Essentially, the search function
    inline nlohmann::json get_manga(nlohmann::json parameters) {
        httplib::Client client("https://api.mangadex.org");

        const std::string json = parameters.dump();

        httplib::Result res = client.Post("/manga", json, "application/json");

        if (res->status == 200)
            return nlohmann::json::parse(res->body);

        nlohmann::json fail = {
            {"status", res->status},
            {"reason", res->reason}
        };

        return fail;
    }

    // this version of the method will get a specific manga as opposed to a list:
    // inline nlohmann::json get_manga(std::string id);
};

#endif
