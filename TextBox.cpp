#include "TextBox.h"

TextBox::TextBox() {
    text[0] = '\0';
    textLen = 0;
    maxLen = 10;
    isSelected = false;
}

void TextBox::setup(float x, float y, float width, float height, int maxCharacters, sf::Font& font) {
    box.setPosition(x, y);
    box.setSize(sf::Vector2f(width, height));
    box.setFillColor(sf::Color::White);
    box.setOutlineThickness(2);
    box.setOutlineColor(sf::Color::Black);

    maxLen = maxCharacters;

    textDisplay.setFont(font);
    textDisplay.setCharacterSize(18);
    textDisplay.setFillColor(sf::Color::Black);
    textDisplay.setPosition(x + 5, y + 5);
}

void TextBox::setSelected(bool selected) {
    isSelected = selected;
    if (isSelected) {
        box.setOutlineColor(sf::Color::Red);
    }
    else {
        box.setOutlineColor(sf::Color::Black);
    }
}

bool TextBox::checkClick(float mouseX, float mouseY) {
    sf::FloatRect bounds = box.getGlobalBounds();
    if (bounds.contains(mouseX, mouseY)) {
        setSelected(true);
        return true;
    }
    setSelected(false);
    return false;
}

void TextBox::typeText(sf::Event& event) {
    if (!isSelected) return;

    if (event.type == sf::Event::TextEntered) {
        char typedChar = static_cast<char>(event.text.unicode);

        if (typedChar == 8 && textLen > 0) {
            textLen--;
            text[textLen] = '\0';
        }
        else if (typedChar >= 32 && typedChar <= 126 && textLen < maxLen - 1) {
            text[textLen] = typedChar;
            textLen++;
            text[textLen] = '\0';
        }
        textDisplay.setString(text);
    }
}

const char* TextBox::getText() const {
    return text;
}

void TextBox::clear() {
    text[0] = '\0';
    textLen = 0;
    textDisplay.setString(text);
}

void TextBox::draw(sf::RenderWindow& window) {
    window.draw(box);
    window.draw(textDisplay);
}