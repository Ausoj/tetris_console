#include <chrono>

#include "tetris.h"

// since its not a class, the variables have to be defined within the cpp file instead of the header so to fix that > rewrite into a class
std::wstring tetromino[7];
int nFieldWidth{ 12 };
int nFieldHeight{ 18 };

int nScreenWidth = 80;			// Console screen size X (columns)
int nScreenHeight = 30;			// Console screen size y (rows)

unsigned char *pField = nullptr;

int Rotate(int px, int py, int r);
bool DoesPieceFit(int nTetromino, int nRotation, int nPosX, int nPosY);
int ManageGameState();
bool startPlaying{ false };


// Game Logic Stuff
bool bGameOver{ false };

int nCurrentPiece{ rand() % 7 };
int nCurrentRotation{ 0 };
int nCurrentX{ nFieldWidth / 2 - 1 };
int nCurrentY{ 0 };

bool bKey[4];
bool bRotateHold{ false };

int nSpeed{ 20 };
int nSpeedCounter{ 0 };
int nPieceCount{ 0 };
int nScore{ 0 };
int nNextPiece{ rand() % 7 };
bool bForceDown{ false };
bool bLine{ false };

std::vector<int> vLines;

int nNextBlockBoxWidth{ 6 };
int nNextBlockBoxHeight{ 6 };
bool bBoxDrawn{ false };



int Rotate(int px, int py, int r) {
	switch (r % 4) {
	case 0: return py * 4 + px;				// 0 degrees
	case 1: return 12 + py - (px * 4);		// 90 degrees
	case 2: return 15 - (py * 4) - px;		// 180 degrees
	case 3: return 3 - py + (px * 4);		// 270 degrees
	}
	return 0;
}

bool DoesPieceFit(int nTetromino, int nRotation, int nPosX, int nPosY) {
	for(int px = 0; px < 4; px++)
		for (int py = 0; py < 4; py++) {
			// Get index into piece
			int pi = Rotate(px, py, nRotation);

			// Get index into field
			int fi = (nPosY + py) * nFieldWidth + (nPosX + px);

			if (nPosX + px >= 0 && nPosX + px < nFieldWidth) {
				if (nPosY + py >= 0 && nPosY + py < nFieldHeight) {
					if (tetromino[nTetromino][pi] == L'X' && pField[fi] != 0)
						return false;
				}
			}
		}

	return true;
}

int ManageGameState(bool startPlaying) {
	if(startPlaying) return 1;
	else return 0;
}

