#pragma once

#include <cstdlib>
#include <nagisa/string/string.h>

namespace nagisa::symbol_loader::details
{
	extern "C" struct HINSTANCE__;
	using module_handle_t = HINSTANCE__*;

	extern "C" __declspec(dllimport) module_handle_t __stdcall LoadLibraryA(char const* name);
	extern "C" __declspec(dllimport) void* __stdcall GetProcAddress(module_handle_t module, char const* name);
	extern "C" __declspec(dllimport) int __stdcall FreeLibrary(module_handle_t module);

}

namespace nagisa::symbol_loader::loaders
{
	struct windows_dll_loader
	{
		using self_type = windows_dll_loader;

		static auto create(char const* name) noexcept
		{
			auto handle = details::LoadLibraryA(name);
			if (!handle)
				::std::abort();
			return self_type(handle);
		}

		explicit(true) windows_dll_loader(details::module_handle_t m) noexcept
			: _module(m)
		{
		}
		~windows_dll_loader() noexcept
		{
			details::FreeLibrary(_module);
		}
		constexpr windows_dll_loader(self_type const&) noexcept = delete;
		constexpr self_type& operator=(self_type const&) noexcept = delete;

		auto load(char const* name) const noexcept
		{
			return details::GetProcAddress(_module, name);
		}
		auto load(strings::character_range auto const& name) const noexcept
			requires requires { {strings::c_str(name) } -> ::std::convertible_to<char const*>; }
		{
			return self_type::load(strings::c_str(name));
		}

		details::module_handle_t _module;
	};
}