#include "player.h"
#include <world.h>
#include <controllers/playercontroller.h>

#include <SFML/Graphics/RectangleShape.hpp>

Player::Player(TextureHolder &textures) :
    Entity(100),
    mIsJumping(true),
    mVerticalVelocity(0.f),
    mFired(false)
{
    textures.get(Textures::Hero).setSmooth(false);

    mStandingSprite.setTexture(textures.get(Textures::Hero));
    mStandingSprite.setTextureRect(sf::IntRect(0,0,75,75));
    mStandingSprite.setScale(2.f, 2.f);

    mAnimations[WalkingWith].setTexture(textures.get(Textures::Hero));
    mAnimations[WalkingWith].setFrameSize(sf::Vector2i(64,64));
    mAnimations[WalkingWith].setNumFrames(4);
    mAnimations[WalkingWith].setDuration(sf::seconds(0.7f));
    mAnimations[WalkingWith].setRepeating(true);
    mAnimations[WalkingWith].setOrigin(sf::Vector2f(32.f, 64.f));
    mAnimations[WalkingWith].setScale(sf::Vector2f(4.f, 4.f));
}



void Player::jump()
{
    if(!mIsJumping) {
        sf::Vector2f pos = getPosition();
        pos.y -= 2.f;
        setPosition(pos); // Hack to avoid unwated collisions with the platform
        mVerticalVelocity = -700;
        mIsJumping = true;
    }
}


sf::FloatRect Player::getBoundingRect() const
{
    return getWorldTransform().transformRect(sf::FloatRect(30.f*2, 65.f*2, 15.f*2, 10.f*2));
}

void Player::updateCurrent(sf::Time dt)
{
    Entity::updateCurrent(dt);

    sf::Vector2f pos = getPosition();
    pos.y = pos.y + mVerticalVelocity * dt.asSeconds();
    setPosition(pos);
}

void Player::drawCurrent(sf::RenderTarget &target, sf::RenderStates states) const
{
    target.draw(mSprite, states);

    sf::FloatRect bound(30.f*2, 65.f*2, 15.f*2, 10.f*2);
    sf::RectangleShape shape(sf::Vector2f(bound.width, bound.height));
    shape.setOutlineColor(sf::Color::Red);
    shape.setOutlineThickness(1.f);
    shape.setFillColor(sf::Color(255,255,255, 0));
    shape.setPosition(bound.left, bound.top);
    target.draw(shape, states);
}
