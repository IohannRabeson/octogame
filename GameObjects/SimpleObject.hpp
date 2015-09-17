#ifndef SIMPLEOBJECT_HPP
# define SIMPLEOBJECT_HPP

# include <AnimatedSprite.hpp>

# include "ResourceDefinitions.hpp"
# include "IPlaceable.hpp"

class SimpleObject : public IPlaceable
{
public:
	virtual ~SimpleObject(void) = default;

	virtual void setPosition(sf::Vector2f const & position);

	virtual void update(sf::Time frameTime);
	virtual void draw(sf::RenderTarget& render, sf::RenderStates states) const;

protected:
	SimpleObject(ResourceKey key);

	void setupAnimation(std::initializer_list<octo::SpriteAnimation::Frame> list, octo::LoopMode loop);

private:
	octo::AnimatedSprite		m_sprite;
	octo::SpriteAnimation		m_animation;

};

#endif
