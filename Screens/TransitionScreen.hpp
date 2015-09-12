#ifndef TRANSITIONSCREEN_HPP
# define TRANSITIONSCREEN_HPP

# include <AbstractState.hpp>
# include <SFML/Graphics.hpp>

class TransitionScreen : public octo::AbstractState
{
public:
	virtual void		start();
	virtual void		pause();
	virtual void		resume();
	virtual void		stop();
	virtual void		update(sf::Time frameTime);
	virtual void		draw(sf::RenderTarget& render)const;
private:
	sf::Time			m_timer;
	sf::Time			m_timerMax;
};

#endif
