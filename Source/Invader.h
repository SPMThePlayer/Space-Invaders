#pragma once
#include "Globals.h"
#include "Projectile.h"
#include "Particle.h"
#include "SoundManager.h"

class Invader
{
public:
	Invader(Vector2I position, Vector2I velocity, const Texture* texture);
	void Update();
	void MoveDown();
	void Shoot(std::vector<InvaderProjectile>& projectiles);
	inline void ReverseVelocityX() { m_velocity.x *= -1; }

	inline Vector2I GetPosition() const { return m_position; }
	inline Vector2I GetVelocity() const { return m_velocity; }

	inline void DeclareAsDestroyed() { m_should_be_removed = true; }
	inline bool ShouldBeDestroyed() const { return m_should_be_removed; }

	inline Vector2I GetSize() const { return m_size; }

private:
	const Texture* m_texture;
	Vector2I m_position{};
	Vector2I m_velocity{};
	Vector2I m_size{};
	
	bool m_should_be_removed{};
};


class InvaderManager
{
public:
	InvaderManager(SoundManager& sound_manager) : ms_sound_manager(sound_manager) {}
	~InvaderManager();
	void Begin();
	void Update(int player_score);
	void Reset();

	inline std::vector<Invader>& GetInvaders() { return m_invaders; }
	inline std::vector<InvaderProjectile>& GetInvaderProjectiles() { return m_invader_projectiles; }
	inline Rectangle GetRectangle() const { return m_rect; }

private:
	void CreateNewInvaders();

private:
	SoundManager& ms_sound_manager;

	std::vector<InvaderProjectile> m_invader_projectiles;
	std::vector<Invader> m_invaders;
	Texture m_invader_texture{};

	double m_last_fire_time = 0;
	double m_fire_interval = 1.5;

	Vector2I m_position{};
	Vector2I m_size{};
	Rectangle m_rect{};

	int m_columns = 0;
	int m_rows = 0;
};