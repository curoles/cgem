
//http://en.cppreference.com/w/cpp/types/conditional

#include <tuple>

template<std::size_t Cond, typename... TL>
struct conditional
{
    //using type = typename std::tuple_element<Cond, std::tuple<TL...>>::type;
    typedef typename std::tuple_element<Cond, std::tuple<TL...>>::type type;
};

/*template <class... Types>
struct type_list
{
   template <std::size_t N>
   using type = typename std::tuple_element<N, std::tuple<Types...>>::type;
};*/

#include <iostream>
#include <type_traits>
#include <typeinfo>

int main() 
{
    typedef std::conditional<true, int, double>::type Type1;
    typedef std::conditional<false, int, double>::type Type2;
    typedef std::conditional<sizeof(int) >= sizeof(double), int, double>::type Type3;

    static_assert( std::is_same<Type1/*decltype(i)*/, int>::value, "!int" );
    Type1 i;
    static_assert( std::is_same<decltype(i), int>::value, "!int" );

    std::cout << typeid(Type1).name() << '\n';
    std::cout << typeid(Type2).name() << '\n';
    std::cout << typeid(Type3).name() << '\n';

    typedef conditional<0, int, double, char>::type Type4;
    typedef conditional<1, int, double, char>::type Type5;
    typedef conditional<2, int, double, char>::type Type6;
    std::cout << typeid(Type4).name() << '\n';
    std::cout << typeid(Type5).name() << '\n';
    std::cout << typeid(Type6).name() << '\n';

    /*typedef type_list<int, double, char> tl;
    std::cout << typeid(tl::type<0>).name() << '\n';
    std::cout << typeid(tl::type<1>).name() << '\n';
    std::cout << typeid(tl::type<2>).name() << '\n';*/

    return 0;
}
