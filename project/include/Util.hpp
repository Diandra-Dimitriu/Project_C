#ifndef UTIL_HPP
#define UTIL_HPP

#include <string>
#include <vector>
#include "Report.hpp"

void clearScreen();
void printColored(const std::string& text, const std::string& colorCode);
void usage();
void storeToCSV(const std::string& filename, const Report& report);
void readFromCSV(const std::string& filename);
void deleteFromCSV(const std::string& filename, const std::string& incidentName, const std::string& incidentLocation, int day, int month, int year);
void modifyInCSV(const std::string& filename, const std::string& incidentName, const std::string& incidentLocation, int day, int month, int year, const std::string& newLocation, const std::string& newName, int newQuantity, int newDay, int newMonth, int newYear) ;
std::vector<Report> readCSV(const std::string& filename, bool skipHeader = true);
void storeToPermanentCSV(const std::vector<Report>& newReports, const std::string& filename);
void deleteFromIncidentsCSV(const std::string& name, const std::string& location, int day, int month, int year);
void modifyInIncidentsCSV(const std::string& name, const std::string& location, int day, int month, int year, const std::string& newName, int newQuantity, const std::string& newLocation, int newDay, int newMonth, int newYear);


#endif // UTIL_HPP
