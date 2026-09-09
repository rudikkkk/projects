#include <iostream>


int f(float x) {
    if (x > 0) {
      x*=x;
    } else {
      x/=x;
    }
    return x;
}


int main() {
    std::cout << f(5);
    return 0;
}

