#pragma once

#include <iostream>
#include <thread>
#include <chrono>
#include <Windows.h>
#include <string>

class Menu {
private:
	HANDLE hConsole;
	DWORD dwBytesWritten;
	wchar_t *screen;


	int nScreenWidth{ 80 };
	int nScreenHeight{ 30 };

	int nMenuSizeX{ 80 };
	int nMenuSizeY{ 30 };

	int nGametitleTimer{ 0 };
	int nCurrentLetterID{ 0 };

	int nCurrentChoice{ 0 };
	int nChoiceTimer{ 0 };

	std::wstring strGametitle = L"TETRIS";
	std::wstring aOptions[3] = {L"PLAY", L"SCORES", L"EXIT"};

	int nOptionWidth{ 12 };
	int nOptionHeight{ 5 };

	bool bBufferCreated{ false };

	void initMenu();
	void destroyMenu();

public:

	bool bMenu();
	
};