#pragma once

#include <pb/math/angle.hpp>

#include <glm/geometric.hpp>

namespace pb::math
{
	class Position
	{
	public:
		using value_type = float;

	private:
		glm::vec2 position_;

	public:
		constexpr explicit Position(const glm::vec2& position) noexcept
			: position_{position} {}

		constexpr Position(const value_type x, const value_type y) noexcept
			: Position{{x, y}} {}

		constexpr Position() noexcept = default;

		[[nodiscard]] constexpr auto x() const noexcept -> value_type
		{
			return position_.x;
		}

		[[nodiscard]] constexpr auto y() const noexcept -> value_type
		{
			return position_.y;
		}

		[[nodiscard]] constexpr explicit (false) operator glm::vec2() const noexcept
		{
			return position_;
		}

		[[nodiscard]] /*constexpr*/ auto angle_to(const Position& other) const noexcept -> Angle
		{
			return Angle::from_position(position_, other.position_);
		}

		[[nodiscard]] /*constexpr*/ auto angle_to(const value_type x, const value_type y) const noexcept -> Angle
		{
			return angle_to(Position{x, y});
		}

		[[nodiscard]] constexpr auto distance_2(const Position& other) const noexcept -> value_type
		{
			const auto delta = other.position_ - position_;
			return glm::dot(delta, delta);
		}

		[[nodiscard]] constexpr auto distance_2(const value_type x, const value_type y) const noexcept -> value_type
		{
			return distance_2(Position{x, y});
		}

		[[nodiscard]] /*constexpr*/ auto distance(const Position& other) const noexcept -> value_type
		{
			return glm::sqrt(distance_2(other));
		}

		[[nodiscard]] /*constexpr*/ auto distance(const value_type x, const value_type y) const noexcept -> value_type
		{
			return distance(Position{x, y});
		}
	};
}
