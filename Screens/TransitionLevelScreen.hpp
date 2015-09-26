#ifndef TRANSITIONLEVELSCREEN_HPP
# define TRANSITIONLEVELSCREEN_HPP
# include <AbstractState.hpp>
# include <AnimatedSprite.hpp>
# include <AudioManager.hpp>

# include <SFML/Graphics/Sprite.hpp>

class TransitionLevelScreen : public octo::AbstractState
{
public:
	TransitionLevelScreen();
	~TransitionLevelScreen();

	virtual void	start();
	virtual void	pause();
	virtual void	resume();
	virtual void	stop();

	virtual void	update(sf::Time frameTime);
	virtual void	draw(sf::RenderTarget& render)const;

private:
	std::shared_ptr<sf::Sound>	m_sound;
	octo::AnimatedSprite		m_sprite;
	octo::SpriteAnimation		m_animation;
	sf::Time					m_timeTransition;
	sf::Time					m_timeTransitionMax;
};

#endif
