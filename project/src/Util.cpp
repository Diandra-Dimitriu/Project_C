#include "../include/Util.hpp"
#include "../include/Report.hpp"
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>

#define RED     "\033[31m"
#define GREEN   "\033[32m"
#define YELLOW  "\033[33m"
#define CYAN    "\033[36m"
#define RESET   "\033[0m"

#ifdef _WIN32
#include <windows.h>
#else
#include <unistd.h>
#endif

void clearScreen() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

void printColored(const std::string& text, const std::string& colorCode) {
    std::cout << colorCode << text << "\033[0m";
}

void usage() {
    printColored("Usage: ./program_name <operation> <args>\n", "\033[1;36m");
    printColored("Operations:\n", "\033[1;33m");
    printColored("  add_incident ", "\033[1;32m");
    printColored("<incident_name> <incident_number> <incident_location> <incident_day> <incident_month> <incident_year>\n", "\033[36m");
    printColored("  delete_incident ", "\033[1;31m");
    printColored("<incident_name> <incident_location> <incident_day> <incident_month> <incident_year>\n", "\033[36m");
    printColored("  modify_incident ", "\033[1;35m");
    printColored("<incident_name> <incident_location> <incident_day> <incident_month> <incident_year> <new_name> <new_quantity> <new_location> <new_day> <new_month> <new_year>\n", "\033[36m");
    printColored("  view_incidents\n", "\033[1;34m");
}

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
        printColored("All 6 rows are filled. Cannot write more reports.\n", "\033[1;31m");
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
        printColored("Incident not found in CSV.\n", "\033[1;31m");
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
    printColored("Incident deleted from CSV.\n", "\033[1;32m");
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
        printColored("Incident not found in CSV.\n", "\033[1;31m");
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
    printColored("Incident modified in CSV.\n", "\033[1;32m");
}



std::vector<Report> readCSV(const std::string& filename, bool skipHeader) { // read from CSV file
    std::vector<Report> reports;
    std::ifstream file(filename);
    if (!file.is_open()) return reports;

    std::string line;
    if (skipHeader) std::getline(file, line);

    while (std::getline(file, line)) {
        if (line.empty()) continue;
        std::stringstream ss(line);
        std::string name, quantityStr, location, dateStr;
        std::getline(ss, name, ',');
        std::getline(ss, quantityStr, ',');
        std::getline(ss, location, ',');
        std::getline(ss, dateStr, ',');

        int quantity = std::stoi(quantityStr);
        int day, month, year;
        char dot;
        std::stringstream dss(dateStr);
        dss >> day >> dot >> month >> dot >> year;

        Incident inc(location, name, quantity);
        Date date(day, month, year);
        reports.emplace_back(inc, date);
    }

    return reports;
}

void storeToPermanentCSV(const std::vector<Report>& newReports, const std::string& filename) {
    std::vector<Report> existingReports = readCSV(filename, false);
    std::ofstream file(filename, std::ios::app);

    for (const auto& newReport : newReports) {
        bool duplicate = false;
        for (const auto& existing : existingReports) {
            if (newReport.equals(existing)) {
                duplicate = true;
                break;
            }
        }
        if (!duplicate) {
            file << newReport.toCSV() << "\n";
        }
    }

    file.close();
}

void deleteFromIncidentsCSV(const std::string& name, const std::string& location, int day, int month, int year) {
    std::ifstream infile("Incidents.csv");
    if (!infile) {
        std::cerr <<"Could not open Incidents.csv for reading.\n";
        return;
    }

    std::vector<std::string> rows;
    std::string line;
    bool found = false;

    while (std::getline(infile, line)) {
        if (line.empty()) continue;
        std::vector<std::string> fields;
        std::stringstream ss(line);
        std::string field;

        while (std::getline(ss, field, ',')) {
            fields.push_back(field);
        }

        if (fields.size() == 4) {
            std::string rname = fields[0];
            std::string rlocation = fields[2];
            std::string date = fields[3];
            int d, m, y;
            char dot1, dot2;
            std::stringstream dss(date);
            dss >> d >> dot1 >> m >> dot2 >> y;

            if (rname == name && rlocation == location && d == day && m == month && y == year) {
                found = true;
                continue;
            }
        }
        rows.push_back(line);
    }
    infile.close();

    if (!found) {
        std::cout << YELLOW << "Incident not found in Incidents.csv. Nothing was deleted.\n" << RESET;
        return;
    }

    std::ofstream outfile("Incidents.csv", std::ios::trunc);
    for (const auto& row : rows) {
        outfile << row << "\n";
    }
    outfile.close();

    std::cout << GREEN << "Incident deleted from Incidents.csv.\n" << RESET;
}

void modifyInIncidentsCSV(const std::string& name, const std::string& location, int day, int month, int year, const std::string& newName, int newQuantity, const std::string& newLocation, int newDay, int newMonth, int newYear) {
    std::ifstream infile("Incidents.csv");
    if (!infile) {
        std::cerr << RED << "Could not open Incidents.csv for reading.\n" << RESET;
        return;
    }

    std::vector<std::string> rows;
    std::string line;
    bool found = false;

    while (std::getline(infile, line)) {
        if (line.empty()) continue;
        std::vector<std::string> fields;
        std::stringstream ss(line);
        std::string field;

        while (std::getline(ss, field, ',')) {
            fields.push_back(field);
        }

        if (fields.size() == 4) {
            std::string rname = fields[0];
            std::string rlocation = fields[2];
            std::string date = fields[3];
            int d, m, y;
            char dot1, dot2;
            std::stringstream dss(date);
            dss >> d >> dot1 >> m >> dot2 >> y;

            if (rname == name && rlocation == location && d == day && m == month && y == year) {
                std::ostringstream mod;
                mod << newName << "," << newQuantity << "," << newLocation << "," << newDay << "." << newMonth << "." << newYear;
                rows.push_back(mod.str());
                found = true;
                continue;
            }
        }
        rows.push_back(line);
    }
    infile.close();

    if (!found) {
        std::cout << YELLOW << "Incident not found in Incidents.csv. Nothing was modified.\n" << RESET;
        return;
    }

    std::ofstream outfile("Incidents.csv", std::ios::trunc);
    for (const auto& row : rows) {
        outfile << row << "\n";
    }
    outfile.close();

    std::cout << GREEN << "Incident modified in Incidents.csv.\n" << RESET;
}
