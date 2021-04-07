#include <iostream>
#include <fstream>
#include <time.h>
#include <vector>
#include "Dungeon.h"
#include <random>

int Dungeon::genRand(int max, int min = 0) {
	int random = min + (rand() % max);

	return random;
}

void Dungeon::setEnemyCount() {
	indexes.clear();

	for (int i = 0; i < nOfRooms; i++) {
		if (i == 4) {
			indexes.push_back(0);
		}
		else {
			indexes.push_back(3);
		}
	}

	int iterations = (4 + (rand() % 3));

	for (int i = 0; i < iterations; i++) {
		int numA = 0;

		do {
			numA = rand() % nOfRooms;
		} while (indexes[numA] < 3 || numA == 4);

		int numB = numA;

		do {
			numB = rand()% nOfRooms;
		} while (numB == numA || (numB == 4 || indexes[numB] > 5));

		indexes[numA]--;
		indexes[numB]++;

		unsuccessful = true;
	}
}

void Dungeon::populateRooms() {
	for (int i = 0; i < nOfRooms; i++) {
		currRoomEnemies.push_back(indexes[i]);
		if (indexes[i]) {
			for (int j = 0; j < indexes[i]; j++) {
				while (unsuccessful) {
					randNum = rand() % 16;
					if (j == 0) {
						currRoomEnemies.push_back(randNum);
						unsuccessful = false;
					}else {
						for (int k = 1; k <= j; k++) {
							if (currRoomEnemies[k] == randNum) {
								recurring = true;
							}
						}
						if (!recurring) {
							currRoomEnemies.push_back(randNum);
							unsuccessful = false;
						}
						else {
							recurring = false;
							unsuccessful = true;
						}
					}
				}
				unsuccessful = true;
			}
		}

		enemiesInRooms.push_back(currRoomEnemies);
		currRoomEnemies.clear();
	}
}

void Dungeon::populateDungeon() {
	setEnemyCount();
	populateRooms();
}



void Dungeon::getPosition(int x) {
	for (int i = 0; i < 6; i++) {
		for (int j = 0; j < 6; j++) {
			if (map[i][j] == x) {
				position[0] = i;
				startPos[0] = i;
				position[1] = j;
				startPos[1] = j;
			}
		}
	}
	currentRoom = x - 1;

}




void Dungeon::setDoors() {
	for (int i = 1; i < 5; i++) {
		for (int j = 1; j < 5; j++) {
			index = map[i][j];
			if (index) {
				index--;

				if (map[i-1][j]) {
					rooms[index][4] = 1;
				}

				if (map[i+1][j]) {
					rooms[index][58] = 1;
				}

				if (map[i][j-1]) {
					rooms[index][27] = 1;
				}

				if (map[i][j+1]) {
					rooms[index][35] = 1;
				}
			}
		}
	}
}

void Dungeon::generateIndexes(){
	for (int i = 0; i < nOfRooms; i++) {
		while (unsuccessful) {
			randNum = 1 + rand() % (roomPool.size() - 1);
			if (i == 0) {
				indexes.push_back(randNum);
				unsuccessful = false;
			}
			else if (i == 4) {
				indexes.push_back(0);
				unsuccessful = false;
			}
			else {
				for (int j = 0; j < i; j++) {
					if (indexes[j] == randNum) {
						recurring = true;
					}
				}
				if (!recurring) {
					indexes.push_back(randNum);
					unsuccessful = false;
				}
				else {
					recurring = false;
					unsuccessful = true;
				}
			}

		}
		unsuccessful = true;
	}
}

void Dungeon::drawRoomsFromPool() {
	generateIndexes();

	for (int i = 0; i < nOfRooms; i++) {
		index = indexes[i];
		rooms.push_back(roomPool[index]);
	}
}

void Dungeon::addToRPool() {
	for (int i = 0; i < 64; i++) {
		currentLine.push_back(readR[i] - '0');
	}
	roomPool.push_back(currentLine);
	currentLine.clear();
}

void Dungeon::getRoomPool() {
	iStream.open("./SGData/roomDatabase.txt");

	if (iStream.is_open()) {
		while (iStream.good()) {
			iStream.getline(readR, 64);
			addToRPool();
		}
		iStream.close();
	}
	else {
		std::cout << "Could not find roomDatabase.txt";
		exit(1);
	}
}


