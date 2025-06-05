#ifndef DATE_HPP
#define DATE_HPP

class Date {
public:
    Date(int day, int month, int year);

protected:
    int day;
    int month;
    int year;
    friend class Report;
};

#endif // DATE_HPP

