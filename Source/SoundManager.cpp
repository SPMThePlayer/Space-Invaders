#include "SoundManager.h"

SoundManager::~SoundManager()
{
	for (auto& [key, sound] : m_sounds) 
	{
		UnloadSound(sound);
	}
	m_sounds.clear();
}

void SoundManager::Begin()
{

	AddSound(SoundType::Explosion, explosion_data, explosion_size);
	AddSound(SoundType::Shoot, shoot_data, shoot_size);
	AddSound(SoundType::Gameover, gameover_data, gameover_size);
}

void SoundManager::AddSound(SoundType type, const uint8_t* data, int length)
{
	m_sounds.insert({type, LoadSoundFromMemory(data, length)});
}

void SoundManager::PlayStoredSound(SoundType type) const 
{
	if (m_sounds.contains(type))
	{
		PlaySound(m_sounds.at(type));
	}
}

