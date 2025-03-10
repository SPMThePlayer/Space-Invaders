#pragma once
#include "Globals.h"

class Particle
{
public:
	Particle(Vector2I position, Vector2I velocity, float radius, Color color, bool fades = true,
		float fade_rate = 0.5015f, float lifetime = particle_default_lifetime) :
		m_position(position), m_velocity(velocity), m_radius(radius), m_color(color), m_fades(fades), m_fade_rate(fade_rate),
		m_lifetime(lifetime), m_creation_time(GetTime()) {}

	bool TimeOver() const { return GetTime() - m_creation_time >= m_lifetime || m_opacity <= 10.f; }
	void Update();

	inline void DeclareAsDestroyed() { m_should_be_removed = true; }
	inline bool ShouldBeDestroyed() const { return m_should_be_removed; }
private:
	Vector2I m_position{};
	Vector2I m_velocity{};
	float m_radius{};
	Color m_color{};
	bool m_fades{};
	float m_fade_rate{};
	float m_opacity{255.f};

	double m_creation_time{};
	double m_lifetime{};

	bool m_should_be_removed{};
};