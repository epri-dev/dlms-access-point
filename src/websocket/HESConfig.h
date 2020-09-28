#ifndef HESCONFIG_H
#define HESCONFIG_H
#include <string>
#include <iostream>

class HESConfig {
public:
    enum class payload { small, medium, large } payload_size = payload::small;
    bool route_only = true;

    void load(const std::string &filename);
    void load(std::istream &json);
    void save(const std::string &filename) const;
    void save(std::ostream &json) const;
};

#endif // HESCONFIG_H
