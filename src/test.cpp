#include <iostream>
#include <string>

#include "MetaLibrary.hpp"

template<int N>
struct print {
    static void execute() {
        std::cerr << N;
    }
};

template<int N>
struct printType {
    typedef print<N> type;
};

int main()
{
    meta::Condition<true>::value();
    meta::Condition<false>::value();

    meta::IF<meta::Condition<true>, printType<1>, printType<0> >::type::execute();
    meta::IF<meta::Condition<false>, printType<1>, printType<0> >::type::execute();

    return 0;
}
