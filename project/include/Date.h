#ifndef DATE_H
#define DATE_H

class Report; // Forward declaration

class Date {
private:
    int day, month, year;
public:
    Date(int d, int m, int y);
    friend class Report;
};

#endif
