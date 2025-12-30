#include <iostream>

const int N = 3; // вроде

int main() {
    int x;
    while (std::cin >> x) {
        std::cout << (x + N) << " " << std::flush;
    }
    return 0;
}