#ifndef FLOW_CONTROL
#define FLOW_CONTROL

#include <tuple>

/*
 *  NOP::execute(...)
 *
 *  IF<Condition, Then, Else> -> Then or Else
 *
 *
 *
 */

namespace meta
{
    struct True;
    struct False;



    // IMPROVE THIS! (using indices? using loops?)
    /*!
     * \brief       MetaFunction takes another template and arguments as a tuple that are applied on the template.
     *
     *  \details    MetaFunction<Template, std::tuple<Parameters...> >::type  :=  Template<Parameters...>
     */
    template<int N, int N_MAX, template<typename... A> class Template, typename ArgTuple, int... Indices>
    struct MetaCall_ : public MetaCall_<N+1, N_MAX, Template, ArgTuple, Indices..., N>{
        // typedef typename MetaCall_<N+1, N_MAX, Template, ArgTuple, Indices..., N>::type type;
    };
    template<int N_MAX, template<typename... A> class Template, typename ArgTuple, int... Indices>
    struct MetaCall_<N_MAX, N_MAX, Template, ArgTuple, Indices...> : public Template<typename std::tuple_element<Indices, ArgTuple>::type...> {
        // typedef Template<typename std::tuple_element<Indices, ArgTuple>::type...> type;
    };

    template<template<typename... A> class Template, typename ArgTuple>
    struct MetaCall : public MetaCall_<0, std::tuple_size<ArgTuple>::value, Template, ArgTuple> {
        //typedef typename MetaCall_<0, std::tuple_size<ArgTuple>::value, Template, ArgTuple>::type type;
    };

    /*!
     * \brief   No Operator
     */
    struct NOP {
        static void execute() { std::cout << "NOPE"; }
    };

    /*!
     * \brief   IF<Condition,Then,Else> -> Then or Else depending on Condition
     */
    /*
    template<typename Condition, typename Then, typename Else>
    struct IF_ {};

    template<typename Then, typename Else>
    struct IF_<True, Then, Else> : public Then {};

    template<typename Then, typename Else>
    struct IF_<False, Then, Else> : public Else {};

    template<typename Condition, typename Then, typename Else = NOP>
    struct IF : public IF_<typename Condition::metaValue, Then, Else> {};
    */

    template<typename Condition, typename Then, typename Else>
    struct IF_ {};

    template<typename Then, typename Else>
    struct IF_<True, Then, Else> {
        typedef typename Then::type type;
    };

    template<typename Then, typename Else>
    struct IF_<False, Then, Else> {
        typedef typename Else::type type;
    };

    template<typename Condition, typename Then, typename Else = NOP>
    struct IF : public IF_<typename Condition::type, Then, Else> {};

    /*!
     * \brief   LOOP<Tuple, >
     */


    /*!
     * \brief   FOR_EACH<Tuple, >
     */
}


#endif
