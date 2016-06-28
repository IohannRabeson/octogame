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
	enum State
	{
		None,
		Zoom,
		Reversed
	};

	TVScreen(std::string const & kernelName = "render_black_kernel");
	virtual ~TVScreen(void) = default;

	virtual void setup(void);
	virtual void collideOctoEvent(CharacterOcto * octo);
	virtual void draw(sf::RenderTarget & render, sf::RenderStates states) const;
	virtual void drawFront(sf::RenderTarget & render, sf::RenderStates states) const;

protected:
	virtual void setupMachine(void);
	virtual void update(sf::Time frametime);
	virtual void updateState(void);

private:
	sf::Shader &	m_shader;
	sf::Shader &	m_shaderReverse;
	State			m_state;
	sf::FloatRect	m_tvScreen;
	sf::Time		m_timer;
	sf::Time		m_duration;
	std::string		m_kernelName;
	bool			m_reverse;

};

#endif
