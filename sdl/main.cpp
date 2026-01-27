#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <cctype>
#include <limits>
#include "Utils.h"
#include "ConsoleControl.h"

//#define MAP_SIZE_W 10
//#define MAP_SIZE_H 10

int MAP_SIZE_W = 0;
int MAP_SIZE_H = 0;

enum Direction {NORTH, SOUTH, EAST, WEST, NO_DIRECTION};

struct Player {
	std::string name;
	int posX = -1; //Horizontal
	int posY = -1; //Vertical (negativo arriba, positivo, abajo)
	int maxHealth = 100;
	int currentHealth = 100;
	int potions = 0;
	int hasSword = true;
	int hasBomb = true;
	int hasKey = true;
};

//TODO: Implementar el struct Enemy que sirva para todos los otros

struct Enemy {
	std::string name = " ";
	int maxHealth = 0;
	int currentHealth = 0;
};

struct Goblin {
	std::string name = "Goblin";
	int maxHealth = 30;
	int currentHealth = 30;
};

struct Orco {
	std::string name = "Orco";
	int maxHealth = 60;
	int currentHealth = 60;
};

struct Troll {
	std::string name = "Troll";
	int maxHealth = 80;
	int currentHealth = 80;
};

struct Boss {
	std::string name = "Boss";
	int maxHealth = 120;
	int currentHealth = 120;
};

//Variables (de momento publicas, lo ideal son punteros)

Player playerInfo;

bool in_combat = false;

bool pedirInput = false;



//TODO: Hacer los structs necesarios para guardar otra info

std::vector <std::vector <char>> map;

bool loadMap() {
	int height = 0;
	std::fstream mapa("savegame.txt");
	mapa.open("map.txt", std::ios::in);
	std::string line;
	std::getline(mapa, line);
	height = std::stoi(line);
	map.resize(height);
	for (int i = 0; i < height; i++) {
		if (mapa.eof()) { break; }
		std::getline(mapa, line);
		int width = line.size();
		map[i].resize(width);
		for (int j = 0; j < width; j++) {
			map[i][j] = line[j];
		}
	}

	MAP_SIZE_H = map.size();
	MAP_SIZE_W = map[0].size();

	return 1;
}


bool saveGame(const Player& player) {

	std::string fileName = "save_" + player.name + ".txt";

	std::ofstream archivoGuardado(fileName);

	if (!archivoGuardado.is_open()) {
	std::cout << "Error el guardar\n";
	return false;
	}

	archivoGuardado << player.name << std::endl;
	archivoGuardado << player.posX << std::endl;
	archivoGuardado << player.posY << std::endl;
	archivoGuardado << player.maxHealth << std::endl;
	archivoGuardado << player.currentHealth << std::endl;
	archivoGuardado << player.potions << std::endl;
	archivoGuardado << player.hasSword << std::endl;
	archivoGuardado << player.hasBomb << std::endl;
	archivoGuardado << player.hasKey << std::endl;

	archivoGuardado.close();
	return true;

}

//bool saveGame(const Player& player) {
//	std::ofstream archivoGuardado("savegame.txt");
//
//	if (!archivoGuardado.is_open()) {
//		std::cout << "Error el guardar\n";
//		return false;
//	}
//
//	archivoGuardado << player.name << std::endl;
//	archivoGuardado << player.posX << std::endl;
//	archivoGuardado << player.posY << std::endl;
//	archivoGuardado << player.maxHealth << std::endl;
//	archivoGuardado << player.currentHealth << std::endl;
//	archivoGuardado << player.potions << std::endl;
//	archivoGuardado << player.hasSword << std::endl;
//	archivoGuardado << player.hasBomb << std::endl;
//	archivoGuardado << player.hasKey << std::endl;
//
//	archivoGuardado.close();
//	return true;
//
//}

