#include <SFML/Graphics.hpp>
#include <iostream>
#include <sstream>
#include <random>
#include <vector>

std::vector<float> ghostTarget;
std::size_t ghostIndex =0;

class gravity 
{
  public:
    float velocity;
    float Gravity;
    sf::Clock Gclock;
    

    gravity():velocity(0.f),Gravity(300.f){}

    void update(float& Yposition )
    {
    float deltatime = Gclock.restart().asSeconds();
    velocity+=Gravity*deltatime;
    Yposition+= velocity*deltatime;

    }
    void reset(float newvelocity = 0.0f)
    {
      velocity=newvelocity;
    }
      
};

class Obstacle
{
  public:
  float speed =200.f;
  sf::Sprite tpill;
  sf::Sprite bpill;
  sf::FloatRect bounds =tpill.getLocalBounds();

      Obstacle(sf::Texture & texture,float startx,float starty,float gap=150.f)
      {
        tpill.setTexture(texture);
        bpill.setTexture(texture);

        tpill.setScale(1.f,-1.f);
        tpill.setPosition(startx,starty-gap/2.f);
        bpill.setScale(1.f,1.f);
        bpill.setPosition(startx,starty+gap/2.f);

        tpill.setOrigin(bounds.width/2.f,bounds.height/2.f);
        bpill.setOrigin(bounds.width/2.f,bounds.height/2.f);
      }
      void update(float deltatime)
      {
        tpill.move(-speed*deltatime,0.f);
        bpill.move(-speed*deltatime,0.f);
      }
      bool isoffscreen() const
      {
        return tpill.getPosition().x + tpill.getGlobalBounds().width < 0;
        return bpill.getPosition().x+bpill.getGlobalBounds().width <0;
      }
};



