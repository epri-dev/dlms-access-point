#include "HESConfig.h"

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <string>
#include <exception>
#include <iostream>
namespace pt = boost::property_tree;

namespace boost {
namespace property_tree {

template<>
struct translator_between<std::string, HESConfig::payload>
{
  struct type {
    typedef std::string internal_type;
    typedef HESConfig::payload external_type;
    boost::optional<external_type> get_value(const internal_type& str) {
        if (str == "medium") {
            return HESConfig::payload::medium;
        } else if (str == "large") {
            return HESConfig::payload::large;
        }
        return HESConfig::payload::small;
    }
    boost::optional<internal_type> put_value(const external_type& obj) {
        using namespace std::string_literals;
        if (obj == HESConfig::payload::medium) {
            return "medium"s;
        } else if (obj == HESConfig::payload::large) {
            return "large"s;
        }
        return "small"s;
    }
  };
};

} // namespace property_tree
} // namespace boost

void HESConfig::load(const std::string& filename) {
    pt::ptree tree;
    pt::read_json(filename, tree);
    payload_size = tree.get<payload>("hes.payload_size"); 
    route_only = tree.get<bool>("hes.route_only"); 
}

void HESConfig::load(std::istream& json) {
    pt::ptree tree;
    pt::read_json(json, tree);
    payload_size = tree.get<payload>("hes.payload_size"); 
    route_only = tree.get<bool>("hes.route_only"); 
}

void HESConfig::save(const std::string& filename) const {
    pt::ptree tree;
    tree.put("hes.payload_size", payload_size);
    tree.put("hes.route_only", route_only); 
    pt::write_json(filename, tree);
}

void HESConfig::save(std::ostream& json) const {
    pt::ptree tree;
    tree.put("hes.payload_size", payload_size);
    tree.put("hes.route_only", route_only); 
    pt::write_json(json, tree);
}

int main() {
    HESConfig hescfg;
    hescfg.load("hes.json");
#if 0
    std::cout 
        << "payload = " << hescfg.payload_size 
        << "\nroute_only = " << std::boolalpha << hescfg.route_only
        << "\n";
#endif
    hescfg.save("smoo.json");
}
