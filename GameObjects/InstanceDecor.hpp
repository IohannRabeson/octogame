#ifndef INSTANCEDECOR_HPP
# define INSTANCEDECOR_HPP

# include <AnimatedSprite.hpp>

# include "ResourceDefinitions.hpp"
# include "IPlaceable.hpp"

class InstanceDecor : public IPlaceable
{
public:
	InstanceDecor(ResourceKey key, sf::Vector2f const & scale, sf::Vector2f const & position, std::size_t nbFrames);
	virtual ~InstanceDecor(void);

	virtual void addMapOffset(float x, float y);
	sf::Vector2f const & getPosition(void) const;
	virtual void setPosition(sf::Vector2f const & position);

	virtual void update(sf::Time frameTime);
	virtual void draw(sf::RenderTarget& render, sf::RenderStates states) const;

protected:

	void setupAnimation(std::initializer_list<octo::SpriteAnimation::Frame> list);

private:
	octo::AnimatedSprite		m_sprite;
	octo::SpriteAnimation		m_animation;

};

#endif
