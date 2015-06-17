/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   DecorManagerDemoScreen.hpp                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: irabeson <irabeson@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/06/10 01:37:01 by irabeson          #+#    #+#             */
/*   Updated: 2015/06/11 18:59:18 by irabeson         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef DECORMANAGERDEMOSCREEN_HPP
# define DECORMANAGERDEMOSCREEN_HPP
# include <AbstractState.hpp>
# include <DefaultGraphicsListeners.hpp>
# include "DecorManager.hpp"
# include "TestBiome.hpp"

# include <map>
# include <array>

class DecorManagerDemoScreen : public octo::AbstractState,
							   public octo::DefaultMouseListener,
							   public octo::DefaultKeyboardListener
{
public:
	DecorManagerDemoScreen();

	virtual void	start();
	virtual void	pause();
	virtual void	resume();
	virtual void	stop();

	virtual void	update(sf::Time frameTime);
	virtual void	draw(sf::RenderTarget& render)const;
	virtual void 	onPressed(sf::Event::MouseButtonEvent const &event);
	virtual void 	onReleased(sf::Event::MouseButtonEvent const &event);
	virtual bool	onPressed(sf::Event::KeyEvent const& event);
	virtual bool	onReleased(sf::Event::KeyEvent const& event);
private:
	std::string		selectDecorType(std::string const& type);
	void			createDecor(sf::Vector2f const& pos);
	void			moveCamera(sf::Time frameTime, octo::Camera& camera)const;
private:
	typedef	std::map<std::string, DecorManager::DecorTypes>	DecorTypeMapper;
	enum	Directions
	{
		Up = 0,
		Right = 1,
		Down = 2,
		Left = 3
	};

	DecorManager				m_manager;
	TestBiome					m_biome;
	DecorTypeMapper				m_typeMapper;
	DecorManager::DecorTypes	m_currentDecorType;
	std::array<bool, 4>			m_viewDirections;
};

#endif
