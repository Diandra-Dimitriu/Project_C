#include "../include/Report.hpp"
#include <iostream>
#include <string>
#include <sstream> // For std::ostringstream

Report::Report(Incident i, Date d) : incident(i), date(d) {}

void Report::displayReport() {
    std::cout << "Incident Location: " << incident.location << "\n";
    std::cout << "Incident Name: " << incident.name << "\n";
    std::cout << "Incident Quantity: " << incident.quantity << "\n";
    std::cout << "Date: " << date.day << "/" << date.month << "/" << date.year << "\n";
}

void Report::modifyReportLocation(Report* report, const std::string& NewLocation) {
    report->incident.location = NewLocation;
}

void Report::modifyReportName(Report* report, const std::string& newName) {
    report->incident.name = newName;
}

void Report::modifyReportQuantity(Report* report, int newQuantity) {
    report->incident.quantity = newQuantity;
}

void Report::modifyReportDate(Report* report, int newDay, int newMonth, int newYear) {
    report->date.day = newDay;
    report->date.month = newMonth;
    report->date.year = newYear;
}

void Report::modifyReport(Report* report, const std::string& newLocation, const std::string& newName, int newQuantity, int newDay, int newMonth, int newYear) {
    report->incident.location = newLocation;
    report->incident.name = newName;
    report->incident.quantity = newQuantity;
    report->date.day = newDay;
    report->date.month = newMonth;
    report->date.year = newYear;
}

int Report::getIncidentQuantity() const { return incident.quantity; }
std::string Report::getIncidentName() const { return incident.name; }
std::string Report::getIncidentLocation() const { return incident.location; }
int Report::getDateDay() const { return date.day; }
int Report::getDateMonth() const { return date.month; }
int Report::getDateYear() const { return date.year; }

bool Report::equals(const Report& other) const {
    return incident.name == other.incident.name &&
           incident.location == other.incident.location &&
           incident.quantity == other.incident.quantity &&
           date.day == other.date.day &&
           date.month == other.date.month &&
           date.year == other.date.year;
}

std::string Report::toCSV() const {
    std::ostringstream oss;
    oss << incident.name << "," << incident.quantity << "," << incident.location << ", "
        << date.day << "." << date.month << "." << date.year;
    return oss.str();
}

// Additional getters for main2.cpp compatibility
std::string Report::getName() const { return incident.name; }
std::string Report::getLocation() const { return incident.location; }
int Report::getDay() const { return date.day; }
int Report::getMonth() const { return date.month; }
int Report::getYear() const { return date.year; }