#include "sleep.hpp"

int main() {
    npp::init(true);
    Timing::TimeManip.sync();

    Sleep::Pi pi;
    pi.main();

    return npp::end(true);    
}
