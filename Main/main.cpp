/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: irabeson <irabeson@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/04/15 19:14:06 by irabeson          #+#    #+#             */
/*   Updated: 2015/09/25 11:40:41 by jbalestr         ###   ########.fr       */
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
#include "FireflyTestScreen.hpp"
#include "PaletteDemoScreen.hpp"
#include "SpriteSheetDemoScreen.hpp"
#include "AnimatedSpriteDemoScreen.hpp"
#include "AudioDemoScreen.hpp"
#include "EngineScreen.hpp"
#include "PhysicsMapScreen.hpp"
#include "LightningDemoScreen.hpp"
#include "GameScreen.hpp"
#include "DecorManagerDemoScreen.hpp"
#include "ParticleDemoScreen.hpp"
#include "FsmDemoScreen.hpp"
#include "ResourceLoadingScreen.hpp"
#include "QuitScreen.hpp"
#include "ElevatorStreamDemo.hpp"
#include "TransitionLevelScreen.hpp"
#include "TransitionLevelZeroScreen.hpp"
#include "TransitionScreen.hpp"
#include "LevelZeroScreen.hpp"
#include "DeathScreen.hpp"
#include "AGameObject.hpp"

static void	setupStateManager(octo::StateManager& manager)
{
	manager.registerState<StateTest>("test");
	manager.registerState<FireflyTestScreen>("firefly");
	manager.registerState<PaletteDemoScreen>("colors");
	manager.registerState<SpriteSheetDemoScreen>("sprite_sheet");
	manager.registerState<AnimatedSpriteDemoScreen>("animated_sprite");
	manager.registerState<AudioDemoScreen>("audio");
	manager.registerState<GameScreen>("game");
	manager.registerState<EngineScreen>("engine");
	manager.registerState<PhysicsMapScreen>("physics_map");
	manager.registerState<LightningDemoScreen>("lightning");
	manager.registerState<ParticleDemoScreen>("particles");
	manager.registerState<DecorManagerDemoScreen>("decor");
	manager.registerState<FsmDemoScreen>("tamagotchi");
	manager.registerState<ElevatorStreamDemo>("elevator");
	manager.registerState<ResourceLoadingScreen>("loading");
	manager.registerState<QuitScreen>("quit");
	manager.registerState<TransitionScreen>("transition");
	manager.registerState<TransitionLevelScreen>("transitionLevel");
	manager.registerState<DeathScreen>("octo_death");
	manager.registerState<LevelZeroScreen>("zero");
	manager.registerState<TransitionLevelZeroScreen>("transitionLevelZero");
	manager.registerTransition<octo::BlackFadeTransition>("default", true);
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
	}
	catch (std::exception const& e)
	{
		std::cerr << "Run exception: " << e.what() << std::endl;
	}
	octo::Application::destroy();
    return (0);
}
