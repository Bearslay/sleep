#include "sleep.hpp"

int main() {
    npp::init(true);

    Sleep::RPi pi;
    pi.main();

    return npp::end(true);    
}
