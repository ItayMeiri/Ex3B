//
//
#pragma once
#include <iostream>
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <unordered_map>
#include <tuple>

namespace ariel {
        struct Unit{
        std::string sign;
        Unit* smaller_unit = nullptr;
        Unit* bigger_unit = nullptr;
        double convert_smaller;
        double convert_bigger;
    };
    class NumberWithUnits {
        //1KM = 1000M
        //1CM = 100MM
        //1M = 100CM
    private:

    public:
        double value;
        std::string my_sign;
        static std::unordered_map <std::string, Unit> units;
        NumberWithUnits(double value, std::string sign);
        static void read_units(std::ifstream &units_file);
        std::tuple<bool, double> isValid(std::string s1, std::string s2);
        friend std::ostream& operator<< (std::ostream &out, const NumberWithUnits &num);
        friend std::istream& operator>>(std::istringstream &is,NumberWithUnits &num);

        friend double operator+( NumberWithUnits &o1, const NumberWithUnits &o2);
        friend double operator-( NumberWithUnits &o1, const NumberWithUnits &o2);
        friend double operator-( NumberWithUnits &o1);

        friend bool operator<(const NumberWithUnits &o1, const NumberWithUnits &o2);
        friend bool operator<=(const NumberWithUnits &o1, const NumberWithUnits &o2);
        friend bool operator>(const NumberWithUnits &o1, const NumberWithUnits &o2);
        friend bool operator>=(const NumberWithUnits &o1, const NumberWithUnits &o2);

        friend double operator*( int ot, const NumberWithUnits &other);
        friend bool operator==(const NumberWithUnits &o1, const NumberWithUnits &o2);
        friend double operator+=( NumberWithUnits &o1, const NumberWithUnits &o2);
        friend double operator-=( NumberWithUnits &o1, const NumberWithUnits &o2);

        NumberWithUnits& operator++();
        NumberWithUnits operator++(int);
        NumberWithUnits& operator--();
        NumberWithUnits operator--(int);


    };


}

