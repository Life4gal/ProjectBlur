#pragma once

#include <algorithm>
#include <numbers>
#include <ranges>

#include <glm/vec2.hpp>
#include <glm/common.hpp>
#include <glm/trigonometric.hpp>

namespace pb::math
{
	class Angle
	{
	public:
		using value_type = float;

		constexpr static auto radians_to_degrees = 180.f / std::numbers::pi_v<value_type>;
		constexpr static auto degrees_to_radians = std::numbers::pi_v<value_type> / 180.f;

	private:
		value_type degrees_;

		constexpr explicit Angle(const value_type degrees) noexcept
			: degrees_(degrees) {}

	public:
		// ============================================================
		// CTOR
		// ============================================================

		[[nodiscard]] constexpr static auto from_degrees(const value_type degrees) noexcept -> Angle
		{
			return Angle{degrees};
		}

		[[nodiscard]] constexpr static auto from_radians(const value_type radians) noexcept -> Angle
		{
			return Angle{radians * radians_to_degrees};
		}

		[[nodiscard]] /*constexpr*/ static auto from_position(const glm::vec2& from, const glm::vec2& to) noexcept -> Angle
		{
			const auto delta = to - from;
			const auto radians = glm::atan(delta.y, delta.x);

			auto degrees = from_radians(radians).to_degrees();
			if (degrees < 0)
			{
				degrees += 360.f;
			}

			return from_degrees(degrees);
		}

		[[nodiscard]] /*constexpr*/ static auto from_position(const value_type x1, const value_type y1, const value_type x2, const value_type y2) noexcept -> Angle
		{
			return from_position({x1, y1}, {x2, y2});
		}

		[[nodiscard]] /*constexpr*/ static auto from_position(const glm::vec2& to) noexcept -> Angle
		{
			return from_position({0, 0}, to);
		}

		[[nodiscard]] /*constexpr*/ static auto from_position(const value_type x, const value_type y) noexcept -> Angle
		{
			return from_position({0, 0}, {x, y});
		}

		[[nodiscard]] /*constexpr*/ static auto from_direction(const glm::vec2& direction) noexcept -> Angle
		{
			return from_radians(glm::atan(direction.x, direction.y));
		}

		[[nodiscard]] /*constexpr*/ static auto from_direction(const value_type x, const value_type y) noexcept -> Angle
		{
			return from_direction({x, y});
		}

		// ============================================================
		// CONSTANT
		// ============================================================

		[[nodiscard]] constexpr static auto zero() noexcept -> Angle
		{
			return from_degrees(0.f);
		}

		[[nodiscard]] constexpr static auto quarter() noexcept -> Angle
		{
			return from_degrees(90.f);
		}

		[[nodiscard]] constexpr static auto half() noexcept -> Angle
		{
			return from_degrees(180.f);
		}

		[[nodiscard]] constexpr static auto full() noexcept -> Angle
		{
			return from_degrees(360.f);
		}

		[[nodiscard]] constexpr static auto up() noexcept -> Angle
		{
			return zero();
		}

		[[nodiscard]] constexpr static auto right() noexcept -> Angle
		{
			return quarter();
		}

		[[nodiscard]] constexpr static auto down() noexcept -> Angle
		{
			return half();
		}

		[[nodiscard]] constexpr static auto left() noexcept -> Angle
		{
			return from_degrees(270.f);
		}

		// ============================================================
		// OPERATION
		// ============================================================

		[[nodiscard]] constexpr auto operator<=>(const Angle& other) const noexcept -> std::partial_ordering = default;

		[[nodiscard]] constexpr auto operator+(const Angle& other) const noexcept -> Angle
		{
			return from_degrees(degrees_ + other.degrees_);
		}

		[[nodiscard]] constexpr auto operator-(const Angle& other) const noexcept -> Angle
		{
			return from_degrees(degrees_ - other.degrees_);
		}

		[[nodiscard]] constexpr auto operator*(const value_type scalar) const noexcept -> Angle
		{
			return from_degrees(degrees_ * scalar);
		}

		[[nodiscard]] constexpr auto operator/(const value_type scalar) const noexcept -> Angle
		{
			return from_degrees(degrees_ / scalar);
		}

