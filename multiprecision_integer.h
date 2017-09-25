//
// Created by Erik Sevre on 9/6/17.
//
//
//  Class to store and compute with multiprecision integers
//


#pragma once

#include <vector>
#include <algorithm>
#include <limits>
#include <ostream>


template <typename UnsignedIntegerType>
class multiprecision_integer {
public:
    // Data
    std::vector<UnsignedIntegerType> coefficients;

    //
    //  5 Core overrides (rule of 5)
    //
    // default constructor
    multiprecision_integer()
            : coefficients(1) { coefficients[0] = 0; }
    // copy constructor
    multiprecision_integer(const multiprecision_integer &multi_int) : coefficients(multi_int.coefficients) { }
    // move constructor
    multiprecision_integer(multiprecision_integer &&multi_int) noexcept
            : coefficients(std::move(multi_int.coefficients)) { }


    //
    //  Custom constructors
    //
    explicit multiprecision_integer(UnsignedIntegerType num)
            : coefficients(1)
    {
        coefficients[0] = num;
    }
    multiprecision_integer(std::initializer_list<UnsignedIntegerType> init_list)
            : coefficients(init_list.size())
    {
        std::copy(std::begin(init_list), std::end(init_list), std::begin(coefficients));
    }


    // copy assignment
    multiprecision_integer &operator=(const multiprecision_integer &multi_int) {
        coefficients = multi_int.coefficients;
        return *this;
    }
    // move assignment
    multiprecision_integer &operator=(multiprecision_integer &&multi_int) noexcept {
        coefficients = std::move(multi_int.coefficients);
        return *this;
    }


    //     *****     *****     *****     *****     *****     *****     *****
    //          Boolean Operators
    //     *****     *****     *****     *****     *****     *****     *****
    //
    //
    //     *****     *****     *****     *****     *****     *****     *****
    //          Equality Operators
    //               ==   !=
    //     *****     *****     *****     *****     *****     *****     *****

    bool operator==(const multiprecision_integer &rhs) const {
        const auto this_rev_it_end = std::rend(coefficients);
        const auto this_rev_it_begin = std::find_if_not(std::rbegin(coefficients), this_rev_it_end, [](UnsignedIntegerType i){return i==0;});

        const auto rhs_rev_it_end = std::rend(rhs.coefficients);
        const auto rhs_rev_it_begin = std::find_if_not(std::rbegin(rhs.coefficients), rhs_rev_it_end, [](UnsignedIntegerType i){return i==0;});

        return std::equal(this_rev_it_begin,
                          this_rev_it_end,
                          rhs_rev_it_begin);
    }
    bool operator!=(const multiprecision_integer &rhs) const {
        return !(rhs == *this);
    }

    //     *****     *****     *****     *****     *****     *****     *****
    //          Inequality Operators
    //     ufo(rhs)   <   <=   >   >=
    //     *****     *****     *****     *****     *****     *****     *****

    //     *****     *****     *****     *****     *****     *****     *****
    //          UFO (Spaceship) Operator
    //              <==>
    //     Checks less than, equal to, and greater than
    //     no oficial operator in C++ yet, so use it as a member function
    //          -1  :  Less than
    //           0  :  Equal to
    //           1  :  Greater than
    //     *****     *****     *****     *****     *****     *****     *****

    int ufo(const multiprecision_integer &rhs) const {
        //
        // remove leading zeros if they exist (currently allow them for buffering to fill 2^n for algorithms)
        //
        const auto this_rev_it_end = std::rend(coefficients);
        const auto this_rev_it_begin = std::find_if_not(std::rbegin(coefficients), this_rev_it_end, [](UnsignedIntegerType i){ return i==0;});

        const auto rhs_rev_it_end = std::rend(rhs.coefficients);
        const auto rhs_rev_it_begin = std::find_if_not(std::rbegin(rhs.coefficients), rhs_rev_it_end, [](UnsignedIntegerType i){ return i==0;});

        //
        //  With leading zeros removed, check equality of remaining sections
        //
        if (std::equal(this_rev_it_begin, this_rev_it_end, rhs_rev_it_begin)) { return 0; }

        //
        //  Compute the sizes, to see if they are the same
        //
        const auto this_size = this_rev_it_end - this_rev_it_begin;
        const auto rhs_size = rhs_rev_it_end - rhs_rev_it_begin;

        //
        // Based on size, try to determine which is bigger
        // compare based on size of vector (removing zero elements)
        //
        if (this_size > rhs_size) { return 1; }
        else if (this_size < rhs_size) { return -1; }

        //
        // size is the same, so check elementwise
        //
        auto val = std::mismatch(this_rev_it_begin, this_rev_it_end, rhs_rev_it_begin);

        // check greater than
        if (*val.first > *val.second) { return 1; }

        // only option left is less than
        return -1;
    }

