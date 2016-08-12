#ifndef DISAPPEARNPC_HPP
# define DISAPPEARNPC_HPP

# include "ANpc.hpp"
# include "RandomGenerator.hpp"

class DisappearNpc : public ANpc
{
public:
	DisappearNpc(ResourceKey const & key, float alphaMin = 200.f, float alphaMax = 255.f, bool isFlying = true);
	virtual ~DisappearNpc(void) = default;

	virtual void setup(void);
	virtual void update(sf::Time frametime);

protected:
	virtual void setupMachine(void);
	virtual void updateState(void);

private:
	void					makeDisappear(sf::Time frametime);

	bool					m_isVisible;
	bool					m_isFlying;
	static RandomGenerator	m_generator;
	float					m_alphaCurrent;
	float					m_alphaTarget;
	float const				m_alphaMin;
	float const				m_alphaMax;
	float					m_alphaAlmostDisappear;
	sf::Time				m_randomDisappearTimer;
	sf::Time				m_randomAppearTimer;
};

#endif
