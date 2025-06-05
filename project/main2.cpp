#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cstdlib>
#include <sys/stat.h>
#include <sstream>

#include "include/Incident.hpp"
#include "include/Date.hpp"
#include "include/Report.hpp"
#include "include/Util.hpp"

#define RED     "\033[31m"
#define GREEN   "\033[32m"
#define YELLOW  "\033[33m"
#define CYAN    "\033[36m"
#define RESET   "\033[0m"

int main(int argc, char* argv[]) {
    auto isValidDate = [](int day, int month, int year) {
        return (day >= 1 && day <= 31) && (month >= 1 && month <= 12) && (year >= 1900 && year <= 2100);
    };

    auto isNonNegative = [](int num) {
        return num >= 0;
    };

    auto isNonEmpty = [](const std::string& str) {
        return !str.empty();
    };

    if (argc < 2) {
        std::cout << "Usage: <app_name> <operation> [...]\n";
        return 1;
    }

    std::string operation = argv[1];

    if (operation == "view_all") {
        std::vector<Report> tempReports = readCSV("temporary.csv");
        storeToPermanentCSV(tempReports, "Incidents.csv");

        if (std::remove("temporary.csv") == 0) {
            std::cout << GREEN << "temporary.csv deleted successfully.\n" << RESET;
        } else {
            std::cerr << RED << "Error deleting temporary.csv or file not found.\n" << RESET;
        }

        std::vector<Report> allReports = readCSV("Incidents.csv", false);
        for (const auto& report : allReports) {
            report.display();
        }
    }
        else if (operation == "filter") {
    std::vector<Report> allReports = readCSV("Incidents.csv", false);

    if (argc < 3) {
        std::cout << "Usage: <app_name> filter <name> [location] [day|month|year] <value>\n";
        return 1;
    }

    std::string name_filter = argv[2];
    if (!isNonEmpty(name_filter)) {
        std::cerr << RED << "Error: Name cannot be empty.\n" << RESET;
        return 1;
    }

    bool use_location_filter = false;
    std::string location_filter;
    int filter_value = -1;
    std::string filter_type;

    // Parse optional filters
    int argIndex = 3;

    // Check if next arg is location (not day/month/year)
    if (argc > argIndex) {
        std::string possibleLocation = argv[argIndex];
        if (possibleLocation != "day" && possibleLocation != "month" && possibleLocation != "year") {
            location_filter = possibleLocation;
            use_location_filter = true;
            argIndex++;
        }
    }

    // Check if date filter is specified
    if (argc > argIndex + 1) {
        filter_type = argv[argIndex];
        std::string value_str = argv[argIndex + 1];
        try {
            filter_value = std::stoi(value_str);
        } catch (...) {
            std::cerr << RED << "Error: Invalid filter value for date.\n" << RESET;
            return 1;
        }

        if (filter_type == "day" && (filter_value < 1 || filter_value > 31)) {
            std::cerr << RED << "Error: Invalid day.\n" << RESET;
            return 1;
        }
        if (filter_type == "month" && (filter_value < 1 || filter_value > 12)) {
            std::cerr << RED << "Error: Invalid month.\n" << RESET;
            return 1;
        }
        if (filter_type == "year" && (filter_value < 1900 || filter_value > 2100)) {
            std::cerr << RED << "Error: Invalid year.\n" << RESET;
            return 1;
        }
    }

    bool found = false;
    for (const auto& report : allReports) {
        if (report.getName() != name_filter) continue;
        if (use_location_filter && report.getLocation() != location_filter) continue;

        if (!filter_type.empty()) {
            if (filter_type == "day" && report.getDay() != filter_value) continue;
            if (filter_type == "month" && report.getMonth() != filter_value) continue;
            if (filter_type == "year" && report.getYear() != filter_value) continue;
        }

        report.display();
        found = true;
    }

    if (!found) {
        std::cerr << RED << "Error: No incidents found matching the criteria.\n" << RESET;
        return 1;
    }
}

    else if (operation == "delete_incident") {
        if (argc != 7) {
            std::cerr << "Usage: <app_name> delete_incident <name> <location> <day> <month> <year>\n";
            return 1;
        }
        std::string name = argv[2];
        std::string location = argv[3];
        int day = std::stoi(argv[4]);
        int month = std::stoi(argv[5]);
        int year = std::stoi(argv[6]);

        if (!isNonEmpty(name) || !isNonEmpty(location)) {
            std::cerr << "Error: Name and location cannot be empty.\n";
            return 1;
        }
        if (!isValidDate(day, month, year)) {
            std::cerr << "Error: Invalid date.\n";
            return 1;
        }

        deleteFromIncidentsCSV(name, location, day, month, year);
    }
    else if (operation == "modify_incident") {
        if (argc != 13) {
            std::cerr << "Usage: <app_name> modify_incident <name> <location> <day> <month> <year> <new_name> <new_quantity> <new_location> <new_day> <new_month> <new_year>\n";
            return 1;
        }

        std::string name = argv[2];
        std::string location = argv[3];
        int day = std::stoi(argv[4]);
        int month = std::stoi(argv[5]);
        int year = std::stoi(argv[6]);

        std::string newName = argv[7];
        int newQuantity = std::stoi(argv[8]);
        std::string newLocation = argv[9];
        int newDay = std::stoi(argv[10]);
        int newMonth = std::stoi(argv[11]);
        int newYear = std::stoi(argv[12]);

        if (!isNonEmpty(name) || !isNonEmpty(location) || !isNonEmpty(newName) || !isNonEmpty(newLocation)) {
            std::cerr << RED << "Error: Name and location fields cannot be empty.\n" << RESET;
            return 1;
        }
        if (!isValidDate(day, month, year) || !isValidDate(newDay, newMonth, newYear)) {
            std::cerr << RED << "Error: Invalid date provided.\n" << RESET;
            return 1;
        }
        if (!isNonNegative(newQuantity)) {
            std::cerr << RED << "Error: Quantity must be non-negative.\n" << RESET;
            return 1;
        }

        modifyInIncidentsCSV(name, location, day, month, year, newName, newQuantity, newLocation, newDay, newMonth, newYear);
    }
    else {
        std::cout << RED << "Invalid operation. Use:" << RESET << "\n";
        std::cout << CYAN << "  view_all\n" << RESET;
        std::cout << CYAN << "  filter <name> <location> [day|month|year] <value>\n" << RESET;
        std::cout << YELLOW << "  delete_incident <name> <location> <day> <month> <year>\n" << RESET;
        std::cout << GREEN << "  modify_incident <name> <location> <day> <month> <year> <new_name> <new_quantity> <new_location> <new_day> <new_month> <new_year>\n" << RESET;
        return 1;
    }

    return 0;
}