#include "sleepNew.hpp"

int main() {
    npp::init(true);

    Sleep::RPi rpi;
    rpi.main();

    return npp::end(true);    
}