void Dungeon::resetRoom() {
	for (int i = 1; i < 6; i++) {
		for (int j = 1; j < 8; j++) {
			rArray[i][j] = 3;
		}
	}

	rArray[1][4] = 1;
	rArray[3][1] = 1;
	rArray[3][7] = 1;
	rArray[5][4] = 1;
}
void Dungeon::pushRoomVec() {
	for (int i = 0; i < 7; i++) {
		for (int j = 0; j < 9; j++) {
			rVec.push_back(rArray[i][j]);
		}
	}

	rooms.push_back(rVec);
	rVec.clear();
}
bool Dungeon::besideFloor(bool timmyCall) {
	if (timmyCall) {
		if ((rArray[y - 1][x] != 1) || (rArray[y + 1][x] != 1)) {
			return true;
		}
		else if ((rArray[y][x - 1] != 1) || (rArray[y][x + 1] != 1)) {
			return true;
		}
		else {
			return false;
		}
	}
	else {
		if ((rArray[y - 1][x] == 1) || (rArray[y + 1][x] == 1)) {
			return true;
		}
		else if ((rArray[y][x - 1] == 1) || (rArray[y][x + 1] == 1)) {
			return true;
		}
		else {
			return false;
		}
	}
}
int Dungeon::countSpaces() {
	int total = 0;
	for (int i = 0; i < 7; i++) {
		for (int j = 0; j < 9; j++) {
			if (rArray[i][j] == 1) {
				total++;
			}
		}
	}

	return total;
}
void Dungeon::verticalPath(int start[2], int dest, int pT) {
	int advance = -1;
	int diff = start[0] - dest;
	int h = start[1];

	if (diff) {
		if (diff < 0) {
			diff *= -1;
			advance *= -1;
		}

		for (int i = 0; i < diff; i++) {
			start[0] += advance;
			rArray[start[0]][h] = pT;
		}
	}
}
void Dungeon::horizontalPath(int start[2], int dest, int pT) {
	int advance = -1;
	int diff = start[1] - dest;
	int v = start[0];

	if (diff) {
		if (diff < 0) {
			diff *= -1;
			advance *= -1;
		}

		for (int i = 0; i < diff; i++) {
			start[1] += advance;
			rArray[v][start[1]] = pT;
		}
	}
}
void Dungeon::flood(int block) {
	for (int i = 0; i < 7; i++) {
		for (int j = 0; j < 9; j++) {
			if (rArray[i][j] == 3) {
				rArray[i][j] = block;
			}
		}
	}
}


void Dungeon::littleJimmy() {
	int space = 31 - countSpaces();
	space = genRand(space - 5, 1);
	std::cout << space << '\n';
	bool failed = true;

	for (int i = 0; i < space; i++) {
		while (failed) {
			std::cout << "fwip";
			y = 1 + rand()%5;
			std::cout << y;
			x = 1 + rand() % 7;
			std::cout << x;

			std::cout << rArray[y][x];
			std::cout << besideFloor(0) << '\n';

			if ((rArray[y][x] != 1)){
				if (besideFloor(0)) {
					rArray[y][x] = 1;
					failed = false;
				}
			}
		}
		failed = true;
	}
}
void Dungeon::shuffle(int array[4][2]) {
	int array2[4][2];
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 2; j++) {
			array2[i][j] = array[i][j];
		}
	}

	bool selected[4]{ false };
	for (int i = 0; i < 4; i++) {
		bool unused = false;
		int index;
		while (!unused) {
			index = genRand(4);
			if (!selected[index]) {
				selected[index] = true;
				unused = true;
				array[i][0] = array2[index][0];
				array[i][1] = array2[index][1];
			}
		}
	}

}
void Dungeon::roomDot(int zone){
	switch (zone) {
	case 1:
		y = genRand(3, 1);
		x = genRand(4, 1);
		break;
	case 2:
		y = genRand(3, 1);
		x = genRand(4, 4);
		break;
	case 3:
		y = genRand(3, 3);
		x = genRand(4, 1);
		break;
	case 4:
		y = genRand(3, 3);
		x = genRand(4, 4);
		break;
	default:
		y = genRand(5, 1);
		x = genRand(7, 1);
	}
}
void Dungeon::roomGen1() {
	int locs[4][2]{ {1, 4}, {3, 1}, {3, 7}, {5, 4} };
	shuffle(locs);
	int start[2];
	int finish[2];
	int tween[2];
	int distY, distX;
	int zone;
	int t;
	int u;
	bool order;

	for (int i = 0; i < 3; i++) {
		if (i < 2) {
			t = i + 1;
			u = i;
		}
		else {
			u = 3;
			t = genRand(3);
		}
		start[0] = locs[u][0];
		start[1] = locs[u][1];
		finish[0] = locs[t][0];
		finish[1] = locs[t][1];

		distY = finish[0] - start[0];
		distX = finish[1] - start[1];

		if (!distY || !distX) {
			zone = 0;
		}
		else {
			if (start[0] == 1 || finish[0] == 1) {
				if (finish[1] == 1 || start[1] == 1) {
					zone = 1;
				}
				else {
					zone = 2;
				}
			}
			else {
				if (finish[1] == 1 || start[1] == 1) {
					zone = 3;
				}
				else {
					zone = 4;
				}
			}

		}

		std::cout << start[0] << start[1] << '\n';
		std::cout << finish[0] << finish[1] << '\n';
		std::cout << zone << '\n';
		roomDot(zone);

		order = genRand(2);
		if (order) {
			horizontalPath(start, x, 1);
			verticalPath(start, y, 1);
		}
		else {
			verticalPath(start, y, 1);
			horizontalPath(start, x, 1);
		}

		order = genRand(2);
		if (order) {
			horizontalPath(start, finish[1], 1);
			verticalPath(start, finish[0], 1);
		}
		else {
			verticalPath(start, finish[0], 1);
			horizontalPath(start, finish[1], 1);
		}

	}

	flood(2 * genRand(2));

	if (!genRand(2)) {
		littleJimmy();
	}
}



