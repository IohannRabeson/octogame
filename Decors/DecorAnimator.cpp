#include "DecorAnimator.hpp"

std::mt19937	DecorAnimator::m_engine;

DecorAnimator::DecorAnimator(void) :
	m_currentState(e_state_grow),
	m_lastState(e_state_grow),
	m_animation(0.f),
	m_lifeTimer(0.f),
	m_lifeTimerMax(10.f),
	m_growTimer(0.f),
	m_growTimerMax(2.f),
	m_dieTimer(0.f),
	m_dieTimerMax(2.f),
	m_beatTimer(0.f),
	m_beatTimerMax(1.f),
	m_beatDirection(true),
	m_beatDelta(0.2f)
{
	std::random_device rd;
	m_engine.seed(rd());
//	m_timer += randomFloat(0.f, 1.f);
}

//TODO: Not tested
DecorAnimator::DecorAnimator(float growTime, float dieTime, float beatTime, float delta) :
	m_currentState(e_state_grow),
	m_lastState(e_state_grow),
	m_animation(0.f),
	m_lifeTimer(0.f),
	m_lifeTimerMax(10.f),
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
}

void DecorAnimator::computeBeat(float frameTime)
{
	(void)m_beatDelta;
	m_animation = 1.f;

	m_beatTimer += frameTime;
	if (m_beatTimer >= m_beatTimerMax)
	{
		m_beatTimer = 0.f;
		if (m_beatDirection == true)
			m_beatDirection = false;
		else
			m_beatDirection = true;
	}
	//TODO: Be smart and find how to do that!
	if (m_beatDirection == true)
	{
	}
	else
	{
	}
}

void DecorAnimator::computeState(float frameTime)
{
	switch (m_currentState)
	{
		case e_state_life:
		{
			m_lifeTimer += frameTime;
			if (m_lifeTimer >= m_lifeTimerMax)
			{
				m_lifeTimer = 0.f;
				m_currentState = e_state_die;
				break;
			}
			computeBeat(frameTime);
			break;
		}
		case e_state_grow:
		{
			m_growTimer += frameTime;
			m_animation = m_growTimer / m_growTimerMax;
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
			m_animation = 1.f - m_dieTimer / m_dieTimerMax;
			if (m_dieTimer >= m_dieTimerMax)
			{
				m_dieTimer = 0.f;
				m_currentState = e_state_grow;
			}
			break;
		}
		case e_state_stop:
		{
			m_animation = 1.f;
			break;
		}
		default:
			break;
	}
}

/*
				else if (m_timer > m_timerMax / 2.f)
					m_animation += frameTime / (20.f + (m_timer - 2.f) * 10.f);
				else if (m_timer < m_timerMax / 2.f)
					m_animation -= frameTime / (20.f + m_timer * 10.f);
*/

//TODO: Not tested
void DecorAnimator::pause(void)
{
	if (m_currentState != e_state_stop)
	{
		m_lastState = m_currentState;
		m_currentState = e_state_stop;
	}
}
//TODO:: Not tested
void DecorAnimator::play(void)
{
	if (m_currentState == e_state_stop)
		m_currentState = m_lastState;
}

void DecorAnimator::setup(sf::Time lifeTime)
{
	m_lifeTimerMax = lifeTime.asSeconds();
}

void DecorAnimator::update(sf::Time frameTime)
{
	computeState(frameTime.asSeconds());
}

float DecorAnimator::getAnimation(void) const
{
	return m_animation;
}

float DecorAnimator::randomFloat(float min, float max)
{
	if (max - min == 0)
		return max;
	std::uniform_real_distribution<float> distribution(min, max);
	return distribution(m_engine);
}

