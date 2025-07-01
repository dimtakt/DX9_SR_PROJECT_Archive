#pragma once

#pragma warning(disable : 4251)

//#define _WIN32_WINNT 0x0601
//#ifndef _WIN32_WINNT
//#define _WIN32_WINNT 0x0A00
//#endif
#include <d3d9.h>
#include <d3dx9.h>

#include "fmod.h"
#include "fmod.hpp"
#pragma comment(lib, "fmod_vc.lib")

//#include "nlohmann/json.hpp"
//using json = nlohmann::json;

#include <vector>
#include <list>
#include <map>
#include <queue>
#include <algorithm>
#include <functional>
#include <string>
#include <unordered_map>
#include <ctime>
#include <functional>
#include <iostream>
#include <io.h> 
using namespace std;

#include "Engine_Enum.h"
#include "Engine_Macro.h"
#include "Engine_Struct.h"
#include "Engine_Typedef.h"
#include "Engine_Function.h"

#define MAXCHANNEL 32

#ifdef _DEBUG

#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

#ifndef DBG_NEW 

#define DBG_NEW new ( _NORMAL_BLOCK , __FILE__ , __LINE__ ) 
#define new DBG_NEW 

#endif
#endif


using namespace Engine;
