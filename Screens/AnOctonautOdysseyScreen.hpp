#ifndef ANOCTONAUTODYSSEYSCREEN_HPP
# define ANOCTONAUTODYSSEYSCREEN_HPP
# include <AbstractState.hpp>

# include "ResourceDefinitions.hpp"

# include <vector>

# include <SFML/System.hpp>
# include <SFML/Graphics/RectangleShape.hpp>
# include <SFML/Graphics/Sprite.hpp>

class AnOctonautOdysseyScreen : public octo::AbstractState//, public InputListener
{
public:
	AnOctonautOdysseyScreen();
private:
	virtual void	start();
	virtual void	pause();
	virtual void	resume();
	virtual void	stop();
	virtual void	update(sf::Time frameTime);
	virtual void	draw(sf::RenderTarget& render)const;

private:
	std::size_t					m_count;
	std::vector<ResourceKey>	m_key;
	std::vector<sf::Texture>	m_startTextures;
	std::vector<sf::Sprite>		m_startSprites;
	std::size_t					m_index;
	sf::Time					m_timer;
	std::vector<sf::Time>		m_timerMax;
};

#endif
