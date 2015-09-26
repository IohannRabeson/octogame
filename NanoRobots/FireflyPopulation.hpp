/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   FireflyPopulation.hpp                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: irabeson <irabeson@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/04/19 13:29:28 by irabeson          #+#    #+#             */
/*   Updated: 2015/05/04 02:50:02 by irabeson         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FIREFLYPOPULATION_HPP
# define FIREFLYPOPULATION_HPP
# include "FireflySwarm.hpp"
# include <random>
# include <IColorProvider.hpp>

class FireflySwarm::AbstractPopulation
{
public:
	virtual ~AbstractPopulation();

	virtual void	setupFirefly(FireflySwarm::Firefly& fly) = 0;
};

/*!	Cree un ensemble de particules avec les memes parametres */
class FireflySwarm::UniquePopulation : public FireflySwarm::AbstractPopulation
{
public:
	virtual void	setupFirefly(FireflySwarm::Firefly& fly);
private:
	sf::Color	m_color;
	float		m_speed;
	float		m_radius;
	float		m_haloRadius;
	sf::Time	m_maxTime;
};

/*!	Cree un ensemble de particules avec des parametres variant de facon uniform. */
class FireflySwarm::UniformPopulation : public FireflySwarm::AbstractPopulation
{
	typedef std::mt19937							Engine;
	typedef std::uniform_real_distribution<float>	Distribution;
public:
	UniformPopulation(std::size_t seed,
					  octo::IColorProvider const* colors,
					  float minSpeed,
					  float maxSpeed,
					  float minRadius,
					  float maxRadius,
					  float minHaloRadius,
					  float maxHaloRadius,
					  sf::Time minMaxTime,
					  sf::Time maxMaxTime);

	virtual void		setupFirefly(FireflySwarm::Firefly& fly);

	void				setColors(octo::IColorProvider const* colors);

	void				setMinSpeed(float value);
	float				getMinSpeed()const;

	void				setMaxSpeed(float value);
	float				getMaxSpeed()const;

	void				setMinRadius(float value);
	float				getMinRadius()const;

	void				setMaxRadius(float value);
	float				getMaxRadius()const;

	void				setMinHaloRadius(float value);
	float				getMinHaloRadius()const;

	void				setMaxHaloRadius(float value);
	float				getMaxHaloRadius()const;

	template <class T>
	inline T			getRandom(T min, T max)
	{
		return (min + (max - min) * m_distribution(m_engine));
	}
private:
	Engine					m_engine;
	Distribution			m_distribution;
	octo::IColorProvider const*	m_colors;
	float					m_minSpeed;
	float					m_maxSpeed;
	float					m_minRadius;
	float					m_maxRadius;
	float					m_minHaloRadius;
	float					m_maxHaloRadius;
	sf::Time				m_minMaxTime;
	sf::Time				m_maxMaxTime;
};

/*!	Cree un ensemble de particules avec des parametres variant
 *	suivant la loi normale
 */
class FireflySwarm::NormalPopulation
{
public:
	virtual ~NormalPopulation();

	virtual void	setupFirefly(FireflySwarm::Firefly& fly);
};

#endif
