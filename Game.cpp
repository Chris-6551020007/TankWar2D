#include <iostream>
#include "Game.h"
#include <ctime>
#include <cstdlib>
#include <vector>
#include <string>
#include <cmath>
using namespace std;

const float MAP_WIDTH = 2000.f;
const float MAP_HEIGHT = 1500.f;
const float TILE_SIZE = 50.f;
//40 cột, 30 hàng

Game::Game()
    : window(sf::VideoMode({ 800, 600 }), "Tank Battle 2D"),//tạo cửa sổ 800x600
    player(100.f, 100.f),//tạo xe tăng (100,100)
    winText(font),
    gameOverText(font),
    scoreText(font),
    healthText(font),
    levelText(font),
    selectLevelText(font),
    titleText(font),
    finalScoreText(font),
    ammoText(font),
    powerText(font),
    startButtonText(font),
    shootSound(shootBuffer),
    startBackgroundSprite(startBackgroundTexture),
    levelSelectBackgroundSprite(levelSelectBackgroundTexture),
    tankTitleSprite(tankTitleTexture),
    selectTitleSprite(selectTitleTexture),
    victoryTitleSprite(victoryTitleTexture),
    startTextSprite(startTextTexture),
    gameOverTitleSprite(gameOverTitleTexture),
    hitSound(hitBuffer)

{
    
    //reset
    srand(static_cast<unsigned int>(time(nullptr)));

    camera.setSize({ 800.f, 600.f });//khung camera
    camera.setCenter(player.getPosition());

    window.setFramerateLimit(60);//60fps
    
    score = 0;
    //Vật phẩm
    rapidFireActive = false;
    bigBulletActive = false;
    shieldActive = false;

    powerText.setCharacterSize(20);
    powerText.setFillColor(sf::Color::Yellow);
    powerText.setPosition({ 10.f, 130.f });

    unlockedLevel = 0;
    currentLevel = 0;
    maxLevel = 3;

    gameOver = false;
    gameStarted = false;
    gameWon = false;

    //start and end
    showMainMenu = true;
    finalVictory = false;
    //máu người chơi
    playerHealth = 10;

    if (!font.openFromFile("assets/arial.ttf")) {
        window.setTitle("Font load failed");
    }
    //win
    winText.setString("YOU WIN!\nPress R to restart");
    winText.setCharacterSize(60);
    winText.setFillColor(sf::Color::Yellow);
    winText.setPosition({ 250.f, 250.f });
    //lose
    gameOverText.setString("GAME OVER\nPress R to restart");
    gameOverText.setCharacterSize(42);
    gameOverText.setFillColor(sf::Color::Red);
    gameOverText.setPosition({ 230.f, 220.f });
    //hiển thị điểm
    scoreText.setCharacterSize(24);
    scoreText.setFillColor(sf::Color::White);
    scoreText.setPosition({ 10.f, 10.f });
    //hiển thị máu
    healthText.setCharacterSize(24);
    healthText.setFillColor(sf::Color::White);
    healthText.setPosition({ 10.f, 40.f });
    
    levelText.setCharacterSize(24);
    levelText.setFillColor(sf::Color::White);
    levelText.setPosition({ 10.f, 70.f });

    //loadMap(currentLevel);

    selectLevelText.setString("SELECT LEVEL");
    selectLevelText.setCharacterSize(50);
    selectLevelText.setFillColor(sf::Color::Yellow);
    selectLevelText.setPosition({ 230.f, 100.f });

    createLevelButtons();
    //start
    titleText.setString("TANK WAR");
    titleText.setCharacterSize(80);
    titleText.setFillColor(sf::Color::Yellow);
    titleText.setPosition({ 190.f, 120.f });

    startButton.setSize({ 220.f, 80.f });
    startButton.setPosition({ 290.f, 300.f });
    startButton.setFillColor(sf::Color(70, 160, 70, 0));
    startButton.setOutlineThickness(0.f);
    //startButton.setOutlineColor(sf::Color::White);

    startButtonText.setString("START");
    startButtonText.setCharacterSize(50);
    startButtonText.setFillColor(sf::Color::Black);
    startButtonText.setPosition({ 345.f, 315.f });

    victoryOverlay.setSize({ 800.f, 600.f });
    victoryOverlay.setPosition({ 0.f, 0.f });
    victoryOverlay.setFillColor(sf::Color(0, 0, 0, 0));

    finalScoreText.setCharacterSize(30);
    finalScoreText.setFillColor(sf::Color::White);
    finalScoreText.setPosition({ 290.f, 330.f });
    //thanh đạn
    maxAmmo = 5;
    currentAmmo = maxAmmo;
    isReloading = false;

    ammoText.setCharacterSize(24);
    ammoText.setFillColor(sf::Color::White);
    ammoText.setPosition({ 10.f, 100.f });
    ammoText.setString("Ammo: 5/5");

    //âm thanh
    if (!shootBuffer.loadFromFile("assets/shoot.wav")) {
        window.setTitle("Shoot sound load failed");
    }

    if (!hitBuffer.loadFromFile("assets/hit.wav")) {
        window.setTitle("Hit sound load failed");
    }

    shootSound.setBuffer(shootBuffer);
    hitSound.setBuffer(hitBuffer);

    shootSound.setVolume(100.f);
    hitSound.setVolume(100.f);

    if (!backgroundMusic.openFromFile("assets/background.ogg")) {
        window.setTitle("Music load failed");
    }
    else {
        backgroundMusic.setLooping(true);
        backgroundMusic.setVolume(200.f);
        backgroundMusic.play();
    }
    //tiêu đề
    //tank war
    if (tankTitleTexture.loadFromFile("assets/title_tankwar.png")) {
        tankTitleSprite.setTexture(tankTitleTexture, true);

        sf::Vector2u size = tankTitleTexture.getSize();
        if (size.x > 0 && size.y > 0) {
            tankTitleSprite.setScale({
                420.f / static_cast<float>(size.x),
                140.f / static_cast<float>(size.y)
                });
        }

        tankTitleSprite.setPosition({ 190.f, 110.f });
    }
    //sellect level
    if (selectTitleTexture.loadFromFile("assets/title_select_level.png")) {
        selectTitleSprite.setTexture(selectTitleTexture, true);

        sf::Vector2u size = selectTitleTexture.getSize();
        if (size.x > 0 && size.y > 0) {
            selectTitleSprite.setScale({
                360.f / static_cast<float>(size.x),
                90.f / static_cast<float>(size.y)
                });
        }

        selectTitleSprite.setPosition({ 220.f, 90.f });
    }
    //victory
    if (victoryTitleTexture.loadFromFile("assets/title_victory.png")) {
        victoryTitleSprite.setTexture(victoryTitleTexture, true);

        sf::Vector2u size = victoryTitleTexture.getSize();
        if (size.x > 0 && size.y > 0) {
            victoryTitleSprite.setScale({
                340.f / static_cast<float>(size.x),
                120.f / static_cast<float>(size.y)
                });
        }

        victoryTitleSprite.setPosition({ 230.f, 180.f });
    }

    //run camera
    cameraShaking = false;
    cameraShakeDuration = 0.f;
    cameraShakePower = 0.f;

    if (!groundTexture.loadFromFile("assets/ground.png")) {
        window.setTitle("Ground texture load failed");
    }
    //hình nền
    if (startBackgroundTexture.loadFromFile("assets/start_background.png")) {
        startBackgroundSprite.setTexture(startBackgroundTexture, true);

        sf::Vector2u size = startBackgroundTexture.getSize();
        if (size.x > 0 && size.y > 0) {
            startBackgroundSprite.setScale({
                800.f / static_cast<float>(size.x),
                600.f / static_cast<float>(size.y)
                });
        }
    }
    else {
        std::cout << "Khong load duoc start_background.png\n";
    }

    if(levelSelectBackgroundTexture.loadFromFile("assets/level_select_background.png")) {
        levelSelectBackgroundSprite.setTexture(levelSelectBackgroundTexture, true);

        sf::Vector2u size = levelSelectBackgroundTexture.getSize();
        if (size.x > 0 && size.y > 0) {
            levelSelectBackgroundSprite.setScale({
                800.f / static_cast<float>(size.x),
                600.f / static_cast<float>(size.y)
                });
        }
    }
    else {
        std::cout << "Khong load duoc level_select_background.png\n";
    }
    //start
    if (startTextTexture.loadFromFile("assets/start_text.png")) {
        startTextSprite.setTexture(startTextTexture, true);

        sf::Vector2u size = startTextTexture.getSize();

        if (size.x > 0 && size.y > 0) {
            startTextSprite.setScale({
                220.f / static_cast<float>(size.x),
                80.f / static_cast<float>(size.y)
                });
        }

        startTextSprite.setPosition({ 290.f, 300.f });
    }
    else {
        std::cout << "Khong load duoc start_text.png\n";
    }
    //lặt vặt
    if (!grassPatchTexture.loadFromFile("assets/grass_patch.png")) {
        std::cout << "Khong load duoc grass_patch.png\n";
    }

    if (!rockSmallTexture.loadFromFile("assets/rock_small.png")) {
        std::cout << "Khong load duoc rock_small.png\n";
    }

    if (!dirtPatchTexture.loadFromFile("assets/dirt_patch.png")) {
        std::cout << "Khong load duoc dirt_patch.png\n";
    }
    //chữ game over
    if (gameOverTitleTexture.loadFromFile("assets/title_gameover.png")) {
        gameOverTitleSprite.setTexture(gameOverTitleTexture, true);

        sf::Vector2u size = gameOverTitleTexture.getSize();

        if (size.x > 0 && size.y > 0) {
            float targetWidth = 420.f;
            float scale = targetWidth / static_cast<float>(size.x);

            gameOverTitleSprite.setScale({ scale, scale });
        }

        sf::FloatRect localBounds = gameOverTitleSprite.getLocalBounds();

        gameOverTitleSprite.setOrigin({
            localBounds.position.x + localBounds.size.x / 2.f,
            localBounds.position.y + localBounds.size.y / 2.f
            });

        gameOverTitleSprite.setPosition({ 400.f, 180.f });
    }
}


