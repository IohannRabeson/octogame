#include "MapManager.hpp"
#include "ResourceDefinitions.hpp"
#include <Application.hpp>
#include <ResourceManager.hpp>
#include <GraphicsManager.hpp>

MapManager::MapManager(void) :
	m_biome(),
	m_terrainManager(),
	m_wave(),
	m_dark(),
	m_negative(),
	m_shader(),
	m_camera(octo::Application::getCamera())
{
	m_wave = "\
	uniform float wave_phase;\
	uniform vec2 wave_amplitude;\
	void main()\
	{\
		vec4 vertex = gl_Vertex;\
		vertex.x += cos(gl_Vertex.y * 0.02 + wave_phase * 3.8) * wave_amplitude.x\
		+ sin(gl_Vertex.y * 0.02 + wave_phase * 6.3) * wave_amplitude.x * 0.3;\
		vertex.y += sin(gl_Vertex.x * 0.02 + wave_phase * 2.4) * wave_amplitude.y\
		+ cos(gl_Vertex.x * 0.02 + wave_phase * 5.2) * wave_amplitude.y * 0.3;\
		gl_Position = gl_ModelViewProjectionMatrix * vertex;\
		gl_TexCoord[0] = gl_TextureMatrix[0] * gl_MultiTexCoord0;\
		gl_FrontColor = gl_Color;\
	}";
	m_dark = "\
	uniform sampler2D texture;\
	void main()\
	{\
		vec4 pixel = texture2D(texture, gl_TexCoord[0].xy);\
		pixel.r = 0;\
		pixel.g = 0;\
		pixel.b = 0;\
		gl_FragColor = gl_Color * pixel;\
	}";
	m_negative = "\
	uniform sampler2D texture;\
	void main()\
	{\
		vec4 pixel = texture2D(texture, gl_TexCoord[0].xy);\
		pixel.r = 1 - pixel.r;\
		pixel.g = 1 - pixel.g;\
		pixel.b = 1 - pixel.b;\
		gl_FragColor = gl_Color * pixel;\
	}";
}

MapManager::~MapManager(void)
{
}

#include <iostream>
void MapManager::init(void)
{
	// TransitionManager init the biome
	m_terrainManager.init(&m_biome);
	if (!m_shader.loadFromMemory(m_wave, sf::Shader::Vertex))
		std::cout << "caca" << std::endl;
	//if (!m_shader.loadFromMemory(m_negative, sf::Shader::Fragment))
	//	std::cout << "caca" << std::endl;
}

void MapManager::update(float pf_deltatime)
{
	/*static float accumulator = 0.f;
	static sf::Clock clock;
	float dt = 1.f / 120.f;

	// Always update TransitionManager first
	m_terrainManager.update(pf_deltatime);
	m_staticObjectManager.update(pf_deltatime);

	accumulator += clock.getElapsedTime().asSeconds();
	if (accumulator > 0.2f)
		accumulator = 0.2f;

	while (accumulator > dt)
	{
		m_collisionManager.update(dt);
		accumulator -= dt;
	}

	//float alpha = accumulator / dt;
	clock.restart();
*/

	// Update the camera position

	m_terrainManager.update(pf_deltatime);

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
		m_camera.move(500.f * pf_deltatime, 0.f);
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
		m_camera.move(-500.f * pf_deltatime, 0.f);
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
		m_camera.move(0.f, 500.f * pf_deltatime);
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
		m_camera.move(0.f, -500.f * pf_deltatime);
//	float x = static_cast<float>(sf::Mouse::getPosition().x) / octo::Application::getGraphicsManager().getVideoMode().width;
//	float y = static_cast<float>(sf::Mouse::getPosition().y) / octo::Application::getGraphicsManager().getVideoMode().height;
//	m_shader.setParameter("wave_phase", pf_deltatime);
//	m_shader.setParameter("wave_amplitude", x * 40, y * 40);
}

void MapManager::draw(sf::RenderTarget& render, sf::RenderStates) const
{
	render.draw(m_terrainManager, &m_shader);

	/*sf::RectangleShape rect;
	rect.setPosition(sf::Vector2f(m_collisionManager.getPlayer().getGlobalBounds().left, m_collisionManager.getPlayer().getGlobalBounds().top));
	rect.setSize(sf::Vector2f(m_collisionManager.getPlayer().getGlobalBounds().width, m_collisionManager.getPlayer().getGlobalBounds().height));
	render.draw(rect);
	*/
	//states.shader = &m_shader;
	//render.draw(&m_collisionManager.getPlayer().m_vertices[0], 4, sf::Quads, states);
}
