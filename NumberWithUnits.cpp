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
    size_t left_sign_index = line.find_first_not_of(
        "0123456789."); // This can only be 1.
    size_t equals_sign_index = line.find("="); // 3
    std::string sub_str =
        line.substr(equals_sign_index + 1,line.length() - (equals_sign_index + 1)); //"1000M"
    size_t right_sign_index = sub_str.find_first_not_of("0123456789."); // 4

    sub_str =
        sub_str.substr(0,
                       right_sign_index); // 1000

    double conversion_rate_to =
        std::stod(sub_str); 

    double conversion_rate_from =
        (1.0 / conversion_rate_to);

    std::string sign_left =
        line.substr(left_sign_index, equals_sign_index - left_sign_index);
    std::string sign_right =
        line.substr(equals_sign_index + 1 + right_sign_index, line.size()- right_sign_index - equals_sign_index);

        std::string s = "line is: " + line;
        s+= " right sign is: " + sign_right;
        s+= " left sign is:" + sign_left;
        s+= " converstion rate to is: " + std::to_string(conversion_rate_to);
        s+= " conversion rate from is: " + std::to_string(conversion_rate_from);

        std::cout<<" \n\n" + s + " \n\n";
        //    std::string sign;
    // Unit* smaller_unit; 
    // Unit* bigger_unit;
    // double convert_smaller;
    // double convert_bigger;

    Unit right, left;
    if(units.find(sign_left) == units.end()){
      //initialize left, insert into map
      left.sign = sign_left;
      units.insert(std::make_pair(sign_left, left));
    }
   if(units.find(sign_right) == units.end()){
      //initialize right, insert into map
      right.sign = sign_right;
      units.insert(std::make_pair(sign_right, right));
    }
    right = units.at(sign_right);
    left = units.at(sign_left);
    if(conversion_rate_to > conversion_rate_from) // left unit is bigger than right unit
    {
      left.smaller_unit = &right;
      right.convert_smaller = conversion_rate_to;

      right.bigger_unit = &left;
      right.convert_bigger = conversion_rate_from;
    }
    else{
      //left unit is smaller(1M=0.001KM)
      left.bigger_unit = &right;
      left.convert_bigger = conversion_rate_to;

      right.smaller_unit = &left;
      right.convert_smaller = conversion_rate_from; 
    }
  }
}

std::tuple<bool, double> NumberWithUnits::isValid(std::string s1, std::string s2)
{
  Unit u1 = units.at(s1);
  Unit u2 = units.at(s1); // KM s1 -> no bigger unit, KM -> M, convert_smaller = 100
  double bigger_conversion = 1;
  double smaller_conversion = 1;
  //check first one
  while(u1.bigger_unit)
  {
    u1 = *u1.bigger_unit;
    bigger_conversion*=u1.convert_bigger;

    if(u1.sign == s2)
    {
      return std::make_tuple(true, bigger_conversion);
    }
  }
  while(u2.smaller_unit)
  {
    u2 = *u2.smaller_unit;
    smaller_conversion*=u2.convert_smaller;
    if(u2.sign == s2)
    {
      return std::make_tuple(true, smaller_conversion);;
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

  num.value=(std::stod(str.substr(0, index_value)));

  num.my_sign=(str.substr(index_value,str.length() -1 ));
  return is;
}

int operator+(NumberWithUnits &o1, const NumberWithUnits &o2) {
  std::tuple<bool, int> tp = o1.isValid(o1.my_sign, o2.my_sign);
  if(std::get<0>(tp))
  {
    //throw error
  }
  
  return 1;
}

int operator-( NumberWithUnits &o1, const NumberWithUnits &o2) {
  return 1;
}
int operator-( NumberWithUnits &o1) { return 1; }

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
int operator*( int ot, const NumberWithUnits &other) { return 1; }

bool operator==(const NumberWithUnits &o1, const NumberWithUnits &o2) {
  return true;
}

int operator+=( NumberWithUnits &o1, const NumberWithUnits &o2) {
  return 1;
}
int operator-=( NumberWithUnits &o1, const NumberWithUnits &o2) {
  return 1;
}

} // namespace ariel