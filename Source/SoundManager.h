#pragma once
#include "Globals.h"

enum class SoundType : uint8_t
{
	Explosion,
	Shoot,
	Gameover
};

class SoundManager
{
public:
	
	~SoundManager();

	void Begin();
	void AddSound(SoundType type, const uint8_t* data, int length);
	inline const Sound& GetSound(SoundType type) const { return m_sounds.at(type); };
	void PlayStoredSound(SoundType type) const;

private:
	std::unordered_map<SoundType, Sound> m_sounds;
};