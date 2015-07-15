/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   FsmDemoScreen.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: irabeson <irabeson@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/07/03 16:25:59 by irabeson          #+#    #+#             */
/*   Updated: 2015/07/15 20:28:33 by irabeson         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "FsmDemoScreen.hpp"
#include "ResourceDefinitions.hpp"
#include <Application.hpp>
#include <Console.hpp>
#include <ResourceManager.hpp>
#include <Camera.hpp>
#include <random>

class TamagotshiState : public FiniteStateMachine::AState
{
public:
	TamagotshiState(std::string const& name, octo::AnimatedSprite& sprite, octo::SpriteAnimation const& animation) :
		AState(name),
		m_sprite(sprite),
		m_animation(animation)
	{
	}

	void	start()
	{
		m_sprite.setAnimation(m_animation);
		m_sprite.restart();
	}

	void	stop()
	{
		m_sprite.stop();
	}

	void	update(sf::Time frameTime)
	{
		m_sprite.update(frameTime);
	}
private:
	octo::AnimatedSprite&			m_sprite;
	octo::SpriteAnimation const&	m_animation;
};

Tamagotshi::Tamagotshi()
{
	typedef octo::SpriteAnimation::Frame	Frame;
	typedef FiniteStateMachine::StatePtr	StatePtr;

	octo::ResourceManager& 					resources = octo::Application::getResourceManager();
	octo::Camera&							camera = octo::Application::getCamera();
	octo::Console&							console = octo::Application::getConsole();
	sf::Time								animTime = sf::seconds(0.5f);
	StatePtr								state0;
	StatePtr								state1;
	StatePtr								state2;
	StatePtr								state3;

	m_animatedSprite.setSpriteSheet(resources.getSpriteSheet(TAMAGOTCHI_OSS));
	m_animatedSprite.setScale(16.f, 16.f);
	m_anim0.setFrames({Frame(animTime, 1u), Frame(animTime, 2u)});
	m_anim0.setLoop(octo::LoopMode::Loop);
	m_anim1.setFrames({Frame(animTime, 3),
						Frame(animTime, 4),
						Frame(animTime, 5),
						Frame(animTime, 6)});
	m_anim1.setLoop(octo::LoopMode::Loop);
	m_anim2.setFrames({Frame(animTime, 10), Frame(animTime, 11)});
	m_anim2.setLoop(octo::LoopMode::Loop);
	m_anim3.setFrames({Frame(animTime, 7),
						Frame(animTime, 8),
						Frame(animTime, 9),
						Frame(animTime, 8)});
	m_anim3.setLoop(octo::LoopMode::Loop);
	state0.reset(new TamagotshiState("state 0", m_animatedSprite, m_anim0));
	state1.reset(new TamagotshiState("state 1", m_animatedSprite, m_anim1));
	state2.reset(new TamagotshiState("state 2", m_animatedSprite, m_anim2));
	state3.reset(new TamagotshiState("state 3", m_animatedSprite, m_anim3));
	m_machine.addTransition(Event1, state0, state1, [this](){m_text.setString("state 1");});
	m_machine.addTransition(Event2, state0, state2, [this](){m_text.setString("state 2");});
	m_machine.addTransition(Event3, state0, state3, [this](){m_text.setString("state 3");});
	m_machine.addTransition(Event0, state1, state0, [this](){m_text.setString("state 0");});
	m_machine.addTransition(Event0, state2, state0, [this](){m_text.setString("state 0");});
	m_machine.addTransition(Event0, state3, state0, [this](){m_text.setString("state 0");});
	m_machine.setStart(state0);
	m_machine.start();
	camera.setCenter(m_animatedSprite.getPosition() + sf::Vector2f(19, 20) * 16.f / 2.f);
	m_text.setFont(resources.getFont(VERAMONO_TTF));
	m_text.setPosition(m_animatedSprite.getPosition() + sf::Vector2f(0, 330));
	m_text.setString("Initial state");
	console.addCommand(L"demo.export_automate", [this](std::string const& filePath)
			{
				std::ofstream	file;
				std::string		result;

				file.open(filePath);
				if (file.is_open())
				{
					m_machine.exportDot(file, "tamagotchi_demo");
					result = "Graph exported as " + filePath;
				}
				else
				{
					result = "Unable to open file " + filePath;
				}
				return (result);
			});
}

std::vector<Tamagotshi::EventId>	Tamagotshi::getPossibleEvents()const
{
	std::vector<EventId>	results;
	auto					state = m_machine.getCurrentState();

	for (auto transition : *state)
	{
		results.push_back(transition.second.getTriggerEvent());
	}
	return (results);
}

void	Tamagotshi::trigger(EventId eventId)
{
	m_machine.setNextEvent(eventId);
}

void	Tamagotshi::update(sf::Time frameTime)
{
	static sf::Time const							BehaviorTime = sf::seconds(5);
	static std::uniform_int_distribution<EventId>	chooseEventDist;
	static std::mt19937								engine(std::time(0));
	std::vector<EventId>							possibles = getPossibleEvents();

	m_elasped += frameTime;
	if (m_elasped >= BehaviorTime)
	{
		auto	p = std::minmax_element(possibles.begin(), possibles.end());

		chooseEventDist.param(std::uniform_int_distribution<EventId>::param_type(
					*p.first, *p.second));
		m_machine.setNextEvent(chooseEventDist(engine));
		m_elasped -= BehaviorTime;
	}
	m_machine.update(frameTime);
	m_animatedSprite.update(frameTime);
}

void	Tamagotshi::draw(sf::RenderTarget& render)const
{
	render.clear();
	m_animatedSprite.draw(render, sf::RenderStates());
	render.draw(m_text);
}

void	FsmDemoScreen::start()
{

}

void	FsmDemoScreen::pause()
{

}

void	FsmDemoScreen::resume()
{

}

void	FsmDemoScreen::stop()
{

}

void	FsmDemoScreen::update(sf::Time frameTime)
{
	m_tamagotshi.update(frameTime);
}

void	FsmDemoScreen::draw(sf::RenderTarget& render)const
{
	m_tamagotshi.draw(render);
}
