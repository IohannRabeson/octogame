#ifndef SUN_HPP
# define SUN_HPP

# include "ADecor.hpp"
# include "DecorAnimator.hpp"
# include <SFML/Graphics/Color.hpp>

class Sun : public ADecor
{
public:
	Sun(void);
	virtual ~Sun(void) = default;

	virtual void setup(ABiome& biome);
	virtual void update(sf::Time frameTime,
						octo::VertexBuilder& builder,
						ABiome& biome);

private:
	sf::Vector2f			m_size;
	sf::Vector2f			m_sizeCorner;
	sf::Color				m_color;
	std::size_t				m_partCount;

	DecorAnimator			m_animator;
	float					m_animation;

	sf::Time				m_glowingTimer;
	sf::Time				m_glowingTimerMax;

	void createOctogon(sf::Vector2f const & size,
						sf::Vector2f const & sizeCorner,
						sf::Vector2f const & origin,
						sf::Color const & color,
						octo::VertexBuilder& builder);

	void createSun(sf::Vector2f const & size,
					sf::Vector2f const & sizeCorner,
					sf::Vector2f const & origin,
					std::size_t partCount,
					sf::Color color,
					octo::VertexBuilder& builder);
};

#endif
