#include "sleepOld.hpp"

int main() {
    npp::init(true);

    Sleep::Pi rpi;
    rpi.main();

    return npp::end(true);    
}
