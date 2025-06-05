#include "Incident.hpp"
#include <string>
#include <iostream>
Incident::Incident(std::string loc, std::string name, int qty)
    : location(loc), name(name), quantity(qty) {}
