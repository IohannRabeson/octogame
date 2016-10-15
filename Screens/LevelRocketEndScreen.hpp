#ifndef LEVELROCKETENDSCREEN_HPP
# define LEVELROCKETENDSCREEN_HPP

# include <AbstractState.hpp>
# include <AudioManager.hpp>
# include <memory>
# include "StarSystem.hpp"
# include "RandomGenerator.hpp"
# include "InputListener.hpp"

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

private:
	RandomGenerator					m_generator;
	std::size_t						m_starsCount;
	std::unique_ptr<StarSystem[]>	m_stars;
	std::unique_ptr<RocketEnd>		m_rocket;

};

#endif
