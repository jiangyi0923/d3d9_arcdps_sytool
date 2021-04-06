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

namespace MyData_Recipes {
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

    class Recipe {
    public:
        Recipe() = default;
        virtual ~Recipe() = default;

    private:
        int id;
        int ot;
        std::string cl;
        int lv;
        std::vector<Itm> itms;
        std::vector<int> av;

    public:
        const int& get_id() const { return id; }
        int& get_mutable_id() { return id; }
        void set_id(const int& value) { this->id = value; }

        const int& get_ot() const { return ot; }
        int& get_mutable_ot() { return ot; }
        void set_ot(const int& value) { this->ot = value; }

        const std::string& get_cl() const { return cl; }
        std::string& get_mutable_cl() { return cl; }
        void set_cl(const std::string& value) { this->cl = value; }

        const int& get_lv() const { return lv; }
        int& get_mutable_lv() { return lv; }
        void set_lv(const int& value) { this->lv = value; }

        const std::vector<Itm>& get_itms() const { return itms; }
        std::vector<Itm>& get_mutable_itms() { return itms; }
        void set_itms(const std::vector<Itm>& value) { this->itms = value; }

        const std::vector<int>& get_av() const { return av; }
        std::vector<int>& get_mutable_av() { return av; }
        void set_av(const std::vector<int>& value) { this->av = value; }
    };

    using Recipes = std::vector<Recipe>;
}

namespace MyData_Recipes {
    using Recipes = std::vector<Recipe>;
}

namespace nlohmann {
    void from_json(const json& j, MyData_Recipes::Itm& x);
    void to_json(json& j, const MyData_Recipes::Itm& x);

    void from_json(const json& j, MyData_Recipes::Recipe& x);
    void to_json(json& j, const MyData_Recipes::Recipe& x);

    inline void from_json(const json& j, MyData_Recipes::Itm& x) {
        x.set_a(j.at("A").get<int>());
        x.set_b(j.at("B").get<int>());
    }

    inline void to_json(json& j, const MyData_Recipes::Itm& x) {
        j = json::object();
        j["A"] = x.get_a();
        j["B"] = x.get_b();
    }

    inline void from_json(const json& j, MyData_Recipes::Recipe& x) {
        x.set_id(j.at("ID").get<int>());
        x.set_ot(j.at("OT").get<int>());
        x.set_cl(j.at("CL").get<std::string>());
        x.set_lv(j.at("LV").get<int>());
        x.set_itms(j.at("ITMS").get<std::vector<MyData_Recipes::Itm>>());
        x.set_av(j.at("AV").get<std::vector<int>>());
    }

    inline void to_json(json& j, const MyData_Recipes::Recipe& x) {
        j = json::object();
        j["ID"] = x.get_id();
        j["OT"] = x.get_ot();
        j["CL"] = x.get_cl();
        j["LV"] = x.get_lv();
        j["ITMS"] = x.get_itms();
        j["AV"] = x.get_av();
    }
}


