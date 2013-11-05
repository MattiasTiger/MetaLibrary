#include <iostream>
#include <string>

#include "MetaLibrary.hpp"

template<int N>
struct print {
    static void execute() {
        std::cout << N;
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
        std::cout << "Fail2";
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

    std::cout << "\n";

    meta::IF<meta::Condition<true>, printType<1>  >::type::execute();
    meta::IF<meta::Condition<false>, printType<1> >::type::execute();

    std::cout << "\n";


    std::cout << "Tuple test\n";
    typedef meta::Tuple<int, double, int> tuple1;
    typedef meta::Tuple<> tuple2;
    typedef meta::Tuple<> emptyTuple;
    std::cout << "TupleLength: "+std::to_string(tuple1::length) << "\n";
    std::cout << "TupleLength: "+std::to_string(emptyTuple::length) << "\n";

    std::cout << "Empty?: "+std::to_string(tuple1::isEmpty) << "\n";
    std::cout << "Empty?: "+std::to_string(emptyTuple::isEmpty) << "\n";

    std::cout << "\nRange:\n";
    typedef typename meta::Range<0,10>::type indexRange1;
    printIndexRange(indexRange1{});
    std::cout << "IndexRange:\n";
    typedef typename meta::IndexRange<10>::type indexRange2;
    printIndexRange(indexRange2{});

    std::cout << "\n";

    std::cout << "sizeBefore: "+std::to_string(meta::Tuple_length<tuple2::type>::value) << "\n";
    std::cout << meta::PrintTupleTypes<tuple2::type>::stream << "\n";
    typedef meta::Tuple_add<tuple2::type, 0, float> newTuple2;
    std::cout << "sizeAfter: "+std::to_string(meta::Tuple_length<newTuple2::type>::value) << "\n";
    std::cout << meta::PrintTupleTypes<newTuple2::type>::stream << "\n";

    return 0;
}
