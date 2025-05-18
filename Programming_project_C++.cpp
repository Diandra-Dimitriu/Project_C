#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cstdlib>  // for system()
#include <sys/stat.h>
#include <sstream>

void clearScreen() {
#ifdef _WIN32
    system("cls");  // Windows
#else
    system("clear");  // UNIX-like
#endif
}
void e() {
    std::cout << "-----------------------\n";
}
void printColored(const std::string& text, const std::string& colorCode) {
    std::cout << colorCode << text << "\033[0m";
}
void usage() {
    std::cout << "Usage: ./program_name <operation> <args>\n";
    std::cout << "Operations:\n";
    std::cout << "  add_incident <incident_name> <incident_number> <incident_location> <incident_day> <incident_month> <incident_year>\n";
    std::cout << "  delete_incident <incident_name> <incident_location> <incident_day> <incident_month> <incident_year>\n";
    std::cout << "  modify_incident <incident_number> <new_location> <new_name> <new_quantity> <new_day> <new_month> <new_year>\n";
    std::cout << "  view_incidents\n";
}
class Incident { // Incident class
private:
    std::string location, name;
    int quantity;
public:
    Incident(std::string a, std::string b, int c)
    {
        location = a;
        name = b;
        quantity = c;
    }
    friend class Report;
};
class Date{ // Date class
private: 
    int day, month, year;
public:
    Date(int d, int m, int y)
    {
        day = d;
        month = m;
        year = y;
    }
    friend class Report;
};
class Report{ // Report class
private:
    Incident incident; // Incident object
    Date date; // Date object
public:
    Report(Incident i, Date d): incident(i), date(d) {} //constructor
    void displayReport() {
        std::cout << "Incident Location: " << incident.location << "\n";
        std::cout << "Incident Name: " << incident.name << "\n";
        std::cout << "Incident Quantity: " << incident.quantity << "\n";
        std::cout << "Date: " << date.day << "/" << date.month << "/" << date.year << "\n";
    }
    void modifyReportLocation(Report* report, const std::string &NewLocation) { // modify location
        report->incident.location = NewLocation;
    }
    void modifyReportName(Report* report, const std::string& newName) { // modify name
        report->incident.name = newName;
    }
    void modifyReportQuantity(Report* report, int newQuantity) { // modify quantity
        report->incident.quantity = newQuantity;
    }
    void modifyReportDate(Report* report, int newDay, int newMonth, int newYear) { // modify date
        report->date.day = newDay;
        report->date.month = newMonth;
        report->date.year = newYear;
    }
    void modifyReport(Report* report, const std::string& newLocation, const std::string& newName, int newQuantity, int newDay, int newMonth, int newYear) { // modify all
        report->incident.location = newLocation;
        report->incident.name = newName;
        report->incident.quantity = newQuantity;
        report->date.day = newDay;
        report->date.month = newMonth;
        report->date.year = newYear;
    }
    int getIncidentQuantity() const { return incident.quantity; }
    std::string getIncidentName() const { return incident.name; }
    std::string getIncidentLocation() const { return incident.location; }
    int getDateDay() const { return date.day; }
    int getDateMonth() const { return date.month; }
    int getDateYear() const { return date.year; }
};
void storeToCSV(const std::string& filename, const Report& report) {
    // Try to read the file and count how many headers ("header 1", ..., "header 6") are present
    std::ifstream infile(filename);
    int headerCount = 0;
    std::vector<std::string> lines;
    std::string line;
    while (std::getline(infile, line)) {
        lines.push_back(line);
        if (line.find("header ") == 0) {
            ++headerCount;
        }
    }
    infile.close();

    if (headerCount >= 6) {
        std::cout << "All 6 headers are filled. Cannot write more reports.\n";
        return;
    }

    // Prepare the new header and report line
    std::ostringstream oss;
    oss << "header " << (headerCount + 1) << "\n";
    oss << report.getIncidentName() << ","
        << report.getIncidentQuantity() << ","
        << report.getIncidentLocation() << ","
        << report.getDateDay() << "." << report.getDateMonth() << "." << report.getDateYear()
        << "\n";

    // Append the new header and report to the file
    std::ofstream outfile(filename, std::ios::app);
    if (!outfile) {
        std::cerr << "Error opening file!\n";
        return;
    }
    outfile << oss.str();
    outfile.close();
}
void readFromCSV(const std::string& filename) {
    std::ifstream file(filename);
    if (!file) {
        std::cerr << "Error opening file!\n";
        return;
    }

    std::string line;

    // Read header
    if (!std::getline(file, line)) {
        std::cerr << "Error reading file!\n";
        return;
    }
    std::vector<std::string> headers;
    std::stringstream header_ss(line);
    std::string header;
    while (std::getline(header_ss, header, ',')) {
        headers.push_back(header);
    }

    // Print only under the headers where data is present
    while (std::getline(file, line)) {
        if (line.empty()) continue;
        std::vector<std::string> fields;
        std::string field;
        size_t start = 0, end = 0;
        // Custom CSV parsing to handle fields with spaces and preserve them
        while ((end = line.find(',', start)) != std::string::npos) {
            field = line.substr(start, end - start);
            fields.push_back(field); // Do not trim or split on spaces
            start = end + 1;
        }
        // Last field (after last comma)
        field = line.substr(start);
        fields.push_back(field);
        // Only print fields that are not empty and under a header
        for (size_t i = 0; i < headers.size() && i < fields.size(); ++i) {
            std::cout << headers[i] << ": " << fields[i] << "\n";
        }
        e();
    }

    file.close();
}

