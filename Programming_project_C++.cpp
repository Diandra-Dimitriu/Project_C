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
    std::cout << "  modify_incident <incident_name> <incident_location> <incident_day> <incident_month> <incident_year> <new_name> <new_quantity> <new_location> <new_day> <new_month> <new_year>\n";
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
    // Try to read the file and count how many rows (excluding header) are present
    std::ifstream infile(filename);
    int rowCount = 0;
    std::string line;
    // Read header if exists
    if (std::getline(infile, line)) {
        // Count the number of data rows
        while (std::getline(infile, line)) {
            if (!line.empty()) ++rowCount;
        }
    }
    infile.close();

    if (rowCount >= 6) {
        std::cout << "All 6 rows are filled. Cannot write more reports.\n";
        return;
    }

    // If file is empty, write header as 1,2,3,4
    std::ofstream outfile;
    if (rowCount == 0) {
        outfile.open(filename, std::ios::app);
        outfile << "1,2,3,4\n";
        outfile.close();
    }

    // Write the report as: name,quantity,location,date
    outfile.open(filename, std::ios::app);
    outfile << report.getIncidentName() << ","
           << report.getIncidentQuantity() << ","
           << report.getIncidentLocation() << ","
           << report.getDateDay() << "." << report.getDateMonth() << "." << report.getDateYear() << "\n";
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

    // Print each report row as: name, quantity, location, date
    int row = 1;
    while (std::getline(file, line)) {
        if (line.empty()) continue;
        std::vector<std::string> fields;
        size_t start = 0, end = 0;
        while ((end = line.find(',', start)) != std::string::npos) {
            fields.push_back(line.substr(start, end - start));
            start = end + 1;
        }
        fields.push_back(line.substr(start));
        std::cout << "Report " << row << ":\n";
        for (size_t i = 0; i < headers.size() && i < fields.size(); ++i) {
            std::cout << headers[i] << ": " << fields[i] << "\n";
        }
        e();
        ++row;
    }
    file.close();
}

void deleteFromCSV(const std::string& filename, const std::string& incidentName, const std::string& incidentLocation, int day, int month, int year) {
    std::ifstream infile(filename);
    if (!infile) {
        std::cerr << "Error opening file!\n";
        return;
    }
    std::vector<std::string> rows;
    std::string line;
    // Read header
    if (!std::getline(infile, line)) {
        std::cerr << "Error reading file!\n";
        return;
    }
    std::string header = line;
    // Read all report rows
    while (std::getline(infile, line)) {
        if (!line.empty()) rows.push_back(line);
    }
    infile.close();

    // Find the row to delete
    int deleteIndex = -1;
    for (size_t i = 0; i < rows.size(); ++i) {
        std::vector<std::string> fields;
        size_t start = 0, end = 0;
        while ((end = rows[i].find(',', start)) != std::string::npos) {
            fields.push_back(rows[i].substr(start, end - start));
            start = end + 1;
        }
        fields.push_back(rows[i].substr(start));
        if (fields.size() == 4) {
            std::string name = fields[0];
            std::string location = fields[2];
            std::string dateStr = fields[3];
            int d, m, y;
            char dot1, dot2;
            std::stringstream dateSS(dateStr);
            dateSS >> d >> dot1 >> m >> dot2 >> y;
            if (name == incidentName && location == incidentLocation && d == day && m == month && y == year) {
                deleteIndex = static_cast<int>(i);
                break;
            }
        }
    }
    if (deleteIndex == -1) {
        std::cout << "Incident not found in CSV.\n";
        return;
    }
    // Remove the row and shift left
    rows.erase(rows.begin() + deleteIndex);
    // Write back to file
    std::ofstream outfile(filename, std::ios::trunc);
    if (!outfile) {
        std::cerr << "Error opening file for writing!\n";
        return;
    }
    outfile << header << "\n";
    for (const auto& row : rows) {
        outfile << row << "\n";
    }
    outfile.close();
    std::cout << "Incident deleted from CSV.\n";
}
void modifyInCSV(const std::string& filename, const std::string& incidentName, const std::string& incidentLocation, int day, int month, int year, const std::string& newLocation, const std::string& newName, int newQuantity, int newDay, int newMonth, int newYear) {
    std::ifstream infile(filename);
    if (!infile) {
        std::cerr << "Error opening file!\n";
        return;
    }
    std::vector<std::string> rows;
    std::string line;
    // Read header
    if (!std::getline(infile, line)) {
        std::cerr << "Error reading file!\n";
        return;
    }
    std::string header = line;
    // Read all report rows
    while (std::getline(infile, line)) {
        if (!line.empty()) rows.push_back(line);
    }
    infile.close();

    // Find the row to modify
    int modifyIndex = -1;
    for (size_t i = 0; i < rows.size(); ++i) {
        std::vector<std::string> fields;
        size_t start = 0, end = 0;
        while ((end = rows[i].find(',', start)) != std::string::npos) {
            fields.push_back(rows[i].substr(start, end - start));
            start = end + 1;
        }
        fields.push_back(rows[i].substr(start));
        if (fields.size() == 4) {
            std::string name = fields[0];
            std::string location = fields[2];
            std::string dateStr = fields[3];
            int d, m, y;
            char dot1, dot2;
            std::stringstream dateSS(dateStr);
            dateSS >> d >> dot1 >> m >> dot2 >> y;
            if (name == incidentName && location == incidentLocation && d == day && m == month && y == year) {
                modifyIndex = static_cast<int>(i);
                break;
            }
        }
    }
    if (modifyIndex == -1) {
        std::cout << "Incident not found in CSV.\n";
        return;
    }
    // Modify the row
    std::ostringstream oss;
    oss << newName << "," << newQuantity << "," << newLocation << "," << newDay << "." << newMonth << "." << newYear;
    rows[modifyIndex] = oss.str();
    // Write back to file
    std::ofstream outfile(filename, std::ios::trunc);
    if (!outfile) {
        std::cerr << "Error opening file for writing!\n";
        return;
    }
    outfile << header << "\n";
    for (const auto& row : rows) {
        outfile << row << "\n";
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
        std::string new_name = argv[7];
        int new_quantity = std::stoi(argv[8]);
        std::string new_location = argv[9];
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
