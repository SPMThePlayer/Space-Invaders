#include "Player.h"

Player::~Player()
{
	UnloadTexture(m_texture);
}

void Player::Begin()
{
	MakeTextureFromRawBuffer(m_texture, spaceship_data, spaceship_size, spaceship_width, spaceship_height, spaceship_scale);

	m_position.x = (GetScreenWidth() / 2) - ((m_size.x) / 2);
	m_position.y = (GetScreenHeight() - 50) - ((m_size.y) / 2);
	m_size = { m_texture.width, m_texture.height };

}

void Player::Update()
{
	m_screen_width = GetScreenWidth();
	m_screen_height = GetScreenHeight();
	m_delta_time = GetFrameTime();


	if (IsKeyDown(KEY_A))
	{
		if (m_position.x > 0 + m_size.x)
		{
			m_position.x -= static_cast<int>(m_velocity * m_delta_time);
			m_rotation = -15;
		}
	}
	else if (IsKeyDown(KEY_D))
	{
		if (m_position.x < m_screen_width)
		{
			m_position.x += static_cast<int>(m_velocity * m_delta_time);
			m_rotation = +15;
		}
	}
	else
	{
		m_rotation = 0;
	}

	ManageProjectiles();

    Rectangle source = { 0.f, 0.f, static_cast<float>(m_size.x), static_cast<float>(m_size.y) };

    Rectangle dest = { static_cast<float>(m_position.x) - (static_cast<float>(m_size.x) * 0.5f),
		static_cast<float>(m_position.y) - (static_cast<float>(m_size.y) * 0.5f), static_cast<float>(m_size.x), static_cast<float>(m_size.y) };

    Vector2 pivot = { static_cast<float>(m_size.x) / 2.f, static_cast<float>(m_size.y) / 2.f };

	if(!m_hidden)
		DrawTexturePro(m_texture, source, dest, pivot, m_rotation, WHITE);
}

void Player::ManageProjectiles()
{
	if (IsKeyDown(KEY_SPACE) && m_shooting)
	{
		if (GetTime() - last_fire_time > player_fire_interval)
		{
			int projectile_x_velocity = (m_rotation > 0) ?
				(player_projectile_velocity / 2) : (m_rotation < 0) ?
				(-player_projectile_velocity / 2) : 0;

			m_player_projectiles.emplace_back(5.f,
				Vector2I{ m_position.x - m_size.x / 2, m_position.y - m_size.x },
				Vector2I{ projectile_x_velocity, -player_projectile_velocity },
				Color{ 255, 0, 0, 255 }
				);


			last_fire_time = GetTime();

			ms_sound_manager.PlayStoredSound(SoundType::Shoot);
		}
	}

	for (auto& projectile : m_player_projectiles)
	{
		if (projectile.CheckIfOutOfBounds())
		{
			projectile.DeclareAsDestroyed();
			continue;
		}
		projectile.Update();
	}

	if (!m_player_projectiles.empty())
	{
		auto new_end = std::remove_if(m_player_projectiles.begin(), m_player_projectiles.end(),
			[](const PlayerProjectile& projectile) {
				return projectile.ShouldBeDestroyed();
			});

		m_player_projectiles.erase(new_end, m_player_projectiles.end());
	}


}

void Player::Reset()
{
	m_position.x = (GetScreenWidth() / 2) - ((m_size.x) / 2);
	m_position.y = (GetScreenHeight() - 50) - ((m_size.y) / 2);
	m_size = { m_texture.width, m_texture.height };
	last_fire_time = 0;

	m_player_projectiles.clear();
	m_player_projectiles.shrink_to_fit();

	Show();
	SetShooting(true);
}

Vector2 Player::RotatePoint(Vector2 point, Vector2 pivot, float angle)
{
	float radians = angle * (PI / 180.0f); // Convert to radians
	float cos_theta = cos(radians);
	float sin_theta = sin(radians);

	float x = pivot.x + (point.x - pivot.x) * cos_theta - (point.y - pivot.y) * sin_theta;
	float y = pivot.y + (point.x - pivot.x) * sin_theta + (point.y - pivot.y) * cos_theta;

	return { x, y };
}

Vector2I Player::GetPosition()
{
	Rectangle bounding_box = GetActualBoundingBox(false);
	return Vector2I
	{
		static_cast<int>(bounding_box.x),
		static_cast<int>(bounding_box.y)
	};
}

Rectangle Player::GetActualBoundingBox(bool account_for_pivot)
{
	// Destination Rectangle (before rotation)
    Rectangle dest = { static_cast<float>(m_position.x) - m_size.x * 0.5f, static_cast<float>(m_position.y) - m_size.y * 0.5f, 
		static_cast<float>(m_size.x), static_cast<float>(m_size.y) };

    // Pivot point in world space
    Vector2 pivot = { static_cast<float>(m_position.x), static_cast<float>(m_position.y) };

	// Get unrotated corners
	Vector2 top_left = { dest.x, dest.y };
	Vector2 top_right = { dest.x + dest.width, dest.y };
	Vector2 bottom_left = { dest.x, dest.y + dest.height };
	Vector2 bottom_right = { dest.x + dest.width, dest.y + dest.height };

	// Rotate corners around pivot
	top_left = RotatePoint(top_left, pivot, m_rotation);
	top_right = RotatePoint(top_right, pivot, m_rotation);
	bottom_left = RotatePoint(bottom_left, pivot, m_rotation);
	bottom_right = RotatePoint(bottom_right, pivot, m_rotation);

	// Find min/max to form the bounding box
	float min_x = std::min({ top_left.x, top_right.x, bottom_left.x, bottom_right.x });
	float max_x = std::max({ top_left.x, top_right.x, bottom_left.x, bottom_right.x });
	float min_y = std::min({ top_left.y, top_right.y, bottom_left.y, bottom_right.y });
	float max_y = std::max({ top_left.y, top_right.y, bottom_left.y, bottom_right.y });

	if (account_for_pivot)
		return { min_x - (m_size.x / 2.f), min_y - (m_size.y / 2.f), max_x - min_x, max_y - min_y };
	else
		return { min_x, min_y, max_x - min_y, max_y - min_y };
}

