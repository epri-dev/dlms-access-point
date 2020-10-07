#ifndef HESCONFIG_H
#define HESCONFIG_H
#include <string>
#include <iostream>
#include <thread>
#include <mutex>

class HESConfig {
public:
    enum class payload { small, medium, large };
    HESConfig(unsigned short portnum = 4060);
    ~HESConfig();

    void load(const std::string &filename);
    void load(std::istream &json);
    void load_from_string(std::string str);
    void save(const std::string &filename) const;
    void save(std::ostream &json) const;
    payload get_payload_size() const;
    bool get_route_only() const;
private:
    mutable std::mutex m;
    payload payload_size = payload::small;
    bool route_only = true;
    std::thread serverthread;
};

#endif // HESCONFIG_H
