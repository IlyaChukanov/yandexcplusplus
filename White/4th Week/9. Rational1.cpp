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
    int gcd = GCD(numerator, denominator);
    numerator /= gcd;
    denominator /= gcd;
    ((double(numerator) / denominator) < 0) ? positive = false : positive = true;
    num = abs(numerator);
    denom = abs(denominator);

  }

  int Numerator() const {
    return positive ? num : -num;
  }

  int Denominator() const {
    return denom;
  }

 private:

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

int main() {
  {
    const Rational r(3, 10);
    if (r.Numerator() != 3 || r.Denominator() != 10) {
      cout << "Rational(3, 10) != 3/10" << endl;
      return 1;
    }
  }

  {
    const Rational r(8, 12);
    if (r.Numerator() != 2 || r.Denominator() != 3) {
      cout << "Rational(8, 12) != 2/3" << endl;
      return 2;
    }
  }

  {
    const Rational r(-4, 6);
    if (r.Numerator() != -2 || r.Denominator() != 3) {
      cout << "Rational(-4, 6) != -2/3" << endl;
      return 3;
    }
  }

  {
    const Rational r(4, -6);
    if (r.Numerator() != -2 || r.Denominator() != 3) {
      cout << "Rational(4, -6) != -2/3" << endl;
      return 3;
    }
  }

  {
    const Rational r(0, 15);
    if (r.Numerator() != 0 || r.Denominator() != 1) {
      cout << "Rational(0, 15) != 0/1" << endl;
      return 4;
    }
  }

  {
    const Rational defaultConstructed;
    if (defaultConstructed.Numerator() != 0 || defaultConstructed.Denominator() != 1) {
      cout << "Rational() != 0/1" << endl;
      return 5;
    }
  }

  cout << "OK" << endl;
  return 0;
}