#include <nagisa/symbol_loader/symbol_loader.h>
#include <nagisa/symbol_loader/loader/windows.h>
#include <fast_io.h>
int main()
{
	namespace sl = ::nagisa::symbol_loader;

	auto loader = sl::loaders::windows_dll_loader::create("opengl32.dll");
	using symbol = sl::meta_symbol<void(unsigned int context, unsigned int target), "glBindBuffer">;
	auto result = sl::typed_load<symbol>(loader);

	::fast_io::io::println(::std::bit_cast<::std::uintptr_t>(result));

	return 0;
}