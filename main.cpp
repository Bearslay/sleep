#include "sleep.hpp"

int main() {
    npp::init(true);

    Sleep::Pi pi;
    pi.main();

    return npp::end(true);    
}