void Dungeon::hRoom() {
	int barPos = genRand(3, 2);
	int tP = 2 * genRand(2);

	rArray[2][2] = tP;
	rArray[3][2] = tP;
	rArray[4][2] = tP;

	rArray[barPos][3] = tP;
	rArray[barPos][4] = tP;
	rArray[barPos][5] = tP;

	rArray[2][6] = tP;
	rArray[3][6] = tP;
	rArray[4][6] = tP;
}
void Dungeon::shapeRoom() {
	int shape = genRand(4);
	int tP = 2 * genRand(2);

	switch (shape) {
	case(0):
		rArray[1][1] = tP;
		rArray[1][2] = tP;
		rArray[1][6] = tP;
		rArray[1][7] = tP;
		rArray[2][2] = tP;
		rArray[2][3] = tP;
		rArray[2][5] = tP;
		rArray[2][6] = tP;
		rArray[4][2] = tP;
		rArray[4][3] = tP;
		rArray[4][5] = tP;
		rArray[4][6] = tP;
		rArray[5][1] = tP;
		rArray[5][2] = tP;
		rArray[5][6] = tP;
		rArray[5][7] = tP;
		break;
	case(1):
		rArray[1][1] = tP;
		rArray[1][7] = tP;
		rArray[2][2] = tP;
		rArray[2][4] = tP;
		rArray[2][6] = tP;
		rArray[3][4] = tP;
		rArray[4][2] = tP;
		rArray[4][4] = tP;
		rArray[4][6] = tP;
		rArray[5][1] = tP;
		rArray[5][7] = tP;
		break;
	default:
		rArray[1][1] = tP;
		rArray[1][2] = tP;
		rArray[1][6] = tP;
		rArray[1][7] = tP;
		rArray[2][1] = tP;
		rArray[2][2] = tP;
		rArray[2][6] = tP;
		rArray[2][7] = tP;
		rArray[4][1] = tP;
		rArray[4][2] = tP;
		rArray[4][6] = tP;
		rArray[4][7] = tP;
		rArray[5][1] = tP;
		rArray[5][2] = tP;
		rArray[5][6] = tP;
		rArray[5][7] = tP;
	}
}
void Dungeon::slabRoom() {
	std::cout << "slab room!\n";
	int sl1, sl2, sl3, sl4;
	int pT = 2 * genRand(2);
	int start[2];
	std::vector<int>xStart;

	sl1 = genRand(2);
	sl2 = genRand(5, 1);
	sl3 = genRand(5, 1);
	sl4 = genRand(2);

	xStart.push_back(sl1 * 4);
	xStart.push_back(sl2 - 1);
	xStart.push_back(sl3 - 1);
	xStart.push_back(sl4 * 4);

	for (int i = 1; i <= 4; i++) {
		if (i >= 3) {
			start[0] = i + 1;
		}
		else {
			start[0] = i;
		}
		start[1] = xStart[i - 1];
		horizontalPath(start, start[1] + 3, pT);
	}

}
void Dungeon::pillarRoom() {
	int colA, colB, colC;
	int startedFromTheBottom = genRand(2);
	int start[2];
	int pT = 2 * genRand(2);

	colA = genRand(2, 1);
	colB = genRand(5);
	colC = genRand((4 - colB) + 1);

	if (startedFromTheBottom) {
		start[0] = 6;
		start[1] = 1;
		verticalPath(start, 6 - colA, pT);

		start[0] = 6;
		start[1] = 2;
		verticalPath(start, 6 - colB, pT);

		start[0] = 0;
		start[1] = 3;
		verticalPath(start, colC, pT);

		start[0] = 0;
		start[1] = 5;
		verticalPath(start, colC, pT);

		start[0] = 6;
		start[1] = 6;
		verticalPath(start, 6 - colB, pT);

		start[0] = 6;
		start[1] = 7;
		verticalPath(start, 6 - colA, pT);
	}
	else {
		start[0] = 0;
		start[1] = 1;
		verticalPath(start, colA, pT);

		start[0] = 0;
		start[1] = 2;
		verticalPath(start, colB, pT);

		start[0] = 6;
		start[1] = 3;
		verticalPath(start, 6 - colC, pT);

		start[0] = 6;
		start[1] = 5;
		verticalPath(start, 6 - colC, pT);

		start[0] = 0;
		start[1] = 6;
		verticalPath(start, colB, pT);

		start[0] = 0;
		start[1] = 7;
		verticalPath(start, colA, pT);
	}
}
void Dungeon::corner(int bType, int y1, int x1, int y2, int x2, int y3, int x3) {
	int tile1 = rArray[y1][x1];
	int tile2 = rArray[y2][x2];
	int tile3 = rArray[y3][x3];
	if (tile1 == 1) {
		if (tile2 + tile3 == 2) {
			rArray[y1][x1] = bType;
			tile1 = rArray[y1][x1];
			if (tile2 == 1) {
				rArray[y2][x2] = tile1;
			}

			if (tile3 == 1) {
				rArray[y3][x3] = tile1;
			}
		}
	}

}
void Dungeon::cornerRoom() {
	int pT = genRand(2) * 2;
	corner(pT, 1, 1, 1, 2, 2, 1);
	corner(pT, 1, 7, 1, 6, 2, 7);
	corner(pT, 5, 1, 4, 1, 5, 2);
	corner(pT, 5, 7, 5, 6, 4, 7);
}
void Dungeon::littleTimmy() {
	int space = countSpaces() - 11;
	bool failed = true;
	int blocktype = 2 * genRand(2);

	space = genRand(space, 1);

	for (int i = 0; i < space; i++) {
		while (failed) {
			y = genRand(5, 1);
			x = genRand(7, 1);

			if ((x != 4 && y != 3) && (rArray[y][x] == 1 && besideFloor(1))) {
				rArray[y][x] = blocktype;
				failed = false;
			}
		}
		failed = true;
	}
}
void Dungeon::roomGen2() {
	flood(1);
	int genMethod = genRand(15);
	if (genMethod > 11) {
		slabRoom();
	}
	else if (genMethod > 8) {
		pillarRoom();
	}
	else if (genMethod > 5) {
		shapeRoom();
	}
	else if (genMethod > 3) {
		hRoom();
	}
	else if (genMethod > 1) {
		cornerRoom();

		if (!genRand(3)) {
			littleTimmy();
		}
	}
	else {
		littleTimmy();
	}
}

