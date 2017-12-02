// SADXPhysicsSwapMod.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"
#include "resource.h"
#include "SADXModLoader.h"
#include "IniFile.hpp"
#include <algorithm>
using std::unordered_map;
using std::string;

HMODULE moduleHandle;

static const string charnames[] = {
	"sonic",
	"eggman",
	"tails",
	"knuckles",
	"tikal",
	"amy",
	"gamma",
	"big",
	"supersonic",
	"sa2bsonic",
	"sa2bshadow",
	"sa2btails",
	"sa2beggman",
	"sa2bknuckles",
	"sa2brouge",
	"sa2bmechtails",
	"sa2bmecheggman",
	"sa2bamy",
	"sa2bsupersonic",
	"sa2bsupershadow",
	"sa2bunused",
	"sa2bmetalsonic",
	"sa2bchaowalker",
	"sa2bdarkchaowalker",
	"sa2btikal",
	"sa2bchaos",
	"sa2bunused2",
	"sa2bunused3",
	"heroessonic",
	"heroesknuckles",
	"heroestails",
	"heroesshadow",
	"heroesomega",
	"heroesrouge",
	"heroesamy",
	"heroesbig",
	"heroescream",
	"heroesespio",
	"heroesvector",
	"heroescharmy"
};

static unordered_map<string, uint8_t> charnamemap;

static string trim(const string &s)
{
	auto st = s.find_first_not_of(' ');
	if (st == string::npos)
		st = 0;
	auto ed = s.find_last_not_of(' ');
	if (ed == string::npos)
		ed = s.size() - 1;
	return s.substr(st, (ed + 1) - st);
}

static uint8_t ParseCharacterID(const string &str, Characters def)
{
	string s = trim(str);
	transform(s.begin(), s.end(), s.begin(), ::tolower);
	auto ch = charnamemap.find(s);
	if (ch != charnamemap.end())
		return ch->second;
	return def;
}

static const string keynames[] = { "Sonic", "Eggman", "Tails", "Knuckles", "Tikal", "Amy", "Gamma", "Big" };

extern "C"
{
	__declspec(dllexport) void __cdecl Init(const char *path, const HelperFunctions &helperFunctions)
	{
		for (uint8_t i = 0; i < LengthOfArray(charnames); i++)
			charnamemap[charnames[i]] = i;
		HRSRC hres = FindResource(moduleHandle, MAKEINTRESOURCE(IDR_MISC1), L"MISC");
		PhysicsData *tmp = (PhysicsData*)LockResource(LoadResource(moduleHandle, hres));
		const IniFile *settings = new IniFile(std::string(path) + "\\mod.ini");
		for (uint8_t i = 0; i < LengthOfArray(keynames); i++)
		{
			uint8_t c = ParseCharacterID(settings->getString("", keynames[i] + "Physics"), (Characters)i);
			if (i == c) continue;
			memcpy(&PhysicsArray[i], &tmp[c], offsetof(PhysicsData, RippleSize));
			PhysicsArray[i].Gravity = tmp[c].Gravity;
		}
		delete settings;
	}

	__declspec(dllexport) ModInfo SADXModInfo = { ModLoaderVer };
}