#pragma once
#include "Globals.h"
#include "Player.h"
#include "Invader.h"
#include "Particle.h"
#include "SoundManager.h"

class Game
{
public:
	Game();
	~Game();

	void Run();
	inline bool ShouldQuit() { return WindowShouldClose(); }
private:

	void GenerateRandomStarPositions();
	void DrawStars();
	void AddParticleAtLocation(Vector2I position, int number = 15);

	void TitleScreen();
	void GameOver();
	void Gameplay();

private:
	int m_screen_width = 1280;
	int m_screen_height = 700;
	float m_delta_time = 0;
	ScreenType m_current_screen = ScreenType::TITLE;

	std::array<Vector2I, number_of_stars> m_star_positions;
	std::array<uint8_t, number_of_stars> m_star_sizes;
	double m_last_star_resize_time = 0;
	double m_star_resize_interval = 0.1;
	int m_star_speed = 100;

	SoundManager m_sound_manager;

	Player m_player;
	InvaderManager m_invader_manager;

	std::vector<Particle> m_particles;
	std::mt19937 m_rng;
	
	bool m_gameover = false;
	double m_gameover_time = 0.f;

	int m_score = 0;
	int m_best_score = 0;
	std::string m_score_text;

	std::fstream m_best_score_file;
};