void Dungeon::generateRooms() {
	for (int i = 0; i < nOfRooms; i++) {
		if (i == 4) {
			flood(1);
		}
		else {
			if (genRand(5) > 2) {
				roomGen1();
			}
			else {
				roomGen2();
			}
		}

		pushRoomVec();
		resetRoom();
	}

	setDoors();
}



void Dungeon::formatMap() {
	int count = 1; 
	for (int i = 0; i < 6; i++) {
		for (int j = 0; j < 6; j++) {
			if (map[i][j]) {
				map[i][j] = count;
				count++;
			}
		}
	}
}

void Dungeon::subtractDots(int remaining) {
	y = 4;
	x = 4;
	while (remaining < 0) {
		if (map[y][x] == 1) {
			map[y][x] = 0;
			remaining++;

		}
		x--;
		if (x == 0) {
			x = 4;
			y--;
		}
	}
}

bool Dungeon::isBesideSomething() {

	if ((map[y - 1][x] == 1) || (map[y + 1][x] == 1)) {
		return true;
	}
	else if ((map[y][x - 1] == 1) || (map[y][x + 1] == 1)) {
		return true;
	}
	else {
		return false;
	}
}

void Dungeon::addDots(int remaining) {
	for (int i = 0; i < remaining; i++) {
		bool successful = false;
		while (!successful) {
			makeDot();
			if (isBesideSomething() && (!map[y][x])) {
				map[y][x] = 1;
				successful = true;
			}
		}
	}
}

int Dungeon::countRooms() {
	int count = 0;
	for (int i = 0; i < 6; i++) {

		for (int j = 0; j < 6; j++) {
			if (map[i][j]) {
				count++;
			}
		}
	}
	return count;
}

