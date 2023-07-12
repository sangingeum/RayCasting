#include <SFML/Graphics.hpp>
#include <vector>
#include <iostream>
#include "GameSystem.hpp"


int main()
{  
    GameSystem system;
    system.run();
/*
    sf::Transform t1 = sf::Transform::Identity;
    sf::Transform t2 = sf::Transform::Identity;
    t2.translate(20.f, 30.f);
    auto r1 = t1 * sf::Vector2f(10.f, 10.f);
    auto r2 = t2 * sf::Vector2f(10.f, 10.f);
    std::cout << r1.x << " " << r1.y << "\n";
    std::cout << r2.x << " " << r2.y << "\n";
*/

    return 0;
}