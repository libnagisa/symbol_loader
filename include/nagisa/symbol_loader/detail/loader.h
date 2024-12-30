#pragma once

#include "./cpo.h"
#include "./symbol.h"
#include "./environment.h"

NAGISA_BUILD_LIB_DETAIL_BEGIN
#if defined(__cpp_static_call_operator)
#	define NAGISA_SYMBOL_LOADER_CALL_STATIC static
#	define NAGISA_SYMBOL_LOADER_CALL_CONST
#else
#	define NAGISA_SYMBOL_LOADER_CALL_STATIC
#	define NAGISA_SYMBOL_LOADER_CALL_CONST const
#endif

namespace cpo_details {
	template<class T, class... U>
	consteval result choose_load() noexcept
	{
		if constexpr (requires(T target, U... args) 
		{
			NAGISA_STL_FREESTANDING_UTILITY_FORWARD(target).load(NAGISA_STL_FREESTANDING_UTILITY_FORWARD(args)...);
		}) {
			return { choose::member, noexcept(::std::declval<T>().load(::std::declval<U>()...)) };
		}
		else if constexpr (requires(T target, U... args) 
		{
			load(NAGISA_STL_FREESTANDING_UTILITY_FORWARD(target), NAGISA_STL_FREESTANDING_UTILITY_FORWARD(args)...);
		}) {
			return { choose::adl, noexcept(load(::std::declval<T>(), ::std::declval<U>()...)) };
		}
		else {
			return { choose::none, true };
		}
	}
	constexpr decltype(auto) call_load(auto&& target, auto&&... args)
		noexcept(cpo_details::choose_load<decltype(target), decltype(args)...>().except)
		requires (cpo_details::choose_load<decltype(target), decltype(args)...>().value != choose::none)
	{
		if constexpr (
			constexpr auto result = cpo_details::choose_load<decltype(target), decltype(args)...>(); result.value == choose::member
			) 
		{
			return NAGISA_STL_FREESTANDING_UTILITY_FORWARD(target).load(NAGISA_STL_FREESTANDING_UTILITY_FORWARD(args)...);
		}
		else if constexpr (result.value == choose::adl) 
		{
			return load(NAGISA_STL_FREESTANDING_UTILITY_FORWARD(target), NAGISA_STL_FREESTANDING_UTILITY_FORWARD(args)...);
		}
	}
};

namespace cpo_details {

	template<class T, class Symbol>
	consteval result choose_load_by_symbol() noexcept
	{
		if constexpr (requires(T target)
		{
			NAGISA_STL_FREESTANDING_UTILITY_FORWARD(target).template load<Symbol>();
		}) {
			return { choose::member, noexcept(::std::declval<T>().template load<Symbol>()) };
		}
		else if constexpr (requires(T target)
		{
			load<Symbol>(NAGISA_STL_FREESTANDING_UTILITY_FORWARD(target));
		}) {
			return { choose::adl, noexcept(load<Symbol>(::std::declval<T>())) };
		}
		else {
			return { choose::none, true };
		}
	}
	template<class Symbol>
	constexpr decltype(auto) call_load_by_symbol(auto&& target)
		noexcept(cpo_details::choose_load_by_symbol<decltype(target), Symbol>().except)
		requires (cpo_details::choose_load_by_symbol<decltype(target), Symbol>().value != choose::none)
	{
		if constexpr (
			constexpr auto result = cpo_details::choose_load_by_symbol<decltype(target), Symbol>();
			result.value == choose::member
		) {
			return NAGISA_STL_FREESTANDING_UTILITY_FORWARD(target).template load<Symbol>();
		}
		else if constexpr (result.value == choose::adl) {
			return load<Symbol>(NAGISA_STL_FREESTANDING_UTILITY_FORWARD(target));
		}
	}
}

inline constexpr struct
{
	constexpr NAGISA_SYMBOL_LOADER_CALL_STATIC decltype(auto) operator()(auto&& context, auto&& symbol_name) NAGISA_SYMBOL_LOADER_CALL_CONST
		noexcept(noexcept(cpo_details::call_load(NAGISA_STL_FREESTANDING_UTILITY_FORWARD(context), NAGISA_STL_FREESTANDING_UTILITY_FORWARD(symbol_name))))
		requires requires { cpo_details::call_load(NAGISA_STL_FREESTANDING_UTILITY_FORWARD(context), NAGISA_STL_FREESTANDING_UTILITY_FORWARD(symbol_name)); }
	{
		return cpo_details::call_load(NAGISA_STL_FREESTANDING_UTILITY_FORWARD(context), NAGISA_STL_FREESTANDING_UTILITY_FORWARD(symbol_name));
	}
}load{};

#undef NAGISA_SYMBOL_LOADER_CALL_STATIC
#undef NAGISA_SYMBOL_LOADER_CALL_CONST

template<class T, class Name>
concept loader = strings::string<Name> && requires(T context, Name symbol_name)
{
	{ load(context, symbol_name) } -> concepts::pointer;
};

template<symbol Symbol>
constexpr decltype(auto) typed_load(auto&& context)
	noexcept(noexcept(load(NAGISA_STL_FREESTANDING_UTILITY_FORWARD(context), symbol_name_v<Symbol>)))
	requires loader<decltype(context), symbol_name_t<Symbol>>
{
	return reinterpret_cast<symbol_type_t<Symbol>*>(
		load(NAGISA_STL_FREESTANDING_UTILITY_FORWARD(context), symbol_name_v<Symbol>)
		);
}

template<symbol Symbol, class Context>
constexpr ::std::same_as<::std::add_pointer_t<symbol_type_t<Symbol>>> decltype(auto) typed_load(Context&& context)
	noexcept(noexcept(cpo_details::call_load_by_symbol<Symbol>(NAGISA_STL_FREESTANDING_UTILITY_FORWARD(context))))
	requires requires { cpo_details::call_load_by_symbol<Symbol>(NAGISA_STL_FREESTANDING_UTILITY_FORWARD(context)); }
{
	return cpo_details::call_load_by_symbol<Symbol>(NAGISA_STL_FREESTANDING_UTILITY_FORWARD(context));
}

NAGISA_BUILD_LIB_DETAIL_END