void deleteFromCSV(const std::string& filename, const std::string& incidentName, const std::string& incidentLocation, int day, int month, int year) {
    std::ifstream infile(filename);
    if (!infile) {
        std::cerr << "Error opening file!\n";
        return;
    }
    std::vector<std::string> lines;
    std::string line;
    int headerCount = 0;
    int deleteIndex = -1;
    std::vector<std::string> reportLines;
    // Read all lines and find the report to delete
    while (std::getline(infile, line)) {
        if (line.find("header ") == 0) {
            ++headerCount;
            reportLines.push_back(line); // header line
            std::getline(infile, line); // column header
            reportLines.push_back(line);
            std::getline(infile, line); // report line
            // Parse the report line
            std::stringstream ss(line);
            std::string name, quantityStr, location, dateStr;
            std::getline(ss, name, ',');
            std::getline(ss, quantityStr, ',');
            std::getline(ss, location, ',');
            std::getline(ss, dateStr);
            int d, m, y;
            char dot1, dot2;
            std::stringstream dateSS(dateStr);
            dateSS >> d >> dot1 >> m >> dot2 >> y;
            if (name == incidentName && location == incidentLocation && d == day && m == month && y == year) {
                deleteIndex = headerCount - 1;
                // Skip adding this report to lines (delete it)
                continue;
            }
            lines.push_back(reportLines[reportLines.size() - 2]); // header
            lines.push_back(reportLines[reportLines.size() - 1]); // column header
            lines.push_back(line); // report line
        }
    }
    infile.close();
    if (deleteIndex == -1) {
        std::cout << "Incident not found in CSV.\n";
        return;
    }
    // Now, shift all headers after deleteIndex to the left
    std::ofstream outfile(filename, std::ios::trunc);
    if (!outfile) {
        std::cerr << "Error opening file for writing!\n";
        return;
    }
    int currentHeader = 1;
    for (size_t i = 0; i < lines.size(); i += 3) {
        if (static_cast<int>(i / 3) == deleteIndex) continue; // skip deleted
        outfile << "header " << currentHeader << "\n";
        outfile << lines[i + 1] << "\n";
        outfile << lines[i + 2] << "\n";
        ++currentHeader;
    }
    outfile.close();
    std::cout << "Incident deleted from CSV and headers shifted.\n";
}
void modifyInCSV(const std::string& filename, const std::string& incidentName, const std::string& incidentLocation, int day, int month, int year, const std::string& newLocation, const std::string& newName, int newQuantity, int newDay, int newMonth, int newYear) {
    std::ifstream infile(filename);
    if (!infile) {
        std::cerr << "Error opening file!\n";
        return;
    }
    std::vector<std::string> lines;
    std::string line;
    int headerCount = 0;
    int modifyIndex = -1;
    std::vector<std::string> reportLines;
    // Read all lines and find the report to modify
    while (std::getline(infile, line)) {
        if (line.find("header ") == 0) {
            ++headerCount;
            reportLines.push_back(line); // header line
            std::getline(infile, line); // column header
            reportLines.push_back(line);
            std::getline(infile, line); // report line
            // Parse the report line
            std::stringstream ss(line);
            std::string name, quantityStr, location, dateStr;
            std::getline(ss, name, ',');
            std::getline(ss, quantityStr, ',');
            std::getline(ss, location, ',');
            std::getline(ss, dateStr);
            int quantity = std::stoi(quantityStr);
            int d, m, y;
            char dot1, dot2;
            std::stringstream dateSS(dateStr);
            dateSS >> d >> dot1 >> m >> dot2 >> y;
            if (name == incidentName && location == incidentLocation && d == day && m == month && y == year) {
                // Found the report to modify
                Incident incident(location, name, quantity);
                Date date(d, m, y);
                Report report(incident, date);
                report.modifyReport(&report, newLocation, newName, newQuantity, newDay, newMonth, newYear);
                std::ostringstream oss;
                oss << report.getIncidentName() << ","
                    << report.getIncidentQuantity() << ","
                    << report.getIncidentLocation() << ","
                    << report.getDateDay() << "." << report.getDateMonth() << "." << report.getDateYear();
                lines.push_back(reportLines[reportLines.size() - 2]); // header
                lines.push_back(reportLines[reportLines.size() - 1]); // column header
                lines.push_back(oss.str()); // modified report line
                modifyIndex = headerCount - 1;
                continue;
            }
            lines.push_back(reportLines[reportLines.size() - 2]); // header
            lines.push_back(reportLines[reportLines.size() - 1]); // column header
            lines.push_back(line); // report line
        }
    }
    infile.close();
    if (modifyIndex == -1) {
        std::cout << "Incident not found in CSV.\n";
        return;
    }
    // Write all lines back to the file
    std::ofstream outfile(filename, std::ios::trunc);
    if (!outfile) {
        std::cerr << "Error opening file for writing!\n";
        return;
    }
    for (size_t i = 0; i < lines.size(); i += 3) {
        outfile << lines[i] << "\n";
        outfile << lines[i + 1] << "\n";
        outfile << lines[i + 2] << "\n";
    }
    outfile.close();
    std::cout << "Incident modified in CSV.\n";
}


