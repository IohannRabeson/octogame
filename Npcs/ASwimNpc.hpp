#ifndef ASWIMNPC_HPP
# define ASWIMNPC_HPP

# include "ANpc.hpp"
# include "RandomGenerator.hpp"

class SkyCycle;

class ASwimNpc : public ANpc, public AGameObject<GameObjectType::SwimNpc>
{
public:
	ASwimNpc(ResourceKey const & npcId, bool isMeetable, bool isShift);
	virtual ~ASwimNpc(void) = default;

	virtual void			setup(void) = 0;
	virtual void			setPosition(sf::Vector2f const & position);
	virtual float			getHeight(void) const { return 0.f; }
	virtual void			collideOctoEvent(CharacterOcto * octo);
	virtual void			draw(sf::RenderTarget & render, sf::RenderStates states) const;

protected:
	virtual void			setupMachine(void);
	virtual void			updateState(void);
	virtual void			updatePhysics(void);
	virtual void			update(sf::Time frametime);
	virtual void			setVelocity(float velocity);

private:
	static RandomGenerator	m_generator;

	void					computeBehavior(sf::Time frametime);

	sf::Vector2f			m_octoPosition;
	float					m_waterLevel;
	bool					m_isMet;
	bool					m_isShift;
	float					m_velocity;
	sf::Vector2f			m_shift;
};

#endif
