#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cstdlib>
#include <sys/stat.h>
#include <sstream>

#include "Incident.hpp"
#include "Date.hpp"
#include "Report.hpp"
#include "Util.hpp"

int main(int argc, char* argv[]) {
    Report* report;
    if (argc < 2) {
        usage();
        return 1;
    }
    std::string operation = argv[1];
    if (argv[1] == std::string("add_incident"))
    {
        try
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
        catch (const std::exception& e) {
            std::cerr << "Error: " << e.what() << "\n";
            usage();
            return 1;
        }
    }
    else if (argv[1] == std::string("delete_incident"))
    {
        try
        {
        
        std::string incident_name = argv[2];
        std::string incident_location = argv[3];
        int incident_day = std::stoi(argv[4]);
        int incident_month = std::stoi(argv[5]);
        int incident_year = std::stoi(argv[6]);
        deleteFromCSV("temporary.csv", incident_name, incident_location, incident_day, incident_month, incident_year);
        }
        catch (const std::exception& e) {
            std::cerr << "Error: " << e.what() << "\n";
            usage();
            return 1;
        }
    }
    else if (argv[1] == std::string("modify_incident"))
    {
        try 
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
        catch (const std::exception& e) {
            std::cerr << "Error: " << e.what() << "\n";
            usage();
            return 1;
        }
    }
    else if (argv[1] == std::string("view_incidents"))
    {
        readFromCSV("temporary.csv");
    }
    else {
        printColored("Invalid operation.\n", "\033[1;31m");
        usage();
    }
    return 0;
}