//Chạy game chính
void Game::run() {
    while (window.isOpen()) {
        processEvents();//sử lý sự kiện
        update();//cập nhật logic
        render();//vẽ lên màn hình
    }
}

//Giải quyết phím
void Game::processEvents() {
    while (std::optional<sf::Event> event = window.pollEvent()) {

        if (event->is<sf::Event::Closed>()) {
            window.close();
        }

        // KEYBOARD
        if (const auto* keyPressed = event->getIf<sf::Event::KeyPressed>()) {

            if (keyPressed->scancode == sf::Keyboard::Scancode::Space) {
                if (gameStarted == true &&
                    gameWon == false &&
                    gameOver == false) {

                    if (rapidFireActive == false) {
                        shootBullet();
                    }
                }
            }

            if (keyPressed->scancode == sf::Keyboard::Scancode::R) {
                if (gameOver == true) {
                    resetGame();
                }
            }
        }

        // MOUSE
        if(const auto* mousePressed = event->getIf<sf::Event::MouseButtonPressed>()) {
            if (mousePressed->button == sf::Mouse::Button::Left) {

                sf::Vector2i mousePixel = sf::Mouse::getPosition(window);
                window.setView(window.getDefaultView());
                sf::Vector2f mousePosition = window.mapPixelToCoords(mousePixel);

                if (showMainMenu == true) {
                    handleMainMenuClick(mousePosition);
                }
                else if (gameStarted == false) {
                    handleLevelSelectClick(mousePosition);
                }
                else {
                    if (gameWon == false && gameOver == false && finalVictory == false) {
                        shootBullet();
                    }
                }
            }
        }
    }
}
//cập nhật trạng thái
void Game::update() {
    if (gameStarted == false) {
        return;
    }
    updateEffects();
    if (gameWon == true || gameOver == true) {
        updateCamera();
        return;
    }
    if (isReloading == true) {
        if (reloadClock.getElapsedTime().asSeconds() >= 3.f) {
            currentAmmo = maxAmmo;
            isReloading = false;
        }
    }
    levelText.setString("Level: " + std::to_string(currentLevel + 1));
    //người chơi
    sf::Vector2f oldPlayerPosition = player.getPosition();

    player.update();

    player.keepInMap(MAP_WIDTH, MAP_HEIGHT);

    handlePlayerWallCollision(oldPlayerPosition);

    updateCamera();

    sf::Vector2i mousePixel = sf::Mouse::getPosition(window);
    sf::Vector2f mouseWorld = window.mapPixelToCoords(mousePixel, camera);

    player.updateAim(mouseWorld);
    //giải quyêt vật phẩm đạn liên hoàn
    if (rapidFireActive == true) {
        if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left)) {

            if (rapidShootClock.getElapsedTime().asSeconds() >= 0.12f) {

                shootBullet();

                rapidShootClock.restart();
            }
        }
    }
    //đạn
    //đạn tank
    for (int i = 0; i < bullets.size(); i++) {
        bullets[i].update();
    }
    //đạn địch
    for (int i = 0; i < enemyBullets.size(); i++) {
        enemyBullets[i].update();
    }
    //Địch
    std::vector<sf::Vector2f> oldEnemyPositions;

    for (int i = 0; i < enemies.size(); i++) {
        oldEnemyPositions.push_back(enemies[i].getPosition());
    }

    for (int i = 0; i < enemies.size(); i++) {
        bool hasLineOfSight = canEnemySeePlayer(i);

        enemies[i].update(player.getPosition(), hasLineOfSight);
    }
    enemyShootBullet();

    if (isReloading == true) {
        ammoText.setString("Ammo: Reloading...");
    }
    else {
        ammoText.setString(
            "Ammo: " + std::to_string(currentAmmo) + "/" + std::to_string(maxAmmo)
        );
    }

    //handleEnemyPlayerCollision();//enemy&tank
    handleEnemyMovementCollision(oldEnemyPositions);
    //xóa đạn
    handleBulletWallCollision();
    handleBulletEnemyCollision();
    removeOffscreenBullets();//xóa đạn tank ra mép
    //điểm và máu
    scoreText.setString("Score: " + std::to_string(score));
    healthText.setString("HP: " + std::to_string(playerHealth));
    //đạn địch 
    removeOffscreenEnemyBullets();//xóa đạn địch ra mép
    handleEnemyBulletPlayerCollision();//trúng tank
    handleEnemyBulletWallCollision();//trúng tường
    //vật phẩm
    handlePlayerItemCollision();
    updatePowerUps();

    checkWinCondition();
    checkGameOver();


}

void Game::render() {
    window.clear();

    if (showMainMenu == true) {
        window.setView(window.getDefaultView());
        drawMainMenu();
        window.display();
        return;
    }

    // Neu chua bat dau game thi ve man hinh bat dau
    if (gameStarted == false) {
        window.setView(window.getDefaultView());
        drawLevelSelect();
        window.display();
        return;
    }

    // View camera cho the gioi game
    window.setView(camera);
    for (int i = 0; i < groundTiles.size(); i++) {
        window.draw(groundTiles[i]);
    }
    for (int i = 0; i < groundDetails.size(); i++) {
        window.draw(groundDetails[i]);
    }

    for (int i = 0; i < walls.size(); i++) {
        walls[i].draw(window);
    }

    for (int i = 0; i < items.size(); i++) {
        items[i].draw(window);
    }

    for (int i = 0; i < enemies.size(); i++) {
        enemies[i].draw(window);
    }

    for (int i = 0; i < bullets.size(); i++) {
        bullets[i].draw(window);
    }
    
    // Ve dan cua enemy
    for (int i = 0; i < enemyBullets.size(); i++) {
        enemyBullets[i].draw(window);
    }

    player.draw(window);
    //effect
    for (int i = 0; i < effects.size(); i++) {
        effects[i].draw(window);
    }

    // View mac dinh cho UI
    window.setView(window.getDefaultView());

    // Ve diem va mau
    window.draw(scoreText);
    drawPlayerHealthBar();
    window.draw(levelText);//level
    window.draw(powerText);//vật phẩm
    drawAmmoBar();//thanh đạn
    // Ve minimap
    drawMiniMap();

    if (gameWon == true) {
        window.draw(winText);
    }

    if (gameOver == true) {
        window.setView(window.getDefaultView());

        if (gameOverTitleTexture.getSize().x > 0) {
            window.draw(gameOverTitleSprite);
        }

        window.draw(gameOverText);
        window.display();
        return;
    }

    if (finalVictory == true) {
        float time = victoryClock.getElapsedTime().asSeconds();

        int alpha = static_cast<int>(time * 120.f);

        if (alpha > 180) {
            alpha = 180;
        }

        victoryOverlay.setFillColor(sf::Color(0, 0, 0, alpha));
        window.draw(victoryOverlay);

        int textAlpha = static_cast<int>((time - 0.5f) * 180.f);

        if (textAlpha < 0) {
            textAlpha = 0;
        }

        if (textAlpha > 255) {
            textAlpha = 255;
        }

        if (victoryTitleTexture.getSize().x > 0) {
            victoryTitleSprite.setColor(sf::Color(255, 255, 255, textAlpha));
            window.draw(victoryTitleSprite);
        }
        else {
            winText.setFillColor(sf::Color(255, 255, 0, textAlpha));
            winText.setScale({ 1.f, 1.f });
            window.draw(winText);
        }

        if (time > 1.5f) {
            finalScoreText.setFillColor(sf::Color(255, 255, 255, textAlpha));
            window.draw(finalScoreText);
        }
    }

    window.display();
}
//vẽ map
void Game::loadMap(int level) {
    groundTiles.clear();
    groundDetails.clear();

    createGroundTiles();
    createGroundDetails();
    std::vector<std::string> map;
    if (level == 0) {
        map = {
            "########################################",
            "#.P...BBBBBBBBBB.........BBBBBBBBB.....#",
            "#...................BB.................#",
            "#......................................#",
            "#.#####.#####...####..#...#.#...#.######",
            "#.#.....#...#...#...#.#...#.##..#.#....#",
            "#.#.....#...#...#...#.#...#.#.#.#.#....#",
            "#.#.....#...#...#...#.#...#.#..##.#.####",
            "#.#.....#...#...#...#.#...#.#...#.#...##",
            "#.#.....#...#...#...#.#...#.#...#.#...##",
            "#.#####.#####...####..#####.#...#.######",
            "#......................................#",
            "#......................................#",
            "#..................E...................#",
            "#......................................#",
            "#......................................#",
            "##...#.#.#...#.#...#..####..#####.####.#",
            "#.#.#..#.##..#.#...#..#...#.#.....#...##",
            "#..#...#.#.#.#.#...#..#...#.#.....#...##",
            "#..#...#.#..##.#####..#...#.####..####.#",
            "#..#...#.#...#.#...#..#...#.#.....#....#",
            "#.#.#..#.#...#.#...#..#...#.#.....#....#",
            "##...#.#.#...#.#...#..####..#####.#....#",
            "#......................................#",
            "#......................................#",
            "#....BBBBBBBBBBBBBBBBBBBBBBBBBBBBBB....#",
            "#....BBBBBBBBBBBBBBBBBBBBBBBBBBBBBB....#",
            "#....BBBBBBBBBBBBBBBBBBBBBBBBBBBBBB....#",
            "#......................................#",
            "########################################"
        };
    }
    else if (level == 1) {
        map = {
            "########################################",
            "#.P....#.............#.............E...#",
            "#......#.............#.................#",
            "#..###.#..#########..#..#########..##..#",
            "#....#.#..#.......#.....#.......#......#",
            "#.B..#....#..E....#######....E..#..B...#",
            "#....######.......#.....#.......######.#",
            "#......................................#",
            "#......E.....######..######.....E......#",
            "#............#............#............#",
            "#..###BB###..#....B..B....#..###BB###..#",
            "#..#......#..#............#..#......#..#",
            "#..#..E...#..######..######..#...E..#..#",
            "#..#......#..................#......#..#",
            "#..########......####........########..#",
            "#................#..#..................#",
            "#......####......#..#......####........#",
            "#......#..#......#..#......#..#........#",
            "#..E...#..#.................#..#...E...#",
            "#......####....########....####........#",
            "#..............#......#................#",
            "#....######....#..B...#....######......#",
            "#....#....#....#......#....#....#......#",
            "#....#....#.................#....#.....#",
            "#....#.E..########..########..E.#......#",
            "#....#..........................#......#",
            "#....##########........##########......#",
            "#.................B....................#",
            "#......................................#",
            "########################################"
        };
    }
    else if (level == 2) {
        map = {
            "########################################",
            "#.P.#.....E....#.......#.....E.....#...#",
            "#...#..........#..BBB..#...........#...#",
            "#...####..######..###..######..####....#",
            "#......#...............#...............#",
            "#..E...#..#######..#######..#....E.....#",
            "#......#..#.............#..#...........#",
            "#..######.#..E..#####..E#..######......#",
            "#..........#.....#B#.....#.............#",
            "#####..#####.....#.#.....#####..####...#",
            "#...#.............#.............#......#",
            "#...#..E..########.########..E..#......#",
            "#...#.............B.............#......#",
            "#...######..####.....####..######......#",
            "#..........#............#..............#",
            "#..E.......#....####....#.......E......#",
            "#..........#....#BB#....#..............#",
            "#...######..####....####..######.......#",
            "#...#.............B.............#......#",
            "#...#..E..########.########..E..#......#",
            "#...#.............#.............#......#",
            "#####..#####.....#.#.....#####..####...#",
            "#..........#.....#B#.....#.............#",
            "#..######.#..E..#####..E#..######......#",
            "#......#..#.............#..#...........#",
            "#..E...#..#######..#######..#....E.....#",
            "#......#...............#...............#",
            "#...####..######..###..######..####....#",
            "#..............E..........E............#",
            "########################################"
        };
    }
        for (int row = 0; row < map.size(); row++) {
            for (int col = 0; col < map[row].size(); col++) {
                char tile = map[row][col];

                float x = col * TILE_SIZE;
                float y = row * TILE_SIZE;

                if (tile == '#') {
                    walls.push_back(Wall(x, y, TILE_SIZE, TILE_SIZE, false));
                }
                else if (tile == 'B') {
                    walls.push_back(Wall(x, y, TILE_SIZE, TILE_SIZE, true));
                }
                else if (tile == 'P') {
                    player.setPosition({ x + TILE_SIZE / 2.f, y + TILE_SIZE / 2.f });
                }
                else if (tile == 'E') {
                    enemies.push_back(Enemy({ x + TILE_SIZE / 2.f, y + TILE_SIZE / 2.f }));
                }
            }
        }
    }

    //Bắn đạn
    void Game::shootBullet() {

        // rapid fire không dùng ammo
        if (rapidFireActive == false) {

            if (isReloading == true) {
                return;
            }

            if (currentAmmo <= 0) {
                isReloading = true;
                reloadClock.restart();
                return;
            }
        }

        sf::Vector2i mousePixel = sf::Mouse::getPosition(window);
        sf::Vector2f mouseWorld = window.mapPixelToCoords(mousePixel, camera);

        player.updateAim(mouseWorld);

        float bulletSpeed = 6.f;
        float bulletRadius = 8.f;
        int bulletDamage = 1;

        if (bigBulletActive == true) {
            bulletSpeed = 6.f;
            bulletRadius = 14.f;
            bulletDamage = 3;
        }

        Bullet bullet(
            player.getBulletSpawnPosition(),
            player.getAimDirection(),
            bulletSpeed,
            bulletRadius,
            bulletDamage
        );

        bullets.push_back(bullet);
        shootSound.play();
        effects.push_back(
            Effect(
                player.getBulletSpawnPosition(),
                EffectType::MuzzleFlash,
                player.getAimDirection()
            )
        );

        // rapid fire không trừ ammo
        if (rapidFireActive == false) {

            currentAmmo--;

            if (currentAmmo <= 0) {
                isReloading = true;
                reloadClock.restart();
            }
        }
    }
    //xóa đạn khi ra khỏi màn hình
    void Game::removeOffscreenBullets() {
        for (int i = 0; i < bullets.size(); i++) {
            sf::Vector2f position = bullets[i].getPosition();

            if (position.x < 0.f ||
                position.x > MAP_WIDTH ||
                position.y < 0.f ||
                position.y > MAP_HEIGHT) {

                bullets.erase(bullets.begin() + i);
                i--;
            }
        }
    }
    
    //Chạm tường
    void Game::handlePlayerWallCollision(sf::Vector2f oldPosition) {
        for (int i = 0; i < walls.size(); i++) {
            if (player.getBounds().findIntersection(walls[i].getBounds())) {

                if (walls[i].isBreakable() == true) {
                    walls.erase(walls.begin() + i);
                    i--;
                }
                else {
                    player.setPosition(oldPosition);
                }
            }
        }
    }
    //Đạn chạm tường
    void Game::handleBulletWallCollision() {
        for (int i = 0; i < bullets.size(); i++) {
            bool bulletRemoved = false;

            for (int j = 0; j < walls.size(); j++) {
                if (bullets[i].getBounds().findIntersection(walls[j].getBounds())) {
                    hitSound.play();
                    effects.push_back(
                        Effect(
                            bullets[i].getPosition(),
                            EffectType::BulletImpact
                        )
                    );
                    if (walls[j].isBreakable() == true) {//tường bị phá
                        walls.erase(walls.begin() + j);
                    }

                    bullets.erase(bullets.begin() + i);
                    i--;

                    bulletRemoved = true;
                    break;
                }
            }

            if (bulletRemoved == true) {
                continue;
            }
        }
    }
    //Địch
    void Game::createEnemies() {
        enemies.push_back(Enemy(650.f, 100.f));
        enemies.push_back(Enemy(650.f, 500.f));
    }
    //Đạn trúng địch
    void Game::handleBulletEnemyCollision() {
        for (int i = 0; i < bullets.size(); ) {
            bool bulletRemoved = false;

            for (int j = 0; j < enemies.size(); j++) {
                if (bullets[i].getBounds().findIntersection(enemies[j].getBounds())) {

                    int damage = bullets[i].getDamage();
                    hitSound.play();
                    effects.push_back(
                        Effect(
                            bullets[i].getPosition(),
                            EffectType::BulletImpact
                        )
                    );
                    startCameraShake(4.f, 0.15f);

                    enemies[j].takeDamage(damage);

                    bullets.erase(bullets.begin() + i);
                    bulletRemoved = true;

                    if (enemies[j].isDead()) {
                        sf::Vector2f dropPosition = enemies[j].getPosition();

                        effects.push_back(
                            Effect(
                                dropPosition,
                                EffectType::ExplosionEnemy
                            )
                        );

                        enemies.erase(enemies.begin() + j);
                        score += 100;

                        dropRandomItem(dropPosition);
                    }

                    break;
                }
            }

            if (bulletRemoved == false) {
                i++;
            }
        }
    }
  
    void Game::checkWinCondition() {
        if (enemies.size() == 0 && gameWon == false) {
            finishLevel();
        }
    }

    //void Game::handleEnemyPlayerCollision()
    //{
    //}

    void Game::checkGameOver() {
        if (playerHealth <= 0 && gameOver == false) {

            effects.push_back(
                Effect(
                    player.getPosition(),
                    EffectType::ExplosionPlayer
                )
            );

            startCameraShake(12.f, 0.5f);

            gameOver = true;

            gameOverText.setString(
                "Score: " + std::to_string(score) + "\n"
                "Press R to restart"
            );

            gameOverText.setCharacterSize(28);
            gameOverText.setFillColor(sf::Color::White);
            gameOverText.setOutlineColor(sf::Color::Black);
            gameOverText.setOutlineThickness(2.f);
            gameOverText.setPosition({ 300.f, 260.f });
        }
    }

    //va chạm của địch
    void Game::handleEnemyMovementCollision(std::vector<sf::Vector2f> oldEnemyPositions) {
        std::vector<sf::Vector2f> possibleDirections;

        possibleDirections.push_back({ 1.f, 0.f });
        possibleDirections.push_back({ -1.f, 0.f });
        possibleDirections.push_back({ 0.f, 1.f });
        possibleDirections.push_back({ 0.f, -1.f });

        for (int i = 0; i < enemies.size(); i++) {
            bool collided = false;

            sf::Vector2f enemyPosition = enemies[i].getPosition();

            if (enemyPosition.x < 25.f ||
                enemyPosition.x > MAP_WIDTH - 25.f ||
                enemyPosition.y < 25.f ||
                enemyPosition.y > MAP_HEIGHT - 25.f) {
                collided = true;
            }

            for (int j = 0; j < walls.size(); j++) {
                if (enemies[i].getBounds().findIntersection(walls[j].getBounds())) {
                    collided = true;
                }
            }

            if (enemies[i].getBounds().findIntersection(player.getBounds())) {
                collided = true;
            }

            if (collided == true) {
                enemies[i].setPosition(oldEnemyPositions[i]);

                if (enemies[i].isPlayerInRange() == false) {
                    for (int t = 0; t < 10; t++) {
                        int randomIndex = rand() % possibleDirections.size();

                        enemies[i].chooseDirection(possibleDirections[randomIndex]);

                        sf::Vector2f testPosition =
                            oldEnemyPositions[i] + possibleDirections[randomIndex] * 10.f;

                        enemies[i].setPosition(testPosition);

                        bool stillCollide = false;

                        if (testPosition.x < 25.f ||
                            testPosition.x > MAP_WIDTH - 25.f ||
                            testPosition.y < 25.f ||
                            testPosition.y > MAP_HEIGHT - 25.f) {
                            stillCollide = true;
                        }

                        for (int j = 0; j < walls.size(); j++) {
                            if (enemies[i].getBounds().findIntersection(walls[j].getBounds())) {
                                stillCollide = true;
                            }
                        }

                        enemies[i].setPosition(oldEnemyPositions[i]);

                        if (stillCollide == false) {
                            enemies[i].chooseDirection(possibleDirections[randomIndex]);
                            break;
                        }
                    }
                }
            }
        }
    }
    ////enemy va chạm với tank
    //void Game::handleEnemyPlayerCollision() {
    //    for (int i = 0; i < enemies.size(); i++) {
    //        if (enemies[i].getBounds().findIntersection(player.getBounds())) {
    //            //trừ máu 1/60s
    //            if (damageClock.getElapsedTime().asSeconds() >= 1.f) {
    //                playerHealth--;
    //                damageClock.restart();
    //
    //                window.setTitle("Health: " + std::to_string(playerHealth) +
    //                    " | Score: " + std::to_string(score));
    //            }
    //        }
    //    }
    //}
    //khởi tạo lại game
    void Game::resetGame() {
        score = 0;
        playerHealth = 10;
        unlockedLevel = 0;
        currentLevel = 0;

        effects.clear();
        items.clear();

        rapidFireActive = false;
        bigBulletActive = false;
        shieldActive = false;
        powerText.setString("");

        currentAmmo = maxAmmo;
        isReloading = false;

        showMainMenu = true;
        finalVictory = false;

        gameStarted = false;
        gameWon = false;
        gameOver = false;

        bullets.clear();
        enemies.clear();
        walls.clear();
        enemyBullets.clear();

        createLevelButtons();

        scoreText.setString("Score: " + std::to_string(score));
        healthText.setString("HP: " + std::to_string(playerHealth));

        window.setTitle("Select Level");
    }
    //địch bắn đạn
    void Game::enemyShootBullet() {
        if (enemyShootClock.getElapsedTime().asSeconds() >= 1.5f) {
            for (int i = 0; i < enemies.size(); i++) {
                if (enemies[i].isPlayerInRange() == true) {
                    Bullet bullet(
                        enemies[i].getBulletSpawnPosition(),
                        enemies[i].getAimDirection()
                    );

                    enemyBullets.push_back(bullet);
                    shootSound.play();//âm nhanh
                    //effect
                    effects.push_back(
                        Effect(
                            enemies[i].getBulletSpawnPosition(),
                            EffectType::MuzzleFlash,
                            enemies[i].getAimDirection()
                        )
                    );
                }
            }

            enemyShootClock.restart();
        }
    }
    //VA CHẠM ĐẠN CỦA ĐỊCH
    //đạn chạm mép
    void Game::removeOffscreenEnemyBullets() {
        for (int i = 0; i < enemyBullets.size(); i++) {
            sf::Vector2f position = enemyBullets[i].getPosition();

            if (position.x < 0.f ||
                position.x > MAP_WIDTH ||
                position.y < 0.f ||
                position.y > MAP_HEIGHT) {

                enemyBullets.erase(enemyBullets.begin() + i);
                i--;
            }
        }
    }
    //đạn chạm tank
    void Game::handleEnemyBulletPlayerCollision() {
        for (int i = 0; i < enemyBullets.size(); i++) {
            if (enemyBullets[i].getBounds().findIntersection(player.getBounds())) {
                hitSound.play();
                effects.push_back(
                    Effect(
                        enemyBullets[i].getPosition(),
                        EffectType::BulletImpact
                    )
                );
                enemyBullets.erase(enemyBullets.begin() + i);
                i--;

                if (shieldActive == false) {
                    playerHealth--;
                }

                healthText.setString("HP: " + std::to_string(playerHealth));
            }
        }
    }
    //đạn chạm tường
    void Game::handleEnemyBulletWallCollision() {
        for (int i = 0; i < enemyBullets.size(); i++) {
            for (int j = 0; j < walls.size(); j++) {
                if (enemyBullets[i].getBounds().findIntersection(walls[j].getBounds())) {
                    hitSound.play();
                    effects.push_back(
                        Effect(
                            enemyBullets[i].getPosition(),
                            EffectType::BulletImpact
                        )
                    );
                    if (walls[j].isBreakable() == true) {
                        walls.erase(walls.begin() + j);
                    }

                    enemyBullets.erase(enemyBullets.begin() + i);
                    i--;

                    break;
                }
            }
        }
    }
    //Giới hạn không cho camera ra ngoài map
    void Game::updateCamera() {
        sf::Vector2f center = player.getPosition();

        float halfWidth = camera.getSize().x / 2.f;
        float halfHeight = camera.getSize().y / 2.f;

        if (center.x < halfWidth) {
            center.x = halfWidth;
        }

        if (center.y < halfHeight) {
            center.y = halfHeight;
        }

        if (center.x > MAP_WIDTH - halfWidth) {
            center.x = MAP_WIDTH - halfWidth;
        }

        if (center.y > MAP_HEIGHT - halfHeight) {
            center.y = MAP_HEIGHT - halfHeight;
        }

        if (cameraShaking == true) {
            float elapsed = cameraShakeClock.getElapsedTime().asSeconds();

            if (elapsed >= cameraShakeDuration) {
                cameraShaking = false;
            }
            else {
                float progress = elapsed / cameraShakeDuration;
                float currentPower = cameraShakePower * (1.f - progress);

                float offsetX = static_cast<float>((rand() % 200) - 100) / 100.f * currentPower;
                float offsetY = static_cast<float>((rand() % 200) - 100) / 100.f * currentPower;

                center.x += offsetX;
                center.y += offsetY;
            }
        }

        camera.setCenter(center);
    }

    //vẽ minimap
    void Game::drawMiniMap() {
        float miniMapX = 580.f;
        float miniMapY = 20.f;
        float miniMapWidth = 200.f;
        float miniMapHeight = 150.f;

        // Nền minimap
        sf::RectangleShape miniMapBackground;
        miniMapBackground.setSize({ miniMapWidth, miniMapHeight });
        miniMapBackground.setPosition({ miniMapX, miniMapY });
        miniMapBackground.setFillColor(sf::Color(30, 30, 30, 180));
        miniMapBackground.setOutlineThickness(2.f);
        miniMapBackground.setOutlineColor(sf::Color::White);

        window.draw(miniMapBackground);

        // Vẽ tường trên minimap
        for (int i = 0; i < walls.size(); i++) {
            sf::FloatRect wallBounds = walls[i].getBounds();

            float wallMiniX = miniMapX + wallBounds.position.x / MAP_WIDTH * miniMapWidth;
            float wallMiniY = miniMapY + wallBounds.position.y / MAP_HEIGHT * miniMapHeight;
            float wallMiniW = wallBounds.size.x / MAP_WIDTH * miniMapWidth;
            float wallMiniH = wallBounds.size.y / MAP_HEIGHT * miniMapHeight;

            sf::RectangleShape wallDot;
            wallDot.setPosition({ wallMiniX, wallMiniY });
            wallDot.setSize({ wallMiniW, wallMiniH });
            wallDot.setFillColor(sf::Color(120, 120, 120));

            window.draw(wallDot);
        }

        // Vẽ player trên minimap
        sf::Vector2f playerPos = player.getPosition();

        float playerMiniX = miniMapX + playerPos.x / MAP_WIDTH * miniMapWidth;
        float playerMiniY = miniMapY + playerPos.y / MAP_HEIGHT * miniMapHeight;

        sf::CircleShape playerDot;
        playerDot.setRadius(5.f);
        playerDot.setOrigin({ 5.f, 5.f });
        playerDot.setPosition({ playerMiniX, playerMiniY });
        playerDot.setFillColor(sf::Color::Green);

        window.draw(playerDot);

        // Vẽ enemy trên minimap
        for (int i = 0; i < enemies.size(); i++) {
            sf::Vector2f enemyPos = enemies[i].getPosition();

            float enemyMiniX = miniMapX + enemyPos.x / MAP_WIDTH * miniMapWidth;
            float enemyMiniY = miniMapY + enemyPos.y / MAP_HEIGHT * miniMapHeight;

            sf::CircleShape enemyDot;
            enemyDot.setRadius(4.f);
            enemyDot.setOrigin({ 4.f, 4.f });
            enemyDot.setPosition({ enemyMiniX, enemyMiniY });
            enemyDot.setFillColor(sf::Color::Red);

            window.draw(enemyDot);
        }
    }
    
    //tạo nút chọn màn
    void Game::createLevelButtons() {
        levelButtons.clear();
        levelButtonTexts.clear();

        for (int i = 0; i < maxLevel; i++) {
            sf::RectangleShape button;
            button.setSize({ 140.f, 80.f });
            button.setPosition({ 150.f + i * 180.f, 250.f });

            if (i <= unlockedLevel) {
                button.setFillColor(sf::Color(70, 160, 70));
            }
            else {
                button.setFillColor(sf::Color(80, 80, 80));
            }

            button.setOutlineThickness(3.f);
            button.setOutlineColor(sf::Color::White);

            levelButtons.push_back(button);

            sf::Text text(font);

            if (i <= unlockedLevel) {
                text.setString("Level " + std::to_string(i + 1));
            }
            else {
                text.setString("Locked");
            }

            text.setCharacterSize(24);
            text.setFillColor(sf::Color::White);
            text.setPosition({ 170.f + i * 180.f, 275.f });

            levelButtonTexts.push_back(text);
        }
    }

    //xử lý kich chọn màn
    void Game::handleLevelSelectClick(sf::Vector2f mousePosition) {
        currentAmmo = maxAmmo;
        isReloading = false;
        for (int i = 0; i < levelButtons.size(); i++) {
            if (levelButtons[i].getGlobalBounds().contains(mousePosition)) {
                if (i <= unlockedLevel) {
                    currentLevel = i;

                    items.clear();
                    effects.clear();

                    rapidFireActive = false;
                    bigBulletActive = false;
                    shieldActive = false;
                    powerText.setString("");

                    bullets.clear();
                    enemyBullets.clear();
                    enemies.clear();
                    walls.clear();

                    playerHealth = 10;
                    gameOver = false;
                    gameWon = false;
                    gameStarted = true;
                    backgroundMusic.setVolume(65.f);

                    loadMap(currentLevel);

                    scoreText.setString("Score: " + std::to_string(score));
                    healthText.setString("HP: " + std::to_string(playerHealth));

                    window.setTitle("Level " + std::to_string(currentLevel + 1));
                }
            }
        }
    }
    void Game::drawLevelSelect() {
        window.setView(window.getDefaultView());

        if (levelSelectBackgroundTexture.getSize().x > 0) {
            window.draw(levelSelectBackgroundSprite);
        }

        if (selectTitleTexture.getSize().x > 0) {
            window.draw(selectTitleSprite);
        }
        else {
            window.draw(selectLevelText);
        }

        for (int i = 0; i < levelButtons.size(); i++) {
            window.draw(levelButtons[i]);
            window.draw(levelButtonTexts[i]);
        }
    }
    void Game::finishLevel() {
        if (currentLevel == maxLevel - 1) {
            finalVictory = true;
            gameWon = true;
            victoryClock.restart();

            winText.setString("VICTORY");
            winText.setCharacterSize(70);
            winText.setFillColor(sf::Color(255, 255, 0, 0));
            winText.setPosition({ 245.f, 230.f });

            finalScoreText.setString("Final Score: " + std::to_string(score));
            finalScoreText.setFillColor(sf::Color(255, 255, 255, 0));

            bullets.clear();
            effects.clear();
            enemyBullets.clear();

            window.setTitle("VICTORY");
            return;
        }

        gameWon = false;
        gameStarted = false;

        if (unlockedLevel < currentLevel + 1) {
            unlockedLevel = currentLevel + 1;
        }

        bullets.clear();
        enemyBullets.clear();
        enemies.clear();
        walls.clear();

        createLevelButtons();

        window.setTitle("Select Level");
    }

    //menu
    void Game::drawMainMenu() {
        if (startBackgroundTexture.getSize().x > 0) {
            window.draw(startBackgroundSprite);
        }

        if (tankTitleTexture.getSize().x > 0) {
            window.draw(tankTitleSprite);
        }
        else {
            window.draw(titleText);
        }

        window.draw(startButton);

        if (startTextTexture.getSize().x > 0) {
            window.draw(startTextSprite);
        }
        else {
            window.draw(startButtonText);
        }
    }
    //start
    void Game::handleMainMenuClick(sf::Vector2f mousePosition) {
        if (startButton.getGlobalBounds().contains(mousePosition)) {
            showMainMenu = false;
            gameStarted = false;
            window.setTitle("Select Level");
        }
    }

    //vẽ thanh đạn
    void Game::drawAmmoBar() {
        float startX = 10.f;
        float startY = 100.f;

        float boxWidth = 28.f;
        float boxHeight = 14.f;
        float spacing = 8.f;

        sf::Text label(font);
        //label.setString("AMMO");
        //label.setCharacterSize(18);
        //label.setFillColor(sf::Color::White);
        //label.setPosition({ startX, startY - 25.f });

        window.draw(label);

        for (int i = 0; i < maxAmmo; i++) {
            sf::RectangleShape ammoBox;
            ammoBox.setSize({ boxWidth, boxHeight });
            ammoBox.setPosition({ startX + i * (boxWidth + spacing), startY });
            ammoBox.setOutlineThickness(2.f);
            ammoBox.setOutlineColor(sf::Color::White);

            if (i < currentAmmo) {
                ammoBox.setFillColor(sf::Color::Yellow);
            }
            else {
                ammoBox.setFillColor(sf::Color(60, 60, 60));
            }

            window.draw(ammoBox);
        }

        if (isReloading == true) {
            sf::Text reloadText(font);
            reloadText.setString("RELOADING...");
            reloadText.setCharacterSize(18);
            reloadText.setFillColor(sf::Color::Red);
            reloadText.setPosition({ startX + 190.f, startY - 4.f });

            window.draw(reloadText);
        }
    }
    //rơi item ngẫu nhiên
    void Game::dropRandomItem(sf::Vector2f position) {
        int randomNumber = rand() % 5;

        if (randomNumber == 0) {
            return;
        }

        if (randomNumber == 1) {
            items.push_back(Item(position, ItemType::Heal));
        }
        else if (randomNumber == 2) {
            items.push_back(Item(position, ItemType::RapidFire));
        }
        else if (randomNumber == 3) {
            items.push_back(Item(position, ItemType::BigBullet));
        }
        else if (randomNumber == 4) {
            items.push_back(Item(position, ItemType::Shield));
        }
    }

    //Nhặt item
    void Game::handlePlayerItemCollision() {
        for (int i = 0; i < items.size(); i++) {
            if (player.getBounds().findIntersection(items[i].getBounds())) {
                ItemType type = items[i].getType();

                if (type == ItemType::Heal) {
                    playerHealth += 3;

                    if (playerHealth > 10) {
                        playerHealth = 10;
                    }
                }
                else if (type == ItemType::RapidFire) {
                    rapidFireActive = true;
                    rapidFireClock.restart();
                }
                else if (type == ItemType::BigBullet) {
                    bigBulletActive = true;
                    bigBulletClock.restart();
                }
                else if (type == ItemType::Shield) {
                    shieldActive = true;
                    shieldClock.restart();
                }

                items.erase(items.begin() + i);
                i--;
            }
        }
    }
    void Game::updatePowerUps() {
        powerText.setString("");

        if (rapidFireActive == true) {
            float timeLeft = 10.f - rapidFireClock.getElapsedTime().asSeconds();

            if (timeLeft <= 0.f) {
                rapidFireActive = false;
            }
            else {
                powerText.setString(powerText.getString() + "Rapid Fire: " + std::to_string(static_cast<int>(timeLeft)) + "s\n");
            }
        }

        if (bigBulletActive == true) {
            float timeLeft = 10.f - bigBulletClock.getElapsedTime().asSeconds();

            if (timeLeft <= 0.f) {
                bigBulletActive = false;
            }
            else {
                powerText.setString(powerText.getString() + "Big Bullet: " + std::to_string(static_cast<int>(timeLeft)) + "s\n");
            }
        }

        if (shieldActive == true) {
            float timeLeft = 10.f - shieldClock.getElapsedTime().asSeconds();

            if (timeLeft <= 0.f) {
                shieldActive = false;
            }
            else {
                powerText.setString(powerText.getString() + "Shield: " + std::to_string(static_cast<int>(timeLeft)) + "s\n");
            }
        }
    }
    void Game::updateEffects() {
        for (int i = 0; i < effects.size(); i++) {
            effects[i].update();

            if (effects[i].isAlive() == false) {
                effects.erase(effects.begin() + i);
                i--;
            }
        }
    }
    void Game::startCameraShake(float power, float duration) {
        cameraShaking = true;
        cameraShakePower = power;
        cameraShakeDuration = duration;
        cameraShakeClock.restart();
    }
    bool Game::canEnemySeePlayer(int enemyIndex) {
        sf::Vector2f enemyPosition = enemies[enemyIndex].getPosition();
        sf::Vector2f playerPosition = player.getPosition();

        sf::Vector2f toPlayer = playerPosition - enemyPosition;

        float distance = std::sqrt(
            toPlayer.x * toPlayer.x +
            toPlayer.y * toPlayer.y
        );

        if (distance == 0.f) {
            return true;
        }

        sf::Vector2f direction = toPlayer / distance;

        float step = 10.f;

        for (float currentDistance = 0.f; currentDistance < distance; currentDistance += step) {
            sf::Vector2f checkPoint = enemyPosition + direction * currentDistance;

            for (int i = 0; i < walls.size(); i++) {
                if (walls[i].getBounds().contains(checkPoint)) {
                    return false;
                }
            }
        }

        return true;
    }
    void Game::createGroundTiles() {
        groundTiles.clear();

        const float tileSize = 50.f;

        for (float y = 0.f; y < MAP_HEIGHT; y += tileSize) {
            for (float x = 0.f; x < MAP_WIDTH; x += tileSize) {
                sf::Sprite tile(groundTexture);

                tile.setPosition({ x, y });

                sf::Vector2u textureSize = groundTexture.getSize();

                if (textureSize.x > 0 && textureSize.y > 0) {
                    tile.setScale({
                        tileSize / static_cast<float>(textureSize.x),
                        tileSize / static_cast<float>(textureSize.y)
                        });
                }

                groundTiles.push_back(tile);
            }
        }
    }
    void Game::createGroundDetails() {
        groundDetails.clear();

        const float tileSize = 50.f;

        for (float y = 0.f; y < MAP_HEIGHT; y += tileSize) {
            for (float x = 0.f; x < MAP_WIDTH; x += tileSize) {
                int randomNumber = rand() % 100;

                if (randomNumber < 8 && grassPatchTexture.getSize().x > 0) {
                    sf::Sprite detail(grassPatchTexture);

                    sf::Vector2u size = grassPatchTexture.getSize();

                    detail.setScale({
                        35.f / static_cast<float>(size.x),
                        35.f / static_cast<float>(size.y)
                        });

                    detail.setPosition({
                        x + static_cast<float>(rand() % 15),
                        y + static_cast<float>(rand() % 15)
                        });

                    groundDetails.push_back(detail);
                }
                else if (randomNumber >= 8 && randomNumber < 12 && rockSmallTexture.getSize().x > 0) {
                    sf::Sprite detail(rockSmallTexture);

                    sf::Vector2u size = rockSmallTexture.getSize();

                    detail.setScale({
                        22.f / static_cast<float>(size.x),
                        22.f / static_cast<float>(size.y)
                        });

                    detail.setPosition({
                        x + static_cast<float>(rand() % 25),
                        y + static_cast<float>(rand() % 25)
                        });

                    groundDetails.push_back(detail);
                }
                else if (randomNumber >= 12 && randomNumber < 18 && dirtPatchTexture.getSize().x > 0) {
                    sf::Sprite detail(dirtPatchTexture);

                    sf::Vector2u size = dirtPatchTexture.getSize();

                    detail.setScale({
                        45.f / static_cast<float>(size.x),
                        45.f / static_cast<float>(size.y)
                        });

                    detail.setPosition({
                        x + static_cast<float>(rand() % 10),
                        y + static_cast<float>(rand() % 10)
                        });

                    groundDetails.push_back(detail);
                }
            }
        }
    }
    //vẽ thanh áu pleyer
    void Game::drawPlayerHealthBar() {
        float startX = 10.f;
        float startY = 40.f;

        float barWidth = 160.f;
        float barHeight = 18.f;

        float healthPercent = static_cast<float>(playerHealth) / 10.f;

        if (healthPercent < 0.f) {
            healthPercent = 0.f;
        }

        if (healthPercent > 1.f) {
            healthPercent = 1.f;
        }

        sf::Text label(font);
        label.setString("HP");
        label.setCharacterSize(18);
        label.setFillColor(sf::Color::White);
        label.setOutlineColor(sf::Color::Black);
        label.setOutlineThickness(2.f);
        label.setPosition({ startX, startY - 3.f });

        window.draw(label);

        sf::RectangleShape backBar;
        backBar.setSize({ barWidth, barHeight });
        backBar.setPosition({ startX + 40.f, startY });
        backBar.setFillColor(sf::Color(50, 50, 50, 180));
        backBar.setOutlineThickness(2.f);
        backBar.setOutlineColor(sf::Color::White);

        window.draw(backBar);

        sf::RectangleShape frontBar;
        frontBar.setSize({ barWidth * healthPercent, barHeight });
        frontBar.setPosition({ startX + 40.f, startY });

        if (playerHealth > 6) {
            frontBar.setFillColor(sf::Color::Green);
        }
        else if (playerHealth > 3) {
            frontBar.setFillColor(sf::Color::Yellow);
        }
        else {
            frontBar.setFillColor(sf::Color::Red);
        }

        window.draw(frontBar);

        sf::Text hpNumber(font);
        hpNumber.setString(std::to_string(playerHealth) + "/10");
        hpNumber.setCharacterSize(16);
        hpNumber.setFillColor(sf::Color::White);
        hpNumber.setOutlineColor(sf::Color::Black);
        hpNumber.setOutlineThickness(2.f);
        hpNumber.setPosition({ startX + 95.f, startY - 1.f });

        window.draw(hpNumber);
    }