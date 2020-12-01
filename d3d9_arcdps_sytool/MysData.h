//  To parse this JSON data, first install
//
//      Boost     http://www.boost.org
//      json.hpp  https://github.com/nlohmann/json
//
//  Then include this file, and then do
//
//     MysData data = nlohmann::json::parse(jsonString);

#pragma once

#include "json.hpp"

#include <boost/optional.hpp>
#include <stdexcept>
#include <regex>

namespace SYtool_MysData {
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

    class MysDatum {
    public:
        MysDatum() = default;
        virtual ~MysDatum() = default;

    private:
        std::string id;
        int64_t od;
        std::vector<It> its;

    public:
        const std::string& get_id() const { return id; }
        std::string& get_mutable_id() { return id; }
        void set_id(const std::string& value) { this->id = value; }

        const int64_t& get_od() const { return od; }
        int64_t& get_mutable_od() { return od; }
        void set_od(const int64_t& value) { this->od = value; }

        const std::vector<It>& get_its() const { return its; }
        std::vector<It>& get_mutable_its() { return its; }
        void set_its(const std::vector<It>& value) { this->its = value; }
    };

    using MysData = std::vector<MysDatum>;
}

namespace SYtool_MysData {
    using MysData = std::vector<MysDatum>;
}

namespace nlohmann {
    void from_json(const json& j, SYtool_MysData::It& x);
    void to_json(json& j, const SYtool_MysData::It& x);

    void from_json(const json& j, SYtool_MysData::MysDatum& x);
    void to_json(json& j, const SYtool_MysData::MysDatum& x);

    inline void from_json(const json& j, SYtool_MysData::It& x) {
        x.set_it(j.at("it").get<int64_t>());
        x.set_ct(j.at("ct").get<int64_t>());
    }

    inline void to_json(json& j, const SYtool_MysData::It& x) {
        j = json::object();
        j["it"] = x.get_it();
        j["ct"] = x.get_ct();
    }

    inline void from_json(const json& j, SYtool_MysData::MysDatum& x) {
        x.set_id(j.at("id").get<std::string>());
        x.set_od(j.at("od").get<int64_t>());
        x.set_its(j.at("its").get<std::vector<SYtool_MysData::It>>());
    }

    inline void to_json(json& j, const SYtool_MysData::MysDatum& x) {
        j = json::object();
        j["id"] = x.get_id();
        j["od"] = x.get_od();
        j["its"] = x.get_its();
    }
}





