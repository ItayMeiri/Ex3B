//
// Created by ischelle on 05/04/2021.
//

#include "NumberWithUnits.hpp"
#include <algorithm>
#include <fstream>
#include <iostream>
#include <string>
#include <tuple>

namespace ariel {
std::unordered_map<std::string, Unit> NumberWithUnits::units;
NumberWithUnits::NumberWithUnits(double value, std::string sign) {
  this->my_sign = sign;
  this->value = value;
}

void NumberWithUnits::read_units(std::ifstream &units_file) {
  std::string line;
  // Do this for every line
  while (std::getline(units_file, line)) {
    // 1KM=1000M // 9 - 5

    // https://stackoverflow.com/questions/83439/remove-spaces-from-stdstring-in-c
    // Since we're not getting negative values, this should be OK!
    // Remove all spaces from string
    line.erase(remove_if(line.begin(), line.end(), isspace), line.end());
    size_t left_sign_index =
        line.find_first_not_of("0123456789."); // This can only be 1.
    size_t equals_sign_index = line.find("="); // 3
    std::string sub_str =
        line.substr(equals_sign_index + 1,
                    line.length() - (equals_sign_index + 1)); //"1000M"
    size_t right_sign_index = sub_str.find_first_not_of("0123456789."); // 4

    sub_str = sub_str.substr(0,
                             right_sign_index); // 1000

    double conversion_rate_to = std::stod(sub_str);

    double conversion_rate_from = (1.0 / conversion_rate_to);

    std::string sign_left =
        line.substr(left_sign_index, equals_sign_index - left_sign_index);
    std::string sign_right =
        line.substr(equals_sign_index + 1 + right_sign_index,
                    line.size() - right_sign_index - equals_sign_index);

    std::string s = "line is: " + line;
    s += " right sign is: " + sign_right;
    s += " left sign is:" + sign_left;
    s += " converstion rate to is: " + std::to_string(conversion_rate_to);
    s += " conversion rate from is: " + std::to_string(conversion_rate_from);

    std::cout << " \n\n" + s + " \n\n";
    //    std::string sign;
    // Unit* smaller_unit;
    // Unit* bigger_unit;
    // double convert_smaller;
    // double convert_bigger;

    //1TON= 1000KM
    Unit *left = nullptr;
    Unit *right = nullptr;
    if (units.find(sign_left) == units.end()) {
      // initialize left, insert into map
      left = new Unit;
      left->sign = sign_left;
      units.insert(std::make_pair(sign_left, *left));
    }
    if (units.find(sign_right) == units.end()) {
      // initialize right, insert into map
      right = new Unit;
      right->sign = sign_right;
      units.insert(std::make_pair(sign_right, *right));
    }
    right = &units.at(sign_right);
    left = &units.at(sign_left);
    if (conversion_rate_to >
        conversion_rate_from) // left unit is bigger than right unit
    {
      left->smaller_unit = right;
      left->convert_smaller = conversion_rate_to;

      right->bigger_unit = left;
      right->convert_bigger = conversion_rate_from;
      std::cout << "\n~~~~~~~~~\n";
      std::cout << "THE UNIT ON THE LEFT: " << left->sign << std::endl;

      std::cout << "THE UNIT ON THE RIGHT: " << right->sign << std::endl;

      std::cout << "CONVERTING FROM LEFT TO RIGHT " << left->convert_smaller << std::endl;

      std::cout << "CONVERTING FROM RIGHT TO LEFT " << right->convert_bigger << std::endl;


      std::cout << "~~~~~~~~~\n";


    } else {
      // left unit is smaller(1M=0.001KM), //to 0.001, from: 1000
      left->bigger_unit = right;
      left->convert_bigger = conversion_rate_to;
      right->smaller_unit = left;
      right->convert_smaller = conversion_rate_from;

      std::cout << "\n~~~~~~~~~\n";
      std::cout << "left is : " << &left << std::endl;
      std::cout << "and the right.smaller unit is..." << right->smaller_unit
                << std::endl;
      std::cout << "~~~~~~~~~\n";
    }
    std::cout << "*************" << std::endl;

    std::cout << "At the end of the line " << line << ":";
    std::cout << "Does this sign exist? " << units.at(sign_right).sign << " end"
              << std::endl;
    std::cout << "What about a pointer to bigger unit? "
              << units.at(sign_right).bigger_unit << " end" << std::endl;
    std::cout << "What about a pointer to smaller unit? "
              << units.at(sign_right).smaller_unit << " end" << std::endl;
    std::cout << "What about the conversion to bigger? "
              << units.at(sign_right).convert_bigger << " end" << std::endl;
    std::cout << "What about the conversion to smaller? "
              << units.at(sign_right).convert_smaller << " end" << std::endl;

    std::cout << "At the end of the line " << line << ":";
    std::cout << "Does this sign exist? " << units.at(sign_left).sign << " end"
              << std::endl;
    std::cout << "What about a pointer to bigger unit? "
              << units.at(sign_left).bigger_unit << " end" << std::endl;
    std::cout << "What about a pointer to smaller unit? "
              << units.at(sign_left).smaller_unit << " end" << std::endl;
    std::cout << "What about the conversion to bigger? "
              << units.at(sign_left).convert_bigger << " end" << std::endl;
    std::cout << "What about the conversion to smaller? "
              << units.at(sign_left).convert_smaller << " end" << std::endl;

    std::cout << "*************" << std::endl;
  }
}

std::tuple<bool, double> NumberWithUnits::isValid(std::string s1,
                                                  std::string s2) {
                                                      
  std::cout << " s1 is " << s1 << std::endl;
  std::cout << " s2 is " << s2 << std::endl;
  if (s1.compare(s2) == 0) {
    return std::make_tuple(true, 1.0);
  }


  Unit u1 = units.at(s1);
  Unit u2 =
  units.at(s1); // KM s1 -> no bigger unit, KM -> M, convert_smaller = 100
  double bigger_conversion = 1;
  double smaller_conversion = 1;
  // check first one
  while (u1.bigger_unit) {
    //BIGGER EXISTS, CONVERT TO BIGGER
    bigger_conversion *= u1.convert_bigger;
    std::cout << "u1 is b4: " << u1.sign << " " << std::endl;
    u1 = units.at(u1.bigger_unit->sign);
    std::cout << "u1 is after: " << u1.sign << " " << std::endl;
    std::cout << "S2 is: " << s2 << " " << std::endl;

    std::cout << "CONVERSION IS: " << bigger_conversion << " " << std::endl;
    if (u1.sign.compare(s2) == 0) {
      std::cout << "RETURNING CONVERSION: " << bigger_conversion << " " << std::endl;

      return std::make_tuple(true, bigger_conversion);
    }
  }
  while (u2.smaller_unit) {
    smaller_conversion *= u2.convert_smaller;
    std::cout << "u2 b4: " << u2.sign << " " << std::endl;
    u2 = units.at(u2.smaller_unit->sign);
    std::cout << "u2 after: " << u2.sign << " "<< std::endl;
    std::cout << "CONVERSION IS: " << smaller_conversion << " " << std::endl;
    std::cout<<"COMPARING TO: " << s2 << std::endl;
    if (u2.sign.compare(s2) == 0) {
      std::cout << "RETURNING CONVERSION: " << smaller_conversion << " " << std::endl;
      return std::make_tuple(true, smaller_conversion);
      ;
    }
  }
  return std::make_tuple(false, 0);
}
// overload operators
std::ostream &operator<<(std::ostream &os, const NumberWithUnits &other) {
  return os << other.value << "[" + other.my_sign + "]";
}
std::istream &operator>>(std::istringstream &is, NumberWithUnits &num) {

  std::string str = is.str(); // 700 [ KM ]

  // removes spaces and brackets

  str.erase(

      std::remove_if(str.begin(), str.end(),

                     [](char c) { return c == '[' || c == ']' || c == ' '; }),

      str.end());
  size_t index_value = str.find_first_not_of("012345679.");

  num.value = (std::stod(str.substr(0, index_value)));

  num.my_sign = (str.substr(index_value, str.length() - 1));
  return is;
}

double operator+(NumberWithUnits &o1, const NumberWithUnits &o2) {

  // convert o2 to o1
  std::cout<<"Sending to isValid: " << o1.my_sign << " and " << o2.my_sign << std::endl;
  std::tuple<bool, double> tp = o1.isValid(o1.my_sign, o2.my_sign);
  bool isValid = std::get<0>(tp);
  double conversion = std::get<1>(tp);
  std::cout << "conversion from: " << o1 << " to " << o2 << "is " << conversion
            << std::endl;
  std::cout << "isValid: " << isValid;
  if (!isValid) {
    // throw error
    return 1;
  }
  return o1.value * conversion;
}

double operator-(NumberWithUnits &o1, const NumberWithUnits &o2) { return 1; }
double operator-(NumberWithUnits &o1) { return 1; }

bool operator<(const NumberWithUnits &o1, const NumberWithUnits &o2) {
  return true;
}

bool operator<=(const NumberWithUnits &o1, const NumberWithUnits &o2) {
  return true;
}

bool operator>(const NumberWithUnits &o1, const NumberWithUnits &o2) {
  return true;
}

bool operator>=(const NumberWithUnits &o1, const NumberWithUnits &o2) {
  return true;
}
double operator*(int ot, const NumberWithUnits &other) { return 1; }

bool operator==(const NumberWithUnits &o1, const NumberWithUnits &o2) {
  return true;
}

double operator+=(NumberWithUnits &o1, const NumberWithUnits &o2) { return 1; }
double operator-=(NumberWithUnits &o1, const NumberWithUnits &o2) { return 1; }

} // namespace ariel