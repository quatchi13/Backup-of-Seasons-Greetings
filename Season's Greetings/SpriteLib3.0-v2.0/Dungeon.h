#pragma once
#include <vector>
#include <fstream>
class Dungeon {
public:
	int map[6][6]{ {0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0} };
	int position[2];
	int startPos[2];
	int nOfRooms;
	int currentRoom;
	int level;
	bool mapCleared;
	bool isTutorial;
	std::vector<std::vector<int>>enemiesInRooms;
	std::vector<std::vector<int>>rooms;
	
	Dungeon(int);
	Dungeon(bool);
	
	void WipeDungeon();
	void setPositions(int);
	void sendToStart();

private:
	std::ifstream iStream;
	char readD[37];
	char readR[64];
	std::vector<int>currentLine;
	int index;
	bool unsuccessful = true;
	bool recurring = false;
	int randNum;
	int eCountForRoom;
	std::vector<int>currRoomEnemies;
	int remainingEnemies;

	int y, x;
	int rArray[7][9]{
		{0, 0, 0, 0, 0, 0, 0, 0, 0},
		{0, 3, 3, 3, 1, 3, 3, 3, 0},
		{0, 3, 3, 3, 3, 3, 3, 3, 0},
		{0, 1, 3, 3, 3, 3, 3, 1, 0},
		{0, 3, 3, 3, 3, 3, 3, 3, 0},
		{0, 3, 3, 3, 1, 3, 3, 3, 0},
		{0, 0, 0, 0, 0, 0, 0, 0, 0},
	};
	

	std::vector<std::vector<int>>dungeonPool;
	std::vector<std::vector<int>>roomPool;
	std::vector<int>indexes;
	std::vector<int>rVec;

	void generateIndexes();
	void getRoomPool();

	void generateMap();
	void tripleDot();
	void makeDot();
	void connect3();
	int countRooms();
	void addDots(int);
	void subtractDots(int);
	bool isBesideSomething();
	void formatMap();

	void selectMap();
	void getPosition(int);
	void generateRooms();

	void addToDPool();
	void addToRPool();
	void drawMapFromPool();
	void drawRoomsFromPool();
	void storeAsArray(std::vector<int>);
	void setDoors();

	void populateDungeon();
	void populateRooms();
	void setEnemyCount();


	//tutorial initialization
	std::vector<int>tutorialMap{ 0, 0, 0, 0, 0, 0, 0, 1, 2, 3, 0, 0, 0, 0, 0, 4, 0, 0, 0, 0, 0, 5, 0, 0, 0, 0, 6 , 7, 0, 0, 0, 0, 0 ,0 ,0 ,0 };
	
	void loadTutorialRooms();
	void storeRoom();
	void populateTutorial();

	void flood(int);
	void roomDot(int);
	void horizontalPath(int[2], int, int);
	void verticalPath(int[2], int, int);
	void shuffle(int[4][2]);
	void roomGen1();
	void littleJimmy();
	bool besideFloor(bool);
	int countSpaces();

	void hRoom();
	void slabRoom();
	void shapeRoom();
	void pillarRoom();
	void cornerRoom();
	void corner(int, int, int, int, int, int, int);
	void littleTimmy();
	void roomGen2();
	void pushRoomVec();
	void resetRoom();
	int genRand(int, int);
	
	
};