#ifndef LEVELZERO_HPP
# define LEVELZERO_HPP

# include <AbstractState.hpp>
# include <SFML/Graphics.hpp>
# include <AudioManager.hpp>
# include <memory>
# include "SpaceShip.hpp"
# include "StarSystem.hpp"
# include "HSL.hpp"
# include "RandomGenerator.hpp"
# include "InputListener.hpp"
# include "Credit.hpp"

class LevelZeroScreen : public octo::AbstractState,
						public InputListener
{
public:
	LevelZeroScreen(void);
	virtual void		start();
	virtual void		pause();
	virtual void		resume();
	virtual void		stop();
	virtual void		update(sf::Time frameTime);
	virtual void		draw(sf::RenderTarget& render)const;

	virtual bool		onInputPressed(InputListener::OctoKeys const & key);
	virtual bool		onInputReleased(InputListener::OctoKeys const & key);

private:
	enum State
	{
		Rising,
		Flying,
		Falling,
		CreditEnd
	};
	SpaceShip						m_spaceShip;
	RandomGenerator					m_generator;
	std::size_t						m_starsCount;
	std::unique_ptr<StarSystem[]>	m_stars;
	sf::VertexArray					m_background;
	sf::Color						m_upColorBackground;
	sf::Color						m_downColorBackground;
	sf::Time						m_timer;
	sf::Time						m_timerMax;
	sf::Time						m_timerStartRedAlarm;
	sf::Time						m_timerStartRedAlarmMax;
	sf::Time						m_timerEnd;
	sf::Time						m_timerEndMax;
	sf::Time						m_timerBlinkShader;
	State							m_state;
	float							m_offsetCamera;
	bool							m_keyUp;
	bool							m_keyDown;
	bool							m_isSoundPlayed;
	bool							m_isSoundExplodePlayed;
	bool							m_blinkShaderState;
	std::shared_ptr<sf::Sound>		m_ground;
	std::unique_ptr<Credit>			m_credit;

	void createBackground(sf::Vector2f const & position, sf::Color const & downColor);
};

#endif
