#pragma once

#include <cstdlib>
#include <nagisa/string/string.h>

namespace nagisa::symbol_loader::details
{
	extern "C" void* dlopen(const char* filename, int flag);
	extern "C" void* dlsym(void* handle, const char* symbol);
	extern "C" int dlclose(void* handle);
	extern "C" char* dlerror(void);

}

namespace nagisa::symbol_loader::loaders
{

	struct apple_dll_loader
	{
		struct flag
		{
			enum type : int {};
			constexpr static auto lazy = static_cast<type>(1 << 0);
			constexpr static auto now = static_cast<type>(1 << 1);
			constexpr static auto no_load = static_cast<type>(1 << 2);
			constexpr static auto deep_bind = static_cast<type>(1 << 3);
			constexpr static auto no_delete = static_cast<type>(1 << 12);
			constexpr static auto binding_mask = static_cast<type>(lazy | now);
		};
		

		using self_type = apple_dll_loader;

		static auto create(char const* name, flag::type f) noexcept
		{
			auto handle = details::dlopen(name, ::std::to_underlying(f));
			if (!handle)
				::std::abort();
			return self_type(handle);
		}

		explicit(true) apple_dll_loader(void* m) noexcept
			: _module(m)
		{
		}
		~apple_dll_loader() noexcept
		{
			details::dlclose(_module);
		}
		constexpr apple_dll_loader(self_type const&) noexcept = delete;
		constexpr self_type& operator=(self_type const&) noexcept = delete;

		auto load(char const* name) const noexcept
		{
			return details::dlsym(_module, name);
		}
		auto load(strings::character_range auto const& name) const noexcept
			requires requires { {strings::c_str(name) } -> ::std::convertible_to<char const*>; }
		{
			return self_type::load(strings::c_str(name));
		}

		void* _module;
	};
}
