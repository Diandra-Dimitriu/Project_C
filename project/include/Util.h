#ifndef UTIL_H
#define UTIL_H

#include "Report.h"
#include <vector>
#include <string>

void clearScreen();
void printColored(const std::string& message, const std::string& color);
void usage();
void exportToCSV(const std::vector<Report>& reports, const std::string& filename);
void importFromCSV(std::vector<Report>& reports, const std::string& filename);

#endif
