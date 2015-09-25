#ifndef FATNPC_HPP
# define FATNPC_HPP

# include "ANpc.hpp"

class FatNpc : public ANpc, public AGameObject<GameObjectType::FatNpc>
{
public:
	FatNpc(void);
	virtual ~FatNpc(void) = default;

	virtual void setup(void);

protected:
	virtual void setupMachine(void);
	virtual void updateState(void);
	void setWalkEvent(std::size_t event);

	std::mt19937								m_engine;
	std::uniform_int_distribution<std::size_t>	m_specialDistribution;
	std::size_t									m_lastState;

};

#endif
