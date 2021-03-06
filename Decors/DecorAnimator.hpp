#ifndef DECORANIMATOR_HPP
# define DECORANIMATOR_HPP

# include <SFML/System/Time.hpp>
# include <random>
# include "RandomGenerator.hpp"

// TODO: This class needs to be rebuilt from scratch (too messy)
// if m_lifeTimerMax == 0.f there isn't life state (default)
// if m_dieTimerMax == 0.f there isn't die state

class DecorAnimator
{
public:
enum class State
{
	Wait,
	Life,
	Grow,
	Die,
	Dead,
	Sleep,
	Stop,

	Count
};

	DecorAnimator(float growTime = 2.f, float dieTime = 4.f, float beatTime = 3.f, float delta = 0.15f, float start = 4.f);
	virtual ~DecorAnimator(void) = default;

	void							pause(void);
	void							play(void);
	void							sleep(void);
	void							die(void);
	void							setup(sf::Time lifeTime = sf::seconds(1.f));
	void							setBeatMouvement(float delta);
	void							update(sf::Time frameTime);
	float							getAnimation(void) const;
	float							getAnimationTime(void) const;
	DecorAnimator::State const &	getState(void) const;

private:
	static RandomGenerator	m_generator;

	State					m_currentState;
	State					m_lastState;
	float					m_animation;
	float					m_finalAnimation;
	float					m_saveFinalAnimation;

	float					m_startTimer;
	float					m_startTimerMax;
	float					m_lifeTimer;
	float					m_lifeTimerMax;
	float					m_growTimer;
	float					m_growTimerMax;
	float					m_dieTimer;
	float					m_dieTimerMax;
	float					m_dieSpeed;
	float					m_beatTimer;
	float					m_beatTimerMax;
	bool					m_beatDirection;
	float					m_beatDelta;
	float					m_beatDeltaValue;

	void					computeState(float frameTime);
	void					computeBeat(float frameTime);
};

#endif
