// This file is part of ProjectBlur
// Copyright (C) 2022-2025 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#pragma once

#if __has_include(<debugging>)
#include <debugging>
#define HAS_STD_DEBUGGING
#endif

#include <pb/platform/exception.hpp>

namespace pb::infra::platform
{
	[[nodiscard]] auto os_error_reason() -> std::string;

	class OsError final : public Exception<void>
	{
	public:
		using Exception::Exception;

		[[noreturn]] static auto panic(
			const std::source_location& location = std::source_location::current(),
			std::stacktrace stacktrace = std::stacktrace::current()
		) noexcept(false) -> void //
		{
			platform::panic<OsError>(os_error_reason(), location, std::move(stacktrace));
		}
	};

	[[nodiscard]] auto is_debugger_present() noexcept -> bool;

	auto write_debug_message(std::string_view message) noexcept -> void;
}
