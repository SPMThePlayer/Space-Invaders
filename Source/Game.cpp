#include "Game.h"

#define M_PI 3.14159265358979323846

Game::Game() : m_rng(std::random_device{}()), m_player(m_sound_manager), m_invader_manager(m_sound_manager) 
{
    InitAudioDevice();
    m_sound_manager.Begin();

    SetConfigFlags(FLAG_WINDOW_RESIZABLE | FLAG_VSYNC_HINT);
    InitWindow(m_screen_width, m_screen_height, "Space Invaders - SPMTheCoder (D.S)");
   // SetTargetFPS(60);

    GenerateRandomStarPositions();


	m_player.Begin();
    m_invader_manager.Begin();

    m_best_score_file.open(best_score_file_name, std::ios::in | std::ios::out);
    if (!m_best_score_file.is_open())
    {
        std::cerr << "Error: Could not open " << best_score_file_name << std::endl;
    }

    std::string best_score_str;
    std::getline(m_best_score_file, best_score_str);

    std::cout << "Best Score: " << best_score_str << std::endl;

    try 
    {
        m_best_score = std::stoi(best_score_str);
    }
    catch (std::invalid_argument)
    {
        m_best_score = 0;
    }

}

Game::~Game()
{
    m_best_score_file.close();
    CloseAudioDevice();
    CloseWindow();
}

void Game::Run()
{
    m_delta_time = GetFrameTime();

    if (IsWindowResized())
    {
		m_screen_height = GetScreenHeight();
		m_screen_width = GetScreenWidth();

        GenerateRandomStarPositions();
    }

    BeginDrawing();

    switch (m_current_screen)
    {
	case ScreenType::TITLE:
		TitleScreen();
		break;
    case ScreenType::GAMEPLAY:
        Gameplay();
        break;
    case ScreenType::GAMEOVER:
        GameOver();
        break;
    }

    EndDrawing();
}

void Game::GenerateRandomStarPositions()
{
    for (auto& pos : m_star_positions)
    {
        pos.x = GetRandomValue(0, m_screen_width);
        pos.y = GetRandomValue(0, m_screen_height);
    }
}

void Game::DrawStars()
{
    if (GetTime() - m_last_star_resize_time > m_star_resize_interval)
    {
        for (auto& size : m_star_sizes)
        {
            size = GetRandomValue(1, 2);
        }
        m_last_star_resize_time = GetTime();
        m_star_resize_interval = GetRandomValue(100, 200) / static_cast<double>(1000);
    }

    for (int i = 0; i < number_of_stars; i++)
    {
		m_star_positions[i].y += static_cast<int>(m_star_speed * m_delta_time);
        DrawCircle(m_star_positions[i].x, m_star_positions[i].y, m_star_sizes[i], WHITE);

		if (m_star_positions[i].y > m_screen_height)
		{
			m_star_positions[i].x = GetRandomValue(0, m_screen_width);
			m_star_positions[i].y = -5;
		}
    }
}

void Game::AddParticleAtLocation(Vector2I position, int number)
{
    std::uniform_real_distribution<float> angle_dist(0, 2.f * static_cast<float>(M_PI));
    std::uniform_real_distribution<float> radius_dist(0, 50);
    std::uniform_real_distribution<float> speed_dist(100, 300); 
    std::uniform_int_distribution<int> color_dist(0, number_of_default_particle_colors - 1);
    std::uniform_real_distribution<float> size_dist(1.f, 5.f);
    std::uniform_real_distribution<float> lifetime_dist(2.f, 4.2f);
    std::uniform_real_distribution<float> fade_dist(0.2015f, 0.8001f);

    m_particles.reserve(m_particles.size() + number);
    for (int i = 0; i < number; i++)
    {
        float angle = angle_dist(m_rng);
        float radius = std::sqrt(radius_dist(m_rng));
        float speed = speed_dist(m_rng); 

        int spawn_x = static_cast<int>(radius * std::cos(angle));
        int spawn_y = static_cast<int>(radius * std::sin(angle));

        int velocity_x = static_cast<int>(speed * std::cos(angle));
        int velocity_y = static_cast<int>(speed * std::sin(angle));

        if (velocity_x == 0 && velocity_y == 0) {
            velocity_x = (std::cos(angle) >= 0) ? 1 : -1;  
            velocity_y = (std::sin(angle) >= 0) ? 1 : -1;
        }

        m_particles.emplace_back(
            Vector2I{ position.x + spawn_x, position.y + spawn_y },
            Vector2I{ velocity_x, velocity_y }, 
            size_dist(m_rng),
            particle_default_colors[color_dist(m_rng)],
            true,
            fade_dist(m_rng),
            lifetime_dist(m_rng)
        );
    }
}

void Game::TitleScreen()
{
    ClearBackground(BLACK);

    DrawStars();
    DrawText("Space Invaders", m_screen_width / 2 - 180, m_screen_height / 2 - 50, 40, title_screen_text_color);
	DrawText("Press Enter to Start", m_screen_width / 2 - 200, m_screen_height / 2 , 35, GREEN);

	if (IsKeyPressed(KEY_ENTER))
	{
		m_current_screen = ScreenType::GAMEPLAY;
	}
}

