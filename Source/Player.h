#pragma once
#include "Globals.h"
#include "Projectile.h"
#include "SoundManager.h"

class Player
{
public:
	Player(const SoundManager& sound_manager) : ms_sound_manager(sound_manager) {}
	~Player();

	void Begin();
	void Update();
	void ManageProjectiles();
	void Reset();

	std::vector<PlayerProjectile>& GetPlayerProjectiles() { return m_player_projectiles; }

	inline Vector2I GetSize() const { return m_size; }
	Vector2I GetPosition();
	Rectangle GetActualBoundingBox(bool account_for_pivot);

	inline void SetShooting(bool value) { m_shooting = value; }

	inline void Hide() { m_hidden = true; }
	inline void Show() { m_hidden = false; }

private:

	Vector2 RotatePoint(Vector2 point, Vector2 pivot, float angle);


private:
	const SoundManager& ms_sound_manager;

	int m_screen_width{};
	int m_screen_height{};
	float m_delta_time{};

	Texture m_texture{};
	Vector2I m_position{};
	Vector2I m_size{};
	int m_velocity = 500;
	float m_rotation = 0;
	bool m_hidden{};
	bool m_shooting{true};

	std::vector<PlayerProjectile> m_player_projectiles;
	double last_fire_time = 0;
};