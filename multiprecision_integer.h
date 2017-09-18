//
// Created by Erik Sevre on 9/6/17.
//

#pragma once

#include <vector>
#include <algorithm>
#include <limits>
#include <ostream>


template <typename UnsignedIntegerType>
class multiprecision_integer {
public:
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
    multiprecision_integer(multiprecision_integer &&multi_int)
            : coefficients(std::move(multi_int.coefficients)) { }

    //
    //  Custom constructors
    //
    multiprecision_integer(UnsignedIntegerType num)
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
    multiprecision_integer &operator=(multiprecision_integer &&multi_int) {
        coefficients = std::move(multi_int.coefficients);
        return *this;
    }


    bool operator==(const multiprecision_integer &rhs) const {
        return std::equal(std::begin(coefficients),
                          std::end(coefficients),
                          std::begin(rhs.coefficients));
    }
    bool operator!=(const multiprecision_integer &rhs) const {
        return !(rhs == *this);
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