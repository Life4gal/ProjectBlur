// This file is part of ProjectBlur
// Copyright (C) 2022-2025 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#pragma once

#include <ranges>
#include <type_traits>
#include <string_view>
#include <source_location>

#include <pb/macro.hpp>

// ReSharper disable once CppInconsistentNaming
struct _pb_meta_name_DO_NOT_USE{};

namespace pb::infra::meta
{
	namespace name_detail
	{
		template<typename...>
		struct unused_type {};

		// template<auto...>
		// struct unused_value {};
	}

	template<typename... Ts>
	[[nodiscard]] constexpr auto get_full_function_name() noexcept -> std::string_view
	{
		// [[maybe_unused]] constexpr name_detail::unused_type<Ts...> _{};
		std::ignore = name_detail::unused_type<Ts...>{};
		return std::source_location::current().function_name();
	}

	template<auto... Vs>
	[[nodiscard]] constexpr auto get_full_function_name() noexcept -> std::string_view
	{
		// [[maybe_unused]] constexpr name_detail::unused_value<Vs...> _{};
		((std::ignore = Vs), ...);
		return std::source_location::current().function_name();
	}

	template<typename T>
	[[nodiscard]] constexpr auto name_of() noexcept -> std::string_view
	{
		#if defined(PB_COMPILER_MSVC)
		// MSVC
		// const class std::basic_string_view<char, struct std::char_traits<char>> `__calling_convention` `namespace`::get_full_function_name<struct `_pb_meta_name_DO_NOT_USE` >(void)
		constexpr std::string_view full_function_name = get_full_function_name<_pb_meta_name_DO_NOT_USE>();
		constexpr auto full_function_name_size = full_function_name.size();

		constexpr std::string_view dummy_struct_name{"struct _pb_meta_name_DO_NOT_USE"};
		constexpr auto dummy_struct_name_size = std::ranges::size(dummy_struct_name);

		// const class std::basic_string_view<char, struct std::char_traits<char>> `__calling_convention` `namespace`::get_full_function_name<
		constexpr auto full_function_name_prefix_size = full_function_name.find(dummy_struct_name);
		static_assert(full_function_name_prefix_size != std::string_view::npos);

		// >(void)
		constexpr auto full_function_name_suffix_size = full_function_name_size - full_function_name_prefix_size - dummy_struct_name_size;
		#elif defined(PB_COMPILER_APPLE_CLANG) or defined(PB_COMPILER_CLANG_CL) or defined(PB_COMPILER_CLANG)
		// CLANG/CLANG-CL
		// std::string_view `namespace`::get_full_function_name() [T = `_pb_meta_name_DO_NOT_USE`]
		constexpr std::string_view full_function_name = get_full_function_name<_pb_meta_name_DO_NOT_USE>();
		constexpr auto full_function_name_size = full_function_name.size();

		constexpr std::string_view dummy_struct_name{"_pb_meta_name_DO_NOT_USE"};
		constexpr auto dummy_struct_name_size = std::ranges::size(dummy_struct_name);

		// std::string_view `namespace`::get_full_function_name() [T =
		constexpr auto full_function_name_prefix_size = full_function_name.find(dummy_struct_name);
		static_assert(full_function_name_prefix_size != std::string_view::npos);

		// ]
		constexpr auto full_function_name_suffix_size = full_function_name_size - full_function_name_prefix_size - dummy_struct_name_size;
		#else
		// GCC
		// constexpr std::string_view `namespace`::get_full_function_name() [with <template-parameter-1-1> = `_pb_meta_name_DO_NOT_USE`; std::string_view = std::basic_string_view<char>]
		constexpr std::string_view full_function_name = get_full_function_name<_pb_meta_name_DO_NOT_USE>();
		constexpr auto full_function_name_size = full_function_name.size();

		constexpr std::string_view dummy_struct_name{"_pb_meta_name_DO_NOT_USE"};
		constexpr auto dummy_struct_name_size = std::ranges::size(dummy_struct_name);

		// constexpr std::string_view `namespace`::get_full_function_name() [with <template-parameter-1-1> =
		constexpr auto full_function_name_prefix_size = full_function_name.find(dummy_struct_name);
		static_assert(full_function_name_prefix_size != std::string_view::npos);

		// ; std::string_view = std::basic_string_view<char>]
		constexpr auto full_function_name_suffix_size = full_function_name_size - full_function_name_prefix_size - dummy_struct_name_size;
		#endif

		auto full_name = get_full_function_name<std::decay_t<T>>();
		full_name.remove_prefix(full_function_name_prefix_size);
		full_name.remove_suffix(full_function_name_suffix_size);
		return full_name;
	}
}
