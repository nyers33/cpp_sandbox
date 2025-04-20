#include <iostream>
#include <Eigen/Dense>

int main() {
    Eigen::IOFormat CleanFmt(4, 0, ", ", "\n", "[", "]");

    Eigen::Matrix3d MatA;
    Eigen::Vector3d b, x;

    MatA << 2, 1,  1,
        -1, 1, -1,
         1, 2,  3;
    b << 2, 3, -10;

    x = MatA.colPivHouseholderQr().solve(b);

    std::cout << "equation A x = b\n"<< std::endl;
    std::cout << "matrix A" << std::endl;
    std::cout << MatA.format(CleanFmt) << "\n" << std::endl;
    std::cout << "vector x (solution)\n" << x << "\n" << std::endl;
    std::cout << "vector b\n" << b << std::endl;
    return 0;
}
