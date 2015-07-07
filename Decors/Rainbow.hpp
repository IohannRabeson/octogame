#ifndef RAINBOW_HPP
# define RAINBOW_HPP

# include "ADecor.hpp"
# include "DecorAnimator.hpp"
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

private:
	std::vector<sf::Vector2f>				m_startPart;
	std::vector<sf::Vector2f>				m_endPart;
	std::vector<sf::Vector2f>				m_partLineOrigin;
	std::vector<float>						m_partSizes;
	std::vector<bool>						m_partSides;
	sf::Vector2f							m_size;
	std::size_t								m_partCount;
	std::size_t								m_stripeCount;
	std::vector<sf::Color>					m_stripesColors;
	float									m_cos;
	float									m_sin;

	DecorAnimator							m_animator;
	float									m_animation;

	void computeHorizontalLine(float thickness,
								sf::Vector2f const & origin,
								std::size_t stripeCount,
								std::vector<sf::Vector2f> & points);

	void computeVerticalLine(float thickness,
								sf::Vector2f const & origin,
								std::size_t stripeCount,
								std::vector<sf::Vector2f> & points);

	void createRainbowPart(float thickness,
							sf::Vector2f const & origin,
							std::vector<sf::Vector2f> & start,
							std::vector<sf::Vector2f> & end,
							std::size_t stripeCount,
							octo::VertexBuilder& builder);

	void createRainbow(sf::Vector2f const & origin,
						std::size_t partCount,
						std::vector<sf::Vector2f> & partLineOrigin,
						sf::Color const & color,
						octo::VertexBuilder& builder);

private:
	static std::default_random_engine	m_engine;
	static std::bernoulli_distribution	m_distribution;
	static bool getPartAngle(void);

	static void rotateVec(sf::Vector2f & vector, float const cosAngle, float const sinAngle);
	static void rotateVec(sf::Vector2f & vector, sf::Vector2f const & origin, float const cosAngle, float const sinAngle);

};

#endif