void Game::GameOver()
{
    ClearBackground(BLACK);
    DrawStars();

    std::string score_text = "Score: " + std::to_string(m_score) + ", Best Score: " + std::to_string(m_best_score);

    DrawText("Eliminated!! :(", m_screen_width / 2 - 140, m_screen_height / 2 - 70, 40, Color{ 255, 0, 0, 255 });

    DrawText(score_text.c_str(), m_screen_width / 2 - 210, m_screen_height / 2 - 20, 35, BLUE);
    DrawText("Press Enter to Restart", m_screen_width / 2 - 210, m_screen_height / 2 + 15, 35, GREEN);
    DrawText("Press Q to Quit", m_screen_width / 2 - 210, m_screen_height / 2 + 50, 35, ORANGE);

    if (IsKeyPressed(KEY_ENTER))
    {
        m_current_screen = ScreenType::GAMEPLAY;
        m_gameover = false;
        m_gameover_time = 0;

        m_player.Reset();
        m_invader_manager.Reset();
        
        m_score = 0;

        m_particles.clear();
        m_particles.shrink_to_fit();

    }
    else if (IsKeyPressed(KEY_Q))
    {
        CloseWindow();
        exit(0);
    }
}

void Game::Gameplay()
{
    ClearBackground(BLACK);
    DrawStars();

    if (m_gameover)
    {
        if (GetTime() - m_gameover_time > 1.0f)
        {
            m_current_screen = ScreenType::GAMEOVER;
            m_sound_manager.PlayStoredSound(SoundType::Gameover);

            if (m_score > m_best_score)
            {
                m_best_score = m_score;

                m_best_score_file.close(); 
                m_best_score_file.open(best_score_file_name, std::ios::out | std::ios::trunc);

                m_best_score_file.seekp(0);
                m_best_score_file << m_best_score;
                m_best_score_file.flush();

            }
        }
    }

    m_player.Update();
    m_invader_manager.Update(m_score);

    std::vector<Invader>& invaders = m_invader_manager.GetInvaders();
    std::vector<InvaderProjectile>& invader_projectiles = m_invader_manager.GetInvaderProjectiles();
    std::vector<PlayerProjectile>& player_projectiles = m_player.GetPlayerProjectiles();

    //printf("Invaders: %d, InvaderProjectiles: %d, PlayerProjectiles: %d\n", invaders.size(),
    //    invader_projectiles.size(), player_projectiles.size());

    if (CheckCollisionRecs(m_invader_manager.GetRectangle(), m_player.GetActualBoundingBox(true)))
    {
        AddParticleAtLocation(m_player.GetPosition(), 30);
        m_player.Hide();
        m_player.SetShooting(false);

        m_gameover = true;
        m_gameover_time = GetTime();
    }

    for (auto& invader : invaders)
    {
        for (auto& player_projectile : player_projectiles)
        {
            Rectangle invader_rect{};
            invader_rect.x = static_cast<float>(invader.GetPosition().x);
            invader_rect.y = static_cast<float>(invader.GetPosition().y);
            invader_rect.width = static_cast<float>(invader.GetSize().x);
            invader_rect.height = static_cast<float>(invader.GetSize().y);

            if (CheckCollisionCircleRec(player_projectile.GetPosition().ToVector2F(), player_projectile.GetRadius(),
                invader_rect))
            {
                invader.DeclareAsDestroyed(); //Will be destroyed in invader manager update loop
                player_projectile.DeclareAsDestroyed(); //Will be destroyed in player update loop
                m_sound_manager.PlayStoredSound(SoundType::Explosion);

                m_score += 100;

                AddParticleAtLocation(invader.GetPosition());

            }
        }
    }

    for (auto& particle : m_particles)
    {
        particle.Update();
        if (particle.TimeOver())
        {
            particle.DeclareAsDestroyed(); // Will be destroyed just after this loop
        }
    }

    if(!m_particles.empty())
    {
        m_particles.erase(
            std::remove_if(m_particles.begin(), m_particles.end(),
                [&](const Particle& particle) {
                    return particle.ShouldBeDestroyed();
                }),
            m_particles.end());
    }

    for (auto& invader_projectile : invader_projectiles)
    {
        if (CheckCollisionRecs(invader_projectile.GetRect(), m_player.GetActualBoundingBox(true)))
        {
            AddParticleAtLocation(m_player.GetPosition(), 30);
            m_sound_manager.PlayStoredSound(SoundType::Explosion);
            m_player.Hide();
            m_player.SetShooting(false);

            m_gameover = true;
            m_gameover_time = GetTime();
        }
    }

    m_score_text = "Score: " + std::to_string(m_score);
    DrawText(m_score_text.c_str(), 20, 20, 20, GREEN);
}
