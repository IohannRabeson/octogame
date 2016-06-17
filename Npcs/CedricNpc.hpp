#ifndef CEDRICNPC_HPP
# define CEDRICNPC_HPP

# include <SFML/Graphics/Shader.hpp>
# include "ANpc.hpp"

class SkyCycle;

class CedricNpc : public ANpc, public AGameObject<GameObjectType::CedricNpc>
{
public:
	CedricNpc(SkyCycle const & skyCycle);
	virtual ~CedricNpc(void);

	void startBalle(void);

	virtual void setup(void);
	virtual void update(sf::Time frametime);
	virtual float getHeight(void) const { return 0.f; }
	std::size_t getId(void) const { return m_id; }
	sf::Time getEffectDuration(void) const;

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
	virtual bool canWalk(void) const;

private:
	static std::size_t			m_count;
	octo::CharacterAnimation	m_idleAnimationNight;
	octo::CharacterAnimation	m_walkAnimationNight;
	octo::CharacterAnimation	m_special1AnimationNight;
	octo::CharacterAnimation	m_special2AnimationNight;
	SkyCycle const &			m_skyCycle;
	bool						m_prevDayState;
	std::size_t					m_id;

};

#endif
