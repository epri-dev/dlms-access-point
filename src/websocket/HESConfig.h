#ifndef HESCONFIG_H
#define HESCONFIG_H
#include <string>

class HESConfig {
public:
    enum class payload { small, medium, large};
    bool route_only;
    void load(const std::string &filename);
    void save(const std::string &filename) const;
    payload payload_size = payload::small;
};

#endif // HESCONFIG_H
