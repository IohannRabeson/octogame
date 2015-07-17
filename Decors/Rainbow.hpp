#ifndef RAINBOW_HPP
# define RAINBOW_HPP

# include "ADecor.hpp"
# include <SFML/Graphics/Color.hpp>

class Rainbow : public ADecor
{
public:
	Rainbow(void);
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

	void setupColors(std::vector<sf::Color> & colors);

	void createFirstLine(Line & line,
						std::size_t stripeCount,
						float thickness);

	void rotateLine(Line const & start,
					Line & end,
					std::size_t stripeCount,
					sf::Vector2f const & origin,
					float cos,
					float sin);

	void createPart(Line const & start,
					Line const & end,
					std::size_t stripeCount,
					sf::Vector2f const & origin,
					std::vector<sf::Color> const & colors,
					float interpolateValue,
					octo::VertexBuilder& builder);

	void createRainbow(sf::Vector2f const & origin,
						std::vector<sf::Vector2f> const & sizes,
						std::size_t stripeCount,
						float thickness,
						std::vector<sf::Color> const & colors,
						octo::VertexBuilder& builder);

	void computeInterpolateValues(sf::Time frameTime,
								std::vector<float> & values);

	float						m_cos;
	float						m_sin;

	Line						m_start;
	Line						m_end;

	std::size_t					m_loopCount;
	std::size_t					m_partCount;
	float						m_thickness;
	std::vector<sf::Vector2f>	m_sizes;
	std::size_t					m_stripeCount;
	std::vector<sf::Color>		m_colors;

	sf::Time					m_timer;
	sf::Time					m_timerMax;
	std::vector<float>			m_interpolateValues;

	sf::Vector2f				m_endPosition;
	bool						m_firstFrame;

private:
	static void rotateVec(sf::Vector2f & vector, float const cosAngle, float const sinAngle);
	static sf::Vector2f rotateVecCopy(sf::Vector2f const & vector, sf::Vector2f const & origin, float const cosAngle, float const sinAngle);

};

#endif
