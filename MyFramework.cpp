#include "MyFramework.h"
#include <cstdlib>
#include <ctime>
#include <thread>

///Features Added:-On the right side of the screen the player moves 50% faster then on the left side / done
///               -The invisible platform that stays on screen 2 seconds then disappears / done
///               -Invincibility Ability: Grants the player 20 seconds of invincibility against monsters / done


void MyFramework::PreInit(int& width, int& height, bool& fullscreen)
{
    width=gameWidth;
    height=gameHeight;
    fullscreen=gameFullscreen;
}

bool MyFramework::Init() {

    srand(time(nullptr));
    playerSprite = createSprite("data/blue-lik-left@2x.png");

    if (gameWidth>gameHeight){
        GameBackground = createSprite("data/bck@2xwide.png");
        numOfPlatformsThatCanSpawn=14;
    }
    else {
        GameBackground = createSprite("data/bck@2x.png");
        numOfPlatformsThatCanSpawn=7;
    }
    setSpriteSize(GameBackground, gameWidth, gameHeight);
    getSpriteSize(playerSprite, playerWidth, playerHeight);
    int randXSpace, randYSpace;
    randXSpace=gameWidth/numOfPlatformsThatCanSpawn;
    randYSpace=gameHeight/numOfPlatformsThatCanSpawn;
    for (int i=0; i<numOfPlatformsThatCanSpawn; i++){
        platformsPos[i].x=rand()%(randXSpace*(rand()%numOfPlatformsThatCanSpawn)+1);
        platformsPos[i].y=randYSpace*i;
        platformsSprites[i]=createSprite("data/platform@2x.png");
        whatIsOnPlatform[i]=0;
    }
    destroySprite(platformsSprites[1]);
    platformsSprites[1]= createSprite("data/platformInvisible@2x.png");
    getSpriteSize(platformsSprites[0], platformWidth, platformHeight);
    platformsPos[0].x=gameWidth/2-platformWidth/2;
    platformsPos[0].y=gameHeight-platformHeight;
    playerPos.x=platformsPos[0].x;
    playerPos.y=platformsPos[0].y-playerHeight-platformHeight;
    playerXSpeed=2;
    velocityY=0;
    heightScroll=gameHeight/3;
    distancePassed=0;
    numPlatforms=0;
    enemySprite= createSprite("data/enemySprite.png");
    getSpriteSize(enemySprite, enemyWidth, enemyHeight);
    invincibilityAbilitySprite = createSprite("data/invincibilityAbility@2x.png");
    getSpriteSize(invincibilityAbilitySprite, invincibilityAbilityWidth, invincibilityAbilityHeight);
    startInvincibilityTimer=std::time(nullptr);
    endInvincibilityTimer=std::time(nullptr);
    startInvisiblePlatform=std::time(nullptr);
    endInvisiblePlatform=std::time(nullptr);
    isPlayerInvincible=false;
    isInvisiblePlatformVisible=true;
    projectileSprite= createSprite("data/projectile@2x.png");
    getSpriteSize(projectileSprite, projectileWidth, projectileHeight);
    isProjectileTrown=false;
    projectileSpeed=6;
    ScoreSprite= createSprite("data/score1.png");
    NumOfPlaforms= createSprite("data/platforms1.png");
    return true;
}

void MyFramework::Close() {
    destroySprite(playerSprite);
    for (int i=0; i<numOfPlatformsThatCanSpawn; i++)
        destroySprite(platformsSprites[i]);
    destroySprite(GameBackground);
}

