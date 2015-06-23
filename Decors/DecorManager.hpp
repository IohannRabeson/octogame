/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   DecorManager.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: irabeson <irabeson@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/06/08 02:45:47 by irabeson          #+#    #+#             */
/*   Updated: 2015/06/23 02:35:59 by irabeson         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef DECORMANAGER_HPP
# define DECORMANAGER_HPP

# include <SFML/System/Vector2.hpp>
# include <SFML/Graphics/Color.hpp>
# include <SFML/Graphics/Vertex.hpp>
# include <SFML/Graphics/RenderTarget.hpp>
# include <SFML/System/Time.hpp>

# include <cstddef>
# include <list>
# include <random>
# include <memory>

# include <GenericFactory.hpp>
# include <Camera.hpp>

class ADecor;
class ABiome;

class DecorManager
{
	typedef std::list<ADecor*>		List;
	typedef std::mt19937			RandomEngine;
public:
	typedef typename List::iterator	Iterator;
	typedef typename List::iterator	ConstIterator;

	enum class DecorTypes
	{
		Invalid,
		Tree,
		Rock,
		Crystal,
		Star,
		Cloud,
		Sun,
		Moon
	};

	explicit DecorManager(std::size_t maxVertexCount);

	void		setup(ABiome* biome);

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
	ABiome*							m_biome;
};

#endif
