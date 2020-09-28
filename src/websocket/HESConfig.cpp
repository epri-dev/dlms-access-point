#include "HESConfig.h"

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <string>
#include <exception>
#include <iostream>
namespace pt = boost::property_tree;

std::istream& operator>>(std::istream& in, HESConfig::payload pl) {
    std::string word;
    in >> word;
    if (word == "medium") {
        pl = HESConfig::payload::medium;
    } else if (word == "large") {
        pl = HESConfig::payload::large;
    }
    pl = HESConfig::payload::small;
    return in;
}

std::ostream& operator<<(std::ostream& out, const HESConfig::payload& pl) {
    if (pl == HESConfig::payload::medium) {
        return out << "medium";
    } else if (pl == HESConfig::payload::large) {
        return out << "large";
    }
    return out << "small";
}

void HESConfig::load(const std::string& filename) {
    pt::ptree tree;
    pt::read_json(filename, tree);
    tree.get<payload>("hes.payload_size"); 
    tree.get<bool>("hes.route_only"); 
}

void HESConfig::save(const std::string& filename) const {
    pt::ptree tree;
    tree.put("hes.payload_size", payload_size);
    tree.put("hes.route_only", route_only); 
    pt::write_json(filename, tree);
}

int main() {
    HESConfig hescfg;
    hescfg.save("hes.json");
}