bool MyFramework::Tick() {

    if (playerPos.y <= heightScroll) {
        distancePassed+=0.005;
        if (isProjectileTrown){
            projectilePos.y-=velocityY;
            if (projectilePos.y>gameHeight){
                isProjectileTrown=false;
            }
        }

        for (int i = 0; i < numOfPlatformsThatCanSpawn; i++) {
            playerPos.y=heightScroll;
            platformsPos[i].y=platformsPos[i].y-velocityY;
            if (platformsPos[i].y>gameHeight) {
                int xRange = gameWidth / 2;
                platformsPos[i].y = 0;
                platformsPos[i].x = rand() % (xRange * 2) - xRange + playerPos.x;
                if (i==1){

                    isInvisiblePlatformVisible=true;
                    platformsSprites[1]= createSprite("data/platformInvisible@2x.png");
                    startInvisiblePlatform=std::time(nullptr);

                }
                while (platformsPos[i].x>gameWidth || platformsPos[i].x<0+platformWidth){
                    platformsPos[i].x = rand() % (xRange * 2) - xRange + playerPos.x;
                }
                if (whatIsOnPlatform[i]==0){
                    int chanceToSpawnAnythingOnAPlatform=rand()%18;
                    if (chanceToSpawnAnythingOnAPlatform==i){
                        chanceToSpawnAnythingOnAPlatform=rand()%18;
                        if (chanceToSpawnAnythingOnAPlatform%2)
                            whatIsOnPlatform[i]=2;
                    }else
                        if (chanceToSpawnAnythingOnAPlatform==0 || chanceToSpawnAnythingOnAPlatform==1)
                            whatIsOnPlatform[i]=1;

                }
                else{

                    whatIsOnPlatform[i]=0;

                }
                numPlatforms+=0.5; /// should be +=1 but then I run out of sprites too quick :)
            }
        }

    }

    drawSprite(GameBackground, 0, 0);
    for (int i=0; i<numOfPlatformsThatCanSpawn; i++){
        drawSprite(platformsSprites[i], platformsPos[i].x, platformsPos[i].y);
        if (whatIsOnPlatform[i] == 1){
            drawSprite(enemySprite, platformsPos[i].x+platformWidth/2-enemyWidth/2, platformsPos[i].y-enemyHeight);
        }
        if (whatIsOnPlatform[i] == 2){
            drawSprite(invincibilityAbilitySprite, platformsPos[i].x+platformWidth/2-invincibilityAbilityWidth/2, platformsPos[i].y-invincibilityAbilityHeight);
        }

    }

    if (isInvisiblePlatformVisible){

        if (std::difftime(endInvisiblePlatform, startInvisiblePlatform)<2){
            endInvisiblePlatform=std::time(nullptr);
        }else {
            isInvisiblePlatformVisible = false;
            destroySprite(platformsSprites[1]);
            platformsSprites[1]=createSprite("data/nothing@2x.png");
        }

    }

    bool isOnPlatform = false;

    for (int i = 0; i < numOfPlatformsThatCanSpawn; i++) {
        int platformTop = platformsPos[i].y;
        int platformBottom = platformsPos[i].y+platformHeight;
        int platformLeft = platformsPos[i].x+platformWidth/2-platformWidth/4;
        int platformRight = platformsPos[i].x+platformWidth-platformWidth/4;
        int playerTop = playerPos.y+playerHeight;
        int playerBottom = playerPos.y+playerHeight;
        int playerLeft = playerPos.x;
        int playerRight = playerLeft+playerWidth;

        if (whatIsOnPlatform[i]==1){

            int enemyTop=platformsPos[i].y-enemyHeight;
            int enemyBottom=enemyTop+enemyHeight;
            int enemyLeft=platformsPos[i].x+platformWidth/2-enemyWidth/2;
            int enemyRight=enemyLeft+enemyWidth;

            int projectileTop=projectilePos.y;
            int projectileBottom=projectileTop+projectileHeight;
            int projectileLeft=projectilePos.x;
            int projectileRight=projectileLeft+projectileWidth;
            if (projectileBottom >= enemyTop &&
                projectileTop <= enemyBottom &&
                projectileLeft <= enemyRight &&
                projectileRight >= enemyLeft){
                whatIsOnPlatform[i]=0;
            }


            if (playerBottom >= enemyTop &&
                playerTop <= enemyBottom &&
                velocityY > 0 &&
                playerLeft <= enemyRight-enemyWidth/4 &&
                playerRight >= enemyLeft+enemyWidth/4) {
                whatIsOnPlatform[i]=0;
                isOnPlatform = true;
                velocityY = -12;
                break;
            }
            int playerTop = playerPos.y;
            int playerBottom = playerPos.y+playerHeight;
            int playerLeft = playerPos.x;
            int playerRight = playerLeft+playerWidth;
            if (playerBottom >= enemyTop+enemyHeight/4 &&
                playerTop <= enemyBottom-enemyHeight/4 &&
                playerLeft <= enemyRight-enemyWidth/4 &&
                playerRight >= enemyLeft+enemyWidth/4 && !isPlayerInvincible) {
                destroySprite(playerSprite);
                for (int i=0; i<numOfPlatformsThatCanSpawn; i++)
                    destroySprite(platformsSprites[i]);
                destroySprite(GameBackground);
                destroySprite(enemySprite);
                destroySprite(invincibilityAbilitySprite);
                Init();

            }


        }
        if (whatIsOnPlatform[i]==2){

            int abilityTop=platformsPos[i].y-invincibilityAbilityHeight;
            int abilityBottom=abilityTop+invincibilityAbilityHeight;
            int abilityLeft=platformsPos[i].x+platformWidth/2-invincibilityAbilityWidth/2;
            int abilityRight=abilityLeft+invincibilityAbilityWidth;
            if (playerBottom >= abilityTop &&
                playerTop <= abilityBottom &&
                playerLeft <= abilityRight-invincibilityAbilityWidth/4 &&
                playerRight >= abilityLeft+invincibilityAbilityWidth/4) {
                isPlayerInvincible=true;
                whatIsOnPlatform[i]=0;
                startInvincibilityTimer=std::time(nullptr);
            }

        }

        if (playerBottom >= platformTop &&
            playerTop <= platformBottom &&
            velocityY > 0 &&
            playerLeft <= platformRight &&
            playerRight >= platformLeft) {

            isOnPlatform = true;
            velocityY = -12;
            break;
        }



    }
    if (!isOnPlatform) {
        velocityY += 0.15;
        playerPos.y += velocityY;
    }

    if (isPlayerInvincible){
        drawSprite(invincibilityAbilitySprite, gameWidth-invincibilityAbilityWidth-invincibilityAbilityWidth/4, gameHeight-invincibilityAbilityHeight-invincibilityAbilityHeight/4);
        if (std::difftime(endInvincibilityTimer, startInvincibilityTimer)<20){
              endInvincibilityTimer=std::time(nullptr);
        }else
            isPlayerInvincible=false;


    }

    if (isRightKeyPressed and isLeftKeyPressed){}
    if (isRightKeyPressed){
        if (playerPos.x>gameWidth)
            playerPos.x=0-playerWidth;
        else
            playerPos.x+=playerXSpeed;
    }
    if (isLeftKeyPressed){
        if (playerPos.x<0-playerWidth){
            playerPos.x=gameWidth;
        }
        else
            playerPos.x-=playerXSpeed;
    }
    if (playerPos.x<=gameWidth/2){
        playerXSpeed=4;
    }
    else
        playerXSpeed=6;

    drawSprite(playerSprite, playerPos.x, playerPos.y);
    if (playerPos.y>gameHeight){
        destroySprite(playerSprite);
        for (int i=0; i<numOfPlatformsThatCanSpawn; i++)
            destroySprite(platformsSprites[i]);
        destroySprite(GameBackground);
        destroySprite(enemySprite);
        destroySprite(invincibilityAbilitySprite);
        Init();

    }

    if (isLeftMousePressed) {
        if (!isPlayerLookingLeft) {
            projectilePos.x = playerPos.x + playerWidth;
            projectilePos.y = playerPos.y + playerHeight / 2 - projectileHeight / 4;
            projectileDir=true;
        }
        else{

            projectilePos.x = playerPos.x - projectileWidth;
            projectilePos.y = playerPos.y + playerHeight / 2 - projectileHeight / 4;
            projectileDir=false;

        }
        isProjectileTrown = true;
    }
    if (isProjectileTrown){

        if (!projectileDir){
            projectilePos.x-=projectileSpeed;
            drawSprite(projectileSprite, projectilePos.x, projectilePos.y);
        }
        else if (projectileDir){
            projectilePos.x+=projectileSpeed;
            drawSprite(projectileSprite, projectilePos.x, projectilePos.y);
        }

        if (projectilePos.x<0-projectileWidth)
            projectilePos.x=gameWidth;
        if (projectilePos.x>gameWidth)
            projectilePos.x=0;
    }
    CheckScore();
    std::this_thread::sleep_for(std::chrono::milliseconds(6));
    return false;
}

