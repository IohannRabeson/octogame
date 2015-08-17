/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   FsmDemoScreen.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: irabeson <irabeson@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/07/03 16:22:20 by irabeson          #+#    #+#             */
/*   Updated: 2015/07/20 17:25:28 by irabeson         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FSMDEMOSCREEN_HPP
# define FSMDEMOSCREEN_HPP

# include <FiniteStateMachine.hpp>
# include <AnimatedSprite.hpp>
# include <AbstractState.hpp>

# include <SFML/Graphics/RectangleShape.hpp>
# include <SFML/Graphics/Text.hpp>

class Tamagotshi
{
	typedef octo::FiniteStateMachine::EventId	EventId;
public:
	enum Event
	{
		Event0 = 0,
		Event1,
		Event2,
		Event3
	};

	Tamagotshi();

	void					start();
	std::vector<EventId>	getPossibleEvents()const;
	void					trigger(EventId eventId);
	void					update(sf::Time frameTime);
	void					draw(sf::RenderTarget& render)const;
private:	
	octo::FiniteStateMachine	m_machine;
	octo::AnimatedSprite		m_animatedSprite;
	octo::SpriteAnimation		m_anim0;
	octo::SpriteAnimation		m_anim1;
	octo::SpriteAnimation		m_anim2;
	octo::SpriteAnimation		m_anim3;
	sf::Time					m_elasped;
	sf::Text					m_text;
};

class FsmDemoScreen : public octo::AbstractState
{
public:
	virtual void	start();
	virtual void	pause();
	virtual void	resume();
	virtual void	stop();

	virtual void	update(sf::Time frameTime);
	virtual void	draw(sf::RenderTarget& render)const;
private:
	Tamagotshi	m_tamagotshi;
};

#endif
