#include <iostream>
#include <Windows.h>
#include <TlHelp32.h>
#include <memory>
#include <string_view>
#include <cstdint>
#include <vector>
#include <string>
#include <ctime>
#include <random>
#include <stdlib.h>
#include "offsets.cpp"
#include <thread>

using namespace std;

namespace APEXTool {
	namespace Controls
	{
		bool colors_based_on_life_and_shield = true;
		bool colors_based_on_vision = false;
	};
}

LPCTSTR WinName = "Apex Legends";
HWND hWnd = FindWindow(NULL, WinName);

uintptr_t pID;
uintptr_t moduleBase;
uintptr_t localPlayer;
uintptr_t entList;
uintptr_t viewRenderer;
uintptr_t viewMatrix;

typedef struct _NULL_MEMORY
{
	void* buffer_address;
	UINT_PTR address;
	ULONGLONG size;
	ULONG pid;
	BOOLEAN write;
	BOOLEAN read;
	BOOLEAN req_base;
	void* output;
	const char* module_name;
	ULONG64 base_address;
}NULL_MEMORY;

uintptr_t base_address = 0;
uint32_t process_id = 0;

template<typename ... Arg>
uint64_t call_hook(const Arg ... args)
{
	void* hooked_func = GetProcAddress(LoadLibrary("win32u.dll"), "NtOpenCompositionSurfaceSectionInfo"); // NtOpenCompositionSurfaceSectionInfo

	auto func = static_cast<uint64_t(_stdcall*)(Arg...)>(hooked_func);

	return func(args ...);
}

struct HandleDisposer
{
	using pointer = HANDLE;
	void operator()(HANDLE handle) const
	{
		if (handle != NULL || handle != INVALID_HANDLE_VALUE)
		{
			CloseHandle(handle);
		}
	}
};

using unique_handle = unique_ptr<HANDLE, HandleDisposer>;
struct GlowMode
{
	int8_t GeneralGlowMode, BorderGlowMode, BorderSize, TransparentLevel;
};

uint32_t get_process_id(string_view process_name)
{
	PROCESSENTRY32 processentry;
	const unique_handle snapshot_handle(CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NULL));

	if (snapshot_handle.get() == INVALID_HANDLE_VALUE)
		return NULL;

	processentry.dwSize = sizeof(MODULEENTRY32);

	while (Process32Next(snapshot_handle.get(), &processentry) == TRUE)
	{
		if (process_name.compare(processentry.szExeFile) == NULL)
		{
			return processentry.th32ProcessID;
		}
	}
	return NULL;
}

static ULONG64 get_module_base_address(const char* module_name)
{
	NULL_MEMORY instructions = { 0 };
	instructions.pid = process_id;
	instructions.req_base = TRUE;
	instructions.read = FALSE;
	instructions.write = FALSE;
	instructions.module_name = module_name;
	call_hook(&instructions);
	
	ULONG64 base = NULL;
	base = instructions.base_address;
	return base;
}
template <class T>
T Read(uintptr_t read_address)
{
	T response{};
	NULL_MEMORY instructions;
	instructions.pid = process_id;
	instructions.size = sizeof(T);
	instructions.address = read_address;
	instructions.read = TRUE;
	instructions.write = FALSE;
	instructions.req_base = FALSE;
	instructions.output = &response;
	call_hook(&instructions);

	return response;
}

bool write_memory(UINT_PTR write_address, UINT_PTR source_address, SIZE_T write_size)
{
	NULL_MEMORY instructions;
	instructions.address = write_address;
	instructions.pid = process_id;
	instructions.write = TRUE;
	instructions.read = FALSE;
	instructions.req_base = FALSE;
	instructions.buffer_address = (void*)source_address;
	instructions.size = write_size;

	call_hook(&instructions);

	return true;
}

template<typename S>
bool write(UINT_PTR write_address, const S& value)
{
	return write_memory(write_address, (UINT_PTR)&value, sizeof(S));
}


DWORD64 GetEntityByID(int Ent, DWORD64 Base)
{
	DWORD64 EntityList = Base + OFFSETS::OFFSET_ENTITYLIST;
	DWORD64 BaseEntity = Read<DWORD64>(EntityList);
	if (!BaseEntity)
	return NULL;
	return  Read<DWORD64>(EntityList + (Ent << 5));
}

string random_string(const int len) {
	const string alpha_numeric("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz1234567890");

	default_random_engine generator{ random_device{}() };
	const uniform_int_distribution< string::size_type > distribution{ 0, alpha_numeric.size() - 1 };

	string str(len, 0);
	for (auto& it : str) {
		it = alpha_numeric[distribution(generator)];
	}

	return str;
}

float entityNewVisTime = 0;
float entityOldVisTime[100];
int visCooldownTime[100];
int entityHealthAndShield[100];

