#include "number.h"
#include <iostream>
#include <stdexcept> // для исключения при делении на 0

Number::Number(double val) : value(val) {}

Number::Number(const Number& other) : value(other.value) {}

Number& Number::operator=(const Number& other) {
    if (this != &other) {
        value = other.value;
    }
    return *this;
}

Number Number::operator+(const Number& other) const {
    return Number(value + other.value);
}

Number Number::operator-(const Number& other) const {
    return Number(value - other.value);
}

Number Number::operator*(const Number& other) const {
    return Number(value * other.value);
}

Number Number::operator/(const Number& other) const {
    if (other.value == 0.0) {
        throw std::runtime_error("Division by zero!");
    }
    return Number(value / other.value);
}

double Number::getValue() const {
    return value;
}

void Number::setValue(double val) {
    value = val;
}

const Number ZERO(0.0);
const Number ONE(1.0);

Number createNumber(double value) {
    return Number(value);
}