/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   FireflyTestScreen.hpp                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: irabeson <irabeson@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/04/16 17:57:45 by irabeson          #+#    #+#             */
/*   Updated: 2015/04/17 23:58:50 by irabeson         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FIREFLYTESTSCREEN_HPP
# define FIREFLYTESTSCREEN_HPP
# include <AbstractState.hpp>
# include <GraphicsManager.hpp>

# include "FireflySwarm.hpp"
# include "PointHandle.hpp"

# include <SFML/Graphics/CircleShape.hpp>
# include <vector>

class FireflyTestScreen : public octo::AbstractState,
						  public octo::IMouseListener
{
public:
	FireflyTestScreen();

	virtual void	start();
	virtual void	pause();
	virtual void	resume();
	virtual void	stop();

	virtual void	update(sf::Time frameTime);
	virtual void	draw(sf::RenderTarget& render)const;

	virtual void	onMoved(sf::Event::MouseMoveEvent const& event);
	virtual void	onPressed(sf::Event::MouseButtonEvent const&);
	virtual void	onReleased(sf::Event::MouseButtonEvent const&);
	virtual void	onWheel(sf::Event::MouseWheelEvent const&);
private:
	FireflySwarm							m_swarm;
	sf::View								m_view;
	std::vector<PointHandle>				m_handles;
	FireflySwarm::SpawnMode					m_spawnMode;
	sf::Color								m_color;
	sf::Vector2f							m_spawn;
	FireflySwarm::CirclePositionBehavior*	m_behavior;
	float									m_speed;
	float									m_radius;
	float									m_haloRadius;
};

#endif
