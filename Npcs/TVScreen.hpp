#ifndef TVSCREEN_HPP
# define TVSCREEN_HPP

# include "ANpc.hpp"

namespace sf
{
	class Shader;
}
class CharacterOcto;

class TVScreen : public ANpc, public AGameObject<GameObjectType::TVScreen>
{
public:
	TVScreen(void);
	virtual ~TVScreen(void) = default;

	virtual void setup(void);
	virtual void collideOctoEvent(CharacterOcto * octo);

protected:
	virtual void setupMachine(void);
	virtual void update(sf::Time frametime);
	virtual void updateState(void);

private:
	sf::Shader &	m_shader;
	sf::FloatRect	m_tvScreen;
	sf::Time		m_timer;
	sf::Time		m_duration;
	bool			m_startZoom;
	bool			m_reverse;

};

#endif
