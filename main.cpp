#include<iostream>
#include"SFML\Graphics.hpp"
#include"SFML\Window.hpp"
#include"SFML\System.hpp"
#include"SFML\Audio.hpp"
#include<cstdlib>
#include<vector>
#include<windows.h>

using namespace sf;
using namespace std;



int main_called = 0;

int main()
{
    srand(time(0));



    RenderWindow window(VideoMode(800, 600), "space invaders");
    window.setFramerateLimit(60);


	Texture background;
	background.loadFromFile("C:/Users/moham/OneDrive/Desktop/Codeblocks/test/images/space.jpg");
	Sprite space;
	space.setTexture(background);


    Sprite player;
    Texture playerTex;
	playerTex.loadFromFile("C:/Users/moham/OneDrive/Desktop/Codeblocks/test/images/plane.png");
    player.setTexture(playerTex);
    player.setPosition(400, 500);
    int life = 3;
    int level = 1;
    bool pause=false;
    int Rock1_kill = 5;
    int score = -1;


    float Y = 3;
    Texture Rock1Tex;
	Rock1Tex.loadFromFile("C:/Users/moham/OneDrive/Desktop/Codeblocks/test/images/Rock1.png");

    Sprite Rock1;
    Rock1.setTexture(Rock1Tex);
    int Rock1SpawnTimer =0;

    vector<Sprite> enemies;
    enemies.push_back(Sprite(Rock1));

    Texture bulletTex;
	bulletTex.loadFromFile("Textures/bullet.png");
	Sprite bullet;
	bullet.setTexture(bulletTex);
	vector<Sprite> bullets;
	bullets.push_back(Sprite(bullet));
	int shootTimer = 20;


    Font font;
	font.loadFromFile("Fonts/Arial.ttf");

    Text scoreText;
    scoreText.setFont(font);
	scoreText.setCharacterSize(20);
	scoreText.setFillColor(Color::White);
	scoreText.setPosition(10, 10);

	Text lifeText;
	lifeText.setFont(font);
	lifeText.setCharacterSize(20);
	lifeText.setFillColor(Color::White);
	lifeText.setPosition(715, 10);

	Text gameOverText;
	gameOverText.setFont(font);
	gameOverText.setCharacterSize(50);
	gameOverText.setStyle(Text::Bold);
	gameOverText.setFillColor(Color::Red);
	gameOverText.setPosition(350 , 250);
	gameOverText.setString("GAME OVER!");

	Text levelText;
	levelText.setFont(font);
	levelText.setCharacterSize(20);
	levelText.setFillColor(Color::White);
	levelText.setPosition(10, 32);

	Text levelDisplay;
	levelDisplay.setFont(font);
	levelDisplay.setCharacterSize(30);
	levelDisplay.setFillColor(Color::White);
	levelDisplay.setPosition(360 , window.getSize().y / 2);

    SoundBuffer buffer_background;
    Sound background_sound;
    SoundBuffer buffer_shoot;
    Sound shoot_sound;
    sf::SoundBuffer buffer_explosion;
    sf::Sound explosion_sound;

    buffer_background.loadFromFile("C:/Users/moham/OneDrive/Desktop/mm.wav");
	background_sound.setBuffer(buffer_background);
	background_sound.setLoop(true);
	background_sound.play();

	buffer_shoot.loadFromFile("Audios/laser.wav");
    shoot_sound.setBuffer(buffer_shoot);

game:
	while (window.isOpen())
	{
		Event event;
		while (window.pollEvent(event))
		{
			if (event.type == Event::Closed)
            {
                window.close();
            }

            if (event.type == Event::KeyPressed && event.key.code == Keyboard::Escape)
            {
                window.close();
            }

            if ((event.type == Event::KeyPressed) && Keyboard::isKeyPressed(Keyboard::P))
                pause = true;
		}

		if (Keyboard::isKeyPressed(Keyboard::Left))
            player.move(-10, 0);
        if (Keyboard::isKeyPressed(Keyboard::Right))
            player.move(10, 0);
        if (Keyboard::isKeyPressed(Keyboard::Up))
            player.move(0, -10);
        if (Keyboard::isKeyPressed(Keyboard::Down))
            player.move(0, 10);

       if(player.getPosition().x <= 0)
           player.setPosition(1,player.getPosition().y);
       if(player.getPosition().x >=735 )
            player.setPosition(735,player.getPosition().y);



        if (shootTimer < 5)
            shootTimer++;

        if (Keyboard::isKeyPressed(Keyboard::Space) && shootTimer >= 5) //Shooting
        {
            shoot_sound.play();

            bullet.setPosition(player.getPosition().x, player.getPosition().y);
            bullets.push_back(bullet);
            shootTimer = 0;
        }

        for (size_t i = 0; i < bullets.size(); i++)
        {

            bullets[i].move(0, -10);

            if (bullets[i].getPosition().y > window.getSize().y)
            {
                bullets.erase(bullets.begin() + i);
                break;
            }

            for (size_t k = 0; k < enemies.size(); k++)
            {
                if (bullets[i].getGlobalBounds().intersects(enemies[k].getGlobalBounds()))
                {
                    score += 1;
                    explosion_sound.play();
                    enemies.erase(enemies.begin() + k);

                    bullets.erase(bullets.begin() + i);
                    break;
                }
            }
        }

		for (size_t i = 0; i < enemies.size(); i++)
		{
			enemies[i].move(0, Y);

			if (enemies[i].getPosition().y >  window.getSize().y)
				enemies.erase(enemies.begin() + i);
            if (enemies[i].getGlobalBounds().intersects(player.getGlobalBounds()))
            {
                explosion_sound.play();
                enemies.erase(enemies.begin() + i);
                life -= 1;
            }
		}

		if (Rock1SpawnTimer < 40)
			Rock1SpawnTimer++;

		if (Rock1SpawnTimer >= 40)
		{
			Rock1.setPosition(rand()%window.getSize().x - Rock1.getGlobalBounds().width, 0);
			enemies.push_back(Sprite(Rock1));
			Rock1SpawnTimer = 0;
		}

		if(score == Rock1_kill )
        {
            level++;
            Y += 5;

            levelDisplay.setString("Level : " + to_string(level));


        }

        scoreText.setString("Score : " + to_string(score));
        lifeText.setString("Life : " + to_string(life));
        levelText.setString("Level  : " +  to_string(level));

		window.clear();
		window.draw(space);
        window.draw(player);
        window.draw(scoreText);
        window.draw(lifeText);
        window.draw(levelText);


        for(size_t i=0; i<enemies.size(); i++)
            window.draw(enemies[i]);


		for (size_t i = 0; i < bullets.size(); i++)
			window.draw(bullets[i]);

        if(score == Rock1_kill)
        {
            window.draw(levelDisplay);
            window.display();
            Rock1_kill += Rock1_kill;
            Sleep(2000);
        }


        if(life <= 0)
        {
            window.draw(gameOverText);
        }

        window.display();

        if(life <= 0)
            break;


	}
	while ((window.isOpen()) && (pause))
    {
        sf::Event event;
        while ((window.pollEvent(event)) && (pause))
        {
            if (event.type == Event::Closed)
            {
                window.close();
                return 1;
            }

            if ((event.type == Event::KeyPressed) && Keyboard::isKeyPressed(Keyboard::P))
            {
                pause = false;
                goto game;
            }
        }
        window.display();
    }


    Sleep(3000);

	return 0;
}


