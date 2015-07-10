/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: irabeson <irabeson@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/04/15 19:14:06 by irabeson          #+#    #+#             */
/*   Updated: 2015/07/02 14:20:46 by pciavald         ###   ########.fr       */
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
#include "ParticleDemoScreen.hpp"

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
	manager.registerState<ParticleDemoScreen>("particles");
	manager.registerState<LightningDemoScreen>("lightning");
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
		octo::Application::initialize("Demo tests", "default.conf", argc, argv);
		octo::StateManager&							states = octo::Application::getStateManager();
		octo::Console&								console = octo::Application::getConsole();
		octo::GraphicsManager&						graphics = octo::Application::getGraphicsManager();
		DefaultApplicationListener					listener;

		graphics.addWindowListener(&listener);
		graphics.addKeyboardListener(&listener);
		setupStateManager(states);
		setupConsole(console);
		octo::Application::run("");
	}
	catch (std::exception const& e)
	{
		std::cerr << "exception: " << e.what() << std::endl;
	}
	octo::Application::destroy();
    return (0);
}
