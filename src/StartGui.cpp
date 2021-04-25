#include "StartGui.h"


StartGui::StartGui(std::unordered_map<std::string, sf::Texture>& textures,  const sf::Font& font):
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
	m_start_text.setOrigin(sf::Vector2f(sb.width/2, sb.height / 2));
	m_start_text.setPosition(400.f, 250.f - 5.f);

	m_credits_text = sf::Text("Credits", font);
	m_credits_text.setColor(sf::Color::Black);
	auto cb = m_credits_text.getLocalBounds();
	m_credits_text.setOrigin(sf::Vector2f(cb.width / 2, cb.height / 2));
	m_credits_text.setPosition(400.f, 350.f-5.f);

	m_quit_text = sf::Text("Quit", font);
	m_quit_text.setColor(sf::Color::Black);
	auto qb = m_quit_text.getLocalBounds();
	m_quit_text.setOrigin(sf::Vector2f(qb.width / 2, qb.height / 2));
	m_quit_text.setPosition(400.f, 450.f - 5.f);

	m_cred_text = sf::Text("(C) Aifryz 2021", font, 12);
	m_cred_text.setColor(sf::Color::White);
	m_cred_text.setPosition(640, 570);

	std::string instructions = "";
	instructions += "Use WASD to move, mouse to shoot torpedoes\n";
	instructions += "Collect treasures to increase your score\n";
	instructions += "Beware of deep sea pirates, they might be dumb, but they are vicious \n";
	instructions += "Torpedoes are strong enough to destroy rocks \n";
	instructions += "Plunge Deeper and Deeper to find the ultimate treasure \n";
	
	
	m_instr2 = sf::Text("THE LD48 TROPHY!", font, 30);
	m_instr2.setPosition(100, 350);

	m_instructions = sf::Text(instructions, font, 14);
	m_instructions.setColor(sf::Color::White);
	m_instructions.setPosition(50, 250);


	auto credits_str = std::string("Created on April 24-25 2021\n");
	credits_str += "Created for Ludum Dare 48 compo\n";
	credits_str += "Done in SFML and C++17\n";
	credits_str += "Font used: TypeWrong by Digital Graphic Labs\n";

	m_credits = sf::Text(credits_str, font, 14);
	m_credits.setColor(sf::Color::White);
	m_credits.setPosition(50, 250);

	m_mode = 0;
}
void StartGui::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
	target.draw(m_title_sprite);

	if (m_mode == 0) // Normal
	{
		target.draw(m_start_sprite);
		target.draw(m_credits_sprite);
		target.draw(m_quit_sprite);

		target.draw(m_start_text);
		target.draw(m_credits_text);
		target.draw(m_quit_text);
	}
	else if(m_mode == 1) // Instructions
	{
		target.draw(m_instructions);
		target.draw(m_instr2);
		target.draw(m_quit_sprite);
		target.draw(m_quit_text);
	}
	else if (m_mode == 2) // Credits
	{
		target.draw(m_credits);

		target.draw(m_quit_sprite);
		target.draw(m_quit_text);
	}

	target.draw(m_cred_text);
}

int StartGui::handleEvent(sf::Event e)
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
			m_mode = 1;
			m_quit_text.setString("Start");
			return 0; // Ignore
		}
		else if (dummy.intersects(cred_bb))
		{
			m_mode = 2;
			m_quit_text.setString("Back");
			return 0; // Ignore for now
		}
		else if (dummy.intersects(quit_bb))
		{
			if (m_mode == 1)
			{
				return 1;
			}
			if (m_mode == 2)
			{
				m_mode = 0;
				m_quit_text.setString("Quit");
				return 0;
			}
			else
			return -1;
		}
	}
	return 0;
}