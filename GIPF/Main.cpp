#include <iostream>
#include <string>
#include <cstdio>

using namespace std;

struct field {
	string index;
	char czar;
};

struct game {
	int size = 0, pawns = 0, whitePawns = 0, blackPawns = 0, whiteReserve = 0, blackReserve = 0, maxWidth = 0, maxHeight = 0, badRows = 0, ibMaxHeight = 0, ibMaxWidth=0;
	char currentPlayer;
	char** array = nullptr;
	field** IB = nullptr;
	bool goodBoard = false;
};

char intToChar(int x) {
	return static_cast<char>(x);
}

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

bool isInsideBoard(const int maxWidth, const int maxHeight, int x, int y) {
	if (x < maxWidth && x >= 0 && y < maxHeight && y >= 0) {
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
				while (isInsideBoard(g.maxWidth, g.maxHeight, x, y) && g.array[y][x] == ch && !visited[y][x].right) {
					visited[y][x].right = true;
					meter++;
					if (meter == g.pawns) {
						g.badRows++;
					}
					y++;
					x++;
				}

				y = i + 1;
				x = j - 1;
				meter = 1;
				while (isInsideBoard(g.maxWidth, g.maxHeight, x, y) && g.array[y][x] == ch && !visited[y][x].left) {
					visited[y][x].left = true;
					meter++;
					if (meter == g.pawns) {
						g.badRows++;
					}
					y++;
					x--;
				}

				y = i;
				x = j + 2;
				meter = 1;
				while (isInsideBoard(g.maxWidth, g.maxHeight, x, y) && g.array[y][x] == ch && !visited[y][x].horizz) {
					visited[y][x].horizz = true;
					meter++;
					if (meter == g.pawns) {
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

void indexBoard(game& g) {
	int meter = 0, x = g.size, y = 0;
	g.ibMaxHeight = g.maxHeight + 2;
	g.ibMaxWidth = g.maxWidth + 4;
	char letter = 'a';
	g.IB = new field * [g.ibMaxHeight];
	for (int i = 0; i < g.ibMaxHeight; i++) {
		g.IB[i] = new field[g.ibMaxWidth];
	}

	for (int i = 0; i < g.ibMaxHeight; i++) {
		for (int j = 0; j < g.ibMaxWidth; j++) {
			g.IB[i][j].czar = ' ';
		}
	}
	for (int i = 1; i < (g.ibMaxHeight - 1); i++) {
		for (int j = 2; j < (g.ibMaxWidth - 2); j++) {
			g.IB[i][j].czar = g.array[i - 1][j - 2];
		}
	}
	while (x >= g.size && x <= 3 * g.size) {
		g.IB[y][x].czar = '+';
		x += 2;
		if (x > 3 * g.size && y != g.ibMaxHeight - 1) {
			
			y = g.ibMaxHeight - 1;
			x = g.size;
		}
	}
	x = g.size;
	while (y != g.size) {
		y--;
		x--;
		if (isInsideBoard(g.ibMaxWidth, g.ibMaxHeight, x, y)) {
			g.IB[y][x].czar = '+';
		}
	}
	while (y > 0) {
		y--;
		x++;
		if (isInsideBoard(g.ibMaxWidth, g.ibMaxHeight, x, y)) {
			g.IB[y][x].czar = '+';
		}
	}
	x = 3 * g.size;
	while (y != g.size) {
		y++;
		x++;
		if (isInsideBoard(g.ibMaxWidth, g.ibMaxHeight, x, y)) {
			g.IB[y][x].czar = '+';
		}
	}
	while (y < g.ibMaxHeight - 1) {
		y++;
		x--;
		if (isInsideBoard(g.ibMaxWidth, g.ibMaxHeight, x, y)) {
			g.IB[y][x].czar = '+';
		}
	}


	bool** wasted = new bool* [g.ibMaxHeight];
	for (int i = 0; i < g.ibMaxHeight; i++) {
		wasted[i] = new bool[g.ibMaxWidth];
	}
	for (int i = 0; i < g.ibMaxHeight; i++) {
		for (int j = 0; j < g.ibMaxWidth; j++) {
			wasted[i][j] = false;
		}
	}

	for (int i = 0; i < g.ibMaxHeight; i++) {
		for (int j = 0; j < g.ibMaxWidth; j++) {
			if ((g.IB[i][j]).czar != ' ' && !wasted[i][j]) {
				meter = 1;
				x = j - 1;
				y = i + 1;
				while (isInsideBoard(g.ibMaxWidth, g.ibMaxHeight, x, y) && (g.IB[y][x]).czar != ' ') {
					meter++;
					y++;
					x--;
				}
				y--;
				x++;
				for (int k = 1; k <= meter; k++) {
					wasted[y][x] = true;
					(g.IB[y][x]).index = letter + to_string(k);
					y--;
					x++;
				}
				letter++;
			}
		}
	}

	for (int i = 0; i < g.ibMaxHeight; i++) {
		for (int j = 0; j < g.ibMaxWidth; j++) {
			if ((g.IB[i][j]).index.size() == 0) {
				(g.IB[i][j]).index = " ";
			}
		}
	}

	for (int i = 0; i < g.ibMaxHeight; i++) {
		delete[] wasted[i];
	}
	delete[] wasted;
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
		indexBoard(g);
	}
}

bool movingVertical(field* f, field*t, game &g, const int i, const int j) {
	int x = 0, y = 0, meter = 0;
	if (f->index[1] == '1' && t->index[1] == '2') {
		if (t->czar == '_') {
			t->czar = g.currentPlayer;
		}
		else {
			meter = 1;
			x = j + 1;
			y = i - 1;
			while (g.IB[y][x].czar != '_' && g.IB[y][x].czar != '+') {
				meter++;
				x++;
				y--;
			}
			if (g.IB[y][x].czar == '+') {
				cout << "BAD_MOVE_ROW_IS_FULL" << endl;
				return false;
			}
			else {
				for (int k = 0; k < meter; k++) {
					g.IB[y][x].czar = g.IB[y + 1][x - 1].czar;
					x--;
					y++;
				}
				g.IB[y][x].czar = g.currentPlayer;
			}
		}
	}
	else if (t->index[1] == intToChar(f->index[1] - 1)) {
		if (t->czar == '_') {
			t->czar = g.currentPlayer;
		}
		else {
			meter = 1;
			x = j - 1;
			y = i + 1;
			while (g.IB[y][x].czar != '_' && g.IB[y][x].czar != '+') {
				meter++;
				x--;
				y++;
			}
			if (g.IB[y][x].czar == '+') {
				cout << "BAD_MOVE_ROW_IS_FULL" << endl;
				return false;
			}
			else {
				for (int k = 0; k < meter; k++) {
					g.IB[y][x].czar = g.IB[y - 1][x + 1].czar;
					x++;
					y--;
				}
				g.IB[y][x].czar = g.currentPlayer;
			}
		}
	}
	return true;
}

bool movingHorizontal(char c, field*t, game &g, const int i, const int j ) {
	int x = j, y = i, meter = 0;
	if (c == '+') {
		if (t->czar == '_') {
			t->czar = g.currentPlayer;
		}
		else {
			meter = 1;
			x = j + 2;
			while (g.IB[y][x].czar != '_' && g.IB[y][x].czar != '+') {
				meter++;
				x += 2;
			}
			if (g.IB[y][x].czar == '+') {
				cout << "BAD_MOVE_ROW_IS_FULL" << endl;
				return false;
			}
			else {
				for (int k = 0; k < meter; k++) {
					g.IB[y][x].czar = g.IB[y][x - 2].czar;
					x -= 2;
				}
				g.IB[y][x].czar = g.currentPlayer;
			}

		}
	}
	else if (c == '-') {
		meter = 1;
		x = j - 2;
		while (g.IB[y][x].czar != '_' && g.IB[y][x].czar != '+') {
			meter++;
			x -= 2;
		}
		if (g.IB[y][x].czar == '+') {
			cout << "BAD_MOVE_ROW_IS_FULL" << endl;
			return false;
		}
		else {
			for (int k = 0; k < meter; k++) {
				g.IB[y][x].czar = g.IB[y][x + 2].czar;
				x += 2;
			}
			g.IB[y][x].czar = g.currentPlayer;
		}
	}
	return true;
}

bool diagonalMove(char c, field* t, game& g, const int i, const int j) {
	int x = j, y = i, meter = 0;
	if (c == '+') {
		meter = 1;
		x = j + 1;
		y = i + 1;
		while (g.IB[y][x].czar != '_' && g.IB[y][x].czar != '+') {
			meter++;
			x++;
			y++;
		}
		if (g.IB[y][x].czar == '+') {
			cout << "BAD_MOVE_ROW_IS_FULL" << endl;
			return false;
		}
		else {
			for (int k = 0; k < meter; k++) {
				g.IB[y][x].czar = g.IB[y - 1][x - 1].czar;
				x--;
				y--;
			}
			g.IB[y][x].czar = g.currentPlayer;
		}
	}
	else if (c == '-') {
		meter = 1;
		x = j - 1;
		y = i - 1;
		while (g.IB[y][x].czar != '_' && g.IB[y][x].czar != '+') {
			meter++;
			x--;
			y--;
		}
		if (g.IB[y][x].czar == '+') {
			cout << "BAD_MOVE_ROW_IS_FULL" << endl;
			return false;
		}
		else {
			for (int k = 0; k < meter; k++) {
				g.IB[y][x].czar = g.IB[y + 1][x + 1].czar;
				x++;
				y++;
			}
			g.IB[y][x].czar = g.currentPlayer;
		}
	}
	return true;
}

void beating(game &g) {
	int meter = 0;

	for (int i = 0; i < g.ibMaxHeight; i++) {
		for (int j = 0; j < g.ibMaxWidth; j++) {
			if (g.IB[i][j].czar == 'W' || g.IB[i][j].czar == 'B') {
				meter = 1;
				char cz = g.IB[i][j].czar;

				int x = j + 1, y = i + 1;
				while (isInsideBoard(g.ibMaxWidth, g.ibMaxHeight, x, y) && (cz==g.IB[y][x].czar || (meter>=g.pawns && (g.IB[y][x].czar == 'W' || g.IB[y][x].czar == 'B')))) {
					meter++;
					y++;
					x++;
				}
				if (meter >= g.pawns) {
					y = i;
					x = j;
					for (int k = 0; k < meter; k++) {
						if (g.IB[y][x].czar == 'W' && cz == 'W') {
							g.whiteReserve++;
						}
						if (g.IB[y][x].czar == 'B' && cz == 'B') {
							g.blackReserve++;
						}
						g.IB[y][x].czar = '_';
						y++;
						x++;
					}
				}

				y = i + 1;
				x = j - 1;
				meter = 1;
				while (isInsideBoard(g.ibMaxWidth, g.ibMaxHeight, x, y) && (cz == g.IB[y][x].czar || (meter >= g.pawns && (g.IB[y][x].czar == 'W' || g.IB[y][x].czar == 'B')))) {
					meter++;
					y++;
					x--;
				}
				if (meter >= g.pawns) {
					y = i;
					x = j;
					for (int k = 0; k < meter; k++) {
						if (g.IB[y][x].czar == 'W' && cz == 'W') {
							g.whiteReserve++;
						}
						if (g.IB[y][x].czar == 'B' && cz == 'B') {
							g.blackReserve++;
						}
						g.IB[y][x].czar = '_';
						y++;
						x--;
					}

				}

				y = i;
				x = j + 2;
				meter = 1;
				while (isInsideBoard(g.ibMaxWidth, g.ibMaxHeight, x, y) && (cz == g.IB[y][x].czar || (meter >= g.pawns && (g.IB[y][x].czar == 'W' || g.IB[y][x].czar == 'B')))) {
					meter++;
					x += 2;
				}
				if (meter >= g.pawns) {
					x -= 2;
					for (int k = 0; k < meter; k++) {
						if (g.IB[y][x].czar == 'W' && cz == 'W') {
							g.whiteReserve++;
						}
						if (g.IB[y][x].czar == 'B' && cz == 'B') {
							g.blackReserve++;
						}
						g.IB[y][x].czar = '_';
						x -= 2;
					}
				}
			}
		}
	}
}

bool hot60(string buff, game &g) {
	bool here = false;
	string from, to;
	from += buff[17];
	from += buff[18];
	to += buff[20];
	to += buff[21];
	char color = buff[14];
	char czarek;

	if (color == 'w') {
		czarek = 'W';
	}
	else if (color == 'b') {
		czarek = 'B';
	}

	int xf = 0, yf = 0, xt = 0, yt = 0;
	for (int i = 0; i < g.ibMaxHeight; i++) {
		for (int j = 0; j < g.ibMaxWidth; j++) {
			if ((g.IB[i][j].index == from || g.IB[i][j].index == to) && xf == 0 && (g.IB[i][j].czar == 'W' || g.IB[i][j].czar == 'B')) {
				yf = i;
				xf = j;
			}
			if ((g.IB[i][j].index == to || g.IB[i][j].index == from) && (g.IB[i][j].czar == 'W' || g.IB[i][j].czar == 'B')) {
				yt = i;
				xt = j;
			}
		}
	}

	if (yf == 0 || yt == 0) {
		cout << "WRONG_INDEX_OF_CHOSEN_ROW" << endl;
		return false;
	}
	else if (g.IB[yf][xf].czar != czarek) {
		cout << "WRONG_COLOR_OF_CHOSEN_ROW" << endl;
		return false;
	}
	else {
		int meter = 1, x = 0, y = 0;

		y = yf + 1;
		x = xf + 1;
		while (isInsideBoard(g.ibMaxWidth, g.ibMaxHeight, x, y) && (g.IB[y][x].czar == 'W' || g.IB[y][x].czar == 'B')) {
			meter++;
			if (g.IB[y][x].index == g.IB[yt][xt].index && meter >=g.pawns) {
				here = true;
			}
			y++;
			x++;
		}
		if (here) {
			y = yf;
			x = xf;
			for (int k = 0; k < meter; k++) {
				if (g.IB[y][x].czar == 'W' && czarek == 'W') {
					g.whiteReserve++;
				}
				if (g.IB[y][x].czar == 'B' && czarek == 'B') {
					g.blackReserve++;
				}
				g.IB[y][x].czar = '_';
				y++;
				x++;
			}
			y = yf - 1;
			x = xf - 1;
			while (isInsideBoard(g.ibMaxWidth, g.ibMaxHeight, x, y) && (g.IB[y][x].czar == 'W' || g.IB[y][x].czar == 'B')) {
				g.IB[y][x].czar = '_';
				y--;
				x--;
			}
			return true;
		}

		meter = 1;
		y = yf + 1;
		x = xf - 1;
		while (isInsideBoard(g.ibMaxWidth, g.ibMaxHeight, x, y) && (g.IB[y][x].czar == 'W' || g.IB[y][x].czar == 'B')) {
			meter++;
			if (g.IB[y][x].index == g.IB[yt][xt].index && meter >= g.pawns) {
				here = true;
			}
			y++;
			x--;
		}
		if (here) {
			y = yf;
			x = xf;
			for (int k = 0; k < meter; k++) {
				if (g.IB[y][x].czar == 'W' && czarek == 'W') {
					g.whiteReserve++;
				}
				if (g.IB[y][x].czar == 'B' && czarek == 'B') {
					g.blackReserve++;
				}
				g.IB[y][x].czar = '_';
				y++;
				x--;
			}
			y = yf - 1;
			x = xf + 1;
			while (isInsideBoard(g.ibMaxWidth, g.ibMaxHeight, x, y) && (g.IB[y][x].czar == 'W' || g.IB[y][x].czar == 'B')) {
				g.IB[y][x].czar = '_';
				y--;
				x++;
			}
			return true;
		}

		meter = 1;
		y = yf;
		x = xf + 2;
		while(isInsideBoard(g.ibMaxWidth, g.ibMaxHeight, x, y) && (g.IB[y][x].czar == 'W' || g.IB[y][x].czar == 'B')) {
			meter++;
			if (g.IB[y][x].index == g.IB[yt][xt].index && meter >= g.pawns) {
				here = true;
			}
			x += 2;
		}
		if (here) {
			y = yf;
			x = xf;
			for (int k = 0; k < meter; k++) {
				if (g.IB[y][x].czar == 'W' && czarek == 'W') {
					g.whiteReserve++;
				}
				if (g.IB[y][x].czar == 'B' && czarek == 'B') {
					g.blackReserve++;
				}
				g.IB[y][x].czar = '_';
				x += 2;
			}
			x = xf - 2;
			while (isInsideBoard(g.ibMaxWidth, g.ibMaxHeight, x, y) && (g.IB[y][x].czar == 'W' || g.IB[y][x].czar == 'B')) {
				g.IB[y][x].czar = '_';
				x -= 2;
			}
			return true;
		}
	}
	cout << "WRONG_INDEX_OF_CHOSEN_ROW" << endl;
	return false;
}

void doMove(string buffer, game& g) {
	string from, to;
	from += buffer[8];
	from += buffer[9];

	to += buffer[11];
	to += buffer[12];
	bool goodFrom = false;
	bool goodTo = false;

	for (int i = 0; i < g.ibMaxHeight; i++) {
		for (int j = 0; j < g.ibMaxWidth; j++) {
			if (g.IB[i][j].index == from) {
				goodFrom = true;
			}
			if (g.IB[i][j].index == to) {
				goodTo = true;
			}
		}
	}

	if (!goodFrom) {
		cout << "BAD_MOVE_" << from << "_IS_WRONG_INDEX" << endl;
	}
	else if (!goodTo) {
		cout << "BAD_MOVE_" << to << "_IS_WRONG_INDEX" << endl;
	}
	else {
		field* f = nullptr, * t = nullptr;
		for (int i = 0; i < g.ibMaxHeight; i++) {
			for (int j = 0; j < g.ibMaxWidth; j++) {
				if (g.IB[i][j].index == from) {
					f = &g.IB[i][j];
				}
				else if (g.IB[i][j].index == to) {
					t = &g.IB[i][j];
				}
			}
		}

		if (f != nullptr && f->czar == '+') {
			if (t != nullptr && t->czar != '+') {
				for (int i = 0; i < g.ibMaxHeight; i++) {
					for (int j = 0; j < g.ibMaxWidth; j++) {
						if (g.IB[i][j].index == t->index) {
							if (f->index[0] < intToChar('a' + g.size) && (t->index[0] == f->index[0] || t->index[0] == intToChar(f->index[0] + 1))) {
								if (t->index[0] == f->index[0]) {
									if (!movingVertical(f, t, g, i, j)) {
										return;
									}
								}
								else {
									if (f->index[1] == t->index[1]) {
										if (t->czar == '_') {
											t->czar = g.currentPlayer;
										}
										else {
											if (!diagonalMove('+', t, g, i, j)) {
												return;
											}
										}
									}
									else if (intToChar(f->index[1]+1) == t->index[1]) {
										if (!movingHorizontal('+', t, g, i, j)) {
											return;
										}
									}
								}
							}
							else if (f->index[0] == intToChar('a' + g.size) && t->index[0] == f->index[0]) {
								if (!movingVertical(f, t, g, i, j)) {
									return;
								}
							}
							else if (f->index[0] > intToChar('a' + g.size) && (t->index[0] == f->index[0] || t->index[0] == intToChar(f->index[0] - 1))) {
								if (t->index[0] == f->index[0]) {
									if (!movingVertical(f, t, g, i, j)) {
										return;
									}
								}
								else {
									int x = j, y = i, meter = 0;
									if (f->index[1] == t->index[1]) {
										if (t->czar == '_') {
											t->czar = g.currentPlayer;
										}
										else {
											if (!movingHorizontal('-', t, g, i, j)) {
												return;
											}
										}
									}
									else if (intToChar(f->index[1] + 1) == t->index[1]) {
										if (t->czar == '_') {
											t->czar = g.currentPlayer;
										}
										else {
											if (!diagonalMove('-', t, g, i, j)) {
												return;
											}
										}
									}
								}
							}
							else {
								cout << "UNKNOWN_MOVE_DIRECTION" << endl;
								return;
							}
						}
					}
				}
			}
			else {
				cout << "BAD_MOVE_" << to << "_IS_WRONG_DESTINATION_FIELD" << endl;
				return;
			}

			if (g.currentPlayer == 'W') {
				g.whiteReserve--;
			}
			else {
				g.blackReserve--;
			}
		}
		else {
			cout << "BAD_MOVE_"<< from <<"_IS_WRONG_STARTING_FIELD" << endl;
			return;
		}

		if (g.currentPlayer == 'W') {
			g.currentPlayer = 'B';
		}
		else {
			g.currentPlayer = 'W';
		}


		if (buffer.size() > 13) {
			if (!hot60(buffer, g)) {
				return;
			}
		}
		else {
			beating(g);

		}

		for (int i = 1; i < g.maxHeight + 1; i++) {
			for (int j = 2; j < g.maxWidth + 2; j++) {
				if (g.IB[i][j].czar != '+') {
					g.array[i - 1][j - 2] = g.IB[i][j].czar;
				}
			}
		}
		cout << "MOVE_COMMITTED" << endl;

		if (g.whiteReserve == 0) {
			cout << "BLACK_HAS_WON" << endl;
		}
		else if (g.blackReserve == 0) {
			cout << "WHITE_HAS_WON" << endl;
		}

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
		else if (buffer[0] == 'D') {
			doMove(buffer, letzgo);
		}
		
	}
	deleteBoard(letzgo.maxHeight , letzgo.array);
	for (int i = 0; i < letzgo.ibMaxHeight; i++) {
		delete[] letzgo.IB[i];
	}
	delete[] letzgo.IB;
	return 0;
}
