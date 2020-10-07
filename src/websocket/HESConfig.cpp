#include "HESConfig.h"
#include "server.h"

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <string>
#include <sstream>
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

HESConfig::HESConfig(unsigned short portnum) {
    serverthread = std::thread(server, portnum, std::ref(*this));
}

HESConfig::~HESConfig() {
    serverthread.join();
}

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

void HESConfig::load_from_string(std::string str) {
    std::stringstream ss{str};
    try {
        load(ss);
    } catch(const boost::property_tree::ptree_error &e) {
        std::err << "Error parsing json string: " << e.what() << '\n';
    }
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

HESConfig::payload HESConfig::get_payload_size() const {
    std::lock_guard<std::mutex> lk(m);
    return payload_size;
}

bool HESConfig::get_route_only() const {
    std::lock_guard<std::mutex> lk(m);
    return route_only;
}
