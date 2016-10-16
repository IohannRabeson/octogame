#ifndef ROCKETEND_HPP
# define ROCKETEND_HPP

# include "SmokeSystem.hpp"
# include <CharacterSprite.hpp>
# include <CharacterAnimation.hpp>

class RocketEnd
{
public:
	RocketEnd(sf::Vector2f const & position);
	virtual ~RocketEnd(void) = default;

	virtual void setPosition(sf::Vector2f const & position);
	virtual void update(sf::Time frametime);
	virtual void drawFront(sf::RenderTarget & render, sf::RenderStates states) const;

private:
	enum State
	{
		Idle
	};
	octo::CharacterSprite			m_spriteRocket;
	octo::CharacterAnimation		m_animationRocket;
	octo::CharacterSprite			m_spriteOcto;
	octo::CharacterAnimation		m_animationOcto;
	std::size_t						m_smokesCount;
	std::unique_ptr<SmokeSystem[]>	m_smokes;
	sf::Vector2f					m_position;
	sf::Vector2f					m_offset;

	virtual void setupMachine(void);

};

#endif