		// ============================================================
		// CONVERSION
		// ============================================================

		[[nodiscard]] constexpr auto to_degrees() const noexcept -> value_type
		{
			return degrees_;
		}

		[[nodiscard]] constexpr auto to_radians() const noexcept -> value_type
		{
			return degrees_ * degrees_to_radians;
		}

		[[nodiscard]] constexpr auto to_normalized() const noexcept -> Angle
		{
			return from_degrees(glm::mod(degrees_, 360.f));
		}

		[[nodiscard]] constexpr auto to_signed_normalized() const noexcept -> Angle
		{
			auto normalized = to_normalized().to_degrees();
			if (normalized > 180.0f)
			{
				normalized -= 360.0f;
			}

			return from_degrees(normalized);
		}

		// ============================================================
		// ANGLE
		// ============================================================

		[[nodiscard]] constexpr auto is_acute() const noexcept -> bool
		{
			return to_normalized().to_degrees() < 90.0f;
		}

		[[nodiscard]] constexpr auto is_obtuse() const noexcept -> bool
		{
			const auto degrees = to_normalized().to_degrees();
			return degrees > 90.0f and degrees < 180.0f;
		}

		[[nodiscard]] constexpr auto is_reflex() const noexcept -> bool
		{
			return to_normalized().to_degrees() > 180.0f;
		}

		// ============================================================
		// DISTANCE
		// ============================================================

		[[nodiscard]] constexpr auto clockwise_distance(const Angle to) const noexcept -> value_type
		{
			return glm::abs(to.to_degrees() - to_degrees());
		}

		[[nodiscard]] constexpr auto counter_clockwise_distance(const Angle to) const noexcept -> value_type
		{
			return 360.f - clockwise_distance(to);
		}

		[[nodiscard]] constexpr auto shortest_distance(const Angle to) const noexcept -> value_type
		{
			const auto diff = glm::abs(to_degrees() - to.to_degrees());
			return glm::min(diff, 360.0f - diff);
		}

		[[nodiscard]] constexpr auto within(const Angle other, const value_type margin) const noexcept -> bool
		{
			return shortest_distance(other) <= margin;
		}

#ifdef PB_PLATFORM_WINDOWS
#pragma push_macro("near")
#undef near
#endif

		[[nodiscard]] constexpr auto near(const Angle other, const value_type range) const noexcept -> bool
		{
			return shortest_distance(other) < range;
		}

#ifdef PB_PLATFORM_WINDOWS
#pragma pop_macro("near")
#endif

		[[nodiscard]] constexpr auto move_toward(const Angle target, const value_type speed) const noexcept -> Angle
		{
			if (const auto distance = shortest_distance(target);
				distance < speed)
			{
				return target;
			}

			if (const auto diff = glm::mod(target.to_degrees() - to_degrees() + 180.f, 360.f) - 180.f;
				diff >= 0)
			{
				return from_degrees(to_degrees() + speed);
			}

			return from_degrees(to_degrees() - speed);
		}

		// ============================================================
		// VALUE
		// ============================================================

		[[nodiscard]] constexpr auto clamp(const Angle dest, const value_type range) const noexcept -> Angle
		{
			const auto distance = shortest_distance(dest);
			return distance <= range ? *this : move_toward(dest, distance - range);
		}

		// ReSharper disable once IdentifierTypo
		[[nodiscard]] constexpr auto lerp(const Angle dest, const value_type t) const noexcept -> Angle
		{
			const auto shortest = shortest_distance(dest);
			const auto direction = glm::mod(dest.to_degrees() - to_degrees() + 180.f, 360.f) - 180.f >= 0 ? 1.f : -1.f;

			return from_degrees(to_degrees() + direction * shortest * t);
		}

		// ReSharper disable once IdentifierTypo
		[[nodiscard]] constexpr auto slerp(const Angle dest, const value_type t) const noexcept -> Angle
		{
			const auto from = to_radians();
			const auto to = dest.to_radians();

			const auto diff = to - from;
			const auto diff_abs = glm::abs(diff);

			const auto shortest =
					diff_abs < std::numbers::pi_v<value_type>
						? diff
						: (
							diff > 0
								? diff - std::numbers::pi_v<value_type> * 2.f
								: diff + std::numbers::pi_v<value_type> * 2.f
						);

			return from_radians(from + shortest * t);
		}