    bool operator<(const multiprecision_integer &rhs) const {
        return this->ufo(rhs) == -1;
    }
    bool operator>(const multiprecision_integer &rhs) const {
        return this->ufo(rhs) == 1;
    }
    bool operator<=(const multiprecision_integer &rhs) const {
        return !(rhs < *this);
    }
    bool operator>=(const multiprecision_integer &rhs) const {
        return !(*this < rhs);
    }

    UnsignedIntegerType operator[](size_t i) const { return coefficients[i]; }
    UnsignedIntegerType &operator[](size_t i) { return coefficients[i]; }


    multiprecision_integer &operator+=(const multiprecision_integer &rhs)
    {
        //
        // lambda functions for adding and tracking remainder
        //
        //  Binary Adder is used for adding the two arrays together
        //    This tracks the remainder and makes sure the digit is carried
        //
        UnsignedIntegerType remainder{0};
        auto adder_binary =
                [&remainder](UnsignedIntegerType a, UnsignedIntegerType b=0) mutable -> UnsignedIntegerType
                {
                    auto val = a + b + remainder;
                    if (val < a or val < b) {
                        remainder = 1;
                    } else if (val > a and val > b) {
                        remainder = 0;
                    }
                    return val;
                };

        size_t coefficients_size = coefficients.size();
        size_t rhs_size = rhs.coefficients.size();

        coefficients.resize(std::max(coefficients_size, rhs_size));

        // coefficients should be bigger, I will deal with this case later
        // when I add negative integers
        if (coefficients_size <= rhs_size) {
            std::transform(std::begin(coefficients),
                           std::end(coefficients),
                           std::begin(rhs.coefficients),
                           std::begin(coefficients),
                           adder_binary);
        } else {
            std::transform(std::begin(rhs.coefficients),
                           std::end(rhs.coefficients),
                           std::begin(coefficients),
                           std::begin(coefficients),
                           adder_binary);
            std::transform(std::begin(coefficients)+rhs_size,
                           std::end(coefficients),
                           std::begin(coefficients) + rhs_size,
                           adder_binary);
        }
        if (remainder == 1) {
            coefficients.push_back(1);
        }

        return *this;
    }

    multiprecision_integer operator+(const multiprecision_integer &rhs) const
    {
        multiprecision_integer result = *this;
        return result+=rhs;
    }

    multiprecision_integer operator+(multiprecision_integer &&rhs) const {
        return rhs+=*this;
    }

    multiprecision_integer &operator-=(const multiprecision_integer &rhs)
    {
        //
        // lambda functions for adding and tracking remainder
        //
        //  Binary Adder is used for adding the two arrays together
        //    This tracks the remainder and makes sure the digit is carried
        //
        UnsignedIntegerType remainder{0};
        auto subtractor =
                [&remainder](UnsignedIntegerType a, UnsignedIntegerType b=0) mutable -> UnsignedIntegerType
                {
                    auto val = a - remainder;
                    auto result = val - b;
                    if (result > val) {
                        remainder = 1;
                    } else if (result < val) {
                        remainder = 0;
                    }
                    return result;
                };

        size_t coefficients_size = coefficients.size();
        size_t rhs_size = rhs.coefficients.size();

        coefficients.resize(std::max(coefficients_size, rhs_size));

        if (coefficients_size <= rhs_size) {
            std::transform(std::begin(coefficients),
                           std::end(coefficients),
                           std::begin(rhs.coefficients),
                           std::begin(coefficients),
                           subtractor);
        } else {
            std::transform(std::begin(rhs.coefficients),
                           std::end(rhs.coefficients),
                           std::begin(coefficients),
                           std::begin(coefficients),
                           subtractor);
            std::transform(std::begin(coefficients)+rhs_size,
                           std::end(coefficients),
                           std::begin(coefficients) + rhs_size,
                           subtractor);
        }
        if (remainder == 1) {
            coefficients.push_back(1);
        }


        return *this;
    }





    //
    //
    //
    friend std::ostream &operator<<(std::ostream &os, const multiprecision_integer &integer) {
        os << "[";
        for (auto c : integer.coefficients) {
            std::cout << c << ", ";
        }
        std::cout << "\b\b]";
        return os;
    }

};