//  To parse this JSON data, first install
//
//      Boost     http://www.boost.org
//      json.hpp  https://github.com/nlohmann/json
//
//  Then include this file, and then do
//
//     Recipes data = nlohmann::json::parse(jsonString);

#pragma once

#include "json.hpp"

#include <boost/optional.hpp>
#include <stdexcept>
#include <regex>

namespace SYtool_recipes {
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

    class It {
    public:
        It() = default;
        virtual ~It() = default;

    private:
        int64_t it;
        int64_t ct;

    public:
        const int64_t& get_it() const { return it; }
        int64_t& get_mutable_it() { return it; }
        void set_it(const int64_t& value) { this->it = value; }

        const int64_t& get_ct() const { return ct; }
        int64_t& get_mutable_ct() { return ct; }
        void set_ct(const int64_t& value) { this->ct = value; }
    };

    class Recipe {
    public:
        Recipe() = default;
        virtual ~Recipe() = default;

    private:
        int64_t id;
        int64_t od;
        int64_t tp;
        std::vector<int64_t> zy;
        std::string cl;
        std::vector<It> its;

    public:
        const int64_t& get_id() const { return id; }
        int64_t& get_mutable_id() { return id; }
        void set_id(const int64_t& value) { this->id = value; }

        const int64_t& get_od() const { return od; }
        int64_t& get_mutable_od() { return od; }
        void set_od(const int64_t& value) { this->od = value; }

        const int64_t& get_tp() const { return tp; }
        int64_t& get_mutable_tp() { return tp; }
        void set_tp(const int64_t& value) { this->tp = value; }

        const std::vector<int64_t>& get_zy() const { return zy; }
        std::vector<int64_t>& get_mutable_zy() { return zy; }
        void set_zy(const std::vector<int64_t>& value) { this->zy = value; }

        const std::string& get_cl() const { return cl; }
        std::string& get_mutable_cl() { return cl; }
        void set_cl(const std::string& value) { this->cl = value; }

        const std::vector<It>& get_its() const { return its; }
        std::vector<It>& get_mutable_its() { return its; }
        void set_its(const std::vector<It>& value) { this->its = value; }
    };

    using Recipes = std::vector<Recipe>;
}

namespace SYtool_recipes {
    using Recipes = std::vector<Recipe>;
}

namespace nlohmann {
    void from_json(const json& j, SYtool_recipes::It& x);
    void to_json(json& j, const SYtool_recipes::It& x);

    void from_json(const json& j, SYtool_recipes::Recipe& x);
    void to_json(json& j, const SYtool_recipes::Recipe& x);

    inline void from_json(const json& j, SYtool_recipes::It& x) {
        x.set_it(j.at("it").get<int64_t>());
        x.set_ct(j.at("ct").get<int64_t>());
    }

    inline void to_json(json& j, const SYtool_recipes::It& x) {
        j = json::object();
        j["it"] = x.get_it();
        j["ct"] = x.get_ct();
    }

    inline void from_json(const json& j, SYtool_recipes::Recipe& x) {
        x.set_id(j.at("id").get<int64_t>());
        x.set_od(j.at("od").get<int64_t>());
        x.set_tp(j.at("tp").get<int64_t>());
        x.set_zy(j.at("zy").get<std::vector<int64_t>>());
        x.set_cl(j.at("cl").get<std::string>());
        x.set_its(j.at("its").get<std::vector<SYtool_recipes::It>>());
    }

    inline void to_json(json& j, const SYtool_recipes::Recipe& x) {
        j = json::object();
        j["id"] = x.get_id();
        j["od"] = x.get_od();
        j["tp"] = x.get_tp();
        j["zy"] = x.get_zy();
        j["cl"] = x.get_cl();
        j["its"] = x.get_its();
    }
}
