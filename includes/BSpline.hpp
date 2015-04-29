/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   BSpline.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: irabeson <irabeson@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/03/29 13:07:36 by irabeson          #+#    #+#             */
/*   Updated: 2015/04/18 00:31:06 by irabeson         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BSPLINE_HPP
# define BSPLINE_HPP
# include <SFML/System/Vector2.hpp>
# include <deque>
# include <Math.hpp>
# include <Interpolations.hpp>

class BSpline
{
public:
	typedef typename std::deque<sf::Vector2f>::iterator				iterator;
	typedef typename std::deque<sf::Vector2f>::const_iterator		const_iterator;

	explicit BSpline(std::size_t capacity) :
		m_points(capacity)
	{
	}

	void	pushBack(sf::Vector2f const& position)
	{
		m_points.push_back(position);
	}

	void	popFront()
	{
		m_points.pop_front();
	}

	void	clear()
	{
		m_points.clear();
	}

	sf::Vector2f	compute(float t)const
	{
		int					intIndex = static_cast<int>(t);
		float				floatIndex = t - intIndex;
		sf::Vector2f const&	p0 = getPoint(intIndex);
		sf::Vector2f const&	p1 = getPoint(intIndex + 1);
		sf::Vector2f const&	p2 = getPoint(intIndex + 2);
		sf::Vector2f const&	p3 = getPoint(intIndex + 3);
		
		return (octo::bsplineInterpolation(p0, p1, p2, p3, floatIndex));
	}

	float				maxT()const
	{
		return (m_points.size());
	}

	sf::Vector2f&		operator[](std::size_t i)
	{
		return (m_points[i]);
	}

	sf::Vector2f const&	operator[](std::size_t i)const
	{
		return (m_points[i]);
	}

	std::size_t	size()const
	{
		return (m_points.size());
	}

	iterator	begin()
	{
		return (m_points.begin());
	}

	iterator	end()
	{
		return (m_points.end());
	}

	const_iterator	begin()const
	{
		return (m_points.begin());
	}

	const_iterator	end()const
	{
		return (m_points.end());
	}

	const_iterator	cbegin()const
	{
		return (m_points.begin());
	}

	const_iterator	cend()const
	{
		return (m_points.end());
	}
private:
	sf::Vector2f const&	getPoint(int i)const
	{
		int			count = static_cast<int>(m_points.size());

		if (i <= 0)
			return (m_points.front());
		else if (i >= count)
			return (m_points.back());
		else
			return (m_points[i]);
	}
private:
	std::deque<sf::Vector2f>	m_points;
};

#endif
