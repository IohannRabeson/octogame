#include "EngineSnow.hpp"
#include "Progress.hpp"

EngineSnow::EngineSnow(void) :
	AIdleNpc(ENGINE_SNOW_OSS),
	m_generator("random")
{
    setType(GameObjectType::EngineSnow);
	setSize(sf::Vector2f(216.f, 700.f));
	setOrigin(sf::Vector2f(142.f, 51.f));
	if (Progress::getInstance().getCurrentDestination() != Level::IceD)
		setScale(0.8f);
	else
		setScale(m_generator.randomFloat(0.4f, 0.9f));
	setTextOffset(sf::Vector2f(0.f, 0.f));
	setup();
}

void EngineSnow::setup(void)
{
	typedef octo::CharacterAnimation::Frame			Frame;
	std::size_t const								nbFrame = 6u;
	int												max = 3u;

	std::vector<Frame>								frames;
	int												index = m_generator.randomInt(0, 3);
	bool											way = true;

	for (std::size_t i = 0u; i < nbFrame; i++)
	{
		if (index <= 0)
			way = true;
		else if (index >= max)
			way = false;

		frames.push_back(Frame(sf::seconds(0.3f), {static_cast<std::size_t>(index), sf::FloatRect(), sf::Vector2f()}));
		if (way)
			index++;
		else
			index--;
	}

	getIdleAnimation().setFrames(frames);
	getIdleAnimation().setLoop(octo::LoopMode::Loop);

	setupMachine();
}
