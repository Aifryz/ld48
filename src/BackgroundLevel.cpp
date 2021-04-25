#include "BackgroundLevel.h"

BackgroundLevel::BackgroundLevel(std::unordered_map<std::string, sf::Texture>& textures):
	m_textures(textures)
{
	m_grad = sf::Sprite(textures["grad.png"]);
}

void BackgroundLevel::handleEvent(sf::Event e)
{
}

void BackgroundLevel::update(float dt)
{
}

void BackgroundLevel::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
	target.clear(sf::Color(5, 80, 120));

	sf::View overlay_view;
	overlay_view.setCenter(400, 300);
	overlay_view.setSize(800, 600);
	target.setView(overlay_view);
	target.draw(m_grad, sf::BlendMultiply);


}