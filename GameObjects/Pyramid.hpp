#ifndef PYRAMID_HPP
# define PYRAMID_HPP

# include "InstanceDecor.hpp"
# include "BeamSystem.hpp"
# include "ABiome.hpp"

class BeamSystem;

class Pyramid : public InstanceDecor
{
public:
	Pyramid(sf::Vector2f const & scale, sf::Vector2f const & position, ABiome & biome);
	virtual ~Pyramid(void) = default;

	virtual void addMapOffset(float x, float y);
	virtual void setPosition(sf::Vector2f const & position);

	virtual void update(sf::Time frameTime);
	virtual void draw(sf::RenderTarget& render, sf::RenderStates states) const;
	virtual void drawFront(sf::RenderTarget& render, sf::RenderStates states) const;

private:
	std::shared_ptr<BeamSystem>			m_particles;
	octo::AnimatedSprite				m_spriteTop;
	octo::SpriteAnimation				m_animationTop;
	octo::AnimatedSprite				m_spriteWater;
	octo::SpriteAnimation				m_animationWater;
	std::vector<octo::AnimatedSprite>	m_spriteBlocs;
	std::vector<octo::SpriteAnimation>	m_animationBlocs;

};

#endif