int main(int argc, char* argv[]) {
    Report* report;
    std::string operation = argv[1];
    if (argv[1] == std::string("add_incident"))
    {
        std::string incident_name = argv[2];
        int incident_number = std::stoi(argv[3]);
        std::string incident_location = argv[4];
        int incident_day = std::stoi(argv[5]);
        int incident_month = std::stoi(argv[6]);
        int incident_year = std::stoi(argv[7]);
        Incident incident_new(incident_location, incident_name, incident_number);
        Date date_new(incident_day, incident_month, incident_year);
        report = new Report(incident_new, date_new);
        storeToCSV("temporary.csv", *report);
        std::cout << "Incident added successfully.\n";
    }
    else if (argv[1] == std::string("delete_incident"))
    {
        std::string incident_name = argv[2];
        std::string incident_location = argv[3];
        int incident_day = std::stoi(argv[4]);
        int incident_month = std::stoi(argv[5]);
        int incident_year = std::stoi(argv[6]);
        deleteFromCSV("temporary.csv", incident_name, incident_location, incident_day, incident_month, incident_year);
    }
    else if (argv[1] == std::string("modify_incident"))
    {
        std::string incident_name = argv[2];
        std::string incident_location = argv[3];
        int incident_day = std::stoi(argv[4]);
        int incident_month = std::stoi(argv[5]);
        int incident_year = std::stoi(argv[6]);
        std::string new_location = argv[7];
        std::string new_name = argv[8];
        int new_quantity = std::stoi(argv[9]);
        int new_day = std::stoi(argv[10]);
        int new_month = std::stoi(argv[11]);
        int new_year = std::stoi(argv[12]);
        modifyInCSV("temporary.csv", incident_name, incident_location, incident_day, incident_month, incident_year, new_location, new_name, new_quantity, new_day, new_month, new_year);
    }
    else if (argv[1] == std::string("view_incidents"))
    {
        readFromCSV("temporary.csv");
    }
    else {
        std::cout << "Invalid operation.\n";
        usage();
    }
    return 0;
}
