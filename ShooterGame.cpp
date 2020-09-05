#include<SFML/Graphics.hpp>
#include<SFML/Audio.hpp>
#include<vector>
using namespace sf;
using namespace std;
int main()
{
    RenderWindow window(VideoMode(1280,1024),"Shooter Game",Style::Default);
    //sounds
    SoundBuffer pew;
    SoundBuffer explode;
    explode.loadFromFile("explosion.wav");
    Sound boom;
    boom.setBuffer(explode);
    pew.loadFromFile("pew.ogg");
    Sound pewPlay;
    pewPlay.setBuffer(pew);
    //music
    Music music;
    music.openFromFile("music.ogg");
    music.setVolume(100);
    music.play();
    music.setLoop(true);
    //space
    Texture space;
    space.loadFromFile("space.png");
    Sprite cover;
    cover.setTexture(space);
    cover.setScale(1.f,1.f);
    //player
    Sprite player;
    Texture playerTexture;
    playerTexture.loadFromFile("ship.png");
    player.setTexture(playerTexture);
    player.setScale(0.1f,0.1f);
    player.setPosition(window.getSize().x/2-player.getScale().x,window.getSize().y-player.getGlobalBounds().height);
    Vector2f playerCenter;
    int shootTimer=0;

    //projectile
    CircleShape projectile;
    projectile.setFillColor(Color::Red);
    projectile.setRadius(5.f);


    //enemy
    Sprite enemy;
    Texture enemyTexture;
    enemyTexture.loadFromFile("enemy.png");
    enemy.setTexture(enemyTexture);
    enemy.setScale(.3f,.3f);
    int enemySpawnTimer=0;




    vector<CircleShape> projectiles;
    projectiles.push_back(CircleShape(projectile));
    vector<Sprite> enemies;
    enemies.push_back(Sprite(enemy));

    while (window.isOpen())
    {
        Event event;
        while(window.pollEvent(event))
        {
            if(event.type==Event::Closed)
            {
                window.close();
            }
         
        }
        
        //player Update
        player.setPosition(Mouse::getPosition(window).x-player.getGlobalBounds().width/2,player.getPosition().y);
        if(player.getPosition().x>=window.getSize().x)
        {
            player.setPosition(window.getSize().x-player.getGlobalBounds().width/2,player.getPosition().y);
        }
        playerCenter=Vector2f(player.getPosition().x+player.getGlobalBounds().width/2,player.getPosition().y+player.getGlobalBounds().height/2);
        //shoot
        if(shootTimer<100)
        {
            shootTimer++;
        }
        if(Mouse::isButtonPressed(Mouse::Left)&&shootTimer>=100)
        {
            pewPlay.play();
            projectile.setPosition(playerCenter);
            projectiles.push_back(CircleShape(projectile));
            shootTimer=0;
        }
        //Projectiles Update
        for(int i=0; i<(int)projectiles.size(); i++)
        {
            projectiles[i].move(0.0f,-1.f);
            if(projectiles[i].getPosition().y<=0)
            {
                projectiles.erase(projectiles.begin()+i);
            }
        }

        //Enemies Update
        if(enemySpawnTimer < 500)
        {
            enemySpawnTimer++;
        }
        if(enemySpawnTimer>=500)
        {
            enemy.setPosition((rand()%int(window.getSize().x-enemy.getGlobalBounds().width)),0);
            enemies.push_back(Sprite(enemy));
            enemySpawnTimer=0;
        }
        for(int i=0; i<(int)enemies.size(); i++)
        {
            enemies[i].move(0.0f,+.1f);
            if(enemies[i].getPosition().y >= window.getSize().y)
            {
                enemies.erase(enemies.begin()+i);
            }
        }

        //collisions
        if(!enemies.empty()&&!projectiles.empty())
        {
            for(int i=0; i<(int)projectiles.size();i++)
            {
                for(int j=0; j<(int)enemies.size(); j++)
                {
                    if(projectiles[i].getGlobalBounds().intersects(enemies[j].getGlobalBounds()))
                    {

                        boom.play();
                        enemies.erase(enemies.begin()+j);
                        projectiles.erase(projectiles.begin()+i);
                        break;
                    }
                }
            }
        }
        window.clear();
        window.draw(cover);

        window.draw(player);

        for(int i=0; i<(int)enemies.size(); i++)
        {
            window.draw(enemies[i]);
        }

        for(int i=0; i<(int)projectiles.size(); i++)
        {
            window.draw(projectiles[i]);
        }
        window.display();

    }
    

    return 0;
}