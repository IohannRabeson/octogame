#ifndef DAYNIGHTLIGHT_HPP
# define DAYNIGHTLIGHT_HPP

# include "ADecor.hpp"
# include "DecorAnimator.hpp"
# include <SFML/Graphics/Color.hpp>

class GameClock;

class DayNightLight : public ADecor
{
public:
	DayNightLight(void);
	DayNightLight(GameClock * clock);
	virtual ~DayNightLight(void) = default;

	virtual void setup(ABiome& biome);
	virtual void update(sf::Time frameTime,
						octo::VertexBuilder& builder,
						ABiome& biome);

private:
	sf::Vector2f			m_size;
	sf::Vector2f			m_dayPos;
	sf::Vector2f			m_nightPos;
	sf::Vector2f			m_middlePos;
	sf::Color				m_transparancy;
	sf::Color				m_colorDay;
	sf::Color				m_colorNight;
	sf::Color				m_colorMiddle;
	sf::Vector2f			m_cameraSize;

	GameClock *				m_clock;

	void createDayNightLight(sf::Vector2f const & size,
					sf::Vector2f const & origin,
					octo::VertexBuilder& builder);

	void createBicolorQuad(sf::Vector2f const & upLeft,
							sf::Vector2f const & upRight,
							sf::Vector2f const & downRight,
							sf::Vector2f const & downLeft,
							sf::Color const & colorUp,
							sf::Color const & colorDown,
							octo::VertexBuilder & builder);
};

#endif
