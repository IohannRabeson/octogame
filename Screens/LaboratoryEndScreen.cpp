#include "LaboratoryEndScreen.hpp"
#include "ResourceDefinitions.hpp"
#include "Progress.hpp"
#include "ScientistJu.hpp"
#include "ScientistLu.hpp"
#include "ScientistFran.hpp"
#include "ScientistCedric.hpp"
#include "LaboBiome.hpp"
#include "Tree.hpp"
#include <Application.hpp>
#include <ResourceManager.hpp>
#include <PostEffectManager.hpp>
#include <Camera.hpp>

LaboratoryEndScreen::LaboratoryEndScreen(void) :
	m_state(Appear),
	m_timer(sf::Time::Zero),
	m_globalTimer(sf::Time::Zero),
	m_timeBeforeNextText(sf::seconds(3.f)),
	m_appearDuration(sf::seconds(2.f)),
	m_cedricWalkTimer(sf::seconds(3.f)),
	m_cedricPutPotionTimer(sf::seconds(3.f)),
	m_changeColorAquaMax(sf::seconds(4.f)),
	m_appearTimerPostEffectMax(sf::seconds(8.f)),
	m_startPostEffectDurationMax(sf::seconds(10.f)),
	m_disappearTimerPostEffectMax(sf::seconds(5.f)),
	m_endPostEffectDurationMax(sf::seconds(20.f)),
	m_textIndex(0u),
	m_lastTextIndex(0u),
	m_stopDialog(false),
	m_biome(nullptr),
	m_decorManager(10000),
	m_keyLeft(false),
	m_keyRight(false),
	m_keyOther(false)
{
}

