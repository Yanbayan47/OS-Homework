#ifndef VECTOR_H
#define VECTOR_H

#include "/home/yan/Documents/GitHub/OS-Homework-1/Task1_linux/Number/number.h" // Используем Number из статической библиотеки

// Для динамической библиотеки нужны специальные макросы
#ifdef _WIN32
    #ifdef BUILD_VECTOR_DLL
        #define VECTOR_API __declspec(dllexport)
    #else
        #define VECTOR_API __declspec(dllimport)
    #endif
#else
    #define VECTOR_API
#endif

class VECTOR_API Vector {
private:
    Number x;  // Используем Number из нашей статической библиотеки
    Number y;  // Вместо double
    
public:
    Vector(const Number& xVal = ZERO, const Number& yVal = ZERO);
    
    // для декартовых координат
    Number getX() const;
    Number getY() const;
    void setX(const Number& val);
    void setY(const Number& val);
    
    // для полярных координат
    Number getRadius() const;
    Number getAngle() const;
    
    // сложение векторов
    Vector operator+(const Vector& other) const;
};

// Глобальные переменные векторов 
extern VECTOR_API const Vector VECTOR_ZERO;
extern VECTOR_API const Vector VECTOR_ONE;

#endif