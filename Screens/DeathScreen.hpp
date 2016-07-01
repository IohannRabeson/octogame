#ifndef DEATHSCREEN_HPP
# define DEATHSCREEN_HPP
# include "InputListener.hpp"
# include <AbstractState.hpp>
# include <AnimatedSprite.hpp>
# include <AudioManager.hpp>

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
	float						m_volumeDefault;
	std::shared_ptr<sf::Sound>	m_sound;
	octo::AnimatedSprite		m_sprite;
	octo::SpriteAnimation		m_animation;
	sf::Time					m_timeTransition;
	sf::Time					m_timeTransitionMax;
};

#endif