void	LaboratoryEndScreen::start()
{
	InputListener::addInputListener();
	Progress::getInstance().setBubbleNpc(true);

	octo::ResourceManager & resources = octo::Application::getResourceManager();
	octo::PostEffectManager & postEffect = octo::Application::getPostEffectManager();
	octo::Camera & camera = octo::Application::getCamera();

	camera.setCenter(camera.getRectangle().width / 2.f, camera.getRectangle().height / 2.f);
	m_background.setTexture(resources.getTexture(LABO_BG_PNG));
	m_background.setPosition(sf::Vector2f(0.f, 0.f));
	m_foreground.setTexture(resources.getTexture(LABO_FG_PNG));
	m_foreground.setPosition(sf::Vector2f(0.f, 0.f));
	m_water.setTexture(resources.getTexture(LABO_WATER_PNG));
	m_water.setPosition(sf::Vector2f(0.f, 0.f));

	m_npcs.emplace_back(new ScientistJu());
	m_npcs.emplace_back(new ScientistLu());
	m_npcs.emplace_back(new ScientistFran());
	m_npcs.emplace_back(new ScientistCedric());
	m_npcs[0]->setPosition(sf::Vector2f(950.f, 827.f));
	m_npcs[1]->setPosition(sf::Vector2f(510.f, 827.f));
	m_npcs[2]->setPosition(sf::Vector2f(1600.f, 790.f));
	m_npcs[3]->setPosition(sf::Vector2f(-70.f, 577.f));

	m_tvNpcs.emplace_back(new TvLaboNpc(TV_LABO_GUI_OSS));
	m_tvNpcs.emplace_back(new TvLaboNpc(TV_LABO_IOHANN_OSS));
	m_tvNpcs.emplace_back(new TvLaboNpc(TV_LABO_FAB_OSS));
	m_tvNpcs.emplace_back(new TvLaboNpc(TV_LABO_JEF_OSS));
	m_tvNpcs.emplace_back(new TvLaboNpc(TV_LABO_PIERRE_OSS));
	m_tvNpcs[0]->setPosition(sf::Vector2f(44.f, 393.f));
	m_tvNpcs[1]->setPosition(sf::Vector2f(32.f, 700.f));
	m_tvNpcs[2]->setPosition(sf::Vector2f(101.f, 957.f));
	m_tvNpcs[3]->setPosition(sf::Vector2f(682.f, 957.f));
	m_tvNpcs[4]->setPosition(sf::Vector2f(857.f, 957.f));

	m_octo.setPosition(sf::Vector2f(700.f, 770.f));

	for (auto & it : m_npcs)
		it->setDisplayText(false);

	for (auto & it : m_npcs)
		m_lastTextIndex = std::max(m_lastTextIndex, it->getLastIndex());

	m_shader.loadFromMemory(resources.getText(HUE_FRAG), sf::Shader::Fragment);
	m_shader.setUniform("texture", sf::Shader::CurrentTexture);
	m_shader.setUniform("hue", 0.f);

	postEffect.removeEffects();
	PostEffectLayer::getInstance().clear();
	PostEffectLayer::getInstance().registerShader(LABORATORY_EFFECT_FRAG, LABORATORY_EFFECT_FRAG);

	PostEffectLayer::getInstance().getShader(LABORATORY_EFFECT_FRAG).setUniform("resolution", sf::Vector2f(camera.getRectangle().width, camera.getRectangle().height));
	PostEffectLayer::getInstance().enableShader(LABORATORY_EFFECT_FRAG, false);

	m_biome.reset(new LaboBiome());
	m_decorManager.setup(m_biome.get());
	Tree * tree = new Tree(true);
	tree->setPosition(sf::Vector2f(1275.f, 662.f));
	m_decorManager.add(tree);

	m_bubbleParticle.setCanEmit(true);
	m_bubbleParticle.setup(sf::Vector2f(1.8f, 1.8f));
	m_bubbleParticle.setVelocity(sf::Vector2f(0.f, -70.f));
	m_bubbleParticle.setEmitTimeRange(0.2f, 0.3f);
	m_bubbleParticle.setGrowTimeRange(0.4f, 0.6f);
	m_bubbleParticle.setLifeTimeRange(1.f, 1.8f);
	m_bubbleParticle.setScaleFactor(10.f);
	m_bubbleParticle.setDispersion(sf::Vector2f(80.f, 0.f));
	m_bubbleParticle.setColor(sf::Color(255, 255, 255, 150));
	m_bubbleParticle.setCanEmit(true);

	octo::AudioManager &		audio = octo::Application::getAudioManager();
	octo::ResourceManager &		resource = octo::Application::getResourceManager();
	audio.startMusic(resource.getSound(MUSIC_MENU_OPUS_II_REVERSE_OGG), sf::seconds(1.f));
}

void	LaboratoryEndScreen::pause()
{
}

void	LaboratoryEndScreen::resume()
{
}

void	LaboratoryEndScreen::stop()
{
	InputListener::removeInputListener();
	Progress::getInstance().setBubbleNpc(false);
	PostEffectLayer::getInstance().enableShader(LABORATORY_EFFECT_FRAG, false);
}

