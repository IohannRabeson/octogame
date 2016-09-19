#ifndef AFLYNPC_HPP
# define AFLYNPC_HPP

# include "ANpc.hpp"
# include "RandomGenerator.hpp"
# include <SFML/Graphics/Shader.hpp>

class AFlyNpc : public ANpc, public AGameObject<GameObjectType::FlyNpc>
{
public:
	AFlyNpc(ResourceKey const & npcId, bool isMeetable = false);
	virtual ~AFlyNpc(void) = default;

	virtual void setup(void);
	virtual void setPosition(sf::Vector2f const & position);
	virtual float getHeight(void) const { return 0.f; }
	virtual void collideOctoEvent(CharacterOcto * octo);
	virtual void draw(sf::RenderTarget & render, sf::RenderStates states) const;

protected:
	virtual void setupMachine(void);
	virtual void updateState(void);
	virtual void update(sf::Time frametime);
	virtual void updatePhysics(void);

private:
	void caseDoubleJump(sf::Time frametime);
	void computeFlight(sf::Time frametime);

	static RandomGenerator	m_generator;

	bool					m_animationEnd;
	sf::Vector2f			m_startPosition;
	sf::Vector2f			m_nextPosition;
	sf::Vector2f			m_octoPosition;
	float					m_speedLimit;
	sf::Vector2f			m_flySpeed;
	sf::Shader				m_shader;
};

#endif