void printMap() {
	for (int i = 0; i < MAP_SIZE_H; i++) {//Primero las filas (Y)
		for (int j = 0; j < MAP_SIZE_W; j++) {
			char c = map[i][j];
			char draw = c;
			switch (c) {
				case '#':
					ConsoleSetColor(ConsoleColor::LIGHTGREY, ConsoleColor::LIGHTGREY);
					break;
				case '.':
					ConsoleSetColor(ConsoleColor::BLACK, ConsoleColor::BLACK);
					break;
				case 'L':
					draw = '#';
					ConsoleSetColor(ConsoleColor::DARKGREY, ConsoleColor::BLACK);
					break;
				case 'P':
					ConsoleSetColor(ConsoleColor::GREEN, ConsoleColor::BLACK);
					break;
				default:
					ConsoleSetColor(ConsoleColor::WHITE, ConsoleColor::BLACK);
					break;
			}
			std::cout << draw;
		}
		std::cout << std::endl;
	}
	ConsoleSetColor(ConsoleColor::WHITE, ConsoleColor::BLACK);
}

void movePlayer(Direction dir) {

	int nextPosX = playerInfo.posX;
	int nextPosY = playerInfo.posY;

	switch (dir) {
		case Direction::NORTH:
			nextPosY--;
			break;
		case Direction::SOUTH:
			nextPosY++;
			break;
		case Direction::EAST:
			nextPosX++;
			break;
		case Direction::WEST:
			nextPosX--;
			break;
		default: break;
	}

	//Pedir input al jugador

	if (nextPosX != playerInfo.posX || nextPosY != playerInfo.posY) {//Si hay un intento de movimiento
		if (nextPosX >= 0 && nextPosX < MAP_SIZE_W && nextPosY >= 0 && nextPosY < MAP_SIZE_H && map[nextPosY][nextPosX] != '#') {//Si no hay una pared
			//Como c++ es "lazy", si las comprobaciones de nextPosX e Y fallan, no se mirar�n las posiciones en el map, pero no todos los lenguajes de programaci�n son as�
			playerInfo.posX = nextPosX;
			playerInfo.posY = nextPosY;
				
			if (map[playerInfo.posY][playerInfo.posX] == 'G') {//Si hay un enemigo

				in_combat = true;
				pedirInput = true;
				std::cout << "Enemigo! (debug)";
				//currentBattle = 'G'; //TODO: La funcionalidad de lo que pasa al matar a un enemigo

			}
		}

		//Mirar qué pasa en la siguiente celda
	}
}


void printEntities() {
	ConsoleSetColor(ConsoleColor::DARKYELLOW, ConsoleColor::BLACK);
	ConsoleXY(playerInfo.posX, playerInfo.posY);
	std::cout << "@";

	ConsoleSetColor(ConsoleColor::WHITE, ConsoleColor::BLACK);
	ConsoleXY(0, MAP_SIZE_H + 1);
}

int mainMenu() {

	int option = 0;

	while (true) {

		system("cls");

		std::cout << "    =====================\n";
		std::cout << "    >>>> HERO'S QUEST <<<<\n";
		std::cout << "    =====================\n\n";

		std::cout << "1. PLAY\n";
		std::cout << "2. LOAD\n";
		std::cout << "0. EXIT\n\n";

		std::cout << "> ";
		std::cin >> option;

		if (option >= 0 && option <= 2) {
			return option;
		}

		std::cout << "Opcion invalida...\n";
		system("pause");
	}
}