void	LaboratoryEndScreen::update(sf::Time frameTime)
{
	switch (m_state)
	{
		case Appear:
			m_timer += frameTime;
			if (m_timer > m_appearDuration)
			{
				m_timer = sf::Time::Zero;
				m_state = State::Dialogs;
				for (auto & it : m_npcs)
				{
					it->setDisplayText(true);
					it->updateText(true);
					it->setTextIndex(m_textIndex);
				}
			}
			break;
		case Dialogs:
			m_timer += frameTime;
			updateTv(frameTime);
			if (m_timer >= m_timeBeforeNextText || m_stopDialog)
			{
				m_timer = sf::Time::Zero;
				m_textIndex++;
				if (m_textIndex >= m_lastTextIndex || m_stopDialog)
				{
					for (auto & it : m_npcs)
					{
						it->setDisplayText(false);
						it->updateText(false);
					}
					m_state = State::CedricWalk;
				}
				else
				{
					for (auto & it : m_npcs)
						it->setTextIndex(m_textIndex);
				}
			}
			break;
		case CedricWalk:
			m_timer += frameTime;
			m_npcs[3]->setPosition(octo::linearInterpolation(sf::Vector2f(-70.f, 577.f), sf::Vector2f(350.f, 577.f), std::min(1.f, m_timer / m_cedricPutPotionTimer)));
			if (m_timer >= m_cedricWalkTimer)
			{
				m_timer = sf::Time::Zero;
				m_state = CedricPutPotion;
				m_npcs[3]->setNextEvent(ANpc::Events::Special1);
				m_npcs[3]->setDisplayText(true);
				m_npcs[3]->updateText(true);
				m_npcs[3]->setTextIndex(m_textIndex);
			}
			break;
		case CedricPutPotion:
			m_timer += frameTime;
			if (m_timer >= m_cedricPutPotionTimer)
			{
				octo::AudioManager &		audio = octo::Application::getAudioManager();
				octo::ResourceManager &		resources = octo::Application::getResourceManager();

				audio.playSound(resources.getSound(OCTO_SOUND_USE_POTION_OGG), 1.f);
				m_timer = sf::Time::Zero;
				m_npcs[3]->setDisplayText(false);
				m_npcs[3]->updateText(false);
				m_state = ChangeAquaColor;
			}
			break;
		case ChangeAquaColor:
			m_changeColorAqua += frameTime;
			m_shader.setUniform("hue", octo::linearInterpolation(0.f, 0.4f, std::min(1.f, m_changeColorAqua / m_changeColorAquaMax)));
			if (m_changeColorAqua >= m_changeColorAquaMax)
			{
				m_state = StartShaderEffect;
				PostEffectLayer::getInstance().enableShader(LABORATORY_EFFECT_FRAG, true);
			}
			break;
		case StartShaderEffect:
			m_appearTimerPostEffect += frameTime;
			m_startPostEffectDuration += frameTime;
			PostEffectLayer::getInstance().getShader(LABORATORY_EFFECT_FRAG).setUniform("appear", std::min(1.f, m_appearTimerPostEffect / m_appearTimerPostEffectMax));
			if (m_startPostEffectDuration >= m_startPostEffectDurationMax)
				m_state = DisappearShaderEffect;
			break;
		case DisappearShaderEffect:
			m_disappearTimerPostEffect += frameTime;
			PostEffectLayer::getInstance().getShader(LABORATORY_EFFECT_FRAG).setUniform("disappear", std::min(1.f, m_disappearTimerPostEffect / m_disappearTimerPostEffectMax));
			if (m_disappearTimerPostEffect >= m_disappearTimerPostEffectMax)
				m_state = StopShaderEffect;
		case StopShaderEffect:
			m_endPostEffectDuration += frameTime;
			if (m_endPostEffectDuration >= m_endPostEffectDurationMax)
			{
				Progress & progress = Progress::getInstance();

				if (progress.getNextDestination() != Level::Portal)
				{
					progress.setNextDestination(Level::Portal);
					progress.save();
					octo::Application::getStateManager().change("anoctonautodyssey");
				}
			}
			break;
		default:
			break;
	}

	if (m_state == StartShaderEffect || m_state == DisappearShaderEffect || m_state == StopShaderEffect)
	{
		m_globalTimer += frameTime;
		PostEffectLayer::getInstance().getShader(LABORATORY_EFFECT_FRAG).setUniform("time", m_globalTimer.asSeconds());
	}

	if (m_state == CedricPutPotion || m_state == VisualEffect || m_state == ChangeAquaColor || m_state == StartShaderEffect)
	{
		for (auto & it : m_tvNpcs)
			it->update(frameTime);
	}

	for (auto & it : m_npcs)
		it->update(frameTime);

	sf::Vector2f const & octoPosition = m_octo.getPosition();
	if (m_keyLeft && !m_keyRight && octoPosition.x > 510.f)
	{
		m_octo.setPosition(sf::Vector2f(octoPosition.x - frameTime.asSeconds() * 40.f, 770.f));
		m_octo.reverseSprite(true);
	}
	else if (m_keyRight && !m_keyLeft && octoPosition.x < 890.f)
	{
		m_octo.setPosition(sf::Vector2f(octoPosition.x + frameTime.asSeconds() * 40.f, 770.f));
		m_octo.reverseSprite(false);
	}

	m_decorManager.update(frameTime, octo::Application::getCamera());
	m_octo.update(frameTime);
	m_bubbleParticle.setPosition(m_octo.getPosition() + sf::Vector2f(60.f, -70.f));
	m_bubbleParticle.update(frameTime);

	if (m_keyOther)
	{
		if (m_textIndex <= 4u)
			m_timeBeforeNextText = sf::seconds(2.f);
		else if (m_textIndex >= 78)
			m_timeBeforeNextText = sf::seconds(2.f);
		else
			m_timeBeforeNextText = sf::seconds(0.1f);
	}
	else
	{
		if (m_textIndex <= 4u)
			m_timeBeforeNextText = sf::seconds(3.f);
		else
			m_timeBeforeNextText = sf::seconds(2.f);
	}
	Progress::getInstance().updateSteam(frameTime);
}

