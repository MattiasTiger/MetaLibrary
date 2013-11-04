#ifndef TUPLE_UTILITIES
#define TUPLE_UTILITIES

#include <tuple>
#include "Sequence.hpp"
#include "FlowControl.hpp"

/*
 * Get
 *  Tuple_getType<Tuple,Index>::type        := type att Index in Tuple
 *
 * Add
 *  Tuple_add<Tuple,Index,Type>::type       := A copy of Tuple in which Type is palced at Index and the previous types at Index and after have their indexes incremented by one.
 *  Tuple_addBack<Tuple,Type>::type         := A copy of Tuple in which Type is added at the back
 *
 * Remove
 *  Tuple_removeType<Tuple,Type>::type      := A copy of Tuple in which the first occurance of type Type has been removed
 *  Tuple_removeAllType<Tuple,Type>::type   := A copy of Tuple in which all occurances of type Type has been removed
 *  Tuple_remove<Tuple,Index>::type         := A copy of Tuple in which the type at Index has been removed
 *
 * Boolean
 *  Tuple_containType<Tuple,Type>::value    := true|false, if Tuple contain type Type
 *  Tuple_isEmpty<Tuple>::value             := true|false, if Tuple is empty
 *
 * Informatic
 *  Tuple_length<Tuple>::value                  := length of tuple T
 *  Tuple_findType<Tuple,Type>::value           := int, index of type Type in tuple T, -1 if it doesn't exist
 *  Tuple_occuranceAmount<Tuple, Type>::value   := int, number of occurances of Type in Tuple, -1 if it doesn't exist
 *  Tuple_occurances<Tuple, Type>::type         := Indices of the occurances of Type'a in Tuple
 *
 * Manage tuples
 *  Tuple_unique<Tuple>::type               := A copy of Tuple in which no type occures more than once (the first occurance is saved)
 *  Tuple_append<Tuple1,Tuple2>::type       := A tuple consisting of the types of first T1 then T2
 *  Tuple_subset<Tuple, Indices>::type      := A tuple consisting of the types indexed in the Indices list (all within range)
 *
 * Set operations
 *  Tuple_union<Tuple1,Tuple2>::type            := A tuple consisting of the union of the types in T1 and T2
 *  Tuple_intersect<Tuple1,Tuple2>::type        := A tuple consisting of the intersection of types in T1 and T2
 *  Tuple_exclude<Tuple1,Tuple2>::type          := A tuple consisting of the types in T1 excluding any types in T2
 *  Tuple_cartesianProduct<Tuple1,Tuple2>::type := A tuple of tuples (pairs) consisting of all pairwise combinations of the types in T1 and T2
 *
 */


template <typename T>
struct TypeName {
    static const char * execute() { return "Type not declared!"; }
};
#define DECLARE_TYPE_NAME(Type) template <> struct TypeName<Type> { static const char * execute() { return #Type; } };

DECLARE_TYPE_NAME(void)
DECLARE_TYPE_NAME(int)
DECLARE_TYPE_NAME(bool)
DECLARE_TYPE_NAME(float)
DECLARE_TYPE_NAME(double)
DECLARE_TYPE_NAME(short)
DECLARE_TYPE_NAME(long)
DECLARE_TYPE_NAME(char)
DECLARE_TYPE_NAME(std::tuple<>)

namespace meta
{

////////
template<typename First, typename... Rest>
struct PrintTupleTypes_ {
    static void execute() {
        std::cout << TypeName<First>::execute() << ", ";
        PrintTupleTypes_<Rest...>::execute();
    }
};
template<typename First>
struct PrintTupleTypes_<First> {
     static void execute() {
         std::cout << TypeName<First>::execute();
     }
 };

// If the tuple contain a tuple and then other types/tuples
template<typename TypeFirst1, typename... TypeFirst, typename Rest1, typename... Rest>
struct PrintTupleTypes_<std::tuple<TypeFirst1, TypeFirst...>, Rest1, Rest...> {
    static void execute() {
        std::cout << "<";
        PrintTupleTypes_<TypeFirst1, TypeFirst...>::execute();
        std::cout << ">, ";
        PrintTupleTypes_<Rest1, Rest...>::execute();
    }
};
// If the tuple contain a tuple
template<typename Types1, typename... Types >
struct PrintTupleTypes_<std::tuple<Types1, Types...>> {
     static void execute() {
         std::cout << "<";
         PrintTupleTypes_<Types1, Types...>::execute();
         std::cout << ">";
     }
 };
template<typename... Types>
struct PrintTupleTypes;

template<typename Types1, typename... Types>
struct PrintTupleTypes<std::tuple<Types1, Types...>> {
    static void execute() {
        std::cout << "<";
        PrintTupleTypes_<Types1, Types...>::execute();
        std::cout << ">\n";
    }
};
// Empty tuple
template<>
struct PrintTupleTypes<std::tuple<>> {
    static void execute() {
        std::cout << "<";
        std::cout << ">\n";
    }
};

////////

/*
template<typename... Types>
struct merge_tuples{
  typedef std::tuple<Types...> type;
};
*/
template<typename... Types>
struct merge_tuples;

// If more than 2 tuples
template<typename...Types1,typename...Types2,typename...Tuples>
struct merge_tuples<std::tuple<Types1...>,std::tuple<Types2...>, Tuples...>
{
  typedef typename merge_tuples<std::tuple<Types1...,Types2...>, Tuples...>::type type;
};

// If 2 tuples
template<typename...Types1,typename...Types2>
struct merge_tuples<std::tuple<Types1...>,std::tuple<Types2...>>
{
  typedef std::tuple<Types1..., Types2...> type;
};


////////