		// ============================================================
		// COORDINATE
		// ============================================================

		[[nodiscard]] /*constexpr*/ auto sin() const noexcept -> value_type
		{
			return glm::sin(to_radians());
		}

		[[nodiscard]] /*constexpr*/ auto cos() const noexcept -> value_type
		{
			return glm::cos(to_radians());
		}

		[[nodiscard]] /*constexpr*/ auto to_cartesian(const value_type length = 1.f) const noexcept -> glm::vec2
		{
			return {length * sin(), length * cos()};
		}

		[[nodiscard]] /*constexpr*/ auto rotate_point(const glm::vec2& point) const noexcept -> glm::vec2
		{
			const auto radians = to_radians();
			const auto cos = glm::cos(radians);
			const auto sin = glm::sin(radians);

			const auto x = point.x * cos - point.y * sin;
			const auto y = point.x * sin + point.y * cos;

			return {x, y};
		}

		// ============================================================
		// VECTOR
		// ============================================================

	private:
		template<typename Collector>
		[[nodiscard]] consteval static auto is_valid_collector([[maybe_unused]] Collector&& collector) noexcept -> bool
		{
			return requires { collector(value_type{}, value_type{}); };
		}

		template<typename Container>
		[[nodiscard]] consteval static auto is_valid_container([[maybe_unused]] Container& container) noexcept -> bool
		{
			return
					requires { container.emplace_back({value_type{}, value_type{}}); } or
					requires { container.push_back({value_type{}, value_type{}}); } or
					requires { container.emplace({value_type{}, value_type{}}); } or
					requires { container.push({value_type{}, value_type{}}); } or
					requires { container.append({value_type{}, value_type{}}); } or
					requires { container.set({value_type{}, value_type{}}); };
		}

		template<typename Container>
		[[nodiscard]] constexpr static auto make_collector(Container& container) noexcept -> auto
		{
			return [&container](const value_type x, const value_type y) noexcept -> void
			{
				if constexpr (requires { container.emplace_back({x, y}); })
				{
					container.emplace_back({x, y});
				}
				else if constexpr (requires { container.push_back({x, y}); })
				{
					container.push_back({x, y});
				}
				else if constexpr (requires { container.emplace({x, y}); })
				{
					container.emplace({x, y});
				}
				else if constexpr (requires { container.push({x, y}); })
				{
					container.push({x, y});
				}
				else if constexpr (requires { container.append({x, y}); })
				{
					container.append({x, y});
				}
				else if constexpr (requires { container.set({x, y}); })
				{
					container.set({x, y});
				}
			};
		}

	public:
		template<typename Collector>
		constexpr static auto circle_vector(const std::size_t points, const value_type length, const value_type offset, Collector&& collector) noexcept -> void //
			requires(Angle::is_valid_collector(collector))
		{
			std::ranges::for_each(
				std::views::iota(points),
				[points, length, offset, c = std::forward<Collector>(collector)](const auto index) noexcept -> void
				{
					const auto angle = offset + from_degrees(static_cast<value_type>(index) * (360.f / static_cast<value_type>(points)));
					c(angle.to_cartesian_x(length), angle.to_cartesian_y(length));
				}
			);
		}

		template<typename Collector>
		constexpr static auto circle_vector(const std::size_t points, const value_type length, Collector&& collector) noexcept -> void //
			requires(Angle::is_valid_collector(collector))
		{
			return Angle::circle_vector(points, length, value_type{0}, std::forward<Collector>(collector));
		}

		template<typename Container>
		constexpr static auto circle_vector(const std::size_t points, const value_type length, const value_type offset, Container& container) noexcept -> void //
			requires(Angle::is_valid_container(container))
		{
			auto insertor = Angle::make_collector(container);
			return Angle::circle_vector(points, length, offset, insertor);
		}

		template<typename Container>
		constexpr static auto circle_vector(const std::size_t points, const value_type length, Container& container) noexcept -> void //
			requires(Angle::is_valid_container(container))
		{
			return Angle::circle_vector(points, length, value_type{0}, container);
		}
	};
}
