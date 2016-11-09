#ifndef LEVELROCKETENDSCREEN_HPP
# define LEVELROCKETENDSCREEN_HPP

# include "StarSystem.hpp"
# include "RandomGenerator.hpp"
# include "InputListener.hpp"
# include "Credit.hpp"
# include <AbstractState.hpp>
# include <AudioManager.hpp>
# include <memory>

class RocketEnd;

class LevelRocketEndScreen : public octo::AbstractState,
						public InputListener
{
public:
	LevelRocketEndScreen(void);
	virtual void		start();
	virtual void		pause();
	virtual void		resume();
	virtual void		stop();
	virtual void		update(sf::Time frameTime);
	virtual void		draw(sf::RenderTarget& render)const;

	virtual bool	onInputPressed(InputListener::OctoKeys const & key);
	virtual bool	onInputReleased(InputListener::OctoKeys const & key);

private:
	RandomGenerator					m_generator;
	std::size_t						m_starsCount;
	std::unique_ptr<StarSystem[]>	m_stars;
	std::unique_ptr<RocketEnd>		m_rocket;
	std::unique_ptr<Credit>			m_credit;
	float							m_speedCredit;
	sf::Time						m_timer;
	sf::Time						m_timerMax;

};

#endif
