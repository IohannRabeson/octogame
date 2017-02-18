#ifndef SIMPLEOBJECT_HPP
# define SIMPLEOBJECT_HPP

# include <SFML/Graphics/Shader.hpp>
# include <AnimatedSprite.hpp>

# include "AGameObject.hpp"
# include "ResourceDefinitions.hpp"
# include "IPlaceable.hpp"

class CircleShape;

class SimpleObject : public IPlaceable
{
public:
	virtual ~SimpleObject(void) = default;

	virtual void setPosition(sf::Vector2f const & position);

	virtual void update(sf::Time frameTime);
	virtual bool isInScreen(void);
	virtual void draw(sf::RenderTarget& render, sf::RenderStates states) const;
	virtual void drawFront(sf::RenderTarget& render, sf::RenderStates states) const;

protected:
	SimpleObject(ResourceKey key);

	octo::AnimatedSprite const & getSprite(void) const;
	octo::AnimatedSprite & getSprite(void);
	bool getCollideWithOcto(void) const;
	void adjustBox(sf::Vector2f const & offset, float radius);
	void setupBox(AGameObjectBase * gameObject, std::size_t type, std::size_t mask);
	void setupAnimation(std::initializer_list<octo::SpriteAnimation::Frame> list, octo::LoopMode loop);

private:
	octo::AnimatedSprite		m_sprite;
	octo::SpriteAnimation		m_animation;
	CircleShape *				m_box;
	sf::Vector2f				m_offset;
	bool						m_collideWithOcto;

};

#endif
