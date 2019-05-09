#include "menu.h"

void Menu::initMenu() {
	if (!bBufferCreated) {
		// Create Screen Buffer
		wchar_t *screen = new wchar_t[Menu::nScreenWidth*Menu::nScreenHeight];
		Menu::screen = screen;
		for (int i = 0; i < Menu::nScreenWidth*Menu::nScreenHeight; i++) Menu::screen[i] = L' ';
		
		Menu::hConsole = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, 0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);
		SetConsoleActiveScreenBuffer(Menu::hConsole);
		Menu::dwBytesWritten = 0;
		Menu::bBufferCreated = true;
	}
}

void Menu::destroyMenu() {
	Menu::bBufferCreated = false;
	CloseHandle(Menu::hConsole);
}

bool Menu::bMenu() {
	initMenu();

	while (true) {
		// MENU TIMING ===============================
		std::this_thread::sleep_for(std::chrono::milliseconds(100));
		if (Menu::nChoiceTimer > 100000) nChoiceTimer = 0;
		Menu::nChoiceTimer++;
		Menu::nGametitleTimer++;

		// INPUT =====================================
		if (GetAsyncKeyState(VK_LEFT) & 0x8000) {
			if (!(Menu::nCurrentChoice == 0)) {
				Menu::nCurrentChoice--;
			}
		}
		if (GetAsyncKeyState(VK_RIGHT) & 0x8000) {
			if (!(Menu::nCurrentChoice == 2)) {
				Menu::nCurrentChoice++;
			}
		}
		if (GetAsyncKeyState(VK_RETURN) & 0x8000) {
			if (nCurrentChoice == 0)
				return true;
			else if (nCurrentChoice == 1) {

			}
			else if (nCurrentChoice == 2) {
				destroyMenu(); // CURRENTLY THROWS ERROR
				return false;

			}
		}

		// RENDER OUTPUT =============================
		// Menu Border
		for (int py = 0; py < Menu::nMenuSizeY; py++)
			for (int px = 0; px < Menu::nMenuSizeX; px++)
				if ((px == 0 && py <= nMenuSizeY) || (px == nMenuSizeX - 1 && py <= nMenuSizeY) ||
					(px <= nMenuSizeX && py == 0) || (px <= nMenuSizeX && py == nMenuSizeY - 1))
						screen[(py*nScreenWidth) + px] = L"#"[0];


		// Title Animation
		// Write a letter every second tick for 15 ticks
		if (!(nGametitleTimer % 4) && nGametitleTimer < 30) {
			if (nCurrentLetterID < Menu::strGametitle.size()) {
				screen[5 * nScreenWidth + (nScreenWidth / 2 - 3 + nCurrentLetterID)] = Menu::strGametitle[nCurrentLetterID];
				nCurrentLetterID++;
			}
		} // Blink the title -> Turn it off
		else if (!(nGametitleTimer % 4) && nGametitleTimer <= 60 && nGametitleTimer >= 30) {
			for(int letterID = 0; letterID < Menu::strGametitle.size(); letterID++)
				screen[5 * nScreenWidth + (nScreenWidth / 2 - 3 + letterID)] = L" "[0];

		} // Blink the title -> Turn it on || Keep it on till 50 ticks have passed
		else if ((nGametitleTimer % 4) && nGametitleTimer <= 100 && nGametitleTimer >= 30) {
			for (int letterID = 0; letterID < Menu::strGametitle.size(); letterID++)
				screen[5 * nScreenWidth + (nScreenWidth / 2 - 3 + letterID)] = Menu::strGametitle[letterID];
		} // Reset
		else if (nGametitleTimer > 100){
			for (int letterID = 0; letterID < Menu::strGametitle.size(); letterID++)
				screen[5 * nScreenWidth + (nScreenWidth / 2 - 3 + letterID)] = L" "[0];
			nGametitleTimer = 0;
			nCurrentLetterID = 0;
		} 

		// Menu Options
		// Start BOX
		for (int py = 0; py < nOptionHeight; py++)
			for (int px = 0; px < nOptionWidth; px++)
				if ((px == 0 && py <= nOptionHeight) || (px == nOptionWidth - 1 && py <= nOptionHeight) ||
					(px <= nOptionWidth && py == 0) || (px <= nOptionWidth && py == nOptionHeight - 1))
					screen[20 * nScreenWidth + (nScreenWidth / 3 - nOptionWidth - 6) + (py*nScreenWidth) + px] = L"#"[0];
		// BLINK THE TEXT
		if (nCurrentChoice == 0 && nChoiceTimer % 4) {
			for (int letterID = 0; letterID < Menu::aOptions[0].size(); letterID++)
				screen[((20 + (nOptionHeight / 2)) * nScreenWidth) + (nScreenWidth / 3 - nOptionWidth - 5 + letterID + ((aOptions[0].size() / 2) + 1))] = L" "[0];
		}
		else {
			for (int letterID = 0; letterID < Menu::aOptions[0].size(); letterID++)
				screen[((20 + (nOptionHeight / 2)) * nScreenWidth) + (nScreenWidth / 3 - nOptionWidth - 5 + letterID + ((aOptions[0].size() / 2) + 1))] = Menu::aOptions[0][letterID];

		}

		// Highscores
		for (int py = 0; py < nOptionHeight; py++)
			for (int px = 0; px < nOptionWidth; px++)
				if ((px == 0 && py <= nOptionHeight) || (px == nOptionWidth - 1 && py <= nOptionHeight) ||
					(px <= nOptionWidth && py == 0) || (px <= nOptionWidth && py == nOptionHeight - 1))
					screen[20 * nScreenWidth + ((nScreenWidth / 3)*2 - nOptionWidth - 6) + (py*nScreenWidth) + px] = L"#"[0];

		// BLINK THE TEXT
		if (nCurrentChoice == 1 && nChoiceTimer % 4) {
			for (int letterID = 0; letterID < Menu::aOptions[1].size(); letterID++)
				screen[((20 + (nOptionHeight / 2)) * nScreenWidth) + ((nScreenWidth / 3) * 2 - nOptionWidth - 5 + letterID + ((aOptions[1].size() / 2) - 1))] = L" "[0];
		}
		else {
			for (int letterID = 0; letterID < Menu::aOptions[1].size(); letterID++)
				screen[((20 + (nOptionHeight / 2)) * nScreenWidth) + ((nScreenWidth / 3) * 2 - nOptionWidth - 5 + letterID + ((aOptions[1].size() / 2) - 1))] = Menu::aOptions[1][letterID];
		}

		// EXIT
		for (int py = 0; py < nOptionHeight; py++)
			for (int px = 0; px < nOptionWidth; px++)
				if ((px == 0 && py <= nOptionHeight) || (px == nOptionWidth - 1 && py <= nOptionHeight) ||
					(px <= nOptionWidth && py == 0) || (px <= nOptionWidth && py == nOptionHeight - 1))
					screen[20 * nScreenWidth + ((nScreenWidth / 3)*3 - nOptionWidth - 6) + (py*nScreenWidth) + px] = L"#"[0];

		// BLINK THE TEXT
		if (nCurrentChoice == 2 && (nChoiceTimer % 4)) {
			for (int letterID = 0; letterID < Menu::aOptions[2].size(); letterID++)
				screen[((20 + (nOptionHeight / 2)) * nScreenWidth) + ((nScreenWidth / 3) * 3 - nOptionWidth - 5 + letterID + ((aOptions[2].size() / 2) + 1))] = L" "[0];
		}
		else {
			for (int letterID = 0; letterID < Menu::aOptions[2].size(); letterID++)
				screen[((20 + (nOptionHeight / 2)) * nScreenWidth) + ((nScreenWidth / 3) * 3 - nOptionWidth - 5 + letterID + ((aOptions[2].size() / 2) + 1))] = Menu::aOptions[2][letterID];

		}

		// Display Frame
		WriteConsoleOutputCharacter(Menu::hConsole, Menu::screen, Menu::nScreenWidth * Menu::nScreenHeight, { 0,0 }, &dwBytesWritten);
	}
	//destroyMenu();
	return true;
}