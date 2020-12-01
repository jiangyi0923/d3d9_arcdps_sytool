//  To parse this JSON data, first install
//
//      Boost     http://www.boost.org
//      json.hpp  https://github.com/nlohmann/json
//
//  Then include this file, and then do
//
//     Items data = nlohmann::json::parse(jsonString);

#pragma once

#include "json.hpp"

#include <boost/optional.hpp>
#include <stdexcept>
#include <regex>

namespace SYtool_item {
    using nlohmann::json;

    inline json get_untyped(const json& j, const char* property) {
        if (j.find(property) != j.end()) {
            return j.at(property).get<json>();
        }
        return json();
    }

    inline json get_untyped(const json& j, std::string property) {
        return get_untyped(j, property.data());
    }

    class Item {
    public:
        Item() = default;
        virtual ~Item() = default;

    private:
        int64_t id;
        std::string ne;
        int64_t tp;
        int64_t zg;
        std::vector<int64_t> pf;
        std::string cl;
        std::vector<std::string> mc;

    public:
        const int64_t& get_id() const { return id; }
        int64_t& get_mutable_id() { return id; }
        void set_id(const int64_t& value) { this->id = value; }

        const std::string& get_ne() const { return ne; }
        std::string& get_mutable_ne() { return ne; }
        void set_ne(const std::string& value) { this->ne = value; }

        const int64_t& get_tp() const { return tp; }
        int64_t& get_mutable_tp() { return tp; }
        void set_tp(const int64_t& value) { this->tp = value; }

        const int64_t& get_zg() const { return zg; }
        int64_t& get_mutable_zg() { return zg; }
        void set_zg(const int64_t& value) { this->zg = value; }

        const std::vector<int64_t>& get_pf() const { return pf; }
        std::vector<int64_t>& get_mutable_pf() { return pf; }
        void set_pf(const std::vector<int64_t>& value) { this->pf = value; }

        const std::string& get_cl() const { return cl; }
        std::string& get_mutable_cl() { return cl; }
        void set_cl(const std::string& value) { this->cl = value; }

        const std::vector<std::string>& get_mc() const { return mc; }
        std::vector<std::string>& get_mutable_mc() { return mc; }
        void set_mc(const std::vector<std::string>& value) { this->mc = value; }
    };

    using Items = std::vector<Item>;
}

namespace SYtool_item {
    using Items = std::vector<Item>;
}

namespace nlohmann {
    void from_json(const json& j, SYtool_item::Item& x);
    void to_json(json& j, const SYtool_item::Item& x);

    inline void from_json(const json& j, SYtool_item::Item& x) {
        x.set_id(j.at("id").get<int64_t>());
        x.set_ne(j.at("ne").get<std::string>());
        x.set_tp(j.at("tp").get<int64_t>());
        x.set_zg(j.at("zg").get<int64_t>());
        x.set_pf(j.at("pf").get<std::vector<int64_t>>());
        x.set_cl(j.at("cl").get<std::string>());
        x.set_mc(j.at("mc").get<std::vector<std::string>>());
    }

    inline void to_json(json& j, const SYtool_item::Item& x) {
        j = json::object();
        j["id"] = x.get_id();
        j["ne"] = x.get_ne();
        j["tp"] = x.get_tp();
        j["zg"] = x.get_zg();
        j["pf"] = x.get_pf();
        j["cl"] = x.get_cl();
        j["mc"] = x.get_mc();
    }
}
