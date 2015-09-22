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
	virtual ~SimpleObject(void);

	virtual void setPosition(sf::Vector2f const & position);

	virtual void startBalle(void);
	virtual void update(sf::Time frameTime);
	virtual void draw(sf::RenderTarget& render, sf::RenderStates states) const;

protected:
	SimpleObject(ResourceKey key, ResourceKey shader, float duration);

	sf::Shader & getShader(void);
	bool getStartBalle(void) const;
	bool getCollideWithOcto(void) const;
	sf::Time getTimer(void) const;
	sf::Time getEffectDuration(void) const;
	void adjustBox(sf::Vector2f const & offset, float radius);
	void setupBox(AGameObjectBase * gameObject, std::size_t type, std::size_t mask);
	void setupAnimation(std::initializer_list<octo::SpriteAnimation::Frame> list, octo::LoopMode loop);

private:
	octo::AnimatedSprite		m_sprite;
	octo::SpriteAnimation		m_animation;
	CircleShape *				m_box;
	sf::Vector2f				m_offset;
	sf::Shader					m_shader;
	std::size_t					m_shaderIndex;
	sf::Time					m_timer;
	sf::Time					m_effectDuration;
	bool						m_startBalle;
	bool						m_collideWithOcto;

};

#endif
