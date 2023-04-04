#ifndef DOODLE_JUMP_MYFRAMEWORK_H
#define DOODLE_JUMP_MYFRAMEWORK_H

#include "./inc/Framework.h"
#include <ctime>
#include "posXY.h"

class MyFramework : public Framework {

    ///Player Logic
    Sprite* playerSprite;
    posXY playerPos;
    int playerWidth, playerHeight;
    int playerXSpeed;
    bool isPlayerLookingLeft;
    bool isPlayerInvincible=false;

    ///user input
    bool isRightKeyPressed=false;
    bool isLeftKeyPressed=false;
    bool isLeftMousePressed=false;

    ///Game logic
    int gameHeight;
    int gameWidth;
    bool gameFullscreen;
    Sprite* GameBackground;
    int numOfPlatformsThatCanSpawn;
    float velocityY;
    int heightScroll;
    double distancePassed;
    double numPlatforms;
    Sprite* enemySprite;
    int enemyWidth, enemyHeight;
    Sprite* invincibilityAbilitySprite;
    int invincibilityAbilityWidth, invincibilityAbilityHeight;
    Sprite* projectileSprite;
    int projectileWidth, projectileHeight;
    bool isProjectileTrown;
    int projectileSpeed;
    posXY projectilePos;
    bool projectileDir;///0 left | 1 right
    Sprite* ScoreSprite, *NumOfPlaforms;


    ///timers for invincibility ability and for the invisible platform
    std::time_t startInvisiblePlatform;
    std::time_t endInvisiblePlatform;
    std::time_t startInvincibilityTimer;
    std::time_t endInvincibilityTimer;

    ///Platforms Logic
    Sprite* platformsSprites[20];
    int whatIsOnPlatform[20]; /// 0 means platform is empty
                              /// 1 means platform contains an enemy
                              /// 2 means platform contains an invincibility ability
    posXY platformsPos[20];
    int platformWidth, platformHeight;
    bool isInvisiblePlatformVisible;

    void CheckScore();

public:

    MyFramework(int gameWidth, int gameHeight, bool gameFullscreen);

    virtual void PreInit(int& width, int& height, bool& fullscreen);

    virtual bool Init();

    virtual void Close();

    virtual bool Tick();

    virtual void onMouseMove(int x, int y, int xrelative, int yrelative);

    virtual void onMouseButtonClick(FRMouseButton button, bool isReleased);

    virtual void onKeyPressed(FRKey k);

    virtual void onKeyReleased(FRKey k);

    virtual const char* GetTitle() override;

};

#endif //DOODLE_JUMP_MYFRAMEWORK_H
