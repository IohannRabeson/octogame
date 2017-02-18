#ifndef INSTANCEDECOR_HPP
# define INSTANCEDECOR_HPP

# include <AnimatedSprite.hpp>

# include "ResourceDefinitions.hpp"
# include "IPlaceable.hpp"

class InstanceDecor : public IPlaceable
{
public:
	InstanceDecor(ResourceKey key);
	InstanceDecor(ResourceKey key, sf::Vector2f const & scale, sf::Vector2f const & position, std::size_t nbFrames, float frameDuration = 0.2f);
	virtual ~InstanceDecor(void);

	virtual void addMapOffset(float x, float y);
	sf::Vector2f const & getPosition(void) const;
	virtual void setPosition(sf::Vector2f const & position);
	virtual octo::SpriteAnimation & getAnimation(void);
	virtual octo::AnimatedSprite & getSprite(void);

	virtual void update(sf::Time frameTime);
	virtual bool isInScreen(void);
	virtual void draw(sf::RenderTarget& render, sf::RenderStates states) const;

protected:
	octo::AnimatedSprite		m_sprite;
	octo::SpriteAnimation		m_animation;

	void setupAnimation(std::initializer_list<octo::SpriteAnimation::Frame> list);

};

#endif
