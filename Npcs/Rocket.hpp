#ifndef ROCKET_HPP
# define ROCKET_HPP

# include "ANpc.hpp"
# include "SmokeSystem.hpp"

class CircleShape;
class CharacterOcto;
namespace sf
{
	class Shader;
}

class Rocket : public ANpc, public AGameObject<GameObjectType::Rocket>
{
public:
	Rocket(void);
	virtual ~Rocket(void) = default;

	virtual void setup(void);
	virtual void update(sf::Time frametime);
	virtual void setPosition(sf::Vector2f const & position);
	virtual void addMapOffset(float x, float y);
	virtual void collideOctoEvent(CharacterOcto * octo);
	virtual void drawFront(sf::RenderTarget & render, sf::RenderStates states) const;

protected:
	virtual void setupMachine(void);

private:
	enum State
	{
		Waiting,
		OctoEntering,
		StartSmoke
	};

	sf::Shader &					m_shader;
	CircleShape *					m_enterRocketShape;
	State							m_state;
	std::size_t						m_smokesCount;
	std::unique_ptr<SmokeSystem[]>	m_smokes;
	sf::Time						m_timerBefore;
	sf::Time						m_timerBeforeMax;
	sf::Time						m_timerFirstBlast;
	sf::Time						m_timerFirstBlastMax;
	sf::Time						m_timerSecondBlast;
	sf::Time						m_timerSecondBlastMax;
	sf::Time						m_timerOctoEntering;
	sf::Time						m_timerOctoEnteringMax;
	sf::Vector2f					m_octoPosition;
	sf::Vector2f					m_lastPosition;
	sf::Vector2f					m_lastPositionDoor;
	bool							m_stopCameraMovement;
};

#endif
