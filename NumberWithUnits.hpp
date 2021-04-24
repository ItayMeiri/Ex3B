#pragma once
#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <stdexcept>
#include <unordered_map>
#include <boost/graph/graph_traits.hpp>

namespace ariel {
        struct Unit{
        std::string sign;
        // std::shared_ptr<Unit> smaller_unit = nullptr;
        // std::shared_ptr<Unit> bigger_unit = nullptr;
        Unit* smaller_unit = nullptr;
        Unit* bigger_unit = nullptr;
        double convert_smaller;
        double convert_bigger;
    };
    class NumberWithUnits {
    protected:

    public:
        double value;
        std::string number_sign;
        static std::unordered_map <std::string, Unit> units;
        NumberWithUnits(double value, std::string sign);
        static void read_units(std::ifstream &units_file);

        static double validateConversion(const std::string& s1, const std::string& s2);
        friend std::ostream& operator<< (std::ostream &out, const NumberWithUnits &num);
        friend std::istream& operator>>(std::istringstream &is,NumberWithUnits &num);

        friend NumberWithUnits operator+( const NumberWithUnits &o1, const NumberWithUnits &o2);
        friend NumberWithUnits operator+( const NumberWithUnits &o1);

        friend NumberWithUnits operator-( const NumberWithUnits &o1, const NumberWithUnits &o2);
        friend NumberWithUnits operator-(const NumberWithUnits &o1);

        friend bool operator<( const NumberWithUnits &o1, const NumberWithUnits &o2);
        friend bool operator<=( const NumberWithUnits &o1, const NumberWithUnits &o2);
        friend bool operator>( const NumberWithUnits &o1, const NumberWithUnits &o2);
        friend bool operator>=( const NumberWithUnits &o1, const NumberWithUnits &o2);

        friend NumberWithUnits operator*(double ot, const NumberWithUnits &other);
        friend NumberWithUnits operator*(const NumberWithUnits &other, double ot);

        friend bool operator==( const NumberWithUnits &o1, const NumberWithUnits &o2);
        friend bool operator!=( const NumberWithUnits &o1, const NumberWithUnits &o2);

        friend NumberWithUnits operator+=( NumberWithUnits &o1, const NumberWithUnits &o2);
        friend NumberWithUnits operator-=( NumberWithUnits &o1, const NumberWithUnits &o2);

        NumberWithUnits& operator++();
        NumberWithUnits operator++(int);
        NumberWithUnits& operator--();
        NumberWithUnits operator--(int);
    };


}

