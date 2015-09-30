#ifndef EVA_HPP
# define EVA_HPP

# include "WaterCascadeSystem.hpp"
# include "ANpc.hpp"

class EvaNpc : public ANpc, public AGameObject<GameObjectType::EvaNpc>
{
public:
	EvaNpc(sf::Color const & color);
	virtual ~EvaNpc(void) = default;

	virtual void setup(void);
	virtual void update(sf::Time frameTime);
	virtual void draw(sf::RenderTarget & render, sf::RenderStates states) const;

protected:
	virtual void setupMachine(void);
	virtual void updateState(void);

private:
	enum State
	{
		StartSpecial1 = Special2 + 1,
		EndSpecial1
	};
	octo::CharacterAnimation	m_startSpecial1;
	octo::CharacterAnimation	m_endSpecial1;
	WaterCascadeSystem			m_particles;

};

#endif
