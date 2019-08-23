//
// Created by ilya on 22.08.19.
//

#include <iostream>
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

int main() {
  {
    Rational r1(4, 6);
    Rational r2(2, 3);
    bool equal = r1 == r2;
    if (!equal) {
      cout << "4/6 != 2/3" << endl;
      return 1;
    }
  }

  {
    Rational a(2, 3);
    Rational b(4, 3);
    Rational c = a + b;
    bool equal = c == Rational(2, 1);
    if (!equal) {
      cout << "2/3 + 4/3 != 2" << endl;
      return 2;
    }
  }

  {
    Rational a(5, 7);
    Rational b(2, 9);
    Rational c = a - b;
    bool equal = c == Rational(31, 63);
    if (!equal) {
      cout << "5/7 - 2/9 != 31/63" << endl;
      return 3;
    }
  }

  cout << "OK" << endl;
  return 0;
}
