#ifndef TVSCREEN_HPP
# define TVSCREEN_HPP

# include "ANpc.hpp"

namespace sf
{
	class Shader;
}

class TVScreen : public ANpc, public AGameObject<GameObjectType::TVScreen>
{
public:
	TVScreen(void);
	virtual ~TVScreen(void) = default;

	virtual void setup(void);

protected:
	virtual void setupMachine(void);
	virtual void updateState(void);

private:
	sf::Shader &	m_shader;
	sf::FloatRect	m_tvScreen;

};

#endif
