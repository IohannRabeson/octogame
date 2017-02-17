#ifndef RAINBOW_HPP
# define RAINBOW_HPP

# include "ADecor.hpp"
# include <SFML/Graphics/Color.hpp>

class SkyCycle;

class Rainbow : public ADecor
{
public:
	Rainbow(void);
	Rainbow(SkyCycle * cycle);
	virtual ~Rainbow(void) = default;

	virtual void setup(ABiome& biome);
	virtual void update(sf::Time frameTime,
						octo::VertexBuilder& builder,
						ABiome& biome);

	virtual bool isDisabledIfOutOfScreen()const;

private:
	typedef std::vector<sf::Vector2f>	Line;

	void setupSizes(ABiome & biome,
					std::vector<sf::Vector2f> & sizes,
					std::size_t loopCount,
					std::size_t partCount,
					float thickness);

	void setupColors(std::vector<sf::Color> & colors,
						std::vector<sf::Color> & transparent, ABiome & biome);

	void createFirstLine(Line & line,
						std::size_t stripeCount,
						float thickness);

	void rotateLine(Line const & start,
					Line & end,
					std::size_t stripeCount,
					sf::Vector2f const & origin,
					float cos,
					float sin);

	void createBicolorQuad(sf::Vector2f const & upLeft,
							sf::Vector2f const & upRight,
							sf::Vector2f const & downRight,
							sf::Vector2f const & downLeft,
							sf::Color const & colorUp,
							sf::Color const & colorDown,
							octo::VertexBuilder & builder);

	void createPart(Line const & start,
					Line const & end,
					std::size_t stripeCount,
					sf::Vector2f const & origin,
					std::vector<sf::Color> colorsStart,
					std::vector<sf::Color> colorsEnd,
					float interpolateValue,
					octo::VertexBuilder& builder);

	void createRainbow(sf::Vector2f const & origin,
						std::vector<sf::Vector2f> const & sizes,
						std::size_t stripeCount,
						float thickness,
						std::vector<sf::Color> const & colors,
						std::vector<sf::Color> const & transparent,
						octo::VertexBuilder& builder);

	void newRainbow(ABiome & biome);

	void computeInterpolateValuesGrow(sf::Time frameTime,
								std::vector<float> & values);

	void computeInterpolateValuesDie(sf::Time frameTime,
								std::vector<float> & values);

	float						m_cos;
	float						m_sin;

	Line						m_start;
	Line						m_end;

	std::size_t					m_loopCountMax;
	std::size_t					m_loopCount;
	std::size_t					m_partCount;
	float						m_thickness;
	std::vector<sf::Vector2f>	m_sizes;
	std::size_t					m_stripeCount;
	std::vector<sf::Color>		m_colors;
	std::vector<sf::Color>		m_transparent;

	sf::Time					m_timer;
	std::vector<sf::Time>		m_timerMax;
	sf::Time					m_intervalTimer;
	sf::Time					m_intervalTimerMax;
	std::vector<float>			m_interpolateValues;
	bool						m_grow;

	sf::Vector2f				m_endPosition;
	bool						m_firstFrame;

	SkyCycle *					m_cycle;

private:
	static sf::Vector2f rotateVecCopy(sf::Vector2f const & vector, sf::Vector2f const & origin, float const cosAngle, float const sinAngle);
};

#endif
