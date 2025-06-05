#ifndef REPORT_H
#define REPORT_H

#include "Incident.h"
#include "Date.h"
#include <string>

class Report {
private:
    Incident incident;
    Date date;
public:
    Report(Incident i, Date d);
    void displayReport();
    void modifyReportLocation(const std::string& newLoc);
    void modifyReportName(const std::string& newName);
    void modifyReportQuantity(int newQty);
    void modifyReportDate(int newDay, int newMonth, int newYear);
    void modifyReport(const std::string& newLoc, const std::string& newName, int newQty, int newDay, int newMonth, int newYear);

    std::string getIncidentName() const;
    int getIncidentQuantity() const;
    std::string getIncidentLocation() const;
    int getDateDay() const;
    int getDateMonth() const;
    int getDateYear() const;
};

#endif
