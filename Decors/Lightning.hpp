/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Lightning.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: irabeson <irabeson@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/06/12 17:23:00 by irabeson          #+#    #+#             */
/*   Updated: 2015/06/25 16:33:30 by irabeson         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LIGHTNING_HPP
# define LIGHTNING_HPP
# include "ADecor.hpp"

# include <Math.hpp>
# include <VertexBuilder.hpp>

# include <list>
# include <memory>
# include <vector>
# include <random>

# include <SFML/Graphics/RenderTarget.hpp>

class Lightning
{
	typedef std::mt19937							Engine;
	typedef std::uniform_real_distribution<float>	Distri;
	
	class Segment;
	class Arc;
public:
	explicit Lightning(std::size_t maxVertices);

	void			update(sf::Time frameTime);
	void			draw(sf::RenderTarget& render)const;

	std::size_t		addArc(sf::Vector2f const& p0, sf::Vector2f const& p1, float thickness);
	void			setArc(std::size_t id, sf::Vector2f const& p0, sf::Vector2f const& p1);
	Arc&			getArc(std::size_t id);
	Arc const&		getArc(std::size_t id)const;
	void			removeArc(std::size_t id);
	void			clear();
	std::size_t		getArcCount()const;
private:
	typedef std::shared_ptr<Arc>	ArcPtr;
	std::unique_ptr<sf::Vertex[]>			m_vertices;
	std::size_t								m_verticeCount;
	octo::VertexBuilder						m_builder;
	std::mt19937							m_engine{3294};
	std::uniform_real_distribution<float>	m_dist{-1.f, 1.f};
	std::vector<ArcPtr>						m_arcs;
	float									m_defaultMiddleOffsetFactor;
	float									m_defaultBranchProbability;
	float									m_defaultBranchMaxAngle;
	float									m_defaultBranchMaxLenght;
	sf::Color								m_defaultColor;
	std::size_t								m_defaultFractalLevel;
	sf::Time								m_defaultCycleTime;
	std::size_t								m_currentArc;
	sf::Time								m_triggerTime;
	sf::Time								m_currentTime;
};

class Lightning::Segment
{
public:
	Segment()
	{
	}

	Segment(sf::Vector2f const& p0, sf::Vector2f const& p1) :
		m_p0(p0),
		m_p1(p1)
	{
	}

	Segment(Segment const&) = default;
	Segment(Segment&&) = default;

	Segment& operator = (Segment const&) = default;

	sf::Vector2f	direction()const
	{
		sf::Vector2f	diff = m_p1 - m_p0;

		octo::normalize(diff);
		return (diff);
	}

	sf::Vector2f	middle()const
	{
		return (sf::Vector2f((m_p0.x + m_p1.x) / 2.f, (m_p0.y + m_p1.y) / 2.f));
	}

	sf::Vector2f const&	p0()const
	{
		return (m_p0);
	}

	sf::Vector2f const&	p1()const
	{
		return (m_p1);
	}
private:
	sf::Vector2f	m_p0;
	sf::Vector2f	m_p1;
};

class Lightning::Arc
{
public:
	Arc(Engine& engine,
		Distri& dist,
		sf::Vector2f const& p0,
		sf::Vector2f const& p1,
		sf::Color const& color,
		float thickness,
		float middleOffsetFactor = 200.f,
		float branchProbability = 0.3f,
		float branchMaxAngle = 45.f,
		float branchMaxLenghtFactor = 0.7f,
		std::size_t fractalLevel = 4u,
		sf::Time cycleTime = sf::seconds(0.25f));

	void							setSegment(sf::Vector2f const& p0, sf::Vector2f const& p1);
	void							setBranchProbability(float probability);
	void							setColor(sf::Color const& color);
	void							setThickness(float thickness);
	void							setMiddleOffsetFactor(float factor);
	void							setFractalLevel(std::size_t level);
	void							setBranchMaxAngle(float angle);
	void							setBranchMaxLenght(float factor);

	void							restart();
	void							update(sf::Time frameTime);
	void							buildVertices(octo::VertexBuilder& builder)const;
	bool							isExpired()const;
private:
	void							fractalizeSegments();
	void							updateColor();
private:
	Engine&							m_engine;
	Distri&							m_dist;
	std::bernoulli_distribution		m_branchDist;
	std::list<Segment>				m_segments;
	std::list<Segment>				m_fractalizedSegments;
	sf::Color						m_color;
	float							m_thickness;
	float							m_middleOffsetFactor;
	std::size_t						m_fractalLevel;
	float							m_branchAngle;
	float							m_branchLenght;
	sf::Time						m_currentTime;
	sf::Time						m_cycleTime;
};
#endif
