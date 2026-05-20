#pragma once
#include "Tank.h"
#include "Bullet.h"
#include "Wall.h"
#include "Enemy.h"
#include "Item.h"
#include "Effect.h"

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <vector>

class Game {
private:
    sf::RenderWindow window;//cửa sổ game
    Tank player;//xe tăng người chơi
    std::vector<Bullet> bullets;//chứa nhiều đạn
    std::vector<Wall> walls;//tường
    std::vector<Enemy> enemies;
    //đạn của địch
    std::vector<Bullet> enemyBullets;
    sf::Clock enemyShootClock;

    int score;
    bool gameWon;

    sf::Font font;
    sf::Text winText;

    int playerHealth;
    bool gameOver;
    
    sf::Text gameOverText;
    sf::Clock damageClock;
    //máu và điểm
    sf::Text scoreText;
    sf::Text healthText;
    //bắt đầu game
    bool gameStarted;
    //sf::Text startText;
    //level 

    sf::View camera;//Di chuyển cam

    //level
    int currentLevel;
    int maxLevel;
    int unlockedLevel;

    std::vector<sf::RectangleShape> levelButtons;
    std::vector<sf::Text> levelButtonTexts;

    sf::Text selectLevelText;
    sf::Text levelText;

    bool showMainMenu;
    bool finalVictory;

    sf::Text titleText;
    sf::Text startButtonText;
    sf::RectangleShape startButton;

    sf::Clock victoryClock;
    sf::RectangleShape victoryOverlay;
    sf::Text finalScoreText;
    //reset đạn
    int currentAmmo;
    int maxAmmo;
    bool isReloading;
    sf::Clock reloadClock;
    sf::Text ammoText;
    //Biến vật phẩm
    std::vector<Item> items;

    bool rapidFireActive;
    bool bigBulletActive;
    bool shieldActive;

    sf::Clock rapidFireClock;
    sf::Clock bigBulletClock;
    sf::Clock shieldClock;
    sf::Clock playerShootClock;

    sf::Text powerText;
    sf::Clock rapidShootClock;

    //âm thanh
    sf::SoundBuffer shootBuffer;
    sf::SoundBuffer hitBuffer;

    sf::Sound shootSound;
    sf::Sound hitSound;

    sf::Music backgroundMusic;
    std::vector<Effect> effects;//hiệu ứng
    //run camera
    bool cameraShaking;
    sf::Clock cameraShakeClock;
    float cameraShakeDuration;
    float cameraShakePower;

    sf::Texture groundTexture;
    std::vector<sf::Sprite> groundTiles;

    //ảnh nền
    sf::Texture startBackgroundTexture;
    sf::Sprite startBackgroundSprite;

    sf::Texture levelSelectBackgroundTexture;
    sf::Sprite levelSelectBackgroundSprite;
    //tiêu đề
    sf::Texture tankTitleTexture;
    sf::Sprite tankTitleSprite;

    sf::Texture selectTitleTexture;
    sf::Sprite selectTitleSprite;

    sf::Texture victoryTitleTexture;
    sf::Sprite victoryTitleSprite;
    //start
    sf::Texture startTextTexture;
    sf::Sprite startTextSprite;
    //lặt vặt
    sf::Texture grassPatchTexture;
    sf::Texture rockSmallTexture;
    sf::Texture dirtPatchTexture;

    std::vector<sf::Sprite> groundDetails;
    //chữ game over
    sf::Texture gameOverTitleTexture;
    sf::Sprite gameOverTitleSprite;
    sf::Texture explosionTexture;
    sf::Texture muzzleFlashTexture;
public:
    Game();
    void run();

private:
    void processEvents();
    void update();
    void loadMap(int level);

    void render();
    void shootBullet();//tạo đạn
    void removeOffscreenBullets();//đạn ra khỏi màn hình
    //void createWalls();//tường
    //Xử lý va chạm tank với tường
    void handlePlayerWallCollision(sf::Vector2f oldPosition);
    //Xử lý va chạm đạn với tường
    void handleBulletWallCollision();
    void createEnemies();//địch
    //địch trúng đạn
    void handleBulletEnemyCollision();

    void checkWinCondition();
    //Xử lý va chạm enemy với tường và mép
    void handleEnemyMovementCollision(std::vector<sf::Vector2f> oldEnemyPositions);
    //enemy va chạm với tank
    //void handleEnemyPlayerCollision();
    void checkGameOver();
    //enemy bắn đạn
    void enemyShootBullet();
    void handleEnemyBulletPlayerCollision();//trúng tank
    void handleEnemyBulletWallCollision();//trúng tường
    void removeOffscreenEnemyBullets();//xóa đạn của enemy
    //khởi tạo lại game
    void resetGame();

    void updateCamera();
    //Minimap
    void drawMiniMap();

    void createLevelButtons();
    void handleLevelSelectClick(sf::Vector2f mousePosition);
    void finishLevel();
    void drawLevelSelect();

    void drawMainMenu();
    void handleMainMenuClick(sf::Vector2f mousePosition);
    void drawAmmoBar();//thanh đạn
    //vật phẩm
    void dropRandomItem(sf::Vector2f position);
    void handlePlayerItemCollision();
    void updatePowerUps();
    void updateEffects();//hiệu ứng
    void startCameraShake(float power, float duration);

    bool canEnemySeePlayer(int enemyIndex);

    void createGroundTiles();
    void createGroundDetails();
    void drawPlayerHealthBar();
};