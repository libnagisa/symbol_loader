#pragma once

#include "./detail/symbol.h"
#include "./detail/loader.h"
#include "./environment.h"

NAGISA_BUILD_LIB_BEGIN

using details::symbol_type;
using details::symbol_type_t;
using details::symbol_name;
using details::symbol_name_v;
using details::symbol_name_t;
using details::symbol;
using details::meta_symbol;

using details::load;
using details::loader;
using details::typed_load;

NAGISA_BUILD_LIB_END