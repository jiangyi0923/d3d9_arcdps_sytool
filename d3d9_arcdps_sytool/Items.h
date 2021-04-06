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

namespace MyData_Items {
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
        int id;
        std::string na;
        std::string nae;
        std::string de;
        std::string dee;
        std::string cl;
        int lv;
        int tp;
        int ra;
        std::vector<int> ro;
        std::vector<int> rt;
        std::vector<int> mo;
        std::vector<int> mt;

    public:
        const int& get_id() const { return id; }
        int& get_mutable_id() { return id; }
        void set_id(const int& value) { this->id = value; }

        const std::string& get_na() const { return na; }
        std::string& get_mutable_na() { return na; }
        void set_na(const std::string& value) { this->na = value; }

        const std::string& get_nae() const { return nae; }
        std::string& get_mutable_nae() { return nae; }
        void set_nae(const std::string& value) { this->nae = value; }

        const std::string& get_de() const { return de; }
        std::string& get_mutable_de() { return de; }
        void set_de(const std::string& value) { this->de = value; }

        const std::string& get_dee() const { return dee; }
        std::string& get_mutable_dee() { return dee; }
        void set_dee(const std::string& value) { this->dee = value; }

        const std::string& get_cl() const { return cl; }
        std::string& get_mutable_cl() { return cl; }
        void set_cl(const std::string& value) { this->cl = value; }

        const int& get_lv() const { return lv; }
        int& get_mutable_lv() { return lv; }
        void set_lv(const int& value) { this->lv = value; }

        const int& get_tp() const { return tp; }
        int& get_mutable_tp() { return tp; }
        void set_tp(const int& value) { this->tp = value; }

        const int& get_ra() const { return ra; }
        int& get_mutable_ra() { return ra; }
        void set_ra(const int& value) { this->ra = value; }

        const std::vector<int>& get_ro() const { return ro; }
        std::vector<int>& get_mutable_ro() { return ro; }
        void set_ro(const std::vector<int>& value) { this->ro = value; }

        const std::vector<int>& get_rt() const { return rt; }
        std::vector<int>& get_mutable_rt() { return rt; }
        void set_rt(const std::vector<int>& value) { this->rt = value; }

        const std::vector<int>& get_mo() const { return mo; }
        std::vector<int>& get_mutable_mo() { return mo; }
        void set_mo(const std::vector<int>& value) { this->mo = value; }

        const std::vector<int>& get_mt() const { return mt; }
        std::vector<int>& get_mutable_mt() { return mt; }
        void set_mt(const std::vector<int>& value) { this->mt = value; }
    };

    using Items = std::vector<Item>;
}

namespace MyData_Items {
    using Items = std::vector<Item>;
}

namespace nlohmann {
    void from_json(const json& j, MyData_Items::Item& x);
    void to_json(json& j, const MyData_Items::Item& x);

    inline void from_json(const json& j, MyData_Items::Item& x) {
        x.set_id(j.at("ID").get<int>());
        x.set_na(j.at("NA").get<std::string>());
        x.set_nae(j.at("NAE").get<std::string>());
        x.set_de(j.at("DE").get<std::string>());
        x.set_dee(j.at("DEE").get<std::string>());
        x.set_cl(j.at("CL").get<std::string>());
        x.set_lv(j.at("LV").get<int>());
        x.set_tp(j.at("TP").get<int>());
        x.set_ra(j.at("RA").get<int>());
        x.set_ro(j.at("RO").get<std::vector<int>>());
        x.set_rt(j.at("RT").get<std::vector<int>>());
        x.set_mo(j.at("MO").get<std::vector<int>>());
        x.set_mt(j.at("MT").get<std::vector<int>>());
    }

    inline void to_json(json& j, const MyData_Items::Item& x) {
        j = json::object();
        j["ID"] = x.get_id();
        j["NA"] = x.get_na();
        j["NAE"] = x.get_nae();
        j["DE"] = x.get_de();
        j["DEE"] = x.get_dee();
        j["CL"] = x.get_cl();
        j["LV"] = x.get_lv();
        j["TP"] = x.get_tp();
        j["RA"] = x.get_ra();
        j["RO"] = x.get_ro();
        j["RT"] = x.get_rt();
        j["MO"] = x.get_mo();
        j["MT"] = x.get_mt();
    }
}


