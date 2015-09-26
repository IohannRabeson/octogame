#include "ResolutionMenu.hpp"
#include <Application.hpp>
#include <GraphicsManager.hpp>
#include <PostEffectManager.hpp>
#include <Camera.hpp>

ResolutionMenu::ResolutionMenu(void)
{
}

void ResolutionMenu::createMenus(void)
{
	addMenu(L"1920 x 1080 x 32", std::unique_ptr<EmptyMenu>(new EmptyMenu()));
	addMenu(L"1280 x 720 x 32", std::unique_ptr<EmptyMenu>(new EmptyMenu()));
	//setCharacterSize();
}

void ResolutionMenu::onSelection(void)
{
	if (getIndexCursor() == 0u)
	{
		sf::VideoMode mode(1920, 1080, 32);
		setupGraphics(mode);
	}
	else if (getIndexCursor() == 1u)
	{
		sf::VideoMode mode(1280, 720, 32);
		setupGraphics(mode);
	}

	setState(AMenu::State::Hide);
	AMenu * backMenu = getBackMenu();
	if (backMenu)
		backMenu->setState(AMenu::State::Active);
}

void ResolutionMenu::setupGraphics(sf::VideoMode mode)
{
	//TODO:
	std::string					title("L'Odyssee Octonaute");

	octo::Camera &				camera = octo::Application::getCamera();
	octo::GraphicsManager &		graphics = octo::Application::getGraphicsManager();
	octo::PostEffectManager &	postEffect = octo::Application::getPostEffectManager();

	sf::VideoMode				fullscreen = sf::VideoMode::getFullscreenModes().front();
	mode.height = mode.width * fullscreen.height / fullscreen.width;
	// Create cameraView before createRender to use the automatic resize of the view
	sf::View					cameraView(sf::FloatRect(0, 0, mode.width, mode.height));

	graphics.setCamera(camera, cameraView);
	graphics.createRender(mode,
						  title,
						  graphics.isFullscreen(),
						  graphics.getAntialiasing());
	graphics.setVerticalSyncEnabled(graphics.isVerticalSyncEnabled());
	graphics.setFramerateLimit(graphics.getFramerateLimit());
	postEffect.createRender(mode);
}

