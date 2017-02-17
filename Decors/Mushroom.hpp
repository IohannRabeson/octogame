#ifndef MUSHROOM_HPP
# define MUSHROOM_HPP

# include "ADecor.hpp"
# include "DecorAnimator.hpp"
# include <SFML/Graphics/Color.hpp>

class Mushroom : public ADecor
{
public:
	Mushroom(void);
	virtual ~Mushroom(void) = default;

	virtual void setup(ABiome& biome);
	virtual void update(sf::Time frameTime,
						octo::VertexBuilder& builder,
						ABiome& biome);

	virtual bool dieOutOfScreen(void);

private:
	sf::Vector2f				m_size;
	sf::Color					m_color;
	std::size_t					m_pointCount;
	std::vector<sf::Vector2f>	m_leftFirst;
	std::vector<sf::Vector2f>	m_leftSecond;
	std::vector<sf::Vector2f>	m_leftFinal;
	std::vector<sf::Vector2f>	m_rightFinal;

	DecorAnimator				m_animator;
	float						m_animation;
	sf::Time					m_bouncingTimer;
	sf::Time					m_bouncingTimerMax;
	bool						m_bouncingBool;
	bool						m_sound;

	void createMushroom(sf::Vector2f const & size,
					sf::Vector2f const & origin,
					sf::Color const & color,
					float bouncingValue,
					octo::VertexBuilder& builder);

	void newMushroom(ABiome & biome);
	float computeBouncingValue(sf::Time frameTime);
	void playSound(ABiome & biome, sf::Vector2f const & position);
};

#endif
