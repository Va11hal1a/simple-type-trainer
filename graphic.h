#ifndef GRAPHIC_H_INCLUDED
#define GRAPHIC_H_INCLUDED
#include <SFML/Graphics.hpp>
#include <string>
#include <vector>
#include <chrono>
#include <random>
#include <ctime>
sf::RenderWindow* window;
sf::Font font;
int x = 20, y = 620;
const float timeforfading = 0.5;
std::chrono::duration<double> elapsed = std::chrono::duration<double>();
std::string assigned_text;
double timed = 0.1;
int c = 0, mistakes = 0;
namespace lettersControl{
    class letter{
       public:
         double elapsedtime = 0.0d;
         char character;
         bool correct = true;
        letter(char chr, bool crct): character(chr), correct(crct){}
    };
    std::vector<letter> letters = std::vector<letter>();
    void showup(char let, bool correct){
        letters.push_back(letter(let,correct));
    }
    void update(double delta){  //including drawing itself
        int l = 0;
        for(auto i: letters){
            letters[l].elapsedtime += delta;
            if(i.elapsedtime > timeforfading)
                letters.erase(begin(letters) + l);
            else{
                sf::Text text = sf::Text(letters[l].character, font);
                text.setPosition(sf::Vector2f(x, y - i.elapsedtime/timeforfading * 100));
                if(i.correct){
                    text.setColor(sf::Color(255 - i.elapsedtime/timeforfading * 255,  i.elapsedtime/timeforfading * 255, 255 - i.elapsedtime/timeforfading * 255));
                }
                else{
                    text.setColor(sf::Color(255 - i.elapsedtime/timeforfading * 255, 0, 0));
                }
                window->draw(text);
            }
            l++;
        }
    }
};
std::string generate_a_text(char beg = 'A', char end = 'Z', int count = 1000){
    std::string res = "";
    clock_t t = clock();
    srand((int)t * rand() * time(0));
    for(int i = 0; i < count; i++){
        res += (char)(beg + (rand() % (end - beg)));
    }
    return res;
}
void assign_the_text(std::string str){
    assigned_text = str;
}
void init(){
    window = new sf::RenderWindow(sf::VideoMode(640, 640), "you suck:)");
    if(font.loadFromFile("C://ANTQUAI.TTF")){
        std::cout << "font has successfully loaded" << std::endl;
    }
    assign_the_text(generate_a_text());
}
void show_letters_order(){
    sf::Text text = sf::Text(assigned_text, font);
    text.setPosition(320 - c * 20, 320);
    window->draw(text);
}
void show_stats(){
    std::string txt = "count of mistakes: " + std::to_string(mistakes);
    std::string txa = " speed: " + std::to_string((int)((double)(c+1) / timed * 60.0) );
    sf::Text text = sf::Text(txt + txa, font);
    text.setPosition(sf::Vector2f(200, 600));
    window->draw(text);
}
void update(){
    auto strt = std::chrono::high_resolution_clock::now();
    window->clear();
    sf::Event event;
    while(window->pollEvent(event)){
        if(event.type == sf::Event::KeyPressed){
            if((char)(65 + event.key.code) == assigned_text[c]){
                    lettersControl::showup((char)(65 + event.key.code), true);
            }
            else{
                lettersControl::showup((char)(65 + event.key.code), false);
                mistakes++;
            }
            c++;
        }
    }
    show_stats();
    show_letters_order();
    lettersControl::update(elapsed.count());
    window->display();
    auto end = std::chrono::high_resolution_clock::now();
    elapsed = end - strt;
    timed += elapsed.count();
}

#endif // GRAPHIC_H_INCLUDED
