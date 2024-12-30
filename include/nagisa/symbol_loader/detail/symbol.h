#pragma once

#include "./cpo.h"
#include "./environment.h"

NAGISA_BUILD_LIB_DETAIL_BEGIN

template<class T>
struct symbol_type { using type = typename ::std::remove_cv_t<T>::type; };
template<class T> requires requires{ typename symbol_type<T>::type; }
using symbol_type_t = typename symbol_type<T>::type;

template<class T>
struct symbol_name { constexpr static auto value = ::std::remove_cv_t<T>::name; };
template<class T> requires requires{ {symbol_name<T>::value}; }
inline constexpr auto symbol_name_v = symbol_name<T>::value;
template<class T> requires requires{ {symbol_name<T>::value}; }
using symbol_name_t = decltype(symbol_name_v<T>);

template<class T>
concept symbol = requires
{
	typename symbol_type_t<T>;
	{ symbol_name_v<T> } -> strings::string;
};

template<class Type, strings::basic_nttp Name>
struct meta_symbol
{
	using type = Type;
	constexpr static strings::character_range auto name = Name;
};

NAGISA_BUILD_LIB_DETAIL_END