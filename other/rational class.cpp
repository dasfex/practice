#include <iostream>
#include <cmath>
#include <cassert>
#include <iomanip>

using namespace std;

class Rational {
 public:

  Rational(const int numerator = 0,
           const int denominator = 1) :
      numerator_{numerator},
      denominator_{denominator} {
    Normalize();
  }

  void Normalize() {
    int divide = Gcd(abs(numerator_), abs(denominator_));
    numerator_ /= divide;
    denominator_ /= divide;
  }

  double GetDoubleInterpretation() const {
    return double(numerator_) / denominator_;
  }

  void Print() const {
    cout << "Value is " << numerator_ << "/" << denominator_;
  }

  string RationalToString() const {
    string rational_string = to_string(numerator_) + "/" +
        to_string(denominator_);
    return rational_string;
  }

  Rational operator+() const {
    return *this;
  }

  Rational operator-() const {
    return {-1 * numerator_, denominator_};
  }

  Rational &operator=(const Rational &num) = default;

  Rational &operator=(int num) {
    Rational tmp(num);
    *this = tmp;
  }

  Rational operator+(const Rational &num) const {
    Rational tmp = *this;
    tmp.denominator_ = Lcm(denominator_, num.denominator_);
    tmp.numerator_ = numerator_ * (tmp.denominator_ / denominator_) +
        num.numerator_ * (tmp.denominator_ / num.denominator_);
    tmp.Normalize();
    return tmp;
  }

  Rational operator+(int num) const {
    Rational tmp(num);
    return *this + tmp;
  }

  Rational operator-(const Rational &num) const {
    return (*this + (-num));
  }

  Rational operator-(int num) const {
    Rational tmp(num);
    return *this - tmp;
  }

  Rational operator*(const Rational &num) const {
    return {numerator_ * num.numerator_, denominator_ * num.denominator_};
  }

  Rational operator*(int num) const {
    return {numerator_ * num, denominator_};
  }

  Rational operator/(const Rational &num) const {
    return {numerator_ * num.denominator_, denominator_ * num.numerator_};
  }

  Rational operator/(int num) const {
    return {numerator_, denominator_ * num};
  }

  bool operator==(const Rational &num) const {
    return numerator_ == num.numerator_ &&
        denominator_ == num.denominator_;
  }

  bool operator!=(const Rational &num) const {
    return !(*this == num);
  }

  //Âìåñòî èñïîëüçîâàíèÿ double ïðèâîæó ê îáùåìó çíàìåíàòåëþ.
  //Òî÷íî íå âîçíèêíåò ïðîáëåì èç-çà âîçìîæíîé ïîòåðè òî÷íîñòè.
  bool operator<(const Rational &num) const {
    return numerator_ * num.denominator_ < num.numerator_ * denominator_;
  }

  bool operator<=(const Rational &num) const {
    return (*this < num || *this == num);
  }

  bool operator>=(const Rational &num) const {
    return !(*this < num);
  }

  bool operator>(const Rational &num) const {
    return !(*this <= num);
  }

  friend Rational operator+(int num, const Rational &tmp);
  friend Rational operator-(int num, const Rational &tmp);
  friend Rational operator*(int num, const Rational &tmp);
  friend Rational operator/(int num, const Rational &tmp);
  friend istream &operator>>(istream &in, Rational &number);
  friend ostream &operator<<(ostream &out, const Rational &number);

 private:
  int numerator_;
  int denominator_;

  int Gcd(const int first, const int second) const {
    if (second != 0) {
      return Gcd(second, first % second);
    }
    return first;
  }

  int Lcm(const int first, const int second) const {
    return (first * second / Gcd(first, second));
  }

  void Reverse() {
    swap(this->numerator_, this->denominator_);
    if (denominator_ < 0) {
      numerator_ *= -1;
      denominator_ *= -1;
    }
  }
};

Rational operator+(int num, const Rational &tmp) {
  return tmp + num;
}

Rational operator-(int num, const Rational &tmp) {
  return -(tmp - num);
}

Rational operator*(int num, const Rational &tmp) {
  return (tmp * num);
}

Rational operator/(int num, const Rational &tmp) {
  Rational number(num);
  Rational other_tmp = tmp;
  other_tmp.Reverse();
  return number * other_tmp;
}

istream &operator>>(istream &in, Rational &number) {
  char div;
  in >> number.numerator_ >> div >> number.denominator_;
  number.Normalize();
  return in;
}

ostream &operator<<(ostream &out, const Rational &number) {
  if (number.numerator_ == 0) {
    out << 0;
    return out;
  }
  if (number.denominator_ == 1) {
    out << number.numerator_;
    return out;
  }
  out << number.numerator_ << "/" << number.denominator_;
  return out;
}

#ifndef IGNORE_MAIN
int main() {
  {
      Rational first(1, 3), second(2, 5);
      first = first + second;
      assert(first.RationalToString() == "11/15");
      first = first - second;
      assert(first.RationalToString() == "1/3");
      first = first * second;
      assert(first.RationalToString() == "2/15");
      first = first / second;
      assert(first.RationalToString() == "1/3");
      cout << "Operators for rational test passed." << endl;
  }
  {
    Rational first;
    assert(first.RationalToString() == "0/1");
    first = first + 1;
    assert(first.RationalToString() == "1/1");
    first = first - 1;
    assert(first.RationalToString() == "0/1");
    first = first + 1;
    first = first * 2;
    assert(first.RationalToString() == "2/1");
    Rational second(2, 7);
    assert(second.RationalToString() == "2/7");
    second = second + 1;
    assert(second.RationalToString() == "9/7");
    second = second - 1;
    assert(second.RationalToString() == "2/7");
    second = second + 1;
    second = second * 2;
    assert(second.RationalToString() == "18/7");
    cout << "Operators for int tests passed." << endl;
  }
  {
    Rational first(2);
    first = first / 2;
    assert(first.RationalToString() == "1/1");
    first = 2 * first;
    assert(first.RationalToString() == "2/1");
    first = 1 + first;
    assert(first.RationalToString() == "3/1");
    first = 1 - first;
    assert(first.RationalToString() == "-2/1");
    first = 4 / first;
    assert(first.RationalToString() == "-2/1");
    cout << "Reverse operators tests passed." << endl;
  }
  {
    const Rational first(2);
    Rational second(1, 3);
    assert((first < second) == false);
    assert((first <= second) == false);
    assert((first > second) == true);
    assert((first >= second) == true);
    second = first;
    assert((first == second) == true);
    cout << "Bool operators tests passed." << endl;
  }
  {
    cout << endl;
    cout << "The beginning of the \"cout\" block." << endl;
    const Rational first(2), second(0, 1), third(13, 25);
    cout << '\t' << first << endl;
    cout << '\t' << second << endl;
    cout << '\t' << third << endl;
    cout << "The end of the \"cout\" block." << endl;
  }
  {
    cout << endl;
    cout << "Other functions:" << endl;
    cout << "Get double interpretation of number(1/2, for example): ";
    const Rational q(1, 2);
    cout << fixed << setprecision(6) << q.GetDoubleInterpretation() << endl;
    cout << "Function \"Print()\": ";
    q.Print();
    cout << endl;
    cout << "Also you can use \"name.RationalToString()\" and "
            "\"Normalize().\"" << endl;
  }
  cout << "Rational tests passed :)" << endl;

  return 0;
}
#endif
