//
// Created by ilya on 22.08.19.
//


#include <iostream>
#include <sstream>
using namespace std;

class Rational {
 public:
  Rational() {
    num = 0;
    denom = 1;
    positive = true;
  }

  Rational(int numerator, int denominator) {
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
  return Rational(lhs.Numerator() * rhs.Denominator(), lhs.Denominator() * rhs.Numerator());
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
  {
    ostringstream output;
    output << Rational(-6, 8);
    if (output.str() != "-3/4") {
      cout << "Rational(-6, 8) should be written as \"-3/4\"" << endl;
      return 1;
    }
  }

  {
    istringstream input("5/7");
    Rational r;
    input >> r;
    bool equal = r == Rational(5, 7);
    if (!equal) {
      cout << "5/7 is incorrectly read as " << r << endl;
      return 2;
    }
  }

  {
    istringstream input("5/7 10/8");
    Rational r1, r2;
    input >> r1 >> r2;
    bool correct = r1 == Rational(5, 7) && r2 == Rational(5, 4);
    if (!correct) {
      cout << "Multiple values are read incorrectly: " << r1 << " " << r2 << endl;
      return 3;
    }

    input >> r1;
    input >> r2;
    correct = r1 == Rational(5, 7) && r2 == Rational(5, 4);
    if (!correct) {
      cout << "Read from empty stream shouldn't change arguments: " << r1 << " " << r2 << endl;
      return 4;
    }


  }

  cout << "OK" << endl;
  return 0;
}