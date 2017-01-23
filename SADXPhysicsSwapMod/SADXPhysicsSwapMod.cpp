// SADXPhysicsSwapMod.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"
#include "SADXModLoader.h"
#include "IniFile.hpp"
#include <algorithm>
using std::unordered_map;
using std::string;

static const unordered_map<string, uint8_t> charnamemap = {
	{ "sonic", Characters_Sonic },
	{ "eggman", Characters_Eggman },
	{ "tails", Characters_Tails },
	{ "knuckles", Characters_Knuckles },
	{ "tikal", Characters_Tikal },
	{ "amy", Characters_Amy },
	{ "gamma", Characters_Gamma },
	{ "big", Characters_Big },
	{ "supersonic", 8 }
};

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

static const string charnames[] = {	"Sonic", "Eggman", "Tails", "Knuckles", "Tikal", "Amy", "Gamma", "Big" };

extern "C"
{
	__declspec(dllexport) void __cdecl Init(const char *path, const HelperFunctions &helperFunctions)
	{
		PhysicsData *tmp = new PhysicsData[PhysicsArray_Length + 1];
		memcpy(tmp, PhysicsArray, PhysicsArray_Length * sizeof(PhysicsData));
		tmp[PhysicsArray_Length] = tmp[Characters_Sonic];
		tmp[PhysicsArray_Length].RollDecel = -0.001f;
		tmp[PhysicsArray_Length].AirDecel = -0.002f;
		tmp[PhysicsArray_Length].AirAccel = 0.05f;
		const IniFile *settings = new IniFile(std::string(path) + "\\mod.ini");
		for (uint8_t i = 0; i < LengthOfArray(charnames); i++)
		{
			uint8_t c = ParseCharacterID(settings->getString("", charnames[i] + "Physics"), (Characters)i);
			if (i == c) continue;
			memcpy(&PhysicsArray[i], &tmp[c], offsetof(PhysicsData, RippleSize));
			PhysicsArray[i].Gravity = tmp[c].Gravity;
		}
		delete settings;
		delete[] tmp;
	}

	__declspec(dllexport) ModInfo SADXModInfo = { ModLoaderVer };
}