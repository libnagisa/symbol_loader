#pragma once

#include "./environment.h"

NAGISA_BUILD_LIB_DETAIL_BEGIN

namespace cpo_details
{
	enum class choose
	{
		none,
		member,
		adl,
	};
	struct result
	{
		choose value;
		bool except;
	};
}

NAGISA_BUILD_LIB_DETAIL_END