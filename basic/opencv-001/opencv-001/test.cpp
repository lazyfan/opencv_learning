#include<iostream>

int main() {
    int b = 0;
    int& a = b;
    a = 4;
    std::cout << a << '\t' << std::endl;
    b = 555554;
    std::cout << a << std::endl; //555554
    return 0;
}