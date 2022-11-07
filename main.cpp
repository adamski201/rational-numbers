#include <iostream>
#include <cmath>
#include <iomanip>
#include <vector>
using namespace std;

long long int gcf(long long int a, long long int b);

class Fraction
{
    /*
        A class to represent rational numbers, as a fraction. This aims to 
        improve precision compared to using doubles.
    */
    public:
    
        Fraction()
        {
            // Default constructor returns 1/1
            numerator_ = 1;
            denominator_ = 1;
        }

        Fraction(long long int a, long long int b)
        {
            // Throws an error if user attempts to create fraction with zero as a denominator
            if (b == 0)
            {
                throw invalid_argument("Division by zero error");
            }

            // Standardises the fraction so that -a/-b -> a/b, and a/-b -> -a/b
            if ((a<0 && b<0) || (b<0 && a>0))
            {
                a *= -1;
                b *= -1;
            }

            long long int factor = gcf(a, b);
            numerator_ = a / factor;
            denominator_ = b / factor;

        }

        Fraction operator+(const Fraction& other)
        {
            // Uses formula: a/b + c/d = ad+bc/bd
            long long int newNum = this->numerator_ * other.denominator_ + other.numerator_ * this->denominator_;
            long long int newDenom = this->denominator_ * other.denominator_;
            Fraction newFract = Fraction(newNum, newDenom);

            return newFract.reduce();
        }

        Fraction operator-(const Fraction& other)
        {
            // Uses formula: a/b - c/d = ad-bc/bd
            long long int newNum = this->numerator_ * other.denominator_ - other.numerator_ * this->denominator_;
            long long int newDenom = this->denominator_ * other.denominator_;
            Fraction newFract = Fraction(newNum, newDenom);

            return newFract.reduce();
        }

        Fraction operator*(const Fraction& other)
        {
            // Uses formula: a/b * c/d = a*c/b*d
            long long int newNum = this->numerator_ * other.numerator_;
            long long int newDenom = this->denominator_ * other.denominator_;
            Fraction newFract = Fraction(newNum, newDenom);

            return newFract.reduce();
        }

        Fraction operator/(const Fraction& other)
        {
            // Uses formula: a/b / c/d = a*d/b*c
            long long int newNum = this->numerator_ * other.denominator_;
            long long int newDenom = this->denominator_ * other.numerator_;
            Fraction newFract = Fraction(newNum, newDenom);
            return newFract.reduce();
        }

        void operator+=(const Fraction& other)
        {
            *this = *this + other;
            this->reduce();
        }

        void operator-=(const Fraction& other)
        {
            *this = *this - other;
            this->reduce();
        }

        bool operator<(const Fraction& other) const
        {
            // For a standardised fraction: a/b < c/d when a*d < b*c
            return (this->numerator_*other.denominator_ < other.numerator_*this->denominator_);
        }

        bool operator<=(const Fraction& other) const
        {
            // For a standardised fraction: a/b <= c/d when a*d <= b*c
            return (this->numerator_*other.denominator_ <= other.numerator_*this->denominator_);
        }

        bool operator>(const Fraction& other) const
        {
            // For a standardised fraction: a/b > c/d when a*d > b*c
            return (this->numerator_*other.denominator_ > other.numerator_*this->denominator_);
        }

        bool operator>=(const Fraction& other) const
        {
            // For a standardised fraction: a/b >= c/d when a*d >= b*c
            return (this->numerator_*other.denominator_ >= other.numerator_*this->denominator_);
        }

        void operator*=(const Fraction& other)
        {
            *this = *this * other;
            this->reduce();
        }

        void operator/=(const Fraction& other)
        {
            *this = *this / other;
            this->reduce();
        }

        bool operator==(const Fraction& other)
        {
            return (this->numerator_ == other.numerator_ && this->denominator_ == other.denominator_);
        }

        bool differentSign(const Fraction& other);
        void output(ostream& out) const;
        Fraction abs();

    private:
        long long int numerator_;
        long long int denominator_;

        long long int gcf(long long int a, long long int b);
        Fraction reduce();

};

