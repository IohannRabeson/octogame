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

	void setup(void);
	void update(sf::Time frameTime, sf::Vector2f const & position);
	void draw(sf::RenderTarget & render, sf::RenderStates states) const;

private:
	std::wstring								m_progressionString;
	std::vector<std::unique_ptr<BubbleText>>	m_progressionBubble;
	std::vector<std::unique_ptr<NanoRobot>>		m_spirit;
	std::wstring								m_missingText;
	RandomGenerator								m_generator;

	std::wstring const & getText(std::string const & text);
	void updateNanoRobots(sf::Time frameTime, sf::Vector2f const & position);
	void updateProgression(sf::Time frameTime, sf::Vector2f const & position);
};

#endif
