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

template<typename T>
struct Wrapper {
    typedef T type;
};

template<int N>
struct FailWithCode2 {
    static_assert(N != 0, "Failed 2");
    static void execute() {
        std::cerr << "Fail2";
    }
};

template<size_t N>
void printIndexRange_() {
    std::cout << N << "\n";
}
template<size_t N1, size_t N2, size_t... N_rest>
void printIndexRange_() {
    std::cout << N1 << " - ";
    printIndexRange_<N2, N_rest...>();
}
template<size_t... sequence>
void printIndexRange(meta::Sequence<sequence...>) {
    printIndexRange_<sequence...>();
}

int main()
{
    meta::Condition<true>::value();
    meta::Condition<false>::value();

    std::cerr << "\n";

    meta::IF<meta::Condition<true>, printType<1>  >::type::execute();
    meta::IF<meta::Condition<false>, printType<1> >::type::execute();

    std::cerr << "\n";


    std::cerr << "Tuple test";
    typedef meta::Tuple<int, double, int> tuple1;
    typedef meta::Tuple<float, float, int> tuple2;
    typedef meta::Tuple<> emptyTuple;
    std::cerr << "TupleLength: "+std::to_string(tuple1::length);
    std::cerr << "TupleLength: "+std::to_string(emptyTuple::length);

    std::cerr << "Empty?: "+std::to_string(tuple1::isEmpty);
    std::cerr << "Empty?: "+std::to_string(emptyTuple::isEmpty);

    std::cerr << "\nRange:\n";
    typedef typename meta::Range<0,10>::type indexRange1;
    printIndexRange(indexRange1{});
    std::cerr << "IndexRange:\n";
    typedef typename meta::IndexRange<10>::type indexRange2;
    printIndexRange(indexRange2{});

    std::cerr << "\n";

    std::cerr << "sizeBefore: "+std::to_string(meta::Tuple_length<tuple2::type>::value);
    typedef meta::Tuple_add<tuple2::type, 0, float> newTuple2;
    std::cerr << "sizeAfter: "+std::to_string(meta::Tuple_length<newTuple2::type>::value);

    return 0;
}