void Dungeon::connect3() {
	int locs[3][2];
	int found = 0;

	int distY, distX;

	for (int i = 1; i < 5; i++) {
		for (int j = 1; j < 5; j++) {
			if (map[i][j]) {
				locs[found][0] = i;
				locs[found][1] = j;
				found++;
			}
		}
	}

	y = locs[0][0];
	x = locs[0][1];

	distY = (locs[1][0] - locs[0][0]);
	distX = (locs[1][1] - locs[0][1]);

	if (distX < 0) {
		for (int i = 0; i > distX; i--) {
			x -= 1;
			map[y][x] = 1;
		}
	}
	else if (distX > 0) {
		for (int i = 0; i < distX; i++) {
			x += 1;
			map[y][x] = 1;
		}
	}


	if (distY > 1) {
		for (int i = 0; i < distY; i++) {
			y += 1;
			map[y][x] = 1;
		}
	}

	distY = (locs[2][0] - locs[1][0]);
	distX = (locs[2][1] - locs[1][1]);

	y = locs[1][0];
	x = locs[1][1];



	if (distX < 0) {
		for (int i = 0; i > distX; i--) {
			x -= 1;
			map[y][x] = 1;
		}
	}
	else if (distX > 0) {
		for (int i = 0; i < distX; i++) {
			x += 1;
			map[y][x] = 1;
		}
	}


	if (distY > 1) {
		for (int i = 0; i < distY; i++) {
			y += 1;
			map[y][x] = 1;
		}
	}

	
}
	

void Dungeon::makeDot() {
	y = (1 + (rand() % 4));
	x = (1 + (rand() % 4));
}

void Dungeon::tripleDot() {
	for(int i = 0; i < 3; i++) {
		bool success = false;
		while (!success) {
			makeDot();
			if (!map[y][x]) {
				map[y][x] = 1;
				success = true;
			}

		}
	}
	
}

void Dungeon::generateMap() {
	tripleDot();
	connect3();
	int remaining = nOfRooms - (countRooms());

	if (remaining > 0) {
		addDots(remaining);
	}
	else if(remaining < 0){
		subtractDots(remaining);
	}

	formatMap();

}


Dungeon::Dungeon(int l)
{
	srand(time(NULL));
	level = l;
	nOfRooms = (7 + ((l - 1) * 2) + rand() % 2);
	
	isTutorial = false;
	generateMap();
	generateRooms();
	getPosition(5);
	populateDungeon();
	std::cout << "success";
}

//tutorial overload
Dungeon::Dungeon(bool t) {
	level = 0;
	isTutorial = t;
	nOfRooms = 7;
	storeAsArray(tutorialMap);
	loadTutorialRooms();
	setDoors();
	getPosition(6);
	populateTutorial();
}

void Dungeon::loadTutorialRooms() {
	iStream.open("./SGData/tutorialRoomDatabase.txt");

	if (iStream.is_open()) {
		while (iStream.good()) {
			iStream.getline(readR, 64);
			storeRoom();
		}
		iStream.close();
	}
	else {
		std::cout << "Could not find tutorialRoomDatabase.txt";
		exit(1);
	}
	
}

void Dungeon::storeRoom() {
	for (int i = 0; i < 64; i++) {
		currentLine.push_back(readR[i] - '0');
	}
	rooms.push_back(currentLine);
	currentLine.clear(); 
	
}

void Dungeon::populateTutorial() {
	iStream.open("./SGData/tutorialEnemies.txt");
	int line;
	if (iStream.is_open()) {
		while (iStream.good()) {
			iStream >> line;
			currRoomEnemies.push_back(line);
			if (line) {
				for (int i = 0; i < currRoomEnemies[0]; i++) {
					iStream >> line;
					currRoomEnemies.push_back(line);
				}
			}

			enemiesInRooms.push_back(currRoomEnemies);
			currRoomEnemies.clear();
		}

		iStream.close();


		for (int i = 0; i < enemiesInRooms.size(); i++) {
			for (int j = 0; j < enemiesInRooms[i].size(); j++) {
				std::cout << enemiesInRooms[i][j] << ' ';
			}
			std::cout << '\n';
		}




	}
	else {
		std::cout << "Could not find tutorialEnemies.txt";
		exit(1);
	}
}

//stores tutorial map as map array
void Dungeon::storeAsArray(std::vector<int> m) {
	for (int i = 0; i < 6; i++) {
		for (int j = 0; j < 6; j++) {
			map[i][j] = m[j + (i*6)];
		}
	}
	std::cout << "done";
}

//this function deletes every data value that does not get overwritten by the constructors
void Dungeon::WipeDungeon() {
	mapCleared = false;
	enemiesInRooms.clear();
	rooms.clear();
	dungeonPool.clear();
	roomPool.clear();
	indexes.clear();
}



