#ifndef NANOEFFECT_HPP
# define NANOEFFECT_HPP

# include "DecorAnimator.hpp"
# include "RandomGenerator.hpp"
# include <AnimatedSprite.hpp>
# include <VertexBuilder.hpp>
# include <SFML/Graphics/Text.hpp>
# include <SFML/Graphics/RenderTarget.hpp>
# include <SFML/Graphics/Drawable.hpp>
# include <SFML/Graphics/Transformable.hpp>

# include <memory>

class NanoEffect : public sf::Drawable,
				public sf::Transformable
{
public:
	NanoEffect(void);
	virtual ~NanoEffect(void) = default;

	void							update(sf::Time frameTime);
	void							draw(sf::RenderTarget & render, sf::RenderStates states = sf::RenderStates()) const;

	sf::Vector2f const &			getPosition(void) const;
	sf::Color const &				getColor(void) const;
	float							getSizeCorner(void) const;
	bool							isActive(void) const;
	void							setPosition(sf::Vector2f const & position);
	void							setColor(sf::Color const & color);
	void							setActive(bool isActive);
	void							onTransfer(void);

private:
	void							createLosange(sf::Vector2f const & size,
												 sf::Vector2f const & origin,
												 sf::Color const & color,
												 octo::VertexBuilder& builder);

	void							createEffect(sf::Vector2f const & size,
												 sf::Vector2f const & origin,
												 sf::Color color,
												 octo::VertexBuilder& builder);

	void playSound(void);

	std::unique_ptr<sf::Vertex[]>	m_vertices;
	std::size_t						m_count;
	std::size_t						m_used;
	octo::VertexBuilder				m_builder;

	RandomGenerator					m_generator;
	sf::Vector2f					m_size;
	sf::Vector2f					m_position;
	sf::Color						m_color;
	bool							m_isActive;
	bool							m_isTransfer;
	bool							m_isRandom;
	sf::Time						m_glowingTimer;
	sf::Time						m_glowingTimerMax;
	sf::Time						m_randomGlowing;
	sf::Time						m_randomGlowingMax;
};

#endif
