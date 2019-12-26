//
// Created by ilya on 23.08.19.
//

#include <iostream>
#include <sstream>
#include <stdexcept>

using namespace std;

class Rational {
 public:
  Rational() {
    num = 0;
    denom = 1;
    positive = true;
  }

  Rational(int numerator, int denominator) {
    if (denominator == 0) {
      std::stringstream ss;
      ss << "Argument must not be zero";
      throw invalid_argument(ss.str());
    }
    ToStandartForm(numerator, denominator);
  }

  int Numerator() const {
    return positive ? num : -num;
  }

  int Denominator() const {
    return denom;
  }

 private:

  void ToStandartForm(int numerator, int denominator) {
    int gcd = GCD(numerator, denominator);
    numerator /= gcd;
    denominator /= gcd;
    ((double(numerator) / denominator) < 0) ? positive = false : positive = true;
    num = abs(numerator);
    denom = abs(denominator);
  }

  int GCD(int first, int second) {
    while (second) {
      first %= second;
      int buff = first;
      first = second;
      second = buff;
    }
    return first;
  }

  int num;
  int denom;
  bool positive;
};

bool operator==(const Rational& lhs, const Rational& rhs) {
  double lhs_res = double(lhs.Numerator())/lhs.Denominator();
  double rhs_res = double(rhs.Numerator())/rhs.Denominator();
  return lhs_res == rhs_res;
}

Rational operator+(const Rational& lhs, const Rational& rhs) {
  int new_denom = lhs.Denominator() * rhs.Denominator();
  return Rational(lhs.Numerator()*rhs.Denominator() + rhs.Numerator()*lhs.Denominator(), new_denom);
}

Rational operator-(const Rational& lhs, const Rational& rhs) {
  int new_denom = lhs.Denominator() * rhs.Denominator();
  return Rational(lhs.Numerator()*rhs.Denominator() - rhs.Numerator()*lhs.Denominator(), new_denom);
}

Rational operator*(const Rational& lhs, const Rational& rhs) {
  return Rational(lhs.Numerator() * rhs.Numerator(), lhs.Denominator() * rhs.Denominator());
}

Rational operator/(const Rational& lhs, const Rational& rhs) {
  int new_num = lhs.Numerator() * rhs.Denominator();
  int new_denom = lhs.Denominator() * rhs.Numerator();
  if (new_denom == 0) {
    std::stringstream ss;
    ss << "New denominator is zero";
    throw domain_error(ss.str());
  }
  return Rational(new_num, new_denom);
}

ostream& operator<<(ostream& stream, const Rational& rational) {
  stream << rational.Numerator() << '/' << rational.Denominator();
  return stream;
}

istream& operator>>(istream& stream, Rational& rational) {
  int num = rational.Numerator(), denom = rational.Denominator();
  stream >> num;
  stream.ignore(1);
  stream >> denom;
  rational = Rational(num, denom);
  return stream;
}


int main() {
  try {
    Rational r(1, 0);
    cout << "Doesn't throw in case of zero denominator" << endl;
    return 1;
  } catch (invalid_argument&) {
  }

  try {
    auto x = Rational(1, 2) / Rational(0, 1);
    cout << "Doesn't throw in case of division by zero" << endl;
    return 2;
  } catch (domain_error&) {
  }

  cout << "OK" << endl;
  return 0;
}