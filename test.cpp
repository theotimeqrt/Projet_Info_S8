#include <iostream>
#include <string>

using namespace std;


class Table {
private:
    sf::RectangleShape net;
    sf::FloatRect boundaries;
    
public:
    void draw(sf::RenderWindow& window);
    bool checkOutOfBounds(const Ball& ball, Side& scoringSide);
};


class Paddle : public sf::RectangleShape {
public:
    float speed;
    bool isPlayer; // Pour différencier joueur/IA
    
    void moveUp(float deltaTime);
    void moveDown(float deltaTime);
    void handleCollision(Ball& ball);
};

class Ball : public sf::CircleShape {
public:
    sf::Vector2f velocity;
    float baseSpeed;
    
    void resetPosition(Side servingSide); // Après un point
    void updatePhysics(float deltaTime, const Table& table);
    void bounce(Direction direction, float angleModifier = 0.0f);
};

class Table {
private:
    sf::RectangleShape net;
    sf::FloatRect boundaries;
    
public:
    void draw(sf::RenderWindow& window);
    bool checkOutOfBounds(const Ball& ball, Side& scoringSide);
};


int main() {
    return 0;
}