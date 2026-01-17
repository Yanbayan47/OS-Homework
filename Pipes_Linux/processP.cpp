#include <iostream>

int main() {
    int x;
    while (std::cin >> x) {
        std::cout << (x * x * x) << " " << std::flush;
    }
    return 0;
}