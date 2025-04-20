#include <boost/numeric/ublas/vector.hpp>
#include <boost/numeric/ublas/io.hpp>
#include <iostream>

namespace ublas = boost::numeric::ublas;

int main() {
    ublas::vector<int> v1(3), v2(3), v3(3);

    v1[0] = 1; v1[1] = 2; v1[2] = 3;
    v2[0] = 4; v2[1] = 5; v2[2] = 6;
    v3 = v1 + v2;

    std::cout << "v1 + v2 = v3" << "\n" << std::endl;
    std::cout << v1 << " + " << v2 << " = " << v3 << std::endl;
    return 0;
}
