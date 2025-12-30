#include <stdio.h>
#include "C:\Yan\OS homework\Task1\Vector\vector.h"
#include "C:\Yan\OS homework\Task1\Number\number.h"

void printNumber(const char* label, const Number& num) {
    printf("%s%.2f\n", label, num.getValue());
}

// Вспомогательная функция для печати Vector
void printVector(const char* label, const Vector& vec) {
    printf("%s(%.2f, %.2f)\n", label, vec.getX().getValue(), vec.getY().getValue());
}

int main() {
    printf("=== Demonstration of library work ===\n\n");
    
    // 1. Демонстрация статической библиотеки Number
    printf("1. Working with the Number library:\n");
    
    Number a = createNumber(10.0);
    Number b = createNumber(3.0);
    
    printNumber("a = ", a);
    printNumber("b = ", b);
    printNumber("a + b = ", a + b);
    printNumber("a - b = ", a - b);
    printNumber("a * b = ", a * b);
    printNumber("a / b = ", a / b);
    
    printNumber("ZERO = ", ZERO);
    printNumber("ONE = ", ONE);
    
    printf("\n");
    
    // 2. Демонстрация динамической библиотеки Vector
    printf("2. Working with the Vector library:\n");
    
    Vector v1(createNumber(3.0), createNumber(4.0));
    Vector v2(createNumber(1.0), createNumber(2.0));
    Vector v3 = v1 + v2;
    
    printVector("v1 = ", v1);
    printVector("v2 = ", v2);
    printVector("v1 + v2 = ", v3);
    
    printNumber("Length of vector v1 (radius) =", v1.getRadius());
    printNumber("Угол вектора v1 = ", v1.getAngle());
    printf("(в градусах: %.2f)\n", v1.getAngle().getValue() * 180 / 3.14159);
    
    printf("\n");
    
    // 3. Демонстрация глобальных векторов
    printf("3. Global vectors:\n");
    printVector("VECTOR_ZERO = ", VECTOR_ZERO);
    printVector("VECTOR_ONE = ", VECTOR_ONE);
    
    printf("\n");
    
    // 4. Показываем, что Vector использует Number
    printf("4. Demonstrating the dependence of Vector on Number:\n");
    Vector v4(a, b); // Используем Number объекты
    printVector("Vector of Number objects: ", v4);
    
    // 5. Демонстрация работы с полярными координатами
    printf("\n5. Polar coordinates:\n");
    Vector v5(createNumber(1.0), createNumber(1.0));
    printVector("Vector: ", v5);
    printNumber("Radius: ", v5.getRadius());
    printNumber("Angle (radians): ", v5.getAngle());
    
    printf("\n=== The program completed successfully ===\n");
    
    return 0;
}