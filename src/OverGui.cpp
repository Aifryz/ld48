#include "OverGui.h"


OverGui::OverGui(std::unordered_map<std::string, sf::Texture>& textures, const sf::Font& font) :
	m_textures(textures)
{
	sf::Vector2f sz = static_cast<sf::Vector2f>(m_textures["button.png"].getSize());

	m_title_sprite = sf::Sprite(m_textures["title.png"]);
	m_title_sprite.setOrigin(0.5f*static_cast<sf::Vector2f>(m_textures["title.png"].getSize()));
	m_title_sprite.setPosition(400.f, 100.f);

	m_start_sprite = sf::Sprite(m_textures["button.png"]);
	m_start_sprite.setOrigin(sz*0.5f);
	m_credits_sprite = sf::Sprite(m_textures["button.png"]);
	m_credits_sprite.setOrigin(sz*0.5f);
	m_quit_sprite = sf::Sprite(m_textures["button.png"]);
	m_quit_sprite.setOrigin(sz*0.5f);

	m_start_sprite.setPosition(400.f, 250.f);
	m_credits_sprite.setPosition(400.f, 350.f);
	m_quit_sprite.setPosition(400.f, 450.f);

	m_start_text = sf::Text("Start", font);
	m_start_text.setColor(sf::Color::Black);
	auto sb = m_start_text.getLocalBounds();
	m_start_text.setOrigin(sf::Vector2f(sb.width / 2, sb.height / 2));
	m_start_text.setPosition(400.f, 250.f - 5.f);

	m_credits_text = sf::Text("Credits", font);
	m_credits_text.setColor(sf::Color::Black);
	auto cb = m_credits_text.getLocalBounds();
	m_credits_text.setOrigin(sf::Vector2f(cb.width / 2, cb.height / 2));
	m_credits_text.setPosition(400.f, 350.f - 5.f);

	m_quit_text = sf::Text("Quit", font);
	m_quit_text.setColor(sf::Color::Black);
	auto qb = m_quit_text.getLocalBounds();
	m_quit_text.setOrigin(sf::Vector2f(qb.width / 2, qb.height / 2));
	m_quit_text.setPosition(400.f, 450.f - 5.f);

	m_cred_text = sf::Text("(C) Aifryz 2021", font, 12);
	m_cred_text.setColor(sf::Color::White);
	m_cred_text.setPosition(640, 570);
}
void OverGui::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
	target.draw(m_title_sprite);
	target.draw(m_start_sprite);
	target.draw(m_credits_sprite);
	target.draw(m_quit_sprite);

	target.draw(m_start_text);
	target.draw(m_credits_text);
	target.draw(m_quit_text);

	target.draw(m_cred_text);
}

int OverGui::handleEvent(sf::Event e)
{
	//if (e.type == sf::Event::KeyPressed)
	//{
	//	return 1;
	//}
	auto btn_size = m_textures["button.png"].getSize();

	auto start_bb = sf::IntRect(400 - btn_size.x / 2, 250 - btn_size.y / 2, btn_size.x, btn_size.y);
	auto cred_bb = sf::IntRect(400 - btn_size.x / 2, 350 - btn_size.y / 2, btn_size.x, btn_size.y);
	auto quit_bb = sf::IntRect(400 - btn_size.x / 2, 450 - btn_size.y / 2, btn_size.x, btn_size.y);

	if (e.type == sf::Event::MouseButtonPressed)
	{
		sf::IntRect dummy = sf::IntRect(e.mouseButton.x, e.mouseButton.y, 1, 1);
		if (dummy.intersects(start_bb))
		{
			return 1; // Load level
		}
		else if (dummy.intersects(cred_bb))
		{
			return 0; // Ignore for now
		}
		else if (dummy.intersects(quit_bb))
		{
			return -1;
		}
	}
	return 0;
}