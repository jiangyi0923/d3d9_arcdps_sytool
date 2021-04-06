//  To parse this JSON data, first install
//
//      Boost     http://www.boost.org
//      json.hpp  https://github.com/nlohmann/json
//
//  Then include this file, and then do
//
//     MysticForge data = nlohmann::json::parse(jsonString);

#pragma once

#include "json.hpp"

#include <boost/optional.hpp>
#include <stdexcept>
#include <regex>


namespace MyData_MysticForge {
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

    class Itm {
    public:
        Itm() = default;
        virtual ~Itm() = default;

    private:
        int a;
        int b;

    public:
        const int& get_a() const { return a; }
        int& get_mutable_a() { return a; }
        void set_a(const int& value) { this->a = value; }

        const int& get_b() const { return b; }
        int& get_mutable_b() { return b; }
        void set_b(const int& value) { this->b = value; }
    };

    class MysticForgeElement {
    public:
        MysticForgeElement() = default;
        virtual ~MysticForgeElement() = default;

    private:
        int id;
        int ot;
        std::vector<Itm> itms;

    public:
        const int& get_id() const { return id; }
        int& get_mutable_id() { return id; }
        void set_id(const int& value) { this->id = value; }

        const int& get_ot() const { return ot; }
        int& get_mutable_ot() { return ot; }
        void set_ot(const int& value) { this->ot = value; }

        const std::vector<Itm>& get_itms() const { return itms; }
        std::vector<Itm>& get_mutable_itms() { return itms; }
        void set_itms(const std::vector<Itm>& value) { this->itms = value; }
    };

    using MysticForge = std::vector<MysticForgeElement>;
}

namespace MyData_MysticForge {
    using MysticForge = std::vector<MysticForgeElement>;
}

namespace nlohmann {
    void from_json(const json& j, MyData_MysticForge::Itm& x);
    void to_json(json& j, const MyData_MysticForge::Itm& x);

    void from_json(const json& j, MyData_MysticForge::MysticForgeElement& x);
    void to_json(json& j, const MyData_MysticForge::MysticForgeElement& x);

    inline void from_json(const json& j, MyData_MysticForge::Itm& x) {
        x.set_a(j.at("A").get<int>());
        x.set_b(j.at("B").get<int>());
    }

    inline void to_json(json& j, const MyData_MysticForge::Itm& x) {
        j = json::object();
        j["A"] = x.get_a();
        j["B"] = x.get_b();
    }

    inline void from_json(const json& j, MyData_MysticForge::MysticForgeElement& x) {
        x.set_id(j.at("ID").get<int>());
        x.set_ot(j.at("OT").get<int>());
        x.set_itms(j.at("ITMS").get<std::vector<MyData_MysticForge::Itm>>());
    }

    inline void to_json(json& j, const MyData_MysticForge::MysticForgeElement& x) {
        j = json::object();
        j["ID"] = x.get_id();
        j["OT"] = x.get_ot();
        j["ITMS"] = x.get_itms();
    }
}


