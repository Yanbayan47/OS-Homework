#ifndef NUMBER_H
#define NUMBER_H

class Number {
private:
    double value;
    
public:
    Number(double val = 0.0);
    
    Number(const Number& other);
    
    Number operator+(const Number& other) const;
    Number operator-(const Number& other) const;
    Number operator*(const Number& other) const;
    Number operator/(const Number& other) const;
    
    // присваивание
    Number& operator=(const Number& other);
    
    double getValue() const;
    void setValue(double val);
};

// Глобальные константы
extern const Number ZERO;
extern const Number ONE;

Number createNumber(double value);

#endif