#ifndef ABUBBLE_HPP
# define ABUBBLE_HPP

# include "DecorAnimator.hpp"
# include <SFML/Graphics/Text.hpp>
# include <SFML/Graphics/RenderTarget.hpp>
# include <VertexBuilder.hpp>
# include <SFML/Graphics/Drawable.hpp>
# include <SFML/Graphics/Transformable.hpp>

class ABubble : public sf::Drawable,
				public sf::Transformable
{
public:
enum Type
{
	None,
	Inactive,
	Speak,
	Think
};
	ABubble(void);
	virtual ~ABubble(void) = default;

	void					update(sf::Time frameTime);
	void					draw(sf::RenderTarget & render, sf::RenderStates states = sf::RenderStates()) const;
	
	virtual sf::Vector2f	getContentSize() const = 0;
	virtual void			updateContent(sf::Time frameTime, sf::Vector2f const & position) = 0;
	virtual void			drawContent(sf::RenderTarget & render, sf::RenderStates states) const = 0;

	Type					getType(void) const;
	sf::Vector2f			getContentUpLeft(void) const;
	sf::Vector2f			getPosition(void) const;
	sf::Color				getColor(void) const;
	void					setType(Type type);
	void					setPosition(sf::Vector2f const & position);
	void					setColor(sf::Color const & color);

private:
	void					createBubble(sf::Vector2f const & size,
										float sizeCorner,
										sf::Vector2f origin,
										sf::Color const & color,
										Type type,
										octo::VertexBuilder& builder);

	void					createExtension(sf::Vector2f const & position,
											sf::Color const & color,
											Type type,
											octo::VertexBuilder& builder);

	static constexpr float			m_sizeCorner = 20.f;

	std::unique_ptr<sf::Vertex[]>	m_vertices;
	std::size_t						m_count;
	std::size_t						m_used;
	octo::VertexBuilder				m_builder;

	Type							m_currentType;
	sf::Vector2f					m_size;
	sf::Vector2f					m_contentUpLeft;
	sf::Vector2f					m_position;
	sf::Vector2f					m_relativePos;
	sf::Color						m_color;
};

#endif
