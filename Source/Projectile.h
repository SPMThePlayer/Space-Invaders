#pragma once
#include "Globals.h"


class Projectile
{
public:
	Projectile(Vector2I position, Vector2I velocity, Color color, bool fades, float fade_rate)
		: m_position(position), m_velocity(velocity), m_color(color), m_fades(fades), m_fade_rate(fade_rate) {}

	virtual void Update();
	bool CheckIfOutOfBounds() const;

	Vector2I GetPosition() const { return m_position; }
	Vector2I GetVelocity() const { return m_velocity; }
	
	inline void DeclareAsDestroyed() { m_should_be_removed = true; }
	inline bool ShouldBeDestroyed() const { return m_should_be_removed; }

protected:
	Vector2I m_position{};
	Vector2I m_velocity{};
	Color m_color{};
	bool m_fades{};
	float m_fade_rate{};

	uint8_t m_opacity{};
	bool m_should_be_removed{};
};

class PlayerProjectile : public Projectile
{
public:
	PlayerProjectile(float radius, Vector2I position, Vector2I velocity, Color color, bool fades = true, float fade_rate = 0.5015f)
		: Projectile(position, velocity, color, fades, fade_rate), m_radius(radius) {}

	void Update() override;
	inline float GetRadius() const { return m_radius; }

private:
	float m_radius;
};


class InvaderProjectile : public Projectile
{
public:
	InvaderProjectile(Vector2I size, Vector2I position, Vector2I velocity, Color color, bool fades = true, float fade_rate = 0.5015f)
		: Projectile(position, velocity, color, fades, fade_rate), m_size(size) {}

	void Update() override;

	inline Vector2I GetSize() const { return m_size; }

	inline Rectangle GetRect() const {
		return { static_cast<float>(m_position.x), static_cast<float>(m_position.y), static_cast<float>(m_size.x),
			static_cast<float>(m_size.y) };
	}
private:
	Vector2I m_size;
};