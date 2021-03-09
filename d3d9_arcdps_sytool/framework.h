#pragma once

#define WIN32_LEAN_AND_MEAN             // 从 Windows 头文件中排除极少使用的内容
// Windows 头文件
#include <windows.h>
#include "acrdps_intomod.h"
#include "DDSTextureLoader.h"
#include "imgui/imgui.h"
#include <d3d9.h>
#include "resource.h"
#include "simpleini/SimpleIni.h"
#include "MysData.h"
#include "Recipes.h"
#include "Items.h"
#include "wke.h"
#define COM_RELEASE(x) { if((x)) { (x)->Release(); (x) = nullptr; } }
using namespace std;