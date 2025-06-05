#ifndef REPORT_HPP
#define REPORT_HPP
#include <string>
#include "Incident.hpp"
#include "Date.hpp"

class Report{ // Report class
private:
    Incident incident; // Incident object
    Date date; // Date object
public:
    Report(Incident i, Date d); //constructor
    void displayReport();
    void modifyReportLocation(Report* report, const std::string &NewLocation); // modify location
    void modifyReportName(Report* report, const std::string& newName); // modify name
    void modifyReportQuantity(Report* report, int newQuantity); // modify quantity
    void modifyReportDate(Report* report, int newDay, int newMonth, int newYear); // modify date
    void modifyReport(Report* report, const std::string& newLocation, const std::string& newName, int newQuantity, int newDay, int newMonth, int newYear); // modify all
    int getIncidentQuantity() const;
    std::string getIncidentName() const;
    std::string getIncidentLocation() const;
    int getDateDay() const;
    int getDateMonth() const;
    int getDateYear() const;
    // Additions for CSV and utility support
    bool equals(const Report& other) const;
    std::string toCSV() const;
    // Additional getters for main2.cpp compatibility
    std::string getName() const;
    std::string getLocation() const;
    int getDay() const;
    int getMonth() const;
    int getYear() const;
    // Optional: display alias for displayReport
    void display() const { const_cast<Report*>(this)->displayReport(); }
    friend class Incident;
    friend class Date;
};

#endif // REPORT_HPP
