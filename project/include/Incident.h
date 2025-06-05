#ifndef INCIDENT_H
#define INCIDENT_H

#include <string>

class Report; // Forward declaration

class Incident {
private:
    std::string location, name;
    int quantity;
public:
    Incident(std::string loc, std::string name, int qty);
    friend class Report;
};

#endif
