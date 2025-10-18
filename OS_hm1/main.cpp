#include <iostream>
#include "../Number/NumberExports.h"
#include "../Vector/Vector.h""

int main() {
    try {
        // тест number
        std::cout << "             Testing number " << std::endl;
        Number a = createNumber(5.0);
        Number b = createNumber(3.0);

        std::cout << "a = " << a.getValue() << std::endl;
        std::cout << "b = " << b.getValue() << std::endl;
        std::cout << "a + b = " << (a + b).getValue() << std::endl;
        std::cout << "a * b = " << (a * b).getValue() << std::endl;
        std::cout << "ZERO = " << ZERO.getValue() << std::endl;

        // тест vector
        std::cout << "\n            Testing vector " << std::endl;
        Vector v1(a, b);  // (5, 3)
        Vector v2(ONE_ONE_VECTOR);

        std::cout << "v1: (" << v1.getX().getValue() << ", " << v1.getY().getValue() << ")" << std::endl;
        std::cout << "v1 radius: " << v1.getRadius().getValue() << std::endl;
        std::cout << "v1 angle: " << v1.getAngle().getValue() << std::endl;

        Vector v3 = v1 + v2;
        std::cout << "v1 + v2 = (" << v3.getX().getValue() << ", " << v3.getY().getValue() << ")" << std::endl;

    }
    catch (const std::exception& e) {
        std::cout << "Error: " << e.what() << std::endl;
    }

    return 0;
}