long long int Fraction::gcf(long long int a, long long int b)
{
    // Recursive function that determines the greatest common factor.

    //Performs integer division. Completes once the result of integer division is zero.
    long long int c = a % b;
    if (c == 0)
    {
        return b;
    }

    return gcf(b, c);
}

Fraction Fraction::reduce()
{
    // Checks that the denominator is zero - can result from use of division operators.
    if (this->denominator_ == 0)
            {
                throw invalid_argument("Division by zero error");
            }

    // Reduces fraction to its lowest equivalent form.
    long long int factor = gcf(numerator_, denominator_);
    Fraction newFract = Fraction(numerator_ / factor, denominator_ / factor);
    
    // Standardises fraction
    if ((newFract.denominator_ < 0 && newFract.numerator_ > 0) ||
    (newFract.denominator_ < 0 && newFract.numerator_ < 0))
    {
        newFract.denominator_ *= -1;
        newFract.numerator_ *= -1;
    }

    return newFract;
}

bool Fraction::differentSign(const Fraction& other)
{
    // Returns true if the fractions have different signs
    return signbit(this->numerator_) != signbit(other.numerator_);
}

void Fraction::output(ostream& out) const
{
    // N/1 will just display as N
    if (this->denominator_ == 1)
    {
        out << numerator_ << endl;
    } else out << numerator_ << "/" << denominator_ << endl;
}

Fraction Fraction::abs()
{
    // Returns the absolute value of the fraction
    if (this->numerator_ < 0) 
    {
        return Fraction(this->numerator_ * -1, denominator_);
    } else return *this;
    
}

ostream& operator<<(ostream& out, const Fraction& fract)
{
    fract.output(out);
    return out;
}

Fraction myQuadratic(Fraction x)
{
    // Quadratic: x^2 - 2
    Fraction twoAsFrac(2,1); // 2 is represented as a fraction
    return x * x - twoAsFrac;
}

bool rootExists(Fraction (*f)(Fraction), Fraction lower, Fraction upper)
{
    Fraction fLower = f(lower);
    Fraction fUpper = f(upper);
    return fLower.differentSign(fUpper);
}


Fraction findKnownRoot(Fraction (*f)(Fraction), Fraction lower, Fraction upper, vector<Fraction>& guesses, Fraction tolerance)
{
    guesses.clear();  // No function evaluations have been performed yet.

    // First check to see whether 'lower' or 'upper' are already roots of the equation.
    Fraction fLower = f(lower);
    if (fLower.abs() <= tolerance)
    {
        return lower;
    }

    Fraction fUpper = f(upper);
    if (fUpper.abs() <= tolerance)
    {
        return upper;
    }

    // Iteratively find the new guess, and the value of the function there.
    // Check if that is a root and, if not, decide whether it should become the
    // new upper, or lower bound.
    Fraction guess = (lower * fUpper - upper * fLower) / (fUpper - fLower); 
    Fraction fGuess = f(guess);
    guesses.push_back(guess);
    while (fGuess.abs() > tolerance)
    {
        if (fGuess.differentSign(fLower))
        {
            upper = guess;
            fUpper = fGuess;
        }
        else
        {
            lower = guess;
            fLower = fGuess;
        }

        guess = (lower * fUpper - upper * fLower) / (fUpper - fLower);
        fGuess = f(guess);
        guesses.push_back(guess);
    }

    return guess;
}


Fraction findRoot(Fraction (*f)(Fraction), Fraction lower, Fraction upper, vector<Fraction>& guesses, Fraction tolerance = Fraction(1,100000000))
{
    if (rootExists(f, lower, upper))
    {
        return findKnownRoot(f, lower, upper, guesses, tolerance);
    }

    cout << "f(lower) and f(upper) have the same sign. Root might not exist in [lower, upper]." << endl;
    exit(0);
}

int main()
{
    vector<Fraction> guesses;
    
    // Representing the search bounds as fractions
    Fraction lower(0,1);
    Fraction upper(2,1);
    cout << "Root of x^2 - 2 is " << findRoot(myQuadratic, lower, upper, guesses) << "." << endl;
    cout << "Intermediate guesses:" << endl;

    for (Fraction guess : guesses)
    {
        cout << " " << guess;
    }
    cout << endl;
}