int main() {

    int option = mainMenu();

	std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); //Esto es para limpiar el cin

    if (option == 0) {
        return 0;
    }

    if (option == 2) {
        std::cout << "LOAD aun no implementado\n";
        system("pause");
        return 0;
    }

    if (!loadMap()) {
        return 1;
    }

	
	bool game_end = false;
	bool attack_end = false;

	int bossPos[2];

	std::string input;

	
	Goblin goblinInfo;
	Orco orcoInfo;
	Troll trollInfo;
	Boss bossInfo;

	char currentBattle = '.'; //Para saber con que tipo de enemigo estamos combatiendo

	bool playerFound = false;

	//Recorro todas las celdas del mapa
	for (int i = 0; i < MAP_SIZE_H; i++) {//Primero las filas (Y)
		for (int j = 0; j < MAP_SIZE_W; j++) {//Por cada fila, las columnas (X)
			if (map[i][j] == '@') {//Encuentro el player
				playerInfo.posX = j;
				playerInfo.posY = i;
				map[i][j] = '.';
				//playerFound = true; //Hacemos esto para luego poder romper el bucle i
			}
			if (map[i][j] == 'J') {//Encuentro el boss
				bossPos[0] = i;
				bossPos[1] = j;
			}
		}
	}

	//TODO: Hacer una intro, pedir el nombre al jugador, etc.
	std::cout << "HERO'S QUEST\nHi, hero, what's your name?" << std::endl;
	std::cout << "> ";
	std::cin >> playerInfo.name;
	//std::getline(std::cin, playerInfo.name);


	while (!game_end) {
		if (!in_combat) {
			int key = ConsoleInKey();
			if (key != 0) {
				switch (key) {
				case 75:
					movePlayer(Direction::WEST);
					break;
				case 77:
					movePlayer(Direction::EAST);
					break;
				case 72:
					movePlayer(Direction::NORTH);
					break;
				case 80:
					movePlayer(Direction::SOUTH);
					break;

				}
				system("cls");
				printMap();
				printEntities();
				std::cout << "Player at [" + itos(playerInfo.posX, 1) + ", " + itos(playerInfo.posY, 1) + "]" << std::endl;
			}
		}

		// MODO COMBATE

		else {

			system("cls");

			std::cout << "===== COMBATE =====\n\n";

			std::cout << playerInfo.name
				<< " [ " << playerInfo.currentHealth
				<< " / " << playerInfo.maxHealth << " ]\n";

			std::cout << "VS\n";

			std::cout << goblinInfo.name
				<< " [ " << goblinInfo.currentHealth
				<< " / " << goblinInfo.maxHealth << " ]\n\n";

			std::cout << "Comandos: attack | use potion | status | help\n";
			std::cout << "> ";

			// Leer input

			std::getline(std::cin, input);

			// Convertir a minúsculas

			for (char& c : input) {
				c = std::tolower(c);
			}

			// Separar palabras

			std::vector<std::string> words = splitString(input, ' ');

			if (words.size() == 0) continue;

			std::string command = words[0];
			std::string target = (words.size() > 1) ? words[1] : "";

			// Comando attack

			if (command == "attack") {

				int damage = 10;

				if (playerInfo.hasSword) {
					damage += 5;
				}

				goblinInfo.currentHealth -= damage;

				std::cout << "\nYour attack does "
					<< damage << " damage!\n";
			}

			// Comando use

			else if (command == "use" && target == "potion") {

				if (playerInfo.potions > 0) {

					playerInfo.currentHealth += 40;

					if (playerInfo.currentHealth > playerInfo.maxHealth)
						playerInfo.currentHealth = playerInfo.maxHealth;

					playerInfo.potions--;

					std::cout << "\nYou used a potion!\n";
				}
				else {
					std::cout << "\nNo potions!\n";
				}
			}

			else if (command == "use" && target == "bomb") {

				if (playerInfo.hasBomb) {

					goblinInfo.currentHealth -= 50;
					playerInfo.hasBomb = false;

					std::cout << "\nYou throw a bomb!\n";
				}
				else {
					std::cout << "\nNo bombs!\n";
				}
			}

			else if (command == "save") {
				if (saveGame(playerInfo)) {
					std::cout << "\nGame saved!\n";
				}
				else {
					std::cout << "\nError.\n";
				}
			}

			// Comando status

			else if (command == "status") {

				std::cout << "\n--- STATUS ---\n";
				std::cout << "Health: " << playerInfo.currentHealth << "\n";
				std::cout << "Potions: " << playerInfo.potions << "\n";
			}

			else if (command == "save") {
				if (saveGame(playerInfo)) {
					std::cout << "\nGame saved!\n";
				}
			}

			else if (command == "help") {

				std::cout << "\nComands:\n";
				std::cout << "attack\n";
				std::cout << "use potion\n";
				std::cout << "use bomb\n";
				std::cout << "status\n";
			}

			else {

				std::cout << "\nComando inválido\n";
			}

			// Turno enemigo

			if (goblinInfo.currentHealth > 0) {

				int enemyDamage = 8;

				playerInfo.currentHealth -= enemyDamage;

				std::cout << "\nThe enemies does "
					<< enemyDamage << " damage!\n";
			}

			// Comprovar muerte

			if (goblinInfo.currentHealth <= 0) {

				std::cout << "\nYou won the combat!\n";

				in_combat = false;
				pedirInput = false;

				map[playerInfo.posY][playerInfo.posX] = '.';
			}

			if (playerInfo.currentHealth <= 0) {

				std::cout << "\nYOU DIED!\n";
				game_end = true;
			}

			system("pause");
		}


#pragma region input

		/*std::cout << "What are you going to do?" << std::endl;
		std::cout << "> ";*/
		if (pedirInput) {
			std::getline(std::cin, input); //esto se hace as� si queremos usar espacios en el input
			for (int i = 0; i < input.length(); i++) {
				input[i] = std::tolower(input[i]); //Convertimos todo a min�sculas
			}

			std::vector<std::string> input_split = splitString(input, ' '); //Separamos las palabras

			//Metemos el comando y el target en unas variables separadas para que sea m�s c�modo
			int num_words = input_split.size();
			if (num_words <= 0) {
				continue; //Me salto lo que queda de bucle, y hago ya la siguiente iteraci�n
			}
			std::string command = "null";
			std::string target = "null";
			if (num_words > 0) {
				command = input_split[0];
			}
			if (num_words > 1) {
				target = input_split[1];
			}

			if (!in_combat && command == "pick") {

			}
			else if (in_combat && command == "use potion") {
				if (playerInfo.potions > 0) {
					playerInfo.currentHealth = playerInfo.currentHealth + 40;
					playerInfo.potions--;
				}
			}

			else if (in_combat && command == "attack") {
				goblinInfo.currentHealth = goblinInfo.currentHealth - 10;

			}

			else if (in_combat && command == "use sword") {
				if (playerInfo.hasSword) {
					goblinInfo.currentHealth = goblinInfo.currentHealth - 20;
				}
			}
			else if (in_combat && command == "use bomb") {
				if (playerInfo.hasBomb) {
					goblinInfo.currentHealth = goblinInfo.currentHealth - 100;
				}
			}
			if (command == "status") {
				std::cout << "\n\n>> STATS: <<\n\n";
				std::cout << "Health: [ " << playerInfo.currentHealth << " / " << playerInfo.maxHealth << " ]\n";
				std::cout << "Inventory:\n";
				if (playerInfo.hasSword) {
					std::cout << "Sword\n";
				}
				else {
					std::cout << "No sword\n";
				}
				if (playerInfo.hasSword) {
					std::cout << "Bomb\n";
				}
				else {
					std::cout << "No bomb\n";
				}
				if (playerInfo.hasKey) {
					std::cout << "Key\n";
				}
				else {
					std::cout << "No key\n";
				}
				std::cout << "Potions: " << playerInfo.potions << std::endl;
			}
			else if (command == "help") { //Comando para la ayuda
				system("cls");
				std::cout << "------" << std::endl;
				std::cout << " HELP" << std::endl;
				std::cout << "------" << std::endl;
				std::cout << "go + [NORTH][SOUTH][WEST][EAST]" << std::endl;
				std::cout << "pick + [POTION][KEY][BOMB][SWORD]" << std::endl;
				std::cout << "status" << std::endl;
				std::cout << "------" << std::endl;
				std::cout << "ONLY IN COMBAT:" << std::endl;
				std::cout << "------" << std::endl;
				std::cout << "attack" << std::endl;
				std::cout << "use + [POTION][KEY][BOMB][SWORD]" << std::endl;
				std::cout << "status" << std::endl;
				std::cout << "------" << std::endl;
			}
			else {
				std::cout << "Not a valid action" << std::endl;
			}

			system("pause");
			system("cls");
		}
#pragma endregion
	}

}