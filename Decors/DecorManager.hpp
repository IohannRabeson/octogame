/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   DecorManager.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: irabeson <irabeson@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/06/08 02:45:47 by irabeson          #+#    #+#             */
/*   Updated: 2015/06/24 05:52:59 by irabeson         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef DECORMANAGER_HPP
# define DECORMANAGER_HPP

# include <SFML/System/Vector2.hpp>
# include <SFML/Graphics/Color.hpp>
# include <SFML/Graphics/Vertex.hpp>
# include <SFML/Graphics/RenderTarget.hpp>
# include <SFML/Graphics/Drawable.hpp>
# include <SFML/Graphics/Transformable.hpp>
# include <SFML/System/Time.hpp>

# include <cstddef>
# include <list>
# include <random>
# include <memory>

# include <VertexBuilder.hpp>
# include <GenericFactory.hpp>
# include <Camera.hpp>

class ADecor;
class ABiome;

class DecorManager : public sf::Drawable,
					 public sf::Transformable
{
	typedef std::list<ADecor*>		List;
	typedef std::mt19937			RandomEngine;
public:
	typedef typename List::iterator			Iterator;
	typedef typename List::const_iterator	ConstIterator;

	enum class DecorTypes
	{
		Invalid,
		Tree,
		Rock,
		Crystal,
		Star,
		Cloud,
		Sun,
		Moon,
		Rainbow,
		Mushroom,
		GroundRock,
		Sky,
		SunLight
	};

	explicit DecorManager(std::size_t maxVertexCount);
	~DecorManager();

	void		setup(ABiome* biome);

	inline Iterator begin() { return m_elements.begin(); };
	inline Iterator end() { return m_elements.end(); };
	ConstIterator begin() const { return m_elements.begin(); };
	ConstIterator end() const { return m_elements.end(); };

	Iterator	add(DecorTypes type);
	Iterator	add(ADecor* decor);
	Iterator	erase(Iterator iterator);
	void		clear();

	void		update(sf::Time frameTime, octo::Camera const& camera);
	void		draw(sf::RenderTarget& render, sf::RenderStates states = sf::RenderStates())const;
private:
	void		registerDecors();
private:
	typedef octo::GenericFactory<DecorTypes, ADecor>	Factory;
	Factory							m_factory;
	List							m_elements;
	std::unique_ptr<sf::Vertex[]>	m_vertices;
	std::size_t						m_count;
	std::size_t						m_used;
	octo::VertexBuilder				m_builder;
	ABiome*							m_biome;
};

#endif
