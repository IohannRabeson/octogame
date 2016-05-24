#ifndef JELLYFISH_HPP
# define JELLYFISH_HPP

# include "ANpc.hpp"

class SkyCycle;

class JellyfishNpc : public ANpc, public AGameObject<GameObjectType::JellyfishNpc>
{
public:
	JellyfishNpc(void);
	virtual ~JellyfishNpc(void) = default;

	virtual void setup(void);
	virtual void setPosition(sf::Vector2f const & position);
	virtual float getHeight(void) const { return 0.f; }
	virtual void collideOctoEvent(CharacterOcto * octo);
	virtual void draw(sf::RenderTarget & render, sf::RenderStates states) const;

protected:
	virtual void setupMachine(void);
	virtual void updateState(void);
	virtual void updatePhysics(void);
	virtual void update(sf::Time frametime);
	void		 computeBehavior(sf::Time frametime);

private:
	sf::Vector2f	m_octoPosition;
	float			m_waterLevel;
	bool			m_isMet;
};

#endif
