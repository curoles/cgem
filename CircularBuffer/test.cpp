//g++ -std=c++11 -pthread test.cpp
//
#include "CircularBuffer.h"

#include <iostream>
#include <thread>

gem::CircularBuffer<int> buf;

void provider()
{
    static int counter = 0;

    while (counter < 100) {
        if (not buf.full()) {
            std::cout << "push " << counter << "\n";
            buf.push_back(counter++);
        }
        else {
            std::cout << "provider waits\n";
            std::this_thread::sleep_for(std::chrono::nanoseconds(1));
        }
    }
}

void consumer()
{
    while (1) {
        if (not buf.empty()) {
            int item = buf.pop();
            std::cout << "pop " << item << "\n";
            if (item > 90) break;
        }
        else {
            std::cout << "consumer waits\n";
            std::this_thread::sleep_for(std::chrono::nanoseconds(1));
        }
    }
}

int main()
{
    std::thread provider_thread(provider);
    std::thread consumer_thread(consumer);
    provider_thread.join();
    consumer_thread.join();
    return 0;
}
