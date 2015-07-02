#ifndef DECORANIMATOR_HPP
# define DECORANIMATOR_HPP

# include <SFML/System/Time.hpp>
# include <random>

// if m_lifeTimerMax == 0.f there isn't life state
// if m_dieTimerMax == 0.f there isn't die state

class DecorAnimator
{
enum EState
{
	e_state_life,
	e_state_grow,
	e_state_die,
	e_state_sleep,
	e_state_stop,

	e_state_nb
};

public:
	DecorAnimator(float growTime = 2.f, float dieTime = 4.f, float beatTime = 3.f, float delta = 0.15f, float start = 4.f);
	virtual ~DecorAnimator(void) = default;

	void			pause(void);
	void			play(void);
	void			sleep(void);
	void			setup(sf::Time lifeTime = sf::seconds(1.f));
	bool			update(sf::Time frameTime);
	float			getAnimation(void) const;
	float			getAnimationTime(void) const;

private:
	EState			m_currentState;
	EState			m_lastState;
	float			m_animation;
	float			m_finalAnimation;

	float			m_startTimer;
	float			m_startTimerMax;
	float			m_lifeTimer;
	float			m_lifeTimerMax;
	float			m_growTimer;
	float			m_growTimerMax;
	float			m_dieTimer;
	float			m_dieTimerMax;
	float			m_beatTimer;
	float			m_beatTimerMax;
	bool			m_beatDirection;
	float			m_beatDelta;

	bool			computeState(float frameTime);
	void			computeBeat(float frameTime);
};

#endif
