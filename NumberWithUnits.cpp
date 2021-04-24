//
// Created by ischelle on 05/04/2021.
//
#include "NumberWithUnits.hpp"
#include <cmath>

namespace ariel {
std::unordered_map<std::string, Unit> NumberWithUnits::units;
NumberWithUnits::NumberWithUnits(double value, std::string sign) {
  if(units.find(sign) == units.end())
  {
    throw std::invalid_argument("Constructor error");
  }
  this->number_sign = std::move(sign);
  this->value = value;
}

void NumberWithUnits::read_units(std::ifstream &units_file) {
  std::string line;
  while (std::getline(units_file, line)) {
    // https://stackoverflow.com/questions/83439/remove-spaces-from-stdstring-in-c
    // Since we're not getting negative values, this should be OK!
    // Remove all spaces from string
    line.erase(remove_if(line.begin(), line.end(), isspace), line.end());
    size_t left_sign_index = line.find_first_not_of("0123456789.");
    size_t equals_sign_index = line.find('=');
    std::string sub_str = line.substr(equals_sign_index + 1,
                                      line.length() - (equals_sign_index + 1));
    size_t right_sign_index = sub_str.find_first_not_of("0123456789.");

    sub_str = sub_str.substr(0, right_sign_index);

    double conversion_rate_to = std::stod(sub_str);

    double conversion_rate_from = (1.0 / conversion_rate_to);

    std::string sign_left =
        line.substr(left_sign_index, equals_sign_index - left_sign_index);
    std::string sign_right =
        line.substr(equals_sign_index + 1 + right_sign_index,
                    line.size() - right_sign_index - equals_sign_index);

    Unit *left = nullptr;
    Unit *right = nullptr;
    if (units.find(sign_left) == units.end()) {
      // initialize left, insert into map
      units.insert(std::make_pair(sign_left, Unit{sign_left}));
    }
    if (units.find(sign_right) == units.end()) {
      // initialize right, insert into map
      units.insert(std::make_pair(sign_right, Unit{sign_right}));
    }
    right = &units.at(sign_right);
    left = &units.at(sign_left);
    if (conversion_rate_to > conversion_rate_from) {
      // left->smaller_unit = std::shared_ptr<Unit>(right);
      left->smaller_unit = right;
      left->convert_smaller = conversion_rate_to;

      // right->bigger_unit = std::shared_ptr<Unit>(left);
      right->bigger_unit = left;
      right->convert_bigger = conversion_rate_from;


    } else {
      // left->bigger_unit = std::shared_ptr<Unit>(right);
      left->bigger_unit = right;
      left->convert_bigger = conversion_rate_to;
      // right->smaller_unit = std::shared_ptr<Unit>(left);
      right->smaller_unit = left;
      right->convert_smaller = conversion_rate_from;
    }
  }
}

// bool nearly_equal(double a1, double a2, double epsilon)
// {
//   if (a1 == 0 && a2 == 0)
//     return true;

//   return std::abs(a1 - a2) < epsilon * pow (2.0, static_cast<int> (std::log2(std::max(std::abs(a1), std::abs(a2)))));
// }

double NumberWithUnits::validateConversion(const std::string& s1, const std::string& s2) {
  if (s1 == s2) {
    return 1.0;
  }
  Unit u1 = units.at(s1);
  Unit u2 = units.at(s1);
  double bigger_conversion = 1;
  double smaller_conversion = 1;
  while (u1.bigger_unit != nullptr) {
    bigger_conversion *= u1.convert_bigger;
    u1 = units.at(u1.bigger_unit->sign);

    if (u1.sign==s2) {
      return bigger_conversion;
    }
  }
  while (u2.smaller_unit != nullptr) {
    smaller_conversion *= u2.convert_smaller;
    u2 = units.at(u2.smaller_unit->sign);

    if (u2.sign==s2) {
      return smaller_conversion;
    }
  }
  throw std::invalid_argument("Don't match");
}
// overload operators
std::ostream &operator<<(std::ostream &out, const NumberWithUnits &num) {
  return out << num.value << "[" + num.number_sign + "]";
}
std::istream &operator>>(std::istringstream &is,NumberWithUnits &num) {
  std::string str = is.str();
  // removes spaces and brackets
  str.erase(
      std::remove_if(str.begin(), str.end(),

                     [](char c) { return c == '[' || c == ']' || c == ' '; }),

      str.end());
  size_t index_value = str.find_first_not_of("012345679.");
  std::string sn = (str.substr(index_value, str.length() - 1));
  num.value = (std::stod(str.substr(0, index_value)));
  if(NumberWithUnits::units.find(sn) == NumberWithUnits::units.end())
  {
    throw std::invalid_argument("istream error");
  }

  num.number_sign = (str.substr(index_value, str.length() - 1));
  return is;
}

NumberWithUnits operator+(const NumberWithUnits &o1, const NumberWithUnits &o2) {
  double conversion = NumberWithUnits::validateConversion(o2.number_sign, o1.number_sign);
  return NumberWithUnits{o1.value + o2.value*conversion, o1.number_sign};
}
NumberWithUnits operator+(const NumberWithUnits &o1) {
  return o1;
}
NumberWithUnits operator-(const NumberWithUnits &o1, const NumberWithUnits &o2) {
  double conversion = NumberWithUnits::validateConversion(o2.number_sign, o1.number_sign);
  return NumberWithUnits{o1.value - o2.value * conversion, o1.number_sign};
}
NumberWithUnits operator-(const NumberWithUnits &o1) {
  return NumberWithUnits{(-1.0) * o1.value, o1.number_sign};
}

bool operator<(const NumberWithUnits &o1, const NumberWithUnits &o2) {
  double conversion = NumberWithUnits::validateConversion(o2.number_sign, o1.number_sign);
  if(o1 == o2) { return false;}
  return o1.value < o2.value * conversion;}

bool operator<=(const NumberWithUnits &o1, const NumberWithUnits &o2) {
  double conversion = NumberWithUnits::validateConversion(o2.number_sign, o1.number_sign);
  return o1 < o2 || o1 == o2;
}

bool operator>(const NumberWithUnits &o1, const NumberWithUnits &o2) {
  double conversion = NumberWithUnits::validateConversion(o2.number_sign, o1.number_sign);
  if(o1 == o2) { return false;}
  return o1.value > o2.value * conversion;
}

bool operator>=(const NumberWithUnits &o1, const NumberWithUnits &o2) {
  double conversion = NumberWithUnits::validateConversion(o2.number_sign, o1.number_sign);
  
  return o1 > o2 || o1 == o2;
}
NumberWithUnits operator*(double ot, const NumberWithUnits &other) {
  return NumberWithUnits{other.value*ot, other.number_sign};
}
NumberWithUnits operator*(const NumberWithUnits &other, double ot) {
  return NumberWithUnits{other.value*ot, other.number_sign};
}


bool operator==(const NumberWithUnits &o1, const NumberWithUnits &o2) {
  //check if they are straight up equal
  double conversion = NumberWithUnits::validateConversion(o2.number_sign, o1.number_sign);
  double a = o1.value;
  double b = o2.value * conversion;
  const double epsilon = 0.0001;
  return std::abs(a-b) < epsilon;
}

bool operator!=(const NumberWithUnits &o1, const NumberWithUnits &o2) {
  return !(o1 == o2);
}

NumberWithUnits operator+=(NumberWithUnits &o1, const NumberWithUnits &o2) {
  double conversion = NumberWithUnits::validateConversion(o2.number_sign, o1.number_sign);
  o1.value += o2.value*conversion;
  return o1;
}
NumberWithUnits operator-=(NumberWithUnits &o1, const NumberWithUnits &o2) {
  double conversion = NumberWithUnits::validateConversion(o2.number_sign, o1.number_sign);
  o1.value -= o2.value*conversion;
  return o1;
}
NumberWithUnits& NumberWithUnits::operator++()
{
  value++;
  return *this;
}

NumberWithUnits NumberWithUnits::operator++(int)
{
  NumberWithUnits temp = *this;
  ++*this;
  return temp;}

NumberWithUnits& NumberWithUnits::operator--()
{
  value--;
  return *this;
}

NumberWithUnits NumberWithUnits::operator--(int)
{
  NumberWithUnits temp = *this;
  --*this;
  return temp;
}
} // namespace ariel