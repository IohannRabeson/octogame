#ifndef GRASS_HPP
# define GRASS_HPP

# include "ADecor.hpp"
# include "DecorAnimator.hpp"
# include "ShineEffect.hpp"
# include <SFML/Graphics/Color.hpp>

class Grass : public ADecor
{
public:
	Grass(bool onInstance = false, bool reverse = false);
	virtual ~Grass(void) = default;

	virtual void setup(ABiome& biome);
	virtual void update(sf::Time frameTime,
						octo::VertexBuilder& builder,
						ABiome& biome);

	virtual bool dieOutOfScreen(void);

private:
	bool						m_reverse;
	sf::Vector2f				m_size;
	sf::Color					m_color;
	sf::Color					m_colorNormal;
	sf::Color					m_colorDeadly;
	bool						m_isDeadlyGrass;
	sf::Vector2f				m_up;
	ShineEffect					m_shine;
	sf::Vector2f				m_shineSize;
	sf::Color					m_shineColor;
	DecorAnimator				m_animator;
	float						m_animation;
	float						m_animationSpeed;
	sf::Time					m_movementTimer;
	sf::Time					m_movementTimerMax;
	std::size_t					m_numberOfTargets;
	std::vector<sf::Vector2f>	m_leftTargets;
	std::vector<sf::Vector2f>	m_rightTargets;
	bool						m_sideTarget;
	std::size_t					m_indexLeftTarget;
	std::size_t					m_indexRightTarget;
	sf::Vector2f				m_lastOctoPosition;
	bool						m_onInstance;
	bool						m_isShining;
	bool						m_firstFrame;
	float						m_octDeathCoef;
	sf::Time					m_deathGrowTimer;

	void setupTargets(void);
	void computeMovement(sf::Time frameTime);
	void createGrass(sf::Vector2f const & size,
					sf::Vector2f const & origin,
					sf::Color const & color,
					octo::VertexBuilder& builder);
};

#endif
