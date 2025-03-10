#include "Invader.h"

//Invader
Invader::Invader(Vector2I position, Vector2I velocity, const Texture* texture)
	: m_position(position), m_velocity(velocity), m_texture(texture)
{
	m_size.x = texture->width;
	m_size.y = texture->height;
}

InvaderManager::~InvaderManager()
{
	UnloadTexture(m_invader_texture);
}

void Invader::Update()
{
	float delta_time = GetFrameTime();
	m_position.x += static_cast<int>(m_velocity.x * delta_time);
	DrawTexture(*m_texture, m_position.x, m_position.y, WHITE);
}

void Invader::MoveDown()
{
	m_position.y += m_size.y;
}

void Invader::Shoot(std::vector<InvaderProjectile>& projectiles)
{
	Vector2I shoot_position = { m_position.x - (m_size.x / 2), m_position.y + m_size.y };
	projectiles.emplace_back(Vector2I{ 4, 10 }, shoot_position, Vector2I{0, invader_projectile_velocity}, 
		invader_bullet_colors[GetRandomValue(0, number_of_invader_bullet_colors - 1)], false);
}

//Invader Manager Class
void InvaderManager::Begin()
{
	MakeTextureFromRawBuffer(m_invader_texture, invader_data, invader_size, invader_width, invader_height, invader_scale);
	CreateNewInvaders();
}

void InvaderManager::Update(int player_score)
{
	for (auto& invader : m_invaders)
	{
		invader.Update();
	}

	for (auto& projectile : m_invader_projectiles)
	{
		if (projectile.CheckIfOutOfBounds())
		{
			projectile.DeclareAsDestroyed();
			continue;
		}
		projectile.Update();
	}

	if (!m_invaders.empty())
	{
		if (GetTime() - m_last_fire_time > m_fire_interval)
		{
			m_last_fire_time = GetTime();

			int number_of_bullets = static_cast<int>(player_score / 2000) + 1;

			for(int i = 0; i < number_of_bullets; i++)
			{
				m_invaders[GetRandomValue(0, static_cast<int>(m_invaders.size()) - 1)].Shoot(m_invader_projectiles);
				ms_sound_manager.PlayStoredSound(SoundType::Shoot);
			}
		}

		Vector2I min_coords{GetScreenWidth(), GetScreenHeight()};
		Vector2I max_coords{0, 0};


		for (const auto& invader : m_invaders)
		{
			int invader_right = invader.GetPosition().x + invader.GetSize().x;
			int invader_left = invader.GetPosition().x;

			if (invader_left < min_coords.x) 
				min_coords.x = invader_left;

			if (invader_right > max_coords.x) 
				max_coords.x = invader_right;

			int invader_top = invader.GetPosition().y;
			int invader_bottom = invader.GetPosition().y + invader.GetSize().y;

			if (invader_top < min_coords.y) 
				min_coords.y = invader_top;

			if (invader_bottom > max_coords.y)
				max_coords.y = invader_bottom;
		}

		m_size = { max_coords.x - min_coords.x, max_coords.y - min_coords.y };
		m_position = { min_coords.x, min_coords.y };

        m_rect = { static_cast<float>(m_position.x), static_cast<float>(m_position.y), static_cast<float>(m_size.x), 
			static_cast<float>(m_size.y) };


		//DrawRectangleLines(m_position.x, m_position.y, m_size.x, m_size.y, GREEN);

		if (max_coords.x >= GetScreenWidth() || min_coords.x < 0)
		{
			for (auto& invader : m_invaders)
			{
				invader.ReverseVelocityX();
				invader.MoveDown();
			}
		}

		m_invaders.erase(
			std::remove_if(m_invaders.begin(), m_invaders.end(),
				[&](const Invader& invader) {
					return invader.ShouldBeDestroyed();
				}),
			m_invaders.end());
	}


	if (!m_invader_projectiles.empty())
	{
		auto new_end = std::remove_if(m_invader_projectiles.begin(), m_invader_projectiles.end(),
			[](const InvaderProjectile& projectile)
			{
				return projectile.ShouldBeDestroyed();
			});

		m_invader_projectiles.erase(new_end, m_invader_projectiles.end());
	}

	if (m_invaders.empty())
	{
		CreateNewInvaders();
	}


}

void InvaderManager::Reset()
{
	m_position = { 0, 0 };
	m_size = { 0, 0 };
	m_rect = { 0, 0 };
	m_columns = 0;
	m_rows = 0;
	m_last_fire_time = 0;

	m_invaders.clear();
	m_invaders.shrink_to_fit();

	m_invader_projectiles.clear();
	m_invader_projectiles.shrink_to_fit();

	CreateNewInvaders();

}

void InvaderManager::CreateNewInvaders()
{
	m_columns = GetRandomValue(3, 17);
	m_rows = GetRandomValue(3, 7);

	m_invaders.clear();
	m_invaders.reserve(m_columns * m_rows);

	for (int i = 0; i < m_columns; i++)
	{
		for (int j = 0; j < m_rows; j++)
		{
			m_invaders.emplace_back(Vector2I{ i * (m_invader_texture.width + 5), j * m_invader_texture.height }, Vector2I{ invader_velocity, 0 }, &m_invader_texture);
		}
	}
}