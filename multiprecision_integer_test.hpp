//
// Created by Erik Sevre on 9/25/17.
//
//
//  Tests for multiprecision integer
//
//
#pragma once

#include "multiprecision_integer.h"

#include <limits>
#include <iostream>

//
//  Equality Operator Test
//
// test equality returns true for true case
bool equality_test_01(bool verbose=true) {
    multiprecision_integer<uint64_t> a{1, 1, 1};
    multiprecision_integer<uint64_t> b{1, 1, 1};
    return a == b;
}

// test equality returns false for false case (different bit in last place)
bool equality_test_02(bool verbose=true) {
    multiprecision_integer<uint64_t> a{1, 1, 1};
    multiprecision_integer<uint64_t> b{1, 1, 2};
    return !(a == b);
}
//
//  Inequality tests
//
bool inequality_test_01(bool verbose=true) {
    multiprecision_integer<uint64_t> a{1, 1, 1};
    multiprecision_integer<uint64_t> b{1, 1, 2};
    return a != b;
}

// Test the case where they are equal and false should be returned
bool inequality_test_02(bool verbose=true) {
    multiprecision_integer<uint64_t> a{1, 1, 1};
    multiprecision_integer<uint64_t> b{1, 1, 1};
    return !(a != b);
}

bool run_equality_tests(bool verbose = true) {
    bool result = true;


    if (verbose) {
        std::cout << "*****     *****     *****     *****     *****     *****     *****\n";
        std::cout << "*****                    Equality Tests                     *****\n";
        std::cout << "*****     *****     *****     *****     *****     *****     *****\n";
        std::cout << " **  Equality test: operator==\n";
    }
    if (equality_test_01(verbose)){
        if (verbose) {
            std::cout << "[OK]  :  Equality test 01 passed, correct positive identified.\n";
        }
    } else {
        if (verbose) {
            std::cout << "[XX]  :  Equality test 01 failed, correct positive not identified.\n";
        }
    }
    if (equality_test_02(verbose)){
        if (verbose) {
            std::cout << "[OK]  :  Equality test 02 passed, correct negative identified.\n";
        }
    } else {
        if (verbose) {
            std::cout << "[XX]  :  Equality test 02 failed, correct negative not identified.\n";
        }
    }
    if (verbose) {
        std::cout << " **  Inequality test: operator!=\n";
    }
    if (inequality_test_01(verbose)){
        if (verbose) {
            std::cout << "[OK]  :  Inequality test 01 passed, correct positive identified.\n";
        }
    } else {
        if (verbose) {
            std::cout << "[XX]  :  Inequality test 01 failed, correct positive not identified.\n";
        }
    }
    if (inequality_test_02(verbose)){
        if (verbose) {
            std::cout << "[OK]  :  Inequality test 02 passed, correct negative identified.\n";
        }
    } else {
        if (verbose) {
            std::cout << "[XX]  :  Inequality test 02 failed, correct negative not identified.\n";
        }
    }
    return result;
}


// simplest test
bool addition_test_01(bool verbose=true) {
    multiprecision_integer<uint64_t> a{1, 1, 1};
    multiprecision_integer<uint64_t> b{1, 1, 1};

    multiprecision_integer<uint64_t> c = a + b;
    multiprecision_integer<uint64_t> c_result{2, 2, 2};

    return c == c_result;
}

// test that addition works and variables carry
bool addition_test_02(bool verbose=true) {
    const unsigned long int max_val = std::numeric_limits<unsigned long int>::max();
    multiprecision_integer<unsigned long int> sv1{max_val, max_val, max_val};
    multiprecision_integer<unsigned long int> sv2{1};
    multiprecision_integer<unsigned long int> c = sv2 + sv1;
    multiprecision_integer<unsigned long int> c_result{0, 0, 0, 1};

    return c == c_result;
}


bool run_addition_tests(bool verbose=true) {
    bool result = true;

    if (verbose) {
        std::cout << "*****     *****     *****     *****     *****     *****     *****\n";
        std::cout << "*****                    Addition Tests                     *****\n";
        std::cout << "*****     *****     *****     *****     *****     *****     *****\n";
        std::cout << " **  Addition test: operator+\n";
    }

    if (addition_test_01(verbose)) {
        std::cout << "[OK]  : Addition Test 1 passed (Super Simple Test)\n";
    } else {
        std::cout << "[XX]  : Addition Test 1 failed (Super Simple Test)\n";
        result = false;
    }
    if (addition_test_02(verbose)) {
        std::cout << "[OK]  : Addition Test 2 passed (remainder carry test)\n";
    } else {
        std::cout << "[XX]  : Addition Test 2 failed (remainder carry test)\n";
        result = false;
    }

    return result;
}