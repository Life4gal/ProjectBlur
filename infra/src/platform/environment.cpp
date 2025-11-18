// This file is part of ProjectBlur
// Copyright (C) 2022-2025 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#include <pb/platform/environment.hpp>

#include <span>

#include <pb/macro.hpp>

#include <pb/platform/os.hpp>

#if defined(PB_PLATFORM_WINDOWS)
#include <cstdlib>
#endif

namespace
{
#if defined(PB_COMPILER_MSVC)
	const int g_argc = *__p___argc();
	const char* const* g_argv = *__p___argv();
#else
	int g_argc = 0;
	const char** g_argv = nullptr;

	__attribute__ ((constructor)) auto read_command_args(const int argc, const char* argv[]) -> void
	{
		g_argc = argc;
		g_argv = argv;
	}
#endif
}

namespace pb::infra::platform
{
	auto command_args() noexcept -> std::span<const char* const>
	{
		PB_ERROR_DEBUG_ASSUME(g_argc != 0);
		PB_ERROR_DEBUG_ASSUME(g_argv != nullptr);

		return {g_argv, static_cast<std::span<const char* const>::size_type>(g_argc)};
	}
}
