#ifndef DEATHSCREEN_HPP
# define DEATHSCREEN_HPP
# include "InputListener.hpp"
# include <AbstractState.hpp>
# include <AnimatedSprite.hpp>

# include <SFML/Graphics/Sprite.hpp>

class DeathScreen : public octo::AbstractState, public InputListener
{
public:
	DeathScreen();
	~DeathScreen();

	virtual void	start();
	virtual void	pause();
	virtual void	resume();
	virtual void	stop();

	virtual void	update(sf::Time frameTime);
	virtual void	draw(sf::RenderTarget& render)const;
	virtual bool	onInputPressed(InputListener::OctoKeys const & key);

	void			setSpriteSheet(octo::SpriteSheet const& spriteSheet);
	void			setAnimation(octo::SpriteAnimation const& animation);
private:
	octo::AnimatedSprite		m_sprite;
	octo::SpriteAnimation		m_animation;
	sf::Time					m_timeDeath;
	sf::Time					m_timeDeathMax;
};

#endif
