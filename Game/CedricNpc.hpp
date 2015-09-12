#ifndef CEDRICNPC_HPP
# define CEDRICNPC_HPP

# include "ANpc.hpp"

class SkyCycle;

class CedricNpc : public ANpc, AGameObject<GameObjectType::NpcCedric>
{
public:
	CedricNpc(SkyCycle const & skyCycle);
	virtual ~CedricNpc(void) = default;

	virtual void setup(void);
	virtual void update(sf::Time frametime);
	virtual float getHeight(void) const { return 0.f; }

protected:
	enum CedricEvents
	{
		LeftNight = Special2 + 1,
		IdleNight,
		RightNight,
		Special1Night,
		Special2Night,
	};

	virtual void setupMachine(void);
	virtual void updateState(void);
	virtual void updatePhysics(void);

private:
	octo::CharacterAnimation	m_idleAnimationNight;
	octo::CharacterAnimation	m_walkAnimationNight;
	octo::CharacterAnimation	m_special1AnimationNight;
	octo::CharacterAnimation	m_special2AnimationNight;
	SkyCycle const &			m_skyCycle;
	bool						m_prevDayState;
	float						m_timerSwitchDayNight;

};

#endif