void	LaboratoryEndScreen::updateTv(sf::Time frameTime)
{
	if (m_npcs[2]->findCurrentText(L"Guillaume LASSET"))
		m_tvNpcs[0]->update(frameTime);
	if (m_npcs[2]->findCurrentText(L"Iohann RABESON"))
		m_tvNpcs[1]->update(frameTime);
	if (m_npcs[2]->findCurrentText(L"Fabien YOU"))
		m_tvNpcs[2]->update(frameTime);
	if (m_npcs[2]->findCurrentText(L"Jeff GUERIN"))
		m_tvNpcs[3]->update(frameTime);
	if (m_npcs[2]->findCurrentText(L"Pierre GLORY"))
		m_tvNpcs[4]->update(frameTime);
}

void	LaboratoryEndScreen::draw(sf::RenderTarget & render) const
{
	sf::RenderStates states;
	render.clear(sf::Color::Black);
	render.draw(m_background);
	m_bubbleParticle.draw(render);
	render.draw(m_octo);
	states.shader = &m_shader;
	render.draw(m_water, states);
	states.shader = nullptr;
	for (auto & it : m_tvNpcs)
		render.draw(*it);
	render.draw(m_decorManager, states);
	for (auto & it : m_npcs)
		render.draw(*it);
	render.draw(m_foreground);
	for (auto & it : m_npcs)
		it->drawText(render, states);
}

bool	LaboratoryEndScreen::onInputPressed(InputListener::OctoKeys const & key)
{
	switch (key)
	{
		case OctoKeys::Left:
		{
			Progress::getInstance().setTryToEscape(true);
			m_keyLeft = true;
			break;
		}
		case OctoKeys::Right:
		{
			Progress::getInstance().setTryToEscape(true);
			m_keyRight = true;
			break;
		}
		case OctoKeys::Menu:
			if (m_textIndex > 4u)
				m_textIndex = 78u;
			break;
		case OctoKeys::Capacity:
		case OctoKeys::Elevator:
		case OctoKeys::Jump:
		case OctoKeys::Entrance:
			m_keyOther = true;
			break;
		default:
			break;
	}
	return true;
}

bool	LaboratoryEndScreen::onInputReleased(InputListener::OctoKeys const & key)
{
	switch (key)
	{
		case OctoKeys::Left:
			m_keyLeft = false;
			break;
		case OctoKeys::Right:
			m_keyRight = false;
			break;
		case OctoKeys::Capacity:
		case OctoKeys::Elevator:
		case OctoKeys::Jump:
		case OctoKeys::Entrance:
			m_keyOther = false;
			break;
		default:
			break;
	}
	return true;
}
