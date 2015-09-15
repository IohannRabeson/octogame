#include "OctoSound.hpp"
#include "AudioManager.hpp"

CharacterOcto::OctoSound::OctoSound()
{
}

void	CharacterOcto::OctoSound::update(sf::Time frameTime, Events event)
{
	(void)frameTime;
	switch(event)
	{
		case Idle:
			break;
		case Right:
		case Left:
			break;
		case Jump:
			break;
		case DoubleJump:
			break;
		default:
			break;
	}
}
