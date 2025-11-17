#pragma once

namespace pb::infra::utility
{
	template<typename T, auto Fun>
	class Guard;

	template<typename T, auto (*Delete)(T*) -> void>
	class [[nodiscard]] Guard<T, Delete>
	{
	public:
		using element_type = T;

	private:
		element_type* pointer_;

	public:
		explicit Guard(T* pointer) noexcept
			: pointer_{pointer} {}

		~Guard() noexcept
		{
			if (pointer_ != nullptr)
			{
				Delete(pointer_);
				pointer_ = nullptr;
			}
		}

		[[nodiscard]] explicit(false) operator T*() const noexcept
		{
			return pointer_;
		}

		Guard(const Guard&) noexcept = delete;
		Guard(Guard&&) noexcept = delete;
		auto operator=(const Guard&) noexcept -> Guard& = delete;
		auto operator=(Guard&&) noexcept -> Guard& = delete;
	};

	template<auto (*Callback)() -> void>
	class [[maybe_unused]] Guard<void, Callback>
	{
	public:
		explicit Guard() noexcept = default;

		~Guard() noexcept
		{
			Callback();
		}

		Guard(const Guard&) noexcept = delete;
		Guard(Guard&&) noexcept = delete;
		auto operator=(const Guard&) noexcept -> Guard& = delete;
		auto operator=(Guard&&) noexcept -> Guard& = delete;
	};
}