//this function is called when frostbite enters a room containing enemies
//it sets the locations of all room enemies, based on which door the room was entered from
//i deeply apologize to anyone who has to read this wretched function
void Dungeon::setPositions(int direction) {
	switch (direction) {
	case 0:
		for (int j = 0; j < (enemiesInRooms[currentRoom][0]); j++) {
			if (j == 0) {
				if (rooms[currentRoom][10] == 1) {
					enemiesInRooms[currentRoom].push_back(10);
				}
				else if (rooms[currentRoom][20] == 1) {
					enemiesInRooms[currentRoom].push_back(20);
				}
				else if (rooms[currentRoom][28] == 1) {
					enemiesInRooms[currentRoom].push_back(28);
				}
				else if (rooms[currentRoom][30] == 1) {
					enemiesInRooms[currentRoom].push_back(30);
				}
				else {
					enemiesInRooms[currentRoom].push_back(15);
				}
			}
			else if (j == 1) {
				if (rooms[currentRoom][46] == 1) {
					enemiesInRooms[currentRoom].push_back(46);
				}
				else if (rooms[currentRoom][38] == 1) {
					enemiesInRooms[currentRoom].push_back(38);
				}
				else if (rooms[currentRoom][29] == 1) {
					enemiesInRooms[currentRoom].push_back(29);
				}
				else if (rooms[currentRoom][31] == 1) {
					enemiesInRooms[currentRoom].push_back(31);
				}
				else {
					enemiesInRooms[currentRoom].push_back(51);
				}
			}
			else if (j == 2) {
				if (rooms[currentRoom][19] == 1) {
					enemiesInRooms[currentRoom].push_back(19);
				}
				else if (rooms[currentRoom][12] == 1) {
					enemiesInRooms[currentRoom].push_back(12);
				}
				else if (rooms[currentRoom][13] == 1) {
					enemiesInRooms[currentRoom].push_back(13);
				}
				else if (rooms[currentRoom][14] == 1) {
					enemiesInRooms[currentRoom].push_back(14);
				}
				else {
					enemiesInRooms[currentRoom].push_back(24);
				}
			}
			else if (j == 3) {
				if (rooms[currentRoom][37] == 1) {
					enemiesInRooms[currentRoom].push_back(37);
				}
				else if (rooms[currentRoom][48] == 1) {
					enemiesInRooms[currentRoom].push_back(48);
				}
				else if (rooms[currentRoom][49] == 1) {
					enemiesInRooms[currentRoom].push_back(49);
				}
				else if (rooms[currentRoom][50] == 1) {
					enemiesInRooms[currentRoom].push_back(50);
				}
				else {
					enemiesInRooms[currentRoom].push_back(42);
				}
			}
			else if (j == 4) {
				if (rooms[currentRoom][11] == 1) {
					enemiesInRooms[currentRoom].push_back(11);
				}
				else if (rooms[currentRoom][21] == 1) {
					enemiesInRooms[currentRoom].push_back(21);
				}
				else if (rooms[currentRoom][22] == 1) {
					enemiesInRooms[currentRoom].push_back(22);
				}
				else if (rooms[currentRoom][23] == 1) {
					enemiesInRooms[currentRoom].push_back(23);
				}
				else {
					enemiesInRooms[currentRoom].push_back(16);
				}
			}
			else {
				if (rooms[currentRoom][47] == 1) {
					enemiesInRooms[currentRoom].push_back(47);
				}
				else if (rooms[currentRoom][39] == 1) {
					enemiesInRooms[currentRoom].push_back(39);
				}
				else if (rooms[currentRoom][40] == 1) {
					enemiesInRooms[currentRoom].push_back(40);
				}
				else if (rooms[currentRoom][41] == 1) {
					enemiesInRooms[currentRoom].push_back(41);
				}
				else {
					enemiesInRooms[currentRoom].push_back(52);
				}
			}
		}
		break;

	case 1:
		for (int j = 0; j < (enemiesInRooms[currentRoom][0]); j++) {
			if (j == 0) {
				if (rooms[currentRoom][16] == 1) {
					enemiesInRooms[currentRoom].push_back(16);
				}
				else if (rooms[currentRoom][24] == 1) {
					enemiesInRooms[currentRoom].push_back(24);
				}
				else if (rooms[currentRoom][34] == 1) {
					enemiesInRooms[currentRoom].push_back(34);
				}
				else if (rooms[currentRoom][32] == 1) {
					enemiesInRooms[currentRoom].push_back(32);
				}
				else {
					enemiesInRooms[currentRoom].push_back(11);
				}
			}
			else if (j == 1) {
				if (rooms[currentRoom][52] == 1) {
					enemiesInRooms[currentRoom].push_back(52);
				}
				else if (rooms[currentRoom][42] == 1) {
					enemiesInRooms[currentRoom].push_back(42);
				}
				else if (rooms[currentRoom][33] == 1) {
					enemiesInRooms[currentRoom].push_back(33);
				}
				else if (rooms[currentRoom][31] == 1) {
					enemiesInRooms[currentRoom].push_back(31);
				}
				else {
					enemiesInRooms[currentRoom].push_back(47);
				}
			}
			else if (j == 2) {
				if (rooms[currentRoom][25] == 1) {
					enemiesInRooms[currentRoom].push_back(25);
				}
				else if (rooms[currentRoom][14] == 1) {
					enemiesInRooms[currentRoom].push_back(14);
				}
				else if (rooms[currentRoom][13] == 1) {
					enemiesInRooms[currentRoom].push_back(13);
				}
				else if (rooms[currentRoom][12] == 1) {
					enemiesInRooms[currentRoom].push_back(12);
				}
				else {
					enemiesInRooms[currentRoom].push_back(20);
				}
			}
			else if (j == 3) {
				if (rooms[currentRoom][43] == 1) {
					enemiesInRooms[currentRoom].push_back(43);
				}
				else if (rooms[currentRoom][50] == 1) {
					enemiesInRooms[currentRoom].push_back(50);
				}
				else if (rooms[currentRoom][49] == 1) {
					enemiesInRooms[currentRoom].push_back(49);
				}
				else if (rooms[currentRoom][48] == 1) {
					enemiesInRooms[currentRoom].push_back(48);
				}
				else {
					enemiesInRooms[currentRoom].push_back(38);
				}
			}
			else if (j == 4) {
				if (rooms[currentRoom][15] == 1) {
					enemiesInRooms[currentRoom].push_back(15);
				}
				else if (rooms[currentRoom][23] == 1) {
					enemiesInRooms[currentRoom].push_back(23);
				}
				else if (rooms[currentRoom][22] == 1) {
					enemiesInRooms[currentRoom].push_back(22);
				}
				else if (rooms[currentRoom][21] == 1) {
					enemiesInRooms[currentRoom].push_back(21);
				}
				else {
					enemiesInRooms[currentRoom].push_back(10);
				}
			}
			else {
				if (rooms[currentRoom][51] == 1) {
					enemiesInRooms[currentRoom].push_back(51);
				}
				else if (rooms[currentRoom][41] == 1) {
					enemiesInRooms[currentRoom].push_back(41);
				}
				else if (rooms[currentRoom][40] == 1) {
					enemiesInRooms[currentRoom].push_back(40);
				}
				else if (rooms[currentRoom][39] == 1) {
					enemiesInRooms[currentRoom].push_back(39);
				}
				else {
					enemiesInRooms[currentRoom].push_back(46);
				}
			}
		}
		break;
	case 2:
		for (int j = 0; j < (enemiesInRooms[currentRoom][0]); j++) {
			if (j == 0) {
				if (rooms[currentRoom][46] == 1) {
					enemiesInRooms[currentRoom].push_back(46);
				}
				else if (rooms[currentRoom][37] == 1) {
					enemiesInRooms[currentRoom].push_back(37);
				}
				else if (rooms[currentRoom][49] == 1) {
					enemiesInRooms[currentRoom].push_back(49);
				}
				else if (rooms[currentRoom][30] == 1) {
					enemiesInRooms[currentRoom].push_back(30);
				}
				else {
					enemiesInRooms[currentRoom].push_back(21);
				}
			}
			else if (j == 1) {
				if (rooms[currentRoom][52] == 1) {
					enemiesInRooms[currentRoom].push_back(52);
				}
				else if (rooms[currentRoom][43] == 1) {
					enemiesInRooms[currentRoom].push_back(43);
				}
				else if (rooms[currentRoom][40] == 1) {
					enemiesInRooms[currentRoom].push_back(40);
				}
				else if (rooms[currentRoom][32] == 1) {
					enemiesInRooms[currentRoom].push_back(32);
				}
				else {
					enemiesInRooms[currentRoom].push_back(23);
				}
			}
			else if (j == 2) {
				if (rooms[currentRoom][47] == 1) {
					enemiesInRooms[currentRoom].push_back(47);
				}
				else if (rooms[currentRoom][38] == 1) {
					enemiesInRooms[currentRoom].push_back(38);
				}
				else if (rooms[currentRoom][28] == 1) {
					enemiesInRooms[currentRoom].push_back(28);
				}
				else if (rooms[currentRoom][19] == 1) {
					enemiesInRooms[currentRoom].push_back(19);
				}
				else {
					enemiesInRooms[currentRoom].push_back(10);
				}
			}
			else if (j == 3) {
				if (rooms[currentRoom][51] == 1) {
					enemiesInRooms[currentRoom].push_back(51);
				}
				else if (rooms[currentRoom][42] == 1) {
					enemiesInRooms[currentRoom].push_back(42);
				}
				else if (rooms[currentRoom][34] == 1) {
					enemiesInRooms[currentRoom].push_back(34);
				}
				else if (rooms[currentRoom][25] == 1) {
					enemiesInRooms[currentRoom].push_back(25);
				}
				else {
					enemiesInRooms[currentRoom].push_back(16);
				}
			}
			else if (j == 4) {
				if (rooms[currentRoom][48] == 1) {
					enemiesInRooms[currentRoom].push_back(48);
				}
				else if (rooms[currentRoom][39] == 1) {
					enemiesInRooms[currentRoom].push_back(39);
				}
				else if (rooms[currentRoom][29] == 1) {
					enemiesInRooms[currentRoom].push_back(29);
				}
				else if (rooms[currentRoom][20] == 1) {
					enemiesInRooms[currentRoom].push_back(20);
				}
				else {
					enemiesInRooms[currentRoom].push_back(11);
				}
			}
			else {
				if (rooms[currentRoom][50] == 1) {
					enemiesInRooms[currentRoom].push_back(50);
				}
				else if (rooms[currentRoom][41] == 1) {
					enemiesInRooms[currentRoom].push_back(41);
				}
				else if (rooms[currentRoom][33] == 1) {
					enemiesInRooms[currentRoom].push_back(33);
				}
				else if (rooms[currentRoom][24] == 1) {
					enemiesInRooms[currentRoom].push_back(24);
				}
				else {
					enemiesInRooms[currentRoom].push_back(15);
				}
			}
		}
		break;

	default:
		for (int j = 0; j < (enemiesInRooms[currentRoom][0]); j++) {
			if (j == 0) {
				if (rooms[currentRoom][10] == 1) {
					enemiesInRooms[currentRoom].push_back(10);
				}
				else if (rooms[currentRoom][19] == 1) {
					enemiesInRooms[currentRoom].push_back(19);
				}
				else if (rooms[currentRoom][13] == 1) {
					enemiesInRooms[currentRoom].push_back(13);
				}
				else if (rooms[currentRoom][30] == 1) {
					enemiesInRooms[currentRoom].push_back(30);
				}
				else {
					enemiesInRooms[currentRoom].push_back(39);
				}
			}
			else if (j == 1) {
				if (rooms[currentRoom][16] == 1) {
					enemiesInRooms[currentRoom].push_back(16);
				}
				else if (rooms[currentRoom][25] == 1) {
					enemiesInRooms[currentRoom].push_back(25);
				}
				else if (rooms[currentRoom][22] == 1) {
					enemiesInRooms[currentRoom].push_back(22);
				}
				else if (rooms[currentRoom][32] == 1) {
					enemiesInRooms[currentRoom].push_back(32);
				}
				else {
					enemiesInRooms[currentRoom].push_back(41);
				}
			}
			else if (j == 2) {
				if (rooms[currentRoom][11] == 1) {
					enemiesInRooms[currentRoom].push_back(11);
				}
				else if (rooms[currentRoom][20] == 1) {
					enemiesInRooms[currentRoom].push_back(20);
				}
				else if (rooms[currentRoom][28] == 1) {
					enemiesInRooms[currentRoom].push_back(28);
				}
				else if (rooms[currentRoom][37] == 1) {
					enemiesInRooms[currentRoom].push_back(37);
				}
				else {
					enemiesInRooms[currentRoom].push_back(46);
				}
			}
			else if (j == 3) {
				if (rooms[currentRoom][15] == 1) {
					enemiesInRooms[currentRoom].push_back(15);
				}
				else if (rooms[currentRoom][24] == 1) {
					enemiesInRooms[currentRoom].push_back(24);
				}
				else if (rooms[currentRoom][34] == 1) {
					enemiesInRooms[currentRoom].push_back(34);
				}
				else if (rooms[currentRoom][43] == 1) {
					enemiesInRooms[currentRoom].push_back(43);
				}
				else {
					enemiesInRooms[currentRoom].push_back(52);
				}
			}
			else if (j == 4) {
				if (rooms[currentRoom][12] == 1) {
					enemiesInRooms[currentRoom].push_back(12);
				}
				else if (rooms[currentRoom][21] == 1) {
					enemiesInRooms[currentRoom].push_back(21);
				}
				else if (rooms[currentRoom][29] == 1) {
					enemiesInRooms[currentRoom].push_back(29);
				}
				else if (rooms[currentRoom][38] == 1) {
					enemiesInRooms[currentRoom].push_back(38);
				}
				else {
					enemiesInRooms[currentRoom].push_back(47);
				}
			}
			else {
				if (rooms[currentRoom][14] == 1) {
					enemiesInRooms[currentRoom].push_back(14);
				}
				else if (rooms[currentRoom][23] == 1) {
					enemiesInRooms[currentRoom].push_back(23);
				}
				else if (rooms[currentRoom][33] == 1) {
					enemiesInRooms[currentRoom].push_back(33);
				}
				else if (rooms[currentRoom][42] == 1) {
					enemiesInRooms[currentRoom].push_back(42);
				}
				else {
					enemiesInRooms[currentRoom].push_back(51);
				}
			}
		}
	}
}