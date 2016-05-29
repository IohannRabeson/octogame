#include "DecorAnimator.hpp"
#include "RandomGenerator.hpp"
#include <Interpolations.hpp>

DecorAnimator::DecorAnimator(float growTime, float dieTime, float beatTime, float delta, float start) :
	m_currentState(State::Grow),
	m_lastState(State::Grow),
	m_animation(0.f),
	m_startTimer(0.f),
	m_startTimerMax(0.f),
	m_lifeTimer(0.f),
	m_lifeTimerMax(0.f),
	m_growTimer(0.f),
	m_growTimerMax(growTime),
	m_dieTimer(0.f),
	m_dieTimerMax(dieTime),
	m_beatTimer(0.f),
	m_beatTimerMax(beatTime),
	m_beatDirection(true),
	m_beatDelta(delta)
{
	RandomGenerator generator;
	generator.setSeed("random");
	m_finalAnimation = 1.0f - m_beatDelta + generator.randomFloat(0.f, 0.1f);
	m_startTimerMax = generator.randomFloat(0.f, start);
}

void DecorAnimator::computeBeat(float frameTime)
{
	m_beatTimer += frameTime;
	if (m_beatTimer >= m_beatTimerMax)
	{
		m_beatTimer = 0.f;
		if (m_beatDirection == true)
			m_beatDirection = false;
		else
			m_beatDirection = true;
	}
	if (m_beatDirection == true)
		m_animation = octo::cosinusInterpolation(m_finalAnimation + m_beatDelta, m_finalAnimation, m_beatTimer / m_beatTimerMax);
	else
		m_animation = octo::cosinusInterpolation(m_finalAnimation, m_finalAnimation + m_beatDelta, m_beatTimer / m_beatTimerMax);
}

bool DecorAnimator::computeState(float frameTime)
{
	switch (m_currentState)
	{
		case State::Life:
		{
			if (!m_lifeTimerMax && m_dieTimerMax)
			{
				m_currentState = State::Die;
				break;
			}
			m_lifeTimer += frameTime;
			if (m_lifeTimer >= m_lifeTimerMax && m_animation == m_finalAnimation)
			{
				m_lifeTimer = 0.f;
				m_beatTimer = 0.f;
				if (m_dieTimerMax)
					m_currentState = State::Die;
				break;
			}
			computeBeat(frameTime);
			break;
		}
		case State::Grow:
		{
			m_growTimer += frameTime;
			m_animation = octo::cosinusInterpolation(0.f, m_finalAnimation + m_beatDelta, m_growTimer / m_growTimerMax);
			if (m_growTimer >= m_growTimerMax)
			{
				m_growTimer = 0.f;
				m_currentState = State::Life;
				m_beatDirection = true;
			}
			break;
		}
		case State::Die:
		{
			m_dieTimer += frameTime;
			m_animation = octo::cosinusInterpolation(m_finalAnimation, 0.f, m_dieTimer / m_dieTimerMax);
			if (m_dieTimer >= m_dieTimerMax)
			{
				m_dieTimer = 0.f;
				m_currentState = State::Grow;
				return true;
			}
			break;
		}
		case State::Sleep:
		{
			m_animation = m_finalAnimation;
			break;
		}
		case State::Stop:
		{
			break;
		}
		default:
			break;
	}
	return false;
}

void DecorAnimator::pause(void)
{
	if (m_currentState != State::Stop)
	{
		m_lastState = m_currentState;
		m_currentState = State::Stop;
	}
}

void DecorAnimator::play(void)
{
	if (m_currentState == State::Stop)
		m_currentState = m_lastState;
	else if (m_currentState == State::Sleep)
		m_currentState = State::Life;
}

void DecorAnimator::sleep(void)
{
	if (m_currentState != State::Grow)
		m_currentState = State::Sleep;
}

void DecorAnimator::die(void)
{
	if (m_currentState == State::Life)
		m_currentState = State::Die;
}

void DecorAnimator::setup(sf::Time lifeTime)
{
	m_lifeTimerMax = lifeTime.asSeconds();
}

void DecorAnimator::setBeatMouvement(float delta)
{
	m_beatDelta = delta;
}

bool DecorAnimator::update(sf::Time frameTime)
{
	if (m_startTimer < m_startTimerMax)
		m_startTimer += frameTime.asSeconds();
	else
	{
		if (computeState(frameTime.asSeconds()))
			return true;
	}
	return false;
}

float DecorAnimator::getAnimation(void) const
{
	return m_animation;
}

float DecorAnimator::getAnimationTime(void) const
{
	return m_lifeTimerMax + m_growTimerMax + m_dieTimerMax;
}

DecorAnimator::State DecorAnimator::getState(void) const
{
	return m_currentState;
}