void MyFramework::onMouseMove(int x, int y, int xrelative, int yrelative) {
         if (x>playerPos.x+playerWidth/2){
             playerSprite= createSprite("data/blue-lik-right@2x.png");
             isPlayerLookingLeft=false;
         }
         else{
             isPlayerLookingLeft= true;
             playerSprite= createSprite("data/blue-lik-left@2x.png");
         }
}

void MyFramework::onMouseButtonClick(FRMouseButton button, bool isReleased) {
    if (button == FRMouseButton::LEFT and !isReleased){

        isLeftMousePressed=true;

    }
    if (button == FRMouseButton::LEFT and isReleased){

        isLeftMousePressed= false;

    }


}

void MyFramework::onKeyPressed(FRKey k) {

    if (k == FRKey::RIGHT) {
        isRightKeyPressed = true;
    }
    else if (k == FRKey::LEFT) {
        isLeftKeyPressed = true;
    }
}

void MyFramework::onKeyReleased(FRKey k) {

    if (k == FRKey::RIGHT) {
        isRightKeyPressed = false;
    }
    else if (k == FRKey::LEFT) {
        isLeftKeyPressed = false;
    }

}

const char* MyFramework::GetTitle()
{
return "Doodle Jump DL";
}

MyFramework::MyFramework(int gameWidth, int gameHeight, bool gameFullscreen) {

    this->gameWidth=gameWidth;
    this->gameHeight=gameHeight;
    this->gameFullscreen=gameFullscreen;

}


