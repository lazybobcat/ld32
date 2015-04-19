#ifndef CAMERANODE_H
#define CAMERANODE_H

#include <entities/scenenode.h>

class CameraNode : public SceneNode
{
public:
    CameraNode(sf::View& camera);

    void    shake(sf::Time duration);
    void    splash(sf::Color color);

private:
    virtual void  updateCurrent(sf::Time dt, CommandQueue &commands);
    virtual void  drawCurrent(sf::RenderTarget &target, sf::RenderStates states) const;

private:
    // Camera State
    sf::View&       mCamera;
    sf::Vector2f    mOriginalCenter;

    // Shake
    bool            mShaking;
    sf::Time        mShakingDuration;
    sf::Time        mShakingET;

    // Splash
    bool            mSplashing;
    float           mTransparency;
    sf::Color       mColor;
    sf::RectangleShape mSplash;
};

#endif // CAMERANODE_H
