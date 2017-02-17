#ifndef SPIRITINFOS_HPP
# define SPIRITINFOS_HPP

# include "BubbleText.hpp"
# include "NanoRobot.hpp"
# include "MenuNanoRobot.hpp"
# include "RandomGenerator.hpp"

class SpiritInfos
{
public:
	SpiritInfos(void);
	~SpiritInfos(void) = default;

	void setup();
	void update(sf::Time frameTime, sf::Vector2f const & position);
	void draw(sf::RenderTarget & render, sf::RenderStates states) const;

private:
	std::size_t									m_progressionCount;
	std::wstring								m_progressionString;
	std::vector<std::unique_ptr<BubbleText>>	m_progressionBubbles;
	std::vector<std::unique_ptr<NanoRobot>>		m_spirits;
	std::wstring								m_missingText;
	RandomGenerator								m_generator;
	sf::Vector2f								m_position;

	std::wstring const & getText(std::string const & text);
	sf::Vector2f getRandomVector2f(void);

	void setupProgression(void);
	void setupSpirit(sf::Vector2f const & position);
	void updateSpirit(sf::Time frameTime, sf::Vector2f const & position);
	void updateProgression(sf::Time frameTime, sf::Vector2f const & position);
};

#endif