int game() {

	while (!(ManageGameState(startPlaying))) {


		//startPlaying = bMenu(bKey);
		startPlaying = true;

	}

	// Create Screen Buffer
	wchar_t *screen = new wchar_t[nScreenWidth*nScreenHeight];
	for (int i = 0; i < nScreenWidth*nScreenHeight; i++) screen[i] = L' ';
	HANDLE hConsole = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, 0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);
	SetConsoleActiveScreenBuffer(hConsole);
	DWORD dwBytesWritten = 0;

	// Create assets
	tetromino[0].append(L"..X.");
	tetromino[0].append(L"..X.");
	tetromino[0].append(L"..X.");
	tetromino[0].append(L"..X.");

	tetromino[1].append(L"..X.");
	tetromino[1].append(L".XX.");
	tetromino[1].append(L".X..");
	tetromino[1].append(L"....");

	tetromino[2].append(L".X..");
	tetromino[2].append(L".XX.");
	tetromino[2].append(L"..X.");
	tetromino[2].append(L"....");

	tetromino[3].append(L"....");
	tetromino[3].append(L".XX.");
	tetromino[3].append(L".XX.");
	tetromino[3].append(L"....");

	tetromino[4].append(L"..X.");
	tetromino[4].append(L".XX.");
	tetromino[4].append(L"..X.");
	tetromino[4].append(L"....");

	tetromino[5].append(L"....");
	tetromino[5].append(L".XX.");
	tetromino[5].append(L"..X.");
	tetromino[5].append(L"..X.");

	tetromino[6].append(L"....");
	tetromino[6].append(L".XX.");
	tetromino[6].append(L".X..");
	tetromino[6].append(L".X..");

	pField = new unsigned char[nFieldWidth*nFieldHeight];	// Create play field buffer
	for (int x = 0; x < nFieldWidth; x++)					// Board Boundary
		for (int y = 0; y < nFieldHeight; y++)
			pField[y*nFieldWidth + x] = (x == 0 || x == nFieldWidth - 1 || y == nFieldHeight - 1) ? 9 : 0;


	while (ManageGameState(startPlaying) == 1){
		while (!bGameOver) {
			// INPUT =====================================
			for (int k = 0; k < 4; k++)                            //  R   L   D  Y
				bKey[k] = (0x8000 & GetAsyncKeyState((unsigned char)("\x27\x25\x28Y"[k]))) != 0;

			// GAME TIMING ===============================
			std::this_thread::sleep_for(std::chrono::milliseconds(50));
			nSpeedCounter++;
			bForceDown = (nSpeedCounter == nSpeed);

			// GAME LOGIC ================================
			nCurrentX += (bKey[0] && DoesPieceFit(nCurrentPiece, nCurrentRotation, nCurrentX + 1, nCurrentY)) ? 1 : 0;
			nCurrentX -= (bKey[1] && DoesPieceFit(nCurrentPiece, nCurrentRotation, nCurrentX - 1, nCurrentY)) ? 1 : 0;
			nCurrentY += (bKey[2] && DoesPieceFit(nCurrentPiece, nCurrentRotation, nCurrentX, nCurrentY + 1)) ? 1 : 0;

			if (bKey[3]) {
				nCurrentRotation += (!bRotateHold && DoesPieceFit(nCurrentPiece, nCurrentRotation + 1, nCurrentX, nCurrentY));
				bRotateHold = true;

			}
			else 
				bRotateHold = false;
		
			if (bForceDown) {
				if (DoesPieceFit(nCurrentPiece, nCurrentRotation, nCurrentX, nCurrentY + 1))
					nCurrentY++;
				else {
					// Lock current piece in the field
					for (int px = 0; px < 4; px++)
						for (int py = 0; py < 4; py++)
							if (tetromino[nCurrentPiece][Rotate(px, py, nCurrentRotation)] == L'X')
								pField[(nCurrentY + py) * nFieldWidth + (nCurrentX + px)] = nCurrentPiece + 1;

					nPieceCount++;
					if (nPieceCount % 10 == 0)
						if (nSpeed >= 10) nSpeed--;

					// Check have we got any lines
					for (int py = 0; py < 4; py++)
						if (nCurrentY + py < nFieldHeight - 1) {
							bLine = true;
							for (int px = 1; px < nFieldWidth - 1; px++)
								bLine &= (pField[(nCurrentY + py) * nFieldWidth + px]) != 0;

							if (bLine) {
								// Remove Line, set to =
								for (int px = 1; px < nFieldWidth - 1; px++)
									pField[(nCurrentY + py) * nFieldWidth + px] = 8;

									vLines.push_back(nCurrentY + py);
							}
						}

					nScore += 25;
					if (!vLines.empty()) nScore += (1 << vLines.size()) * 100;

					// Set next piece
					nCurrentX = nFieldWidth / 2 - 1;
					nCurrentY = 0;
					nCurrentRotation = 0;
					nCurrentPiece = nNextPiece;
					nNextPiece = rand() % 7;

					// if piece does not fit
					bGameOver = !DoesPieceFit(nCurrentPiece, nCurrentRotation, nCurrentX, nCurrentY);
				}

				nSpeedCounter = 0;
			}


			// RENDER OUTPUT =============================
			// Draw Field
			for (int x = 0; x < nFieldWidth; x++)
				for (int y = 0; y < nFieldHeight; y++)
					screen[(y + 2)*nScreenWidth + (x + 2)] = L" ABCDEFG=#"[pField[y*nFieldWidth + x]];

			// Draw Current Piece
			for (int px = 0; px < 4; px++)
				for (int py = 0; py < 4; py++)
					if (tetromino[nCurrentPiece][Rotate(px, py, nCurrentRotation)] == L'X')
						screen[(nCurrentY + py + 2)*nScreenWidth + (nCurrentX + px + 2)] = nCurrentPiece + 65;

			// Draw Score
			swprintf_s(&screen[2 * nScreenWidth + nFieldWidth + 6], 16, L"SCORE: %8d", nScore);

			// Draw box for next Block
			if(!bBoxDrawn)
				for (int x = 0; x < nNextBlockBoxWidth; x++)
					for (int y = 0; y < nNextBlockBoxHeight; y++)
						if ((x == 0 && y <= nNextBlockBoxHeight) || (x == nNextBlockBoxWidth - 1 && y <= nNextBlockBoxHeight) ||
							(x <= nNextBlockBoxWidth && y == 0) || (x <= nNextBlockBoxWidth && y == nNextBlockBoxHeight - 1)) {
							screen[((6 + y)* nScreenWidth + nFieldWidth) + 6 + x] = int('#');
							bBoxDrawn = true;
						}
							
			// Clear Next Piece box
				for (int x = 0; x < nNextBlockBoxWidth; x++)
					for (int y = 0; y < nNextBlockBoxHeight; y++)
						if((x != 0 && x != nNextBlockBoxWidth - 1 && y != 0 && y != nNextBlockBoxHeight - 1))
							screen[((6 + y)* nScreenWidth + nFieldWidth) + 6 + x] = int(' ');

			// Draw Next Piece in box
			for (int px = 0; px < 4; px++)
				for (int py = 0; py < 4; py++)
					if (tetromino[nNextPiece][Rotate(px, py, 0)] == L'X')
						screen[(7 + py)* nScreenWidth + nFieldWidth + 7 + px] = nNextPiece + 65;


			// Draw current Speed
			swprintf_s(&screen[4 * nScreenWidth + nFieldWidth + 6], 16, L"SPEED: %8d", (nSpeed - 20)*(-1));


			if (!vLines.empty()) {
				// Display Frame (cheekily to draw lines)
				WriteConsoleOutputCharacter(hConsole, screen, nScreenWidth * nScreenHeight, { 0,0 }, &dwBytesWritten);
				std::this_thread::sleep_for(std::chrono::milliseconds(400)); // Delay a bit

				for (auto &v : vLines) {
					for (int px = 1; px < nFieldWidth - 1; px++) {
						for (int py = v; py > 0; py--)
							pField[py*nFieldWidth + px] = pField[(py - 1)* nFieldWidth + px];
						pField[px] = 0;
					}
					vLines.clear();
				}

			}


			// Display Frame
			WriteConsoleOutputCharacter(hConsole, screen, nScreenWidth * nScreenHeight, { 0,0 }, &dwBytesWritten);
		}

	}

	// Game Over
	CloseHandle(hConsole);
	std::cout << "Game Over! Score: " << nScore << std::endl;
	system("pause");


	return 0;
} 