int main() {
  sf::RenderWindow window(sf::VideoMode(800, 600), "kullama_flappy");

  gravity gravity;
  
bool gameover =false;
  sf::Font font;
  if (!font.loadFromFile("assets/monogram.ttf")) {
    std::cout << "failed to load font";
    return -1;
  }
  sf::Text fpstext;
  fpstext.setFont(font);
  fpstext.setCharacterSize(40);
  fpstext.setFillColor(sf::Color::Green);
  fpstext.setPosition(10.f, 10.f);

  sf::Clock clock;
  sf::Clock fpsclock;
  sf::Time previoustime = clock.getElapsedTime();
  sf::Time Previoustime =fpsclock.getElapsedTime();
  int framecount = 0;
  float fps = 0.0f;

  sf::Texture texture;
  if (!texture.loadFromFile("assets/sky.png"))
    std::cout << "error didnt load the image";
  sf::Sprite sky(texture);

  sf::Texture Bird_1;
  if (!Bird_1.loadFromFile("assets/kullama.png"))
    std::cout << "failed to load the image";
  sf::Sprite shape(Bird_1);
  shape.setScale(0.75f,0.75f);

  sf::Texture Ptexture,p1texture,p2texture,p3texture;
  if(!Ptexture.loadFromFile("assets/cucumber.png"))
    std::cout<<"error didnt load the iamge";
  if(!p1texture.loadFromFile("assets/radish.png"))
    std::cout<<"error didnt load the iamge";
  if(!p2texture.loadFromFile("assets/CARROOOOT.png"))
    std::cout<<"error didnt load the iamge";
  if(!p3texture.loadFromFile("assets/banana.png"))
    std::cout<<"error didnt load the iamge";

  std::vector<sf::Texture*> rand_obs ={&p1texture,&p2texture,&p3texture,&Ptexture};

  sf::Texture end;
  if(!end.loadFromFile("assets/origbig.png"))
    std::cout<<"error didnt load the image";
  sf::Sprite ender(end);
  
  sf::Texture over;
  if(!over.loadFromFile("assets/game_over_red.png"))
    std::cout<<"error didnt load the image";
  sf::Sprite gover(over);
  gover.setPosition(300.f,300.f);

  sf::Texture chicken;
  if(!chicken.loadFromFile("assets/shawarma.png"))
     std::cout<<"error didnt load the image";
  sf::Sprite ghostbird(chicken);
    ghostbird.setScale(1.5f,1.5f);
    ghostbird.setPosition(300.f,300.f);
  

  shape.setPosition(100.f,300.f);

  std::vector<Obstacle> obstacle;

  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_real_distribution<float> ydist(75.f,500.f);
  std::uniform_int_distribution<std::size_t> tdist(0,rand_obs.size()-1);
  sf::Clock rar;
  float spwaninterval =2.f;

  auto restart =[&]()
  {
    ghostTarget.clear();
    ghostIndex=0;
    shape.setPosition(100.f,300.f);
    gravity.reset();
    ghostbird.setPosition(300.f,300.f);
    obstacle.clear();
    gameover=false;
    rar.restart();

  };

  while (window.isOpen()) 
  {
    sf::Event event;
                  while (window.pollEvent(event)) 
                {
                  if (event.type == sf::Event::Closed)
                    window.close();

                    if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Space )&& !gameover)
                    {
                      gravity.reset(-200.f);
                      
                    }
                  if(event.type==sf::Event::KeyPressed&&event.key.code==sf::Keyboard::R&& gameover)
                  {
                      restart();
                  }
                }

                  framecount++;
                  
          float raw=clock.restart().asSeconds();
          if(rar.getElapsedTime().asSeconds()>spwaninterval)
          {
                float yrand =ydist(gen);
                std::size_t tindex=tdist(gen);
                obstacle.emplace_back(*rand_obs[tindex],800.f,yrand);
                ghostTarget.push_back(yrand);
                rar.restart();
          }
        
        

          for(auto it = obstacle.begin();it!=obstacle.end();)
          {
            it->update(raw);
            if (it->isoffscreen())
            {
              it=obstacle.erase(it);
            }  
              else
            {   
              ++it;
            }
          }
          float shawarma_pos = shape.getPosition().x + 4.f*(shape.getPosition().x);
          if(ghostIndex<ghostTarget.size())  
          {
          float targetY  = ghostTarget[ghostIndex];
          float currenty = ghostbird.getPosition().y;
          float speed =300.f;

          if(std::abs(currenty-targetY)<5.0f)
          {
            ghostIndex++;
          }
          else
          {
            if(currenty<targetY)
            currenty+=speed*raw;
            else
            currenty-=speed*raw;
            ghostbird.setPosition(shawarma_pos,currenty);

          }
          }

          
          float posy = shape.getPosition().y;
          gravity.update(posy);
          shape.setPosition(shape.getPosition().x ,posy);   

          if(shape.getPosition().y<0||shape.getPosition().y>window.getSize().y)
          {
            gameover=true;
            
          }
    
          sf::Time currenttime = fpsclock.getElapsedTime();
          sf::Time elapsedtime = currenttime - Previoustime;

    if (elapsedtime.asSeconds() >= 0.1f)
    {
      fps = framecount / elapsedtime.asSeconds();
      framecount = 0;
      Previoustime = currenttime;

      std::stringstream ss;
      ss << "fps:" << static_cast<int>(fps);
      fpstext.setString(ss.str());
    }
    for(const auto&obs:obstacle)
    {
      if(shape.getGlobalBounds().intersects(obs.tpill.getGlobalBounds())||
          shape.getGlobalBounds().intersects(obs.bpill.getGlobalBounds()))
          {
            gameover=true;
            break;
          }
    }
    

    window.clear();
    if(gameover)
    {
      window.draw(ender);
      window.draw(gover);
    }
    else
    {
    window.draw(sky);
    window.draw(shape);
    window.draw(ghostbird);
  for(auto const& obs :obstacle )
  {
    window.draw(obs.tpill);
    window.draw(obs.bpill);
  }
    window.draw(fpstext);   
}  
    window.display();

  }

  return 0;

}