void MyFramework::CheckScore(){

    switch (int(distancePassed)){

        case 0:
            break;
        case 5:

            destroySprite(ScoreSprite);
            ScoreSprite= createSprite("data/score2.png");

            break;

        case 10:

            destroySprite(ScoreSprite);
            ScoreSprite= createSprite("data/score3.png");
            break;

        case 15:
            destroySprite(ScoreSprite);
            ScoreSprite= createSprite("data/score4.png");

            break;

        case 20:
            destroySprite(ScoreSprite);
            ScoreSprite= createSprite("data/score5.png");

            break;

        case 25:

            destroySprite(ScoreSprite);
            ScoreSprite= createSprite("data/score6.png");
            break;


        case 30:

            destroySprite(ScoreSprite);
            ScoreSprite= createSprite("data/score7.png");
            break;


        case 35:

            destroySprite(ScoreSprite);
            ScoreSprite= createSprite("data/score8.png");
            break;


        case 40:

            destroySprite(ScoreSprite);
            ScoreSprite= createSprite("data/score9.png");
            break;


        case 45:
            destroySprite(ScoreSprite);
            ScoreSprite= createSprite("data/score10.png");
            break;

        case 50:

            destroySprite(ScoreSprite);
            ScoreSprite= createSprite("data/score11.png");
            break;

        default:
            break;
    }
    if (distancePassed>60){
        destroySprite(ScoreSprite);
        ScoreSprite= createSprite("data/score12.png");
    }

    int scoreWidth, scoreHeight;
    getSpriteSize(ScoreSprite, scoreWidth, scoreHeight);

    drawSprite(ScoreSprite, 0, 0);

    switch(int(numPlatforms)){

        case 0:
            break;
        case 14:

            destroySprite(NumOfPlaforms);
            NumOfPlaforms= createSprite("data/platforms2.png");
            break;
        case 28:
            destroySprite(NumOfPlaforms);
            NumOfPlaforms= createSprite("data/platforms3.png");

            break;
        case 42:

            destroySprite(NumOfPlaforms);
            NumOfPlaforms= createSprite("data/platforms4.png");
            break;
        case 56:
            destroySprite(NumOfPlaforms);
            NumOfPlaforms= createSprite("data/platforms5.png");

            break;
        case 70:
            destroySprite(NumOfPlaforms);
            NumOfPlaforms= createSprite("data/platforms6.png");

            break;
        case 84:
            destroySprite(NumOfPlaforms);
            NumOfPlaforms= createSprite("data/platforms7.png");

            break;
        case 98:

            destroySprite(NumOfPlaforms);
            NumOfPlaforms= createSprite("data/platforms8.png");
            break;
        case 112:

            destroySprite(NumOfPlaforms);
            NumOfPlaforms= createSprite("data/platforms9.png");
            break;
        case 126:

            destroySprite(NumOfPlaforms);
            NumOfPlaforms= createSprite("data/platforms10.png");
            break;
        default:
            break;
    }
    if (numPlatforms>140){

        destroySprite(NumOfPlaforms);
        NumOfPlaforms= createSprite("data/platforms11.png");

    }
    drawSprite(NumOfPlaforms, gameWidth-scoreWidth, 0);

}