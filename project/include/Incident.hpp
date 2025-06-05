#ifndef INCIDENT_HPP
#define INCIDENT_HPP

#include <string>

class Incident {
public:
    Incident(std::string loc, std::string name, int qty);

protected:
    std::string location;
    std::string name;
    int quantity;
    friend class Report;
};

#endif // INCIDENT_HPP