    /*!
     *
     */
    template<typename Tuple, int Index>
    struct Tuple_getType {
        typedef typename std::tuple_element<Index, Tuple>::type type;
    };


    /*!
     *
     */
    template<typename Tuple, typename Type>
    struct Tuple_addBack {
        typedef typename merge_tuples<Tuple,std::tuple<Type> >::type type;
    };

    /*!
     *
     */
    template<typename Tuple, typename S>
    struct Tuple_add__;

    template<typename Tuple, size_t... sequence>
    struct Tuple_add__<Tuple, Sequence<sequence...>> {
        typedef std::tuple<typename std::tuple_element<sequence, Tuple>::type...> type;
    };

    template<typename Tuple, int Index, typename Type>
    struct Tuple_add_ {
        typedef typename Range<0, Index-1>::type preSequence;
        typedef typename Range<Index, std::tuple_size<Tuple>::value-1>::type postSequence;
        typedef typename Tuple_add__<Tuple, postSequence>::type preTuple;
        typedef typename Tuple_add__<Tuple, postSequence>::type postTuple;

        typedef typename merge_tuples<preTuple, std::tuple<Type>, postTuple>::type type;
    };

    // Base
    template<typename Tuple, int Index, typename Type>
    struct Tuple_add;
    // Add at the end
    template<typename... types, int Index, typename Type>
    struct Tuple_add<std::tuple<types...>, Index, Type> {
        static_assert(Index <= int(sizeof...(types)), "Tuple_add<Tuple, index, Type> got a index that was out of bounds (to large)");
        static_assert(Index >= 0,                     "Tuple_add<Tuple, index, Type> got a index that was out of bounds (negative)");
        typedef typename IF<Condition<Index == sizeof...(types)>, Tuple_addBack<std::tuple<types...>, Type>, Tuple_add_<std::tuple<types...>, Index, Type>>::type type;
    };
    // Add at the beginning
    template<typename... types, typename Type>
    struct Tuple_add<std::tuple<types...>, 0, Type> {
        typedef typename merge_tuples<std::tuple<Type>, std::tuple<types...>>::type type;
    };


    /*!
     *
     */
    template<typename Tuple, typename Type>
    struct Tuple_removeType {
        typedef int type;
    };


    /*!
     *
     */
    template<typename Tuple, typename Type>
    struct Tuple_removeAllType {
        typedef int type;
    };


    /*!
     *
     */
    template<typename Tuple, int Index>
    struct Tuple_remove {
        typedef int type;
    };


    /*!
     *
     */
    template<typename Tuple, typename Type>
    struct Tuple_containType {
        typedef int value;
    };

    /*!
     *
     */
    template<typename Tuple>
    struct Tuple_isEmpty {
        static const bool value = std::tuple_size<Tuple>::value == 0;
    };


    /*!
     *
     */
    template<typename Tuple>
    struct Tuple_length {
        static const unsigned int value = std::tuple_size<Tuple>::value;
    };



    /*!
     * \brief   Tuple contains the entire Tuple_... interface (for easy access)
     */
    template<typename... Variadic>
    struct Tuple {
        typedef std::tuple<Variadic...> type;

        // Get
        template<int index>
        struct getType : public Tuple_getType<type, index> {};

        // Add
        template<int index, typename Type>
        struct add : public Tuple_add<type, index, Type> {};

        // Remove

        // Boolean
        static const bool isEmpty = Tuple_isEmpty<type>::value;

        // Informatic
        static const unsigned int length = Tuple_length<type>::value;

        // Manage tuples

        // Set operations

    };

}

#endif
