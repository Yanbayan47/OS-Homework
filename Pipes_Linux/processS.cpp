#include <iostream>
#include <cstdlib> // для exit()

int main() {
    int x;
    long long sum = 0;
    
    while (std::cin >> x) {
        sum += x;
    }
    
    std::cout << "Final Sum: " << sum << std::endl;
    return 0;
}