#ifndef MANGADEXTAG_HPP
#define MANGADEXTAG_HPP

#include <vector>
#ifndef DISABLE_SAGACITY
    #include <ranges>
#endif

#include <nlohmann/json.hpp>
#include <cpp-httplib/httplib.h>

constexpr int STATUS_OK = 200;

// The mangadex tag abstracts away the need for interacting with a tag via json.
// Just in case, however, the json is saved and provided.

#ifndef DISABLE_SAGACITY
const std::vector<std::string> CONTENTIOUS_TAGS {
    "2d1f5d56-a1e5-4d0d-a961-2193588b08ec",
    "5bd0e105-4481-44ca-b6e7-7544da56b1a3",
    "97893a4c-12af-4dac-b6be-0dffb353568e",
    "ddefd648-5140-4e5f-ba18-4eca4071d19b",
};
#else
/* Colossians 3:5 - Put to death therefore what is earthly in you: sexual immorality, impurity, passion, evil desire, and covetousness, which is idolatry.
 * 1 Peter 5:8 - Be sober minded; be watchful. Your adversary the devil prowls around like a roaring lion, seeking someone to devour.
 */
#endif

const nlohmann::json EMPTY_TAG_JSON = {
    {"id", ""},
    {"type", ""},
    {"attributes", {
        {"name", {
            {"en", ""}
        }},
        {"group", ""},
        {"version", 0}
    }}
};

struct mangadextag_t {
private:
    struct _attribute_t {
        std::string name;
        std::string group;
        uint64_t version;
    };
public:

    mangadextag_t()
    : json{EMPTY_TAG_JSON}
    , id{""}
    , type{""}
    , attributes{"", "", 0}
    {
    }

    mangadextag_t(nlohmann::json tag) {
        json = tag;
        id = tag.contains("id") ? tag["id"] : "";
        type = tag.contains("type") ? tag["type"] : "";

        if (tag.contains("attributes") and tag["attributes"].is_object()) {
            nlohmann::json attr = tag["attributes"];

            attributes.name = attr.contains("name") and attr["name"].is_object() and attr["name"].contains("en") ? attr["name"]["en"] : "";

            attributes.group = attr.contains("group") ? attr["group"] : "";

            attributes.version = attr.contains("version") ? attr["version"].get<uint64_t>() : 0;
        }
        else {
            attributes = {"", "", 0};
        }
    }

    nlohmann::json json;

    std::string id;
    std::string type;

    _attribute_t attributes;

    bool valid() const {
        return !id.empty() and !type.empty() and !attributes.name.empty() and !attributes.group.empty();
    }

// static helper methods
public:

    // returns empty string if not found
    static inline std::string tag_name2id(std::string name, std::vector<mangadextag_t> tags) {
        for (const mangadextag_t &tag : tags) {
            if (tag.attributes.name == name)
                return tag.id;
        }

        return "";
    }

    // returns invalid tag if not found
    static inline mangadextag_t tag_name2tag(std::string name, std::vector<mangadextag_t> tags) {
        for(mangadextag_t &tag : tags) {
            if (tag.attributes.name == name)
                return tag;
        }

        return mangadextag_t();
    }

    static inline std::vector<mangadextag_t> get_tags(int *status = nullptr) {
        httplib::Client client("https://api.mangadex.org");

        httplib::Result res = client.Get("/manga/tag");

        if (!res or res->status != STATUS_OK) {
            if (status) *status = res->status;
            return std::vector<mangadextag_t>();
        }

        nlohmann::json response_json;

        try {
            response_json = nlohmann::json::parse(res->body);
        }
        catch(const nlohmann::json::exception &e) {
            return std::vector<mangadextag_t>();
        }

        if (!response_json.contains("data") or !response_json["data"].is_array())
            return std::vector<mangadextag_t>();

        std::vector<mangadextag_t> tags;

        for (const nlohmann::json& tag :response_json["data"]) {
            mangadextag_t mdtag(tag);
            if (mdtag.valid())
                tags.push_back(mdtag);
        }
        return tags;
    }

    #ifndef DISABLE_SAGACITY
        static inline bool is_contentious(mangadextag_t tag) {
            return std::ranges::find(CONTENTIOUS_TAGS, tag.id) != CONTENTIOUS_TAGS.end();
        }
    #endif
};

#endif