int main()
{
	string console_title = "Apex Tool - ";
	console_title += random_string(16).c_str();
	SetConsoleTitle(console_title.c_str());

	if (hWnd)
		cout << "[+] Found Apex Legends process..." << endl;
	else
		cout << "[+] Cant find Apex Legends process..." << endl;

	process_id = get_process_id("r5apex.exe");				// r5apex // EasyAntiCheat_launcher
	base_address = get_module_base_address("r5apex.exe");	// r5apex // EasyAntiCheat_launcher
	cout << "[i] Process ID: " << process_id << "\n"
			<< "[i] Base Address: " << base_address << "\n"
			<< "[+] Hooking game...\n\n"
			<< "[!] Only one option can be enabled at once!" << endl;
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_GREEN);
		cout << "	>> [NUM1] Glow based on life and shield: Enabled" << endl;
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED);
		cout << "	>> [NUM2] Glow based on vision: Disabled" << endl;
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);

	while (true)
	{
		if (GetAsyncKeyState(VK_NUMPAD1))
		{
			if (!APEXTool::Controls::colors_based_on_life_and_shield)
			{
				APEXTool::Controls::colors_based_on_life_and_shield = true;
				APEXTool::Controls::colors_based_on_vision = false;

				system("CLS");
				cout << "[i] Process ID: " << process_id << "\n"
					<< "[i] Base Address: " << base_address << "\n"
					<< "[+] Hooking game...\n\n"
					<< "[!] Only one option can be enabled at once!" << endl;
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_GREEN);
				cout << "	>> [NUM1] Glow based on life and shield: Enabled" << endl;
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED);
				cout << "	>> [NUM2] Glow based on vision: Disabled" << endl;
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
			}
		}

		if (GetAsyncKeyState(VK_NUMPAD2))
		{
			if (!APEXTool::Controls::colors_based_on_vision) 
			{
				APEXTool::Controls::colors_based_on_life_and_shield = false;
				APEXTool::Controls::colors_based_on_vision = true;

				system("CLS");
				cout << "[i] Process ID: " << process_id << "\n"
					<< "[i] Base Address: " << base_address << "\n"
					<< "[+] Hooking game...\n\n"
					<< "[!] Only one option can be enabled at once!" << endl;
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED);
				cout << "	>> [NUM1] Glow based on life and shield: Disabled" << endl;
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_GREEN);
				cout << "	>> [NUM2] Glow based on vision: Enabled" << endl;
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
			}
		}
		// local player
		uintptr_t locPlayer = Read<uintptr_t>(base_address + OFFSETS::OFFSET_LOCAL_ENT);

		for (int i = 0; i < 100; i++)
		{
			DWORD64 Entity = GetEntityByID(i, base_address);
			if (Entity == 0)
				continue;

			DWORD64 EntityHandle = Read<DWORD64>(Entity + 0x589);
			string Identifier = Read<string>(EntityHandle);
			LPCSTR IdentifierC = Identifier.c_str();

			if (strcmp(IdentifierC, "player"))
			{
				int playerTeamID = Read<int>(locPlayer + OFFSETS::OFFSET_TEAM);					// player team id
				int entityTeamID = Read<int>(Entity + OFFSETS::OFFSET_TEAM);					// entity team id

				int entityHealth = Read<int>(Entity + OFFSETS::OFFSET_HEALTH);					// entity health
				int entityShield = Read<int>(Entity + OFFSETS::OFFSET_SHEILD);					// entity shield

				int entityHnS = (entityHealth + entityShield)/2;

				entityNewVisTime = Read<float>(Entity + OFFSETS::OFFSET_VISIBLE_TIME);

				write<int>(Entity + OFFSETS::GLOW_CONTEXT, 1);
				write<int>(Entity + OFFSETS::GLOW_VISIBLE_TYPE, 2);
				write<GlowMode>(Entity + OFFSETS::GLOW_TYPE, { 101, 101, 100, 100 }); // GeneralGlowMode, BorderGlowMode, BorderSize, TransparentLevel;

				static float r = 0, g = 0, b = 0;

				if (APEXTool::Controls::colors_based_on_life_and_shield) // life'n'shield
				{
					if (playerTeamID != entityTeamID) // enemies
					{
						entityHealthAndShield[i] = entityHnS;
						int red = (int)min(2 * (100 - entityHealthAndShield[i]), 100.f);
						int green = (int)min(2 * entityHealthAndShield[i], 100.f);

						r = red / 100.0f;
						g = green / 100.0f;
						b = 10;
					}
					else // allies
					{
						entityHealthAndShield[i] = entityHnS;
						int red = (int)min(2 * (100 - entityHealthAndShield[i]), 100.f);
						int green = (int)min(2 * entityHealthAndShield[i], 100.f);

						r = red / 100.0f;
						g = green / 100.0f;
						b = 30;
					}
				}
				else // vision
				{
					if (entityNewVisTime != entityOldVisTime[i]) // VISIBLE enemies
					{
						visCooldownTime[i] = 32; // low values = less latency

						r = 61.f;
						g = 2.f;
						b = 2.f;

						entityOldVisTime[i] = entityNewVisTime;
					}
					else
					{
						if (visCooldownTime[i] <= 0) // NON-VISIBLE enemies
						{
							r = 2.f;
							g = 2.f;
							b = 61.f;
						}
					}
				}

				write<float>(Entity + OFFSETS::GLOW_COLOR, r);
				write<float>(Entity + 0x1D4, g);
				write<float>(Entity + 0x1D8, b);

				if (visCooldownTime[i] >= 0) visCooldownTime[i] -= 1;
			}
		}
	}
}