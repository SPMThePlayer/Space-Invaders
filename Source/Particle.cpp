#include "Particle.h"

void Particle::Update()
{
	float delta_time = GetFrameTime();
	m_position.x += static_cast<int>(m_velocity.x * delta_time);
	m_position.y += static_cast<int>(m_velocity.y * delta_time);

	if (m_fades)
	{
		m_opacity -= static_cast<int>(m_fade_rate * 255.f * delta_time);
		if (m_opacity < 10) m_opacity = 10;

		m_color = { m_color.r, m_color.g, m_color.b, static_cast<uint8_t>(m_opacity) };
	}

	DrawCircle(m_position.x, m_position.y, m_radius , m_color);
}
