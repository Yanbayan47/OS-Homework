#include "C:\Yan\OS homework\Task1\Vector\vector.h"
#include <cmath>

// при компиляции этой библиотеки нужно определить BUILD_VECTOR_DLL
//  через флаг компилятора -DBUILD_VECTOR_DLL

Vector::Vector(const Number& xVal, const Number& yVal) : x(xVal), y(yVal) {}

Number Vector::getX() const { return x; }
Number Vector::getY() const { return y; }

void Vector::setX(const Number& val) { x = val; }
void Vector::setY(const Number& val) { y = val; }

Number Vector::getRadius() const {
    Number x2 = x * x;
    Number y2 = y * y;
    Number sum = x2 + y2;
    return createNumber(sqrt(sum.getValue()));
}

Number Vector::getAngle() const {
    return createNumber(atan2(y.getValue(), x.getValue()));
}

Vector Vector::operator+(const Vector& other) const {
    return Vector(x + other.x, y + other.y);
}

const Vector VECTOR_ZERO(ZERO, ZERO);
const Vector VECTOR_ONE(ONE, ONE);