//  Written by: Erik Sevre
//  Date: September 14, 2017
//

#include <iostream>
#include <vector>

#include "multiprecision_integer.h"
#include "multiprecision_integer_test.hpp"
#include <cmath>




int main() {
    const unsigned long int max_val = std::numeric_limits<unsigned long int>::max();
    multiprecision_integer<unsigned long int> sv1{1, max_val, max_val, max_val};
    multiprecision_integer<unsigned long int> sv2{1, 2};
    multiprecision_integer<unsigned long int> c = sv2 + sv1;

    std::cout << sv1 << '\n';
    std::cout << sv2 << '\n';
    std::cout << (sv2 += sv1) << '\n';
    std::cout << c << "\n";

    std::cout << "c.ufo(sv2) : " << c.ufo(sv2) << "\n";

    sv2 += sv1;
    std::cout << "c.ufo(sv2) : " << c.ufo(sv2) << "\n";

    c += c;
    c += c;
    std::cout << "c.ufo(sv2) : " << c.ufo(sv2) << "\n";

    run_equality_tests();
    run_addition_tests();

}



