#ifndef DISAPPEARNPC_HPP
# define DISAPPEARNPC_HPP

# include "ANpc.hpp"
# include "RandomGenerator.hpp"

class ADisappearNpc : public ANpc, public AGameObject<GameObjectType::DisappearNpc>
{
public:
	ADisappearNpc(ResourceKey const & key, float alphaMin = 200.f, float alphaMax = 255.f, bool isFlying = true);
	virtual ~ADisappearNpc(void) = default;

	virtual void setup(void) = 0;
	virtual void update(sf::Time frametime);

protected:
	virtual void setupMachine(void);
	virtual void updateState(void) {};
	static RandomGenerator & getGenerator(void);

private:
	static RandomGenerator	m_generator;

	bool					m_isVisible;
	bool					m_isFlying;
	float					m_alphaCurrent;
	float					m_alphaTarget;
	float const				m_alphaMin;
	float const				m_alphaMax;
	float					m_alphaAlmostDisappear;
	sf::Time				m_randomDisappearTimer;
	sf::Time				m_randomAppearTimer;

	void					makeDisappear(sf::Time frametime);
};

#endif
