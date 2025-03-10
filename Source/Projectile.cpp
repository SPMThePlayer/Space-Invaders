#include "Projectile.h"

//Projectile Base Class
void Projectile::Update()
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
}

bool Projectile::CheckIfOutOfBounds() const
{
	return m_position.x < 0 || m_position.x > GetScreenWidth() ||
		m_position.y < 0 || m_position.y > GetScreenHeight();
}

//Projectile Class For Player
void PlayerProjectile::Update()
{
	DrawCircle(m_position.x, m_position.y, m_radius, m_color);
	Projectile::Update();
}

//Projectile Class For Invader
void InvaderProjectile::Update()
{
	float rotation = (m_velocity.x > 0) ? 15.f : (m_velocity.x < 0) ? -15.f : 0.f;

	DrawRectanglePro(
		{ static_cast<float>(m_position.x), static_cast<float>(m_position.y), static_cast<float>(m_size.x), static_cast<float>(m_size.y) }, 
		{ m_size.x / 2.f, m_size.y / 2.f }, rotation, m_color);

	Projectile::Update();
}
