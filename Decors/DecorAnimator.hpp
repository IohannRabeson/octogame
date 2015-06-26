#ifndef DECORANIMATOR_HPP
# define DECORANIMATOR_HPP

# include <SFML/System/Time.hpp>
# include <random>

class DecorAnimator
{
enum EState
{
	e_state_life,
	e_state_grow,
	e_state_die,
	e_state_stop,

	e_state_nb
};

public:
	DecorAnimator(void);
	DecorAnimator(float growTime, float dieTime, float beatTime, float delta);
	virtual ~DecorAnimator(void) = default;

	void			pause(void);
	void			play(void);
	void			setup(sf::Time lifeTime = sf::Time::Zero);
	void			update(sf::Time frameTime);
	float			getAnimation(void) const;

private:
	EState			m_currentState;
	EState			m_lastState;
	float			m_animation;

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

	void			computeState(float frameTime);
	void			computeBeat(float frameTime);

private:
	static			std::mt19937 m_engine;
	static float	randomFloat(float min, float max);
};

#endif
