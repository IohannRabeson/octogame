/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: irabeson <irabeson@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/04/15 19:14:06 by irabeson          #+#    #+#             */
/*   Updated: 2015/04/16 18:16:25 by irabeson         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <Application.hpp>
#include <GraphicsManager.hpp>
#include <Console.hpp>
#include "DefaultApplicationListener.hpp"

// Ecran de jeu
#include "StateTest.hpp"
#include "FireflyTestScreen.hpp"

static void	setupStateManager(octo::StateManager& manager)
{
	manager.registerState<StateTest>("test");
	manager.registerState<FireflyTestScreen>("firefly");
}

static void	setupConsole(octo::Console& console)
{
	console.addCommand(L"quit", &octo::Application::stop);
}

int main(int argc, char **argv)
{
	try
	{
		octo::Application::initialize("Application works!", "default.conf", argc, argv);
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
