#include "buffered_channel.h"
#include <iostream>
#include <thread>

int main() {
    BufferedChannel<int> chan(3);
    
    std::thread t1([&]() {
        for (int i = 0; i < 5; i++) {
            chan.send(i);
            std::cout << "Sent " << i << std::endl;
        }
        chan.close();
    });
    
    std::thread t2([&]() {
        for (int i = 0; i < 5; i++) {
            auto [val, ok] = chan.recv();
            if (ok) {
                std::cout << "Received " << val << std::endl;
            }
        }
    });
    
    t1.join();
    t2.join();
    
    return 0;
}