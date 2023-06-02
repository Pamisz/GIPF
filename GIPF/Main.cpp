#include <iostream>
#include <string>
#include <cstdio>

using namespace std;

struct game {
	int size = 0, pawns = 0, whitePawns = 0, blackPawns = 0, whiteReserve = 0, blackReserve = 0, maxWidth = 0, maxHeight = 0, badRows = 0;
	char currentPlayer;
	char** array = nullptr;
	bool goodBoard = false;
};

void drawBoard(game g) {
	if (g.goodBoard) {
		cout << g.size << " " << g.pawns << " " << g.whitePawns << " " << g.blackPawns << endl;
		cout << g.whiteReserve << " " << g.blackReserve << " " << g.currentPlayer << endl;
		for (int i = 0; i < g.maxHeight; i++) {
			for (int j = 0; j < g.maxWidth; j++) {
				cout << g.array[i][j];
			}
			cout << endl;
		}
	}
	else {
		cout << "EMPTY_BOARD" << endl;
	}
}

void deleteBoard(int const maxHeight, char** array) {
	for (int i = 0; i < maxHeight; i++) {
		delete[] array[i];
	}
	delete[] array;
}

bool isInsideBoard(game g, int y, int x) {
	if (x < g.maxWidth && x >= 0 && y < g.maxHeight && y >= 0) {
		return true;
	}
	return false;
}

struct orientation {
	bool left = false, right = false, horizz = false;
};

void checkConnections(game &g) {
	int meter = 0;
	orientation** visited = new orientation* [g.maxHeight];
	for (int i = 0; i < g.maxHeight; i++) {
		visited[i] = new orientation[g.maxWidth];
	}

	for (int i = 0; i < g.maxHeight; i++) {
		for (int j = 0; j < g.maxWidth; j++) {
			if (g.array[i][j] != ' ' && g.array[i][j] != '_') {
				meter=1;
				char ch = g.array[i][j];

				int x = j+1, y = i+1;
				while (isInsideBoard(g, y, x) && g.array[y][x] == ch && !visited[y][x].right) {
					visited[y][x].right = true;
					meter++;
					if (meter == 4) {
						g.badRows++;
					}
					y++;
					x++;
				}

				y = i + 1;
				x = j - 1;
				meter = 1;
				while (isInsideBoard(g, y, x) && g.array[y][x] == ch && !visited[y][x].left) {
					visited[y][x].left = true;
					meter++;
					if (meter == 4) {
						g.badRows++;
					}
					y++;
					x--;
				}

				y = i;
				x = j + 2;
				meter = 1;
				while (isInsideBoard(g, y, x) && g.array[y][x] == ch && !visited[y][x].horizz) {
					visited[y][x].horizz = true;
					meter++;
					if (meter == 4) {
						g.badRows++;
					}
					x+=2;
				}
			}
		}
	}
	for (int i = 0; i < g.maxHeight; i++) {
		delete[] visited[i];
	}
	delete[] visited;
}

void loadBoard(game &g) {
	int x = 0, y = 0;
	int w = 0, b = 0, o = 0, buff = 0;
	bool badRow = false;
	cin >> g.size >> g.pawns >> g.whitePawns >> g.blackPawns;
	cin >> g.whiteReserve >> g.blackReserve >> g.currentPlayer;
	g.maxWidth = (2 * g.size - 1) + (2 * g.size - 2);
	g.maxHeight = (2 * g.size - 1);

	g.array = new char* [g.maxHeight];
	for (int i = 0; i < g.maxHeight; i++) {
		g.array[i] = new char[g.maxWidth];
	}

	for (int i = 0; i < g.maxHeight; i++) {
		for (int j = 0; j < g.maxWidth; j++) {
			g.array[i][j] = ' ';
		}
	}

	char ch = getchar();
	while (ch == '\n') {
		ch = getchar();
	}
	while (!(y == g.maxHeight - 1 && ch == '\n')) {
		if (ch == '\n') {
			ch = getchar();
			y++;
			x = 0;
		}
		if (y < g.maxHeight && x < g.maxWidth) {
			g.array[y][x] = ch;
		}
		else if (x > g.maxWidth) {
			badRow = true;
		}
		x++;
		ch = getchar();
		if (ch == EOF) {
			break;
		}
	}

	for (int i = 0; i < g.maxHeight; i++) {
		for (int j = 0; j < g.maxWidth; j++) {
			if (g.array[i][j] == 'W') {
				w++;
			}
			if (g.array[i][j] == 'B') {
				b++;
			}
			if (g.array[i][j] != ' ') {
				o++;
			}

		}
		if (i < g.size && o != i + g.size) {
			badRow = true;
		}
		else if (i >= g.size) {
			buff++;
			if (o != (2 * g.size - 1) - buff) {
				badRow = true;
			}
		}
		o = 0;
	}

	checkConnections(g);

	if (w + g.whiteReserve > g.whitePawns) {
		cout << "WRONG_WHITE_PAWNS_NUMBER" << endl;
	}
	else if (b + g.blackReserve > g.blackPawns) {
		cout << "WRONG_BLACK_PAWNS_NUMBER" << endl;
	}
	else if (badRow == true) {
		cout << "WRONG_BOARD_ROW_LENGTH" << endl;
	}
	else if (g.badRows > 0) {
		if (g.badRows == 1) {
			cout << "ERROR_FOUND_1_ROW_OF_LENGTH_K" << endl;
		}
		else {
			cout << "ERROR_FOUND_" << g.badRows << "_ROWS_OF_LENGTH_K" << endl;
		}
		g.badRows = 0;
	}
	else {
		cout << "BOARD_STATE_OK" << endl;
		g.goodBoard = true;
	}
}

int main() {
	game letzgo;
	string buffer = "";
	while (getline(cin, buffer)) {
		if (buffer == "LOAD_GAME_BOARD") {
			loadBoard(letzgo);
		}
		else if (buffer == "PRINT_GAME_BOARD") {
			drawBoard(letzgo);
			letzgo.goodBoard = false;
		}
		
	}
	deleteBoard(letzgo.maxHeight , letzgo.array);
	return 0;
}