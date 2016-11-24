/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: irabeson <irabeson@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/04/15 19:14:06 by irabeson          #+#    #+#             */
/*   Updated: 2016/10/14 18:37:37 by jbalestr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <Application.hpp>
#include <GraphicsManager.hpp>
#include <StateManager.hpp>
#include <Console.hpp>
#include <DefaultTransition.hpp>
#include "DefaultApplicationListener.hpp"
// Ecran de jeu
#include "StateTest.hpp"
#include "IntroScreen.hpp"
#include "LogoScreen.hpp"
#include "AnOctonautOdysseyScreen.hpp"
#include "GameScreen.hpp"
#include "MenuScreen.hpp"
#include "ResourceLoadingScreen.hpp"
#include "QuitScreen.hpp"
#include "TransitionLevelScreen.hpp"
#include "TransitionLevelZeroScreen.hpp"
#include "TransitionScreen.hpp"
#include "LevelZeroScreen.hpp"
#include "DeathScreen.hpp"
#include "AGameObject.hpp"
#include "MusicManager.hpp"
#include "LaboratoryEndScreen.hpp"
#include "LevelRocketEndScreen.hpp"

static void	setupStateManager(octo::StateManager& manager)
{
	manager.registerState<StateTest>("test");
	manager.registerState<IntroScreen>("intro");
	manager.registerState<GameScreen>("game");
	manager.registerState<MenuScreen>("menu");
	manager.registerState<ResourceLoadingScreen>("loading");
	manager.registerState<QuitScreen>("quit");
	manager.registerState<TransitionScreen>("transition");
	manager.registerState<TransitionLevelScreen>("transitionLevel");
	manager.registerState<DeathScreen>("octo_death");
	manager.registerState<LevelZeroScreen>("zero");
	manager.registerState<TransitionLevelZeroScreen>("transitionLevelZero");
	manager.registerState<LogoScreen>("logo");
	manager.registerState<AnOctonautOdysseyScreen>("anoctonautodyssey");
	manager.registerState<LaboratoryEndScreen>("laboratory_end");
	manager.registerState<LevelRocketEndScreen>("rocket_end");

	manager.registerTransition<octo::BlackFadeTransition>("default", true);
	manager.registerTransition<octo::WhiteFadeTransition>("white", false);
	manager.registerTransition<octo::BlueFadeTransition>("blue", false);
	manager.registerTransition<octo::RedFadeTransition>("red", false);
}

static void	setupConsole(octo::Console& console)
{
	// Ajoutez ici vos commandes ;)

	static_cast<void>(console);
}

int main(int argc, char **argv)
{
	try
	{
		octo::Application::initialize("An Octonaut Odyssey", "default.conf", argc, argv);
		octo::StateManager&							states = octo::Application::getStateManager();
		octo::Console&								console = octo::Application::getConsole();
		octo::GraphicsManager&						graphics = octo::Application::getGraphicsManager();
		DefaultApplicationListener					listener;

		graphics.addWindowListener(&listener);
		graphics.addKeyboardListener(&listener);
		setupStateManager(states);
		setupConsole(console);
		octo::Application::run("loading");
		MusicManager::deleteInstance();
	}
	catch (std::exception const& e)
	{
		std::cerr << "Run exception: " << e.what() << std::endl;
	}
	octo::Application::destroy();
    return (0);
}
