#include "player.h"
#include <world.h>
#include <controllers/playercontroller.h>
#include <entities/unicorn.h>

#include <SFML/Graphics/RectangleShape.hpp>
#include <iostream>

Player::Player(TextureHolder &textures) :
    Entity(100),
    mTextures(textures),
    mMovingAnimation(false),
    mIsFiring(false),
    mFired(false)
{
    textures.get(Textures::Hero).setSmooth(false);

    mStandingWithoutSprite.setTexture(textures.get(Textures::Hero));
    mStandingWithoutSprite.setTextureRect(sf::IntRect(0,0,75,75));
    mStandingWithoutSprite.setScale(2.f, 2.f);

    mStandingWithSprite.setTexture(textures.get(Textures::Hero));
    mStandingWithSprite.setTextureRect(sf::IntRect(0,75,75,75));
    mStandingWithSprite.setScale(2.f, 2.f);

    //

    mAnimations[WalkingWithout].setTexture(textures.get(Textures::HeroWalkingWithout));
    mAnimations[WalkingWithout].setFrameSize(sf::Vector2i(75,75));
    mAnimations[WalkingWithout].setNumFrames(4);
    mAnimations[WalkingWithout].setDuration(sf::seconds(0.7f));
    mAnimations[WalkingWithout].setRepeating(true);
    mAnimations[WalkingWithout].setScale(sf::Vector2f(2.f, 2.f));

    mAnimations[WalkingWith].setTexture(textures.get(Textures::HeroWalkingWith));
    mAnimations[WalkingWith].setFrameSize(sf::Vector2i(75,75));
    mAnimations[WalkingWith].setNumFrames(4);
    mAnimations[WalkingWith].setDuration(sf::seconds(0.7f));
    mAnimations[WalkingWith].setRepeating(true);
    mAnimations[WalkingWith].setScale(sf::Vector2f(2.f, 2.f));

    //

    mAnimations[JumpingWithout].setTexture(textures.get(Textures::HeroJumpingWithout));
    mAnimations[JumpingWithout].setFrameSize(sf::Vector2i(75,75));
    mAnimations[JumpingWithout].setNumFrames(2);
    mAnimations[JumpingWithout].setDuration(sf::seconds(0.7f));
    mAnimations[JumpingWithout].setRepeating(true);
    mAnimations[JumpingWithout].setScale(sf::Vector2f(2.f, 2.f));

    mAnimations[JumpingWith].setTexture(textures.get(Textures::HeroJumpingWith));
    mAnimations[JumpingWith].setFrameSize(sf::Vector2i(75,75));
    mAnimations[JumpingWith].setNumFrames(2);
    mAnimations[JumpingWith].setDuration(sf::seconds(0.7f));
    mAnimations[JumpingWith].setRepeating(true);
    mAnimations[JumpingWith].setScale(sf::Vector2f(2.f, 2.f));
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

void Player::fire(std::vector<sf::Vector2f>& path)
{
    if(mFired || path.size() < 2) return;

    mFirePath = path;
    mIsFiring = true;
}

void Player::retrieve()
{
    mFired = false;
    mFirePath.clear();
}


sf::FloatRect Player::getBoundingRect() const
{
    return getWorldTransform().transformRect(sf::FloatRect(30.f*2, 65.f*2, 15.f*2, 10.f*2));
}

void Player::setDirection(Direction dir)
{
    switch(dir)
    {
        case Left:
            setScale(-1.f, 1.f);
            break;

        case Right:
        default:
            setScale(1.f, 1.f);
            break;
    }

    Entity::setDirection(dir);
}

void Player::updateCurrent(sf::Time dt, CommandQueue &commands)
{
    // Update animations
    updateAnimation(dt);

    // Is firing ?
    if(mIsFiring && !mFired)
    {
        Command command;
        command.category = Category::Foreground;
        command.action = derivedAction<SceneNode>([&](SceneNode& node, sf::Time) {
            std::unique_ptr<Unicorn> unicorn(new Unicorn(mFirePath, mTextures));
            node.attachChild(std::move(unicorn));
        });
        commands.push(command);

        mIsFiring = false;
        mFired    = true;
    }

    Entity::updateCurrent(dt, commands);
}

void Player::updateAnimation(sf::Time dt)
{
    if(!isDestroyed())
    {
        if(mFired)
        {
            if(mIsJumping)
            {
                mAnimations[JumpingWithout].update(dt);
            }
            else
            {
                mAnimations[WalkingWithout].update(dt);
            }
        }
        else
        {
            if(mIsJumping)
            {
                mAnimations[JumpingWith].update(dt);
            }
            else
            {
                mAnimations[WalkingWith].update(dt);
            }
        }
    }

    mMovingAnimation = isMoving();
}

void Player::drawCurrent(sf::RenderTarget &target, sf::RenderStates states) const
{
    if(mIsJumping)
    {
        if(mFired)
            target.draw(mAnimations[JumpingWithout], states);
        else
            target.draw(mAnimations[JumpingWith], states);
    }
    else if(mMovingAnimation)
    {
        if(mFired)
            target.draw(mAnimations[WalkingWithout], states);
        else
            target.draw(mAnimations[WalkingWith], states);
    }
    else
    {
        if(mFired)
            target.draw(mStandingWithoutSprite, states);
        else
            target.draw(mStandingWithSprite, states);
    }

    /*sf::FloatRect bound(30.f*2, 65.f*2, 15.f*2, 10.f*2);
    sf::RectangleShape shape(sf::Vector2f(bound.width, bound.height));
    shape.setOutlineColor(sf::Color::Red);
    shape.setOutlineThickness(1.f);
    shape.setFillColor(sf::Color(255,255,255, 0));
    shape.setPosition(bound.left, bound.top);
    target.draw(shape, states);*/
}
