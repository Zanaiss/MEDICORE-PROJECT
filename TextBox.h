#ifndef TEXTBOX_H
#define TEXTBOX_H

#include <SFML/Graphics.hpp>
#include "CustomStringHelper.h"

class TextBox {
private:
    sf::RectangleShape box;
    sf::Text textDisplay;
    char text[50];
    int textLen;
    int maxLen;
    bool isSelected;

public:
    TextBox();
    void setup(float x, float y, float width, float height, int maxCharacters, sf::Font& font);
    void setSelected(bool selected);
    bool checkClick(float mouseX, float mouseY);
    void typeText(sf::Event& event);
    const char* getText() const;
    void clear();
    void draw(sf::RenderWindow& window);
};

#endif