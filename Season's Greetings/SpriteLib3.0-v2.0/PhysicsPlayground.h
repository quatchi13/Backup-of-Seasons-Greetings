#pragma once
#include "Vector.h"
#include "Scene.h"
#include "PhysicsPlaygroundListener.h"

class PhysicsPlayground : public Scene
{
public:
	PhysicsPlayground();

	PhysicsPlayground(std::string name);

	void InitScene(float windowWidth, float windowHeight) override;

	void Update() override;

	//Input overrides
	void KeyboardHold() override;
	void KeyboardDown() override;
	void KeyboardUp() override;

	void makeImage(std::string filename, int width, int height, float opacity, float x, float y, float z);
	void makeCamFocus();
	void makeWall(int index);
	void makeFrame(std::string fileName, int width, int height, vec3 pos);
	void makeJAnimFrame(std::vector<std::string>frames, std::vector<int>pauses, int width, int height, vec3 pos);
	void makeLockedDoor();
	void makeNonDoor();

	void makeSpike(int index);
	void makeShooter(int index);
	void makeEnemy(int index);
	void makeClockwiseEnemy(int index);
	void makeChaser(int index);

	void makeBullet(int index);
	void makeHostileBullet();

	void fireBullet();

	void newRoom(int, int);

	void makeDoor(int);

	void fireEnemyBullet(int, int);

	void applyKnockBack();

	void setupRoomEnemies(int);
	void initEnemy(int, Enemy);
	void pushBackAnimations(Enemy, int);
	void updateEnvironment();

protected:
	enum gameState {
		STARTSCREEN,
		MENU,
		TRANSITION,
		TSCREEN,
		PLAY,
		END
	};

	int stateOfGame = STARTSCREEN;

	int tSi = 0;
	std::vector<std::string>tutorialMess{ "1t.png", "2t.png", "3t.png", "4t.png", "5t.png", "6t.png", "7t.png", "8t.png" };
	std::vector<std::string>genScreens{ "BigMenu.png", "instructions.png", "Credit.png", "Transition_1.png", "Transition_2.png", "Transition_3.png", "Win.png", "gameOver.png", "aSprites/Winter/GrassSnow.png", "aSprites/Winter/GroundSnow.png", "ground.png",  "Start.png" };
	std::vector<std::string>tileSet3{"aSprites/T/F/1.png", "aSprites/T/F/2.png", "aSprites/T/F/3.png", "aSprites/T/F/4.png", "aSprites/T/F/5.png", "aSprites/T/F/6.png", "aSprites/T/F/SnowTileBackground.png", "aSprites/T/SnowTop.png",  "aSprites/T/SnowLeft.png", "aSprites/T/SnowRight.png", "aSprites/T/SnowBottom.png", "aSprites/T/SnowTopBlock.png",  "aSprites/T/SnowLeftBlock.png", "aSprites/T/SnowRightBlock.png", "aSprites/T/SnowBottomBlock.png", "aSprites/Winter/Igloo.png"};
	std::vector<std::string>tileSet2{ "aSprites/Winter/A/1/1.png", "aSprites/Winter/A/2/1.png", "aSprites/Winter/A/3/1.png", "aSprites/Winter/A/4/1.png", "aSprites/Winter/A/5/1.png", "aSprites/Winter/A/6/1.png", "aSprites/Winter/GrassSnow.png", "aSprites/Winter/F/Top_Bottom/1.png", "aSprites/Winter/F/Left/1.png", "aSprites/Winter/F/Right/1.png", "aSprites/Winter/F/Top_Bottom/1.png",  "aSprites/Winter/F/GrassTopBlock/1.png", "aSprites/Winter/F/GrassLeftBlock/1.png", "aSprites/Winter/F/GrassRightBlock/1.png", "aSprites/Winter/F/GrassBottomBlock/1.png", "aSprites/T/SnowPile.png" };
	std::vector<std::string>tileSet1{ "aSprites/TileTopLeft/1.png", "aSprites/TileTopRight/1.png", "aSprites/Left Side/1.png", "aSprites/RightSide/1.png", "aSprites/Bottom Left/1.png", "aSprites/Bottom Right/1.png", "aSprites/Winter/GroundSnow.png", "tWall.png", "lWall.png", "rWall.png", "bWall.png", "tBlocked.png", "lBlocked.png", "rBlocked.png", "bBlocked.png", "aSprites/BS/FPumpkin.png" };
	std::vector<std::string>tileSetTut{ "aSprites/TileTopLeft/1.png", "aSprites/TileTopRight/1.png", "aSprites/Left Side/1.png", "aSprites/RightSide/1.png", "aSprites/Bottom Left/1.png", "aSprites/Bottom Right/1.png", "ground.png", "tWall.png", "lWall.png", "rWall.png", "bWall.png", "tBlocked.png", "lBlocked.png", "rBlocked.png", "bBlocked.png", "aSprites/BS/SPumpkin.png" };
	std::vector<std::string>* tSet = &tileSetTut;


	PhysicsPlaygroundListener listener;
	int healthBar;
	int attackFrames = 0;
	bool needToAdd = false;

	bool hasStarted = false;
	bool hasEnded = false;

	int spawnCount = 5;
	int totalEnt = 117;
	int tempBullet;

	std::vector <int> enemies;
	std::vector<int> activeEnemies;
	std::vector<int> hostileBullets;
	std::vector <int> activeBullets{ 0 };
	std::vector <int> bulletHold{ 0 };
	std::vector <int> walls;
	std::vector<int> spikes;
	std::vector<int>allTiles;
	std::vector<int>doors;
	std::vector<int>blockedDoors;
	std::vector<int>nonDoors;
	std::vector<int>jAnimatedEntities;
	std::vector<int>frameTiles;

	int groundTile;
	int block;

	int screen;
	int nonEnemyJanims = 0;

	std::vector<std::vector<std::string>>enemyAnimations{
		{"aSprites/Bean Shooter/Bean shooter1.png"},
		{"aSprites/Corn knight/CornKnight1.png", "aSprites/Corn knight/CornKnight2.png", "aSprites/Corn knight/CornKnight3.png", "aSprites/Corn knight/CornKnight4.png", "aSprites/Corn knight/CornKnight5.png", "aSprites/Corn knight/CornKnight6.png", "aSprites/Corn knight/CornKnight7.png", "aSprites/Corn knight/CornKnight8.png", "aSprites/Corn knight/CornKnight9.png"},
		{"aSprites/wisp/WilloWisp1.png", "aSprites/wisp/WilloWisp2.png", "aSprites/wisp/WilloWisp3.png", "aSprites/wisp/WilloWisp4.png", "aSprites/wisp/WilloWisp5.png", "aSprites/wisp/WilloWisp6.png", "aSprites/wisp/WilloWisp7.png", "aSprites/wisp/WilloWisp8.png", "aSprites/wisp/WilloWisp9.png", "aSprites/wisp/WilloWisp10.png"},
		{"aSprites/Spectre/Spectre1.png", "aSprites/Spectre/Spectre2.png", "aSprites/Spectre/Spectre.png", "aSprites/Spectre/Spectre4.png"}
	};

	
};
