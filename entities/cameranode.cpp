#include "cameranode.h"
#include <utils.h>

CameraNode::CameraNode(sf::View &camera) :
    SceneNode(Category::Camera),
    mCamera(camera),
    mOriginalCenter(camera.getCenter()),
    mShaking(false),
    mShakingDuration(sf::Time::Zero),
    mShakingET(sf::Time::Zero),
    mSplashing(false),
    mTransparency(128.f)
{
}

void CameraNode::shake(sf::Time duration)
{
    mShaking = true;
    mShakingDuration = duration;
    mShakingET = sf::Time::Zero;
}

void CameraNode::splash(sf::Color color)
{
    mSplashing = true;
    mTransparency = 128.f;
    mColor = color;
    mSplash.setFillColor(sf::Color(color.r, color.g, color.b, mTransparency));
    mSplash.setOutlineThickness(0);
    mSplash.setSize(sf::Vector2f(1280, 728));
}

void CameraNode::updateCurrent(sf::Time dt, CommandQueue &/*commands*/)
{
    if(mShaking)
    {
        mShakingET += dt;

        mCamera.setCenter(mOriginalCenter);
        mCamera.move(sf::Vector2f(0.f, randomFloat(-300.f, 300.f)*dt.asSeconds()));

        if(mShakingET >= mShakingDuration)
        {
            mShaking = false;
            mCamera.setCenter(mOriginalCenter);
        }
    }

    if(mSplashing)
    {
        mTransparency -= (dt.asSeconds() * 250.f);

        if(mTransparency <= 0)
        {
            mSplashing = false;
            mTransparency = 128.f;
        }
        else
        {
            mSplash.setFillColor(sf::Color(mColor.r, mColor.g, mColor.b, (int)mTransparency));
        }
    }
}

void CameraNode::drawCurrent(sf::RenderTarget &target, sf::RenderStates states) const
{
    if(mSplashing) target.draw(mSplash, states);
}
