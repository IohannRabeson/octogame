#include "ClementineNpc.hpp"

#include <Application.hpp>
#include <Console.hpp>
ClementineNpc::ClementineNpc(void) :
	ASpecialNpc(CLEMENTINE_OSS)
{
	setSize(sf::Vector2f(50.f, 75.f));
	setOrigin(sf::Vector2f(75.f, 100.f));
	setScale(0.8f);
	setTextOffset(sf::Vector2f(-20.f, -70.f));
	setup();
	octo::Application::getConsole().addCommand(L"ori", [this](sf::Vector2f const & p)
	{
		setOrigin(p);
		std::cout << "origin " << p.x << " " << p.y << std::endl;
		});
	octo::Application::getConsole().addCommand(L"size", [this](sf::Vector2f const & p)
	{
		setSize(p);
		std::cout << "size " << p.x << " " << p.y << std::endl;
		});
	octo::Application::getConsole().addCommand(L"texOff", [this](sf::Vector2f const & p)
	{
		setTextOffset(p);
		std::cout << "textOff " << p.x << " " << p.y << std::endl;
		});
}

void ClementineNpc::setup(void)
{
	typedef octo::CharacterAnimation::Frame			Frame;

	getIdleAnimation().setFrames({
			Frame(sf::seconds(0.6f), {0u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.6f), {1u, sf::FloatRect(), sf::Vector2f()}),
			});
	getIdleAnimation().setLoop(octo::LoopMode::Loop);

	getSpecial1Animation().setFrames({
			Frame(sf::seconds(0.6f), {6u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.6f), {7u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.6f), {8u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.6f), {9u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.6f), {10u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.6f), {11u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.6f), {10u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.6f), {11u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.6f), {10u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.6f), {11u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.6f), {10u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.6f), {9u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.6f), {8u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.6f), {7u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.6f), {6u, sf::FloatRect(), sf::Vector2f()}),
			});
	getSpecial1Animation().setLoop(octo::LoopMode::NoLoop);

	setupMachine();
}
