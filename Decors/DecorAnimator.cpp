#include "DecorAnimator.hpp"

std::mt19937	DecorAnimator::m_engine;

DecorAnimator::DecorAnimator(float growTime, float dieTime, float beatTime, float delta, float start) :
	m_currentState(e_state_grow),
	m_lastState(e_state_grow),
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
	std::random_device rd;
	m_engine.seed(rd());
	m_finalAnimation = 1.0f - m_beatDelta + randomFloat(0.f, 0.1f);
	m_startTimerMax = randomFloat(0.f, start);
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
		m_animation = m_finalAnimation + (m_beatDelta * m_beatTimer / m_beatTimerMax);
	else
		m_animation = m_finalAnimation + m_beatDelta - (m_beatDelta * m_beatTimer / m_beatTimerMax);
}

bool DecorAnimator::computeState(float frameTime)
{
	switch (m_currentState)
	{
		case e_state_life:
		{
			if (!m_lifeTimerMax && m_dieTimerMax)
			{
				m_currentState = e_state_die;
				break;
			}
			m_lifeTimer += frameTime;
			if (m_lifeTimer >= m_lifeTimerMax && m_animation == m_finalAnimation)
			{
				m_lifeTimer = 0.f;
				m_beatTimer = 0.f;
				if (m_dieTimerMax)
					m_currentState = e_state_die;
				break;
			}
			computeBeat(frameTime);
			break;
		}
		case e_state_grow:
		{
			m_growTimer += frameTime;
			m_animation = m_growTimer / m_growTimerMax * m_finalAnimation;
			if (m_growTimer >= m_growTimerMax)
			{
				m_growTimer = 0.f;
				m_currentState = e_state_life;
			}
			break;
		}
		case e_state_die:
		{
			m_dieTimer += frameTime;
			m_animation = m_finalAnimation - m_dieTimer / m_dieTimerMax * m_finalAnimation;
			if (m_dieTimer >= m_dieTimerMax)
			{
				m_dieTimer = 0.f;
				m_currentState = e_state_grow;
				return true;
			}
			break;
		}
		case e_state_sleep:
		{
			m_animation = m_finalAnimation;
			break;
		}
		case e_state_stop:
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
	if (m_currentState != e_state_stop)
	{
		m_lastState = m_currentState;
		m_currentState = e_state_stop;
	}
}

void DecorAnimator::play(void)
{
	if (m_currentState == e_state_stop)
		m_currentState = m_lastState;
	else if (m_currentState == e_state_sleep)
		m_currentState = e_state_life;
}

void DecorAnimator::sleep(void)
{
	if (m_currentState != e_state_grow)
		m_currentState = e_state_sleep;
}

void DecorAnimator::setup(sf::Time lifeTime)
{
	m_lifeTimerMax = lifeTime.asSeconds();
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

float DecorAnimator::randomFloat(float min, float max)
{
	if (max - min == 0)
		return max;
	std::uniform_real_distribution<float> distribution(min, max);
	return distribution(m_engine);
}

