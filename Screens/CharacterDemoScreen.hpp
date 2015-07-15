/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CharacterDemoScreen.hpp                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: irabeson <irabeson@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/07/15 19:49:23 by irabeson          #+#    #+#             */
/*   Updated: 2015/07/15 20:30:16 by irabeson         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CHARACTERDEMOSCREEN_HPP
# define CHARACTERDEMOSCREEN_HPP
# include <AbstractState.hpp>

class ACharacter;

class CharacterDemoScreen : public octo::AbstractState
{
public:
	CharacterDemoScreen();

	virtual void	start();
	virtual void	pause();
	virtual void	resume();
	virtual void	stop();

	virtual void	update(sf::Time frameTime);
	virtual void	draw(sf::RenderTarget& render)const;
private:
	std::unique_ptr<ACharacter>	m_character;
};


#endif
