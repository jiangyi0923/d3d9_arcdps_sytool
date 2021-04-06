// pch.cpp: 与预编译标头对应的源文件

#include "pch.h"
#include <thread>
#include <iostream>
#include <fstream>
#include "imgui/imgui_internal.h"
#include <boost/thread.hpp>
#include "imgui/imgui_stdlib.h"
#include "imgui/imgui_impl_win32.h"
// 当使用预编译的头时，需要使用此源文件，编译才能成功。


HWND hwnd = nullptr;

namespace SYtool 
{
	void dll_init(HMODULE hModule)
	{
		dll_Module = hModule;
		return;
	}
	void dll_exit()
	{
		return;
	}
	void* get_init_addr(char* arcversionstr, void* imguicontext, IDirect3DDevice9* id3dd9, HANDLE arcdll, void* mallocfn, void* freefn)
	{
		arcvers = arcversionstr;
		//添加图片
		//bgtx = CreateTextureFromResource(id3dd9, dll_Module, IDR_BG);
		//设置上下文
		ImGui::SetCurrentContext((ImGuiContext*)imguicontext);
		ImGui::SetAllocatorFunctions((void* (*)(size_t, void*))mallocfn, (void(*)(void*, void*))freefn);
		ImGui_ImplWin32_Init(hwnd);

		parseIni();//读配置
		return mod_init;
	}

	void* get_release_addr()
	{
		arcvers = 0;
		return mod_release;
	}

	uintptr_t mod_release()
	{
		//COM_RELEASE(bgtx);
		writeIni();//写配置
		return 0;
	}


	void settingALLlang()
	{
		alllang_.clear();
		if (langseting == 1)
		{
			//if (langseting == 1)
			//{
			//    alllang_[1] = u8"物品:" + std::to_string(data_Items.size()) + u8",配方:" + std::to_string(data_Recipes.size()) + u8",马桶:" + std::to_string(data_MysticForge.size());
			//}
			//else
			//{
			//    alllang_[1] = "Items:" + std::to_string(data_Items.size()) + ",Recipes:" + std::to_string(data_Recipes.size()) + ",Mystic Forges:" + std::to_string(data_MysticForge.size());
			//}
			alllang_.push_back(u8"神油配方搜索工具");//0
			alllang_.push_back(u8"物品:" + std::to_string(data_Items.size()) + u8",配方:" + std::to_string(data_Recipes.size()) + u8",马桶:" + std::to_string(data_MysticForge.size()) + u8", 数据日期:2021-04-06");
			alllang_.push_back(u8"搜索");//2
			alllang_.push_back(u8"正在加载中");
			alllang_.push_back(u8"点击复制聊天代码");//4
			alllang_.push_back(u8"副业配方:");
			alllang_.push_back(u8"熔炉配方:");//6
			alllang_.push_back(u8"关联配方:");
			alllang_.push_back(u8"点击显示材料");//8
			alllang_.push_back(u8"点击复制文本");//9
		}
		else
		{
			alllang_.push_back("Recipe search tool");
			alllang_.push_back("Items:" + std::to_string(data_Items.size()) + ",Recipes:" + std::to_string(data_Recipes.size()) + ",Mystic Forges:" + std::to_string(data_MysticForge.size()) + ", DATA:2021-04-06");
			alllang_.push_back("Search");
			alllang_.push_back("Under loading");
			alllang_.push_back("copy chat link");
			alllang_.push_back("Avocation Recipe:");
			alllang_.push_back("Mystic Forge Recipe:");
			alllang_.push_back("Correlation formula:");
			alllang_.push_back("Click Show Material");
			alllang_.push_back("Click Copy Text");//9
		}
	}



	uintptr_t mod_wnd(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		hwnd = hWnd;
		
		if (ImGui::GetCurrentContext() == NULL)
			return uMsg;

		//if (lParam >> 31 == 0) //按下。
		//{
			if (Show_settingwindows && wParam == VK_ESCAPE) {
				Show_settingwindows = false;
				return uMsg;
			}
		//}
		//auto const io = &ImGui::GetIO();
		//if (shetingkey)
		//{
		//	const int vkey = (int)wParam;
		//	if (vkey != VK_CONTROL && vkey != VK_MENU && vkey != VK_SHIFT && vkey != VK_TAB && vkey != VK_ESCAPE && vkey != VK_NUMLOCK && vkey != VK_CAPITAL)
		//	{
		//		BUFF = (int)wParam;
		//	}

		//}


		switch (uMsg)
		{
		case WM_KEYDOWN:
		case WM_SYSKEYDOWN:
		case WM_KEYUP:
		case WM_SYSKEYUP:
		{
			auto const key = (int)wParam;
			bool const down = uMsg == WM_KEYDOWN || uMsg == WM_SYSKEYDOWN;
			if (key >= 256)
				break;

			auto& io = ImGui::GetIO();
			io.KeysDown[key] = down;
			io.KeysDownDuration[key] = down ? 0.0f : -1.0f;
			switch (key)
			{
			case VK_CONTROL: io.KeyCtrl = down; break;
			case VK_MENU:    io.KeyAlt = down; break;
			case VK_SHIFT:   io.KeyShift = down; break;
			}
			io.KeysDownDuration[key] = -1;

			if (io.KeysDown[arc_global_mod1] && io.KeysDown[arc_global_mod2])
			{
				if (io.KeysDown[setting_key]) return 0;
			}

			break;
		}
		case WM_CHAR:
		{
			if (wParam > 0 && wParam < 0x10000) 
			{
				auto const& io = &ImGui::GetIO();
				if (wanttoinput)
				{
					io->AddInputCharacterUTF16((unsigned short)wParam);
					return 0;
				}
			}
			break;
		}
			
		}


		//switch (uMsg)
		//{
		//case WM_KEYUP:
		//{
		//	const int vkey = (int)wParam;
		//	io->KeysDown[vkey] = 0;
		//	if (vkey == VK_CONTROL)
		//	{
		//		io->KeyCtrl = false;
		//	}
		//	else if (vkey == VK_MENU)
		//	{
		//		io->KeyAlt = false;
		//	}
		//	else if (vkey == VK_SHIFT)
		//	{
		//		io->KeyShift = false;
		//	}
		//	break;
		//}
		//case WM_KEYDOWN:
		//{
		//	const int vkey = (int)wParam;
		//	io->KeysDown[vkey] = 1;
		//	if (vkey == VK_CONTROL)
		//	{
		//		io->KeyCtrl = true;
		//	}
		//	else if (vkey == VK_MENU)
		//	{
		//		io->KeyAlt = true;
		//	}
		//	else if (vkey == VK_SHIFT)
		//	{
		//		io->KeyShift = true;
		//	}
		//	break;
		//}
		//case WM_SYSKEYUP:
		//{
		//	const int vkey = (int)wParam;
		//	io->KeysDown[vkey] = 0;
		//	if (vkey == VK_CONTROL)
		//	{
		//		io->KeyCtrl = false;
		//	}
		//	else if (vkey == VK_MENU)
		//	{
		//		io->KeyAlt = false;
		//	}
		//	else if (vkey == VK_SHIFT)
		//	{
		//		io->KeyShift = false;
		//	}
		//	break;
		//}
		//case WM_SYSKEYDOWN:
		//{
		//	const int vkey = (int)wParam;
		//	io->KeysDown[vkey] = 1;
		//	if (vkey == VK_CONTROL)
		//	{
		//		io->KeyCtrl = true;
		//	}
		//	else if (vkey == VK_MENU)
		//	{
		//		io->KeyAlt = true;
		//	}
		//	else if (vkey == VK_SHIFT)
		//	{
		//		io->KeyShift = true;
		//	}
		//	break;
		//}
		//case WM_ACTIVATEAPP:
		//{
		//	if (!wParam)
		//	{
		//		io->KeysDown[arc_global_mod1] = false;
		//		io->KeysDown[arc_global_mod2] = false;
		//	}
		//	break;
		//}
		//case WM_CHAR:
		//	// You can also use ToAscii()+GetKeyboardState() to retrieve characters.
		//	if (wParam > 0 && wParam < 0x10000)
		//		//io.AddInputCharacterUTF16((unsigned short)wParam);
		//		//AddInputCharacter
		//		io->AddInputCharacterUTF16((unsigned short)wParam);
		//	break;
		////break;
		//}




		return uMsg;
	}

	uintptr_t mod_combat(cbtevent* ev, ag* src, ag* dst, char* skillname, uint64_t id, uint64_t revision)
	{
		return 0;
	}
	bool LoadFontResource(HMODULE hModule, UINT resId, void*& dataPtr, size_t& dataSize)
	{
		auto res = FindResource(hModule, MAKEINTRESOURCE(resId), RT_RCDATA);
		if (res)
		{
			auto handle = LoadResource(hModule, res);
			if (handle)
			{
				dataSize = SizeofResource(hModule, res);
				dataPtr = LockResource(handle);

				return true;
			}
		}

		return false;
	}
	IDirect3DTexture9* CreateTextureFromResource(IDirect3DDevice9* pDev, HMODULE hModule, unsigned uResource)
	{
		void* dataPtr;
		size_t dataSz;

		if (!LoadFontResource(hModule, uResource, dataPtr, dataSz))
			return 0;

		IDirect3DBaseTexture9* ret = NULL;

		CreateDDSTextureFromMemory(pDev, dataPtr, dataSz, &ret);

		return (IDirect3DTexture9*)ret;
	}
	void readArcExports()
	{
		uint64_t e6_result = arc_export_e6();
		uint64_t e7_result = arc_export_e7();

		arc_hide_all = (e6_result & 0x01);
		arc_panel_always_draw = (e6_result & 0x02);
		arc_movelock_altui = (e6_result & 0x04);
		arc_clicklock_altui = (e6_result & 0x08);
		arc_window_fastclose = (e6_result & 0x10);


		uint16_t* ra = (uint16_t*)&e7_result;
		if (ra)
		{
			arc_global_mod1 = ra[0];
			arc_global_mod2 = ra[1];
			arc_global_mod_multi = ra[2];
		}
	}
	void parseIni()
	{
		SI_Error rc = table_ini.LoadFile("addons\\arcdps\\arcdps_sytool.ini");
		valid_table_ini = rc >= 0;

		std::string pszValue = table_ini.GetValue("allkeys", "show_settingui_key", "80");
		setting_key = std::stoi(pszValue);//按键读取

		isfistutose = table_ini.GetBoolValue("setting", "first_time_to_use", true);
	}
	void writeIni()
	{
		SI_Error rc = table_ini.SetValue("allkeys", "show_settingui_key", std::to_string(setting_key).c_str());//按键储存
		rc = table_ini.SetBoolValue("setting", "first_time_to_use", isfistutose);//按键储存
		rc = table_ini.SaveFile("addons\\arcdps\\arcdps_sytool.ini");
	}

	uintptr_t mod_imgui(uint32_t not_charsel_or_loading)
	{

		if (!not_charsel_or_loading) return 0;

		readArcExports();

		auto const io = &ImGui::GetIO();

		if (io->KeysDown[arc_global_mod1] && io->KeysDown[arc_global_mod2])
		{
			if (ImGui::IsKeyPressed((int)setting_key))
			{
				Show_settingwindows = !Show_settingwindows;
			}
		}

		if (Show_settingwindows)
		{
			seting.ShowUI(&Show_settingwindows);
		}

		return 0;
	}

	arcdps_exports* mod_init() {
		//AllocConsole();
		memset(&arc_exports, 0, sizeof(arcdps_exports));
		arc_exports.sig = 0xE3C07A26;
		arc_exports.imguivers = IMGUI_VERSION_NUM;
		arc_exports.size = sizeof(arcdps_exports);

		if (langseting == 1)
		{
			arc_exports.out_name = u8"神油工具-配方搜索插件";
		}
		else
		{
			arc_exports.out_name = "SY tool";
		}
		arc_exports.out_build = "0.2";
		arc_exports.wnd_nofilter = mod_wnd;
		//arc_exports.combat = mod_combat;
		arc_exports.imgui = mod_imgui;
		arc_exports.options_end = mod_options;
		return &arc_exports;
		
	}

	uintptr_t mod_options()
	{

		if (langseting == 1)
		{
			if (ImGui::Button(u8"神油工具-配方搜索插件"))
			{
				Show_settingwindows = true;
			}
		}
		else
		{
			if (ImGui::Button("SY tool"))
			{
				Show_settingwindows = true;
			}
		}
		return 0;
	}

	void LoadMays_Datas()
	{
		loadMysticForgestat = 1;

#pragma region 从文件加载
		//filebuf* pbuf;
		//ifstream filestr;
		//long size;
		//char* buffer;
		//char buffe_r[MAX_PATH];
		//GetModuleFileNameA(NULL, buffe_r, MAX_PATH);
		//string::size_type pos = string(buffe_r).find_last_of("\\/");
		//string currentPath = string(buffe_r).substr(0, pos);
		//CHAR fullPath[MAX_PATH];
		//sprintf_s(fullPath, "%s\\addons\\itemdata\\%s", currentPath.c_str(), "mysdata.json");
		//filestr.open(fullPath, ios::binary);
		//pbuf = filestr.rdbuf();
		//size = pbuf->pubseekoff(0, ios::end, ios::in);
		//pbuf->pubseekpos(0, ios::in);
		//buffer = new char[size];
		//pbuf->sgetn(buffer, size);
		//filestr.close();
		//std::string cur_str = buffer ;
		//MysData data2 = nlohmann::json::parse(cur_str);
		//data = data2;
		//delete[]buffer;
		//isloadmaysOK = 2;
#pragma endregion

		try
		{
			auto res = FindResource(dll_Module, MAKEINTRESOURCE(IDR_MYSDATA), RT_RCDATA);
			if (res)
			{
				auto handle = LoadResource(dll_Module, res);
				if (handle)
				{
					DWORD dataSize = SizeofResource(dll_Module, res);
					LPVOID dataPtr = LockResource(handle);
					BYTE* pByte = new BYTE[(int)dataSize + 1];
					memcpy(pByte, dataPtr, dataSize);
					char* p = new char[dataSize];
					memcpy(p, pByte, dataSize);
					p[dataSize] = 0;
					std::string cur_s(p);
					MyData_MysticForge::MysticForge data2 = nlohmann::json::parse(cur_s);
					data_MysticForge = data2;
					loadMysticForgestat = 2;
					GlobalUnlock(handle);
				}
				else
				{
					loadMysticForgestat = 4;
				}
			}
			else
			{
				loadMysticForgestat = 3;
			}
		}
		catch (...)
		{
			loadMysticForgestat = 5;
		}


	}
	void Loaditems_Datas()
	{
		loaditemdatestat = 1;
		try
		{
			auto res = FindResource(dll_Module, MAKEINTRESOURCE(IDR_ITEMDATA), RT_RCDATA);
			if (res)
			{
				auto handle = LoadResource(dll_Module, res);
				if (handle)
				{
					DWORD dataSize = SizeofResource(dll_Module, res);
					LPVOID dataPtr = LockResource(handle);
					BYTE* pByte = new BYTE[(int)dataSize + 1];
					memcpy(pByte, dataPtr, dataSize);
					char* p = new char[dataSize];
					memcpy(p, pByte, dataSize);
					p[dataSize] = 0;
					std::string cur_s(p);
					MyData_Items::Items data2 = nlohmann::json::parse(cur_s);
					data_Items = data2;
					loaditemdatestat = 2;
					GlobalUnlock(handle);
				}
				else
				{
					loaditemdatestat = 4;
				}
			}
			else
			{
				loaditemdatestat = 3;
			}
		}
		catch (...)
		{
			loaditemdatestat = 5;
		}

	}
	void Loadrecis_Datas()
	{

		loadRecipesstat = 1;
		try
		{
			auto res = FindResource(dll_Module, MAKEINTRESOURCE(IDR_RECIDATA), RT_RCDATA);
			if (res)
			{
				auto handle = LoadResource(dll_Module, res);
				if (handle)
				{
					DWORD dataSize = SizeofResource(dll_Module, res);
					LPVOID dataPtr = LockResource(handle);
					BYTE* pByte = new BYTE[(int)dataSize + 1];
					memcpy(pByte, dataPtr, dataSize);
					char* p = new char[dataSize];
					memcpy(p, pByte, dataSize);
					p[dataSize] = 0;
					std::string cur_s(p);
					MyData_Recipes::Recipes data2 = nlohmann::json::parse(cur_s);
					data_Recipes = data2;
					loadRecipesstat = 2;
					GlobalUnlock(handle);
				}
				else
				{
					loadRecipesstat = 4;
				}
			}
			else
			{
				loadRecipesstat = 3;
			}
		}
		catch (...)
		{
			loadRecipesstat = 5;
		}

	}

	template<typename charT>
	struct my_equal {
		my_equal(const std::locale& loc) : loc_(loc) {}
		bool operator()(charT ch1, charT ch2) {
			return std::toupper(ch1, loc_) == std::toupper(ch2, loc_);
		}
	private:
		const std::locale& loc_;
	};

	// find substring (case insensitive)
	template<typename T>
	int ci_find_substr(const T& str1, const T& str2, const std::locale& loc = std::locale())
	{
		typename T::const_iterator it = std::search(str1.begin(), str1.end(),
			str2.begin(), str2.end(), my_equal<typename T::value_type>(loc));
		if (it != str1.end()) return it - str1.begin();
		else return -1; // not found
	}


	void fuzi(int otdd) 
	{
		
		//for (size_t i = 0; i < Articles_found.size(); i++)
		{
			if (Articles_found[otdd].get_ro()[0] != 0)
			{
				for (size_t j = 0; j < Articles_found[otdd].get_ro().size(); j++)
				{
					for (size_t t = 0; t < data_Recipes.size(); t++)
					{
						if (Articles_found[otdd].get_ro()[j] == data_Recipes[t].get_id())
						{
							selectedRecipes.push_back(data_Recipes[t]);//存储找到的副业配方
							for (size_t outitem = 0; outitem < data_Items.size(); outitem++)
							{
								if (data_Recipes[t].get_ot() == data_Items[outitem].get_id())
								{
									selected_of_out_item_Recipes.push_back(data_Items[outitem]);//储存该配方的输出物品
								}
							}
						}
					}
				}
			}
			if (Articles_found[otdd].get_rt()[0] != 0)
			{
				for (size_t j = 0; j < Articles_found[otdd].get_rt().size(); j++)
				{
					for (size_t t = 0; t < data_Recipes.size(); t++)
					{
						if (Articles_found[otdd].get_rt()[j] == data_Recipes[t].get_id())
						{
							selectedRecipes.push_back(data_Recipes[t]);//存储找到的副业配方
							for (size_t outitem = 0; outitem < data_Items.size(); outitem++)
							{
								if (data_Recipes[t].get_ot() == data_Items[outitem].get_id())
								{
									selected_of_out_item_Recipes.push_back(data_Items[outitem]);//储存该配方的输出物品
								}
							}
						}
					}
				}
			}
			if (Articles_found[otdd].get_mo()[0] != 999999)
			{
				for (size_t j = 0; j < Articles_found[otdd].get_mo().size(); j++)
				{
					for (size_t t = 0; t < data_MysticForge.size(); t++)
					{
						if (Articles_found[otdd].get_mo()[j] == data_MysticForge[t].get_id())
						{
							selectedMysticForge.push_back(data_MysticForge[t]);//存储找到的马桶配方
							for (size_t outitem = 0; outitem < data_Items.size(); outitem++)
							{
								if (data_MysticForge[t].get_ot() == data_Items[outitem].get_id())
								{
									selected_of_out_item_MysticForge.push_back(data_Items[outitem]);//储存该配方的输出物品
								}
							}
						}
					}
				}
			}
			if (Articles_found[otdd].get_mt()[0] != 999999)
			{
				for (size_t j = 0; j < Articles_found[otdd].get_mt().size(); j++)
				{
					for (size_t t = 0; t < data_MysticForge.size(); t++)
					{
						if (Articles_found[otdd].get_mt()[j] == data_MysticForge[t].get_id())
						{
							selectedMysticForge.push_back(data_MysticForge[t]);//存储找到的马桶配方
							for (size_t outitem = 0; outitem < data_Items.size(); outitem++)
							{
								if (data_MysticForge[t].get_ot() == data_Items[outitem].get_id())
								{
									selected_of_out_item_MysticForge.push_back(data_Items[outitem]);//储存该配方的输出物品
								}
							}
						}
					}
				}
			}
		}
		Selected_item = otdd; //给下一阶段判断
	}




	SY_settings::SY_settings()
	{
	}
	SY_settings::~SY_settings()
	{
	}
#pragma region MyRegion
	//LPCWSTR SY_settings::stringToLPCWSTR(std::string orig)
	//{
	//	size_t origsize = orig.length() + 1;
	//	const size_t newsize = 100;
	//	size_t convertedChars = 0;
	//	wchar_t* wcstring = (wchar_t*)malloc(sizeof(wchar_t) * (orig.length() - 1));
	//	mbstowcs_s(&convertedChars, wcstring, origsize, orig.c_str(), _TRUNCATE);
	//	return wcstring;
	//}

	//void SY_settings::GET_json_toit()
	//{
	//	wancheng = false;
	//	std::string retVal;
	//	std::string _a = "http://do.gw2.kongzhong.com/gw2task/completes?date=";

	//	time_t time_seconds = time(0);
	//	GetTime();
	//	std::string _b = datenow;
	//	std::string _tmp = _a + _b + "&_=" + std::to_string(time_seconds);

	//	DWORD response_length = 0;
	//	LPCWSTR ul = stringToLPCWSTR(_tmp);

	//	try
	//	{
	//		HINTERNET hSession = InternetOpen(ul, INTERNET_OPEN_TYPE_PRECONFIG, NULL, NULL, 0);
	//		if (hSession != NULL)
	//		{
	//			HINTERNET handle2 = InternetOpenUrl(hSession, ul, NULL, 0, INTERNET_FLAG_RELOAD | INTERNET_FLAG_PRAGMA_NOCACHE | INTERNET_FLAG_NO_CACHE_WRITE, 0);
	//			if (handle2 != NULL)
	//			{
	//				char response_data[8000];//缓冲区
	//				if (InternetReadFile(handle2, response_data, sizeof(response_data) - 1, &response_length) && response_length > 0)
	//				{
	//					response_data[response_length] = '\0';
	//					response_length++;
	//				}
	//				if (response_length > 0)
	//				{
	//					response_data[response_length] = '\0';
	//				}
	//				else
	//				{
	//					response_data[0] = '\0';
	//				}
	//				retVal = response_data;
	//				InternetCloseHandle(handle2);
	//				handle2 = NULL;
	//			}
	//			else
	//			{
	//				result[0] = "0";
	//				geting = false;
	//				wancheng = true;
	//				return;
	//			}
	//			InternetCloseHandle(hSession);
	//			hSession = NULL;
	//		}
	//		else
	//		{
	//			result[0] = "0";
	//			geting = false;
	//			wancheng = true;
	//			return;
	//		}
	//	}
	//	catch (...)
	//	{

	//	}

	//	if (!getjsonstring(retVal))result[0] = "0";
	//	geting = false;
	//	wancheng = true;

	//}

	//void SY_settings::GetTime()
	//{
	//	time_t time_seconds = time(0);
	//	tm now;
	//	localtime_s(&now, &time_seconds);
	//	sprintf_s(datenow, u8"%d-%02d-%02d", now.tm_year + 1900, now.tm_mon + 1, now.tm_mday);
	//}

	//bool SY_settings::is_not_dateofstada() 
	//{
	//	time_t time_seconds = time(0);
	//	tm now;
	//	char datenow2[64];
	//	localtime_s(&now, &time_seconds);
	//	sprintf_s(datenow2, u8"%d-%02d-%02d", now.tm_year + 1900, now.tm_mon + 1, now.tm_mday);
	//	return datenow == datenow2 ? true : false;
	//}

	//bool SY_settings::getjsonstring(std::string inits)
	//{
	//	const auto rawJsonLength = static_cast<int>(inits.length());
	//	constexpr bool shouldUseOldWay = false;
	//	JSONCPP_STRING err;
	//	Json::Value root;

	//	Json::CharReaderBuilder builder;
	//	const std::unique_ptr<Json::CharReader> reader(builder.newCharReader());
	//	if (!reader->parse(inits.c_str(), inits.c_str() + rawJsonLength, &root,
	//		&err)) {
	//		return false;
	//	}
	//	if (!root["data"].isArray())
	//	{
	//		return false;
	//	}
	//	if (root["data"].isArray())
	//	{
	//		int nArraySize = root["data"].size();
	//		for (int i = 0; i < nArraySize; i++)
	//		{
	//			result[i] = root["data"][i]["task_name"].asString();
	//		}
	//	}
	//	return true;
	//}

#pragma endregion

	//vector<string> searchresults;
	//vector<string> searchresults_cl;
	//vector<int> searchresults_id;
	//vector<int> searchresults_zg;
	//vector<int> searchresults_tp;

	//vector<int> cailiao_id;
	//vector<int> cailiao_ct;
	//int Selected_item = -1;
	

	void SY_settings::websiye()
	{
		::wkeSetWkeDllPath(L".\\addons\\miniblink_x64.dll");

		::wkeInitialize();

		windowof = ::wkeCreateWebWindow(WKE_WINDOW_TYPE_TRANSPARENT, NULL, 0, 0, 1080, 680);

		::wkeShowWindow(windowof, TRUE);

		::wkeLoadURL(windowof,"http://news.sina.com.cn");
	}


	void SY_settings::ShowUI(bool* INITbool)
	{
		if (INITbool && loadMysticForgestat == 0)
		{
			loadMysticForgestat = 1;
			boost::thread thServer2(LoadMays_Datas);
		}
		if (INITbool && loadRecipesstat == 0)
		{
			loadRecipesstat = 1;
			boost::thread thServer3(Loadrecis_Datas);
		}
		if (INITbool && loaditemdatestat == 0)
		{
			loaditemdatestat = 1;
			boost::thread thServer1(Loaditems_Datas);
			
		}

		ImGui::PushFont(ImGui::GetIO().Fonts->Fonts.back());


		settingALLlang();//显示文本赋值
		if (isfistutose)
		{
			ImGui::SetNextWindowPos(ImVec2(ImGui::GetIO().DisplaySize.x / 2, ImGui::GetIO().DisplaySize.y / 2), 1, ImVec2(0.5f, 0.5f));
			ImGui::SetNextWindowSize(ImVec2(600.0f, 400.0f));
			isfistutose = false;
		}
		ImGui::Begin(alllang_[0].c_str(), INITbool);   // Pass a pointer to our bool variable (the window will have a closing button that will clear the bool when clicked)



		if (langseting == 1)
		{
			//ImGui::Begin(u8"神油工具_配方搜索插件", INITbool);
			if (loaditemdatestat == 1)ImGui::Text(u8"正在加载物品数据库");
			if (loaditemdatestat > 2)
			{
				ImGui::Text(u8"加载物品数据库失败:错误代码=%d", loaditemdatestat);
				ImGui::SameLine();
				if (ImGui::Button(u8"重新加载物品数据库"))
				{
					loaditemdatestat = 0;
				}
			}
			if (loadRecipesstat == 1)ImGui::Text(u8"正在加载配方数据库");
			if (loadRecipesstat > 2)
			{
				ImGui::Text(u8"加载配方数据库失败:错误代码=%d", loadRecipesstat);
				ImGui::SameLine();
				if (ImGui::Button(u8"重新加载配方数据库"))
				{
					loadRecipesstat = 0;
				}
			}
			if (loadMysticForgestat == 1)ImGui::Text(u8"正在加载马桶数据库");
			if (loadMysticForgestat > 2)
			{
				ImGui::Text(u8"加载数据库失败:错误代码=%d", loadMysticForgestat);
				ImGui::SameLine();
				if (ImGui::Button(u8"重新加载马桶数据库"))
				{
					loadMysticForgestat = 0;
				}
			}
		}
		else
		{
			//ImGui::Begin("SY_Recipe Search Plugin", INITbool);
			if (loaditemdatestat == 1)ImGui::Text("Loading the Item database");
			if (loaditemdatestat > 2)
			{
				ImGui::Text("Failed to load item database: error code=%d", loaditemdatestat);
				ImGui::SameLine();
				if (ImGui::Button("Reload the item database"))
				{
					loaditemdatestat = 0;
				}
			}
			if (loadRecipesstat == 1)ImGui::Text("Loading The Recipe database");
			if (loadRecipesstat > 2)
			{
				ImGui::Text("Failed to load Recipe database: error code=%d", loadRecipesstat);
				ImGui::SameLine();
				if (ImGui::Button("Reload the Recipe database"))
				{
					loadRecipesstat = 0;
				}
			}
			if (loadMysticForgestat == 1)ImGui::Text("Loading The Mystic Forge database");
			if (loadMysticForgestat > 2)
			{
				ImGui::Text("Failed to load Mystic Forge database: error code=%d", loadMysticForgestat);
				ImGui::SameLine();
				if (ImGui::Button("Reload the Mystic Forge database"))
				{
					loadMysticForgestat = 0;
				}
			}
		}

#pragma region 数据库加载反馈







		ImGui::BeginGroup();

		static std::string inputstring;

		ImGui::SetNextItemWidth(100.0f);
		ImGui::InputText(u8"", &inputstring);
		wanttoinput = ImGui::IsItemFocused();
		ImGui::SameLine();
		if (ImGui::Button(alllang_[2].c_str()))  //搜索物品
		{
			if (inputstring != "")
			{
				Articles_found.clear();
				Selected_item = -1;

				for (size_t i = 0; i < data_Items.size(); i++)
				{
					if (langseting == 1)
					{
						if (data_Items[i].get_na().find(inputstring) != std::string::npos)
						{
							Articles_found.push_back(data_Items[i]);  //找到的物品
						}
					}
					else
					{
						if (ci_find_substr(data_Items[i].get_nae(), inputstring) != -1)
						{
							Articles_found.push_back(data_Items[i]);  //找到的物品
						}
					}
				}
			}
			//搜索物品
		}
		//根据搜索到的物品显示列表:
		ImGuiWindowFlags window_flags = ImGuiWindowFlags_HorizontalScrollbar;
		ImGui::PushStyleVar(ImGuiStyleVar_ChildRounding, 5.0f);
		ImGui::BeginChild("ChildL1", ImVec2(ImGui::GetWindowContentRegionWidth() * 0.4f, 270), false, window_flags);
		ImGui::Columns(1);
		for (size_t i = 0; i < Articles_found.size(); i++)
		{
			//根据搜索值显示左边列表(显示找到的物品)
			char buf[256];
			if (langseting == 1)
			{
				sprintf_s(buf, "%s##%03d_c", Articles_found[i].get_na().c_str(), (int)i);
			}
			else
			{
				sprintf_s(buf, "%s##%03d_e", Articles_found[i].get_nae().c_str(), (int)i);
			}
			ImGui::PushStyleColor(ImGuiCol_Text, PZ_color(Articles_found[i].get_ra()));
			if (ImGui::Button(buf, ImVec2(-FLT_MIN, 0.0f)))
			{
				Selected_item = -1;
				selectedRecipes.clear();
				selectedMysticForge.clear();
				//清空选定的配方排序
				Selected_Recipes = -1;
				Selected_MysticForge = -1;
				//清空选定的配方
				selected_of_materials_item.clear();//材料储存
				selected_of_out_item_Recipes.clear(); //选中的配方输出物品储存
				selected_of_out_item_MysticForge.clear();//选中的马桶输出物品储存

				boost::thread thServer4(fuzi , (int)i);

				////重置参数
				////清空找到的相关配方
				//selectedRecipes.clear();
				//selectedMysticForge.clear();
				////清空选定的配方排序
				//Selected_Recipes = -1;
				//Selected_MysticForge = -1;
				////清空选定的配方
				//selected_of_materials_item.clear();//材料储存
				//selected_of_out_item_Recipes.clear(); //选中的配方输出物品储存
				//selected_of_out_item_MysticForge.clear();//选中的马桶输出物品储存

				//if (Articles_found[i].get_ro()[0] != 0)
				//{
				//	for (size_t j = 0; j < Articles_found[i].get_ro().size(); j++)
				//	{
				//		for (size_t t = 0; t < data_Recipes.size(); t++)
				//		{
				//			if (Articles_found[i].get_ro()[j] == data_Recipes[t].get_id())
				//			{
				//				selectedRecipes.push_back(data_Recipes[t]);//存储找到的副业配方
				//				for (size_t outitem = 0; outitem < data_Items.size(); outitem++)
				//				{
				//					if (data_Recipes[t].get_ot() == data_Items[outitem].get_id())
				//					{
				//						selected_of_out_item_Recipes.push_back(data_Items[outitem]);//储存该配方的输出物品
				//					}
				//				}
				//			}
				//		}
				//	}
				//}
				//if (Articles_found[i].get_rt()[0] != 0)
				//{
				//	for (size_t j = 0; j < Articles_found[i].get_rt().size(); j++)
				//	{
				//		for (size_t t = 0; t < data_Recipes.size(); t++)
				//		{
				//			if (Articles_found[i].get_rt()[j] == data_Recipes[t].get_id())
				//			{
				//				selectedRecipes.push_back(data_Recipes[t]);//存储找到的副业配方
				//				for (size_t outitem = 0; outitem < data_Items.size(); outitem++)
				//				{
				//					if (data_Recipes[t].get_ot() == data_Items[outitem].get_id())
				//					{
				//						selected_of_out_item_Recipes.push_back(data_Items[outitem]);//储存该配方的输出物品
				//					}
				//				}
				//			}
				//		}
				//	}
				//}
				//if (Articles_found[i].get_mo()[0] != 999999)
				//{
				//	for (size_t j = 0; j < Articles_found[i].get_mo().size(); j++)
				//	{
				//		for (size_t t = 0; t < data_MysticForge.size(); t++)
				//		{
				//			if (Articles_found[i].get_mo()[j] == data_MysticForge[t].get_id())
				//			{
				//				selectedMysticForge.push_back(data_MysticForge[t]);//存储找到的马桶配方
				//				for (size_t outitem = 0; outitem < data_Items.size(); outitem++)
				//				{
				//					if (data_MysticForge[t].get_ot() == data_Items[outitem].get_id())
				//					{
				//						selected_of_out_item_MysticForge.push_back(data_Items[outitem]);//储存该配方的输出物品
				//					}
				//				}
				//			}
				//		}
				//	}
				//}
				//if (Articles_found[i].get_mt()[0] != 999999)
				//{
				//	for (size_t j = 0; j < Articles_found[i].get_mt().size(); j++)
				//	{
				//		for (size_t t = 0; t < data_MysticForge.size(); t++)
				//		{
				//			if (Articles_found[i].get_mt()[j] == data_MysticForge[t].get_id())
				//			{
				//				selectedMysticForge.push_back(data_MysticForge[t]);//存储找到的马桶配方
				//				for (size_t outitem = 0; outitem < data_Items.size(); outitem++)
				//				{
				//					if (data_MysticForge[t].get_ot() == data_Items[outitem].get_id())
				//					{
				//						selected_of_out_item_MysticForge.push_back(data_Items[outitem]);//储存该配方的输出物品
				//					}
				//				}
				//			}
				//		}
				//	}
				//}
			}
			ImGui::PopStyleColor(1);
			//弹出窗口提示
			if (ImGui::IsItemHovered())
			{
				ImGui::BeginTooltip();
				ImGui::PushTextWrapPos(ImGui::GetFontSize() * 25.0f);
				if (langseting == 1)
				{
					ImGui::TextUnformatted(Articles_found[i].get_na().c_str());
					ImGui::TextUnformatted((u8"描述:\n" + Articles_found[i].get_de()).c_str());
					ImGui::Text(u8"等级: %d    类型:%s", Articles_found[i].get_lv(), TP_yp(Articles_found[i].get_tp()));
					if (Articles_found[i].get_ro()[0] == 0 && Articles_found[i].get_rt()[0] == 0 && Articles_found[i].get_mo()[0] == 999999 && Articles_found[i].get_mt()[0] == 999999)
					{
						ImGui::Text(u8"无关联配方公式");
					}
				}
				else
				{
					ImGui::TextUnformatted(Articles_found[i].get_nae().c_str());
					ImGui::TextUnformatted(("Description:\n" + Articles_found[i].get_dee()).c_str());
					ImGui::Text("level: %d   Type:%s", Articles_found[i].get_lv(), TP_yp(Articles_found[i].get_tp()));
					if (Articles_found[i].get_ro()[0] == 0 && Articles_found[i].get_rt()[0] == 0 && Articles_found[i].get_mo()[0] == 999999 && Articles_found[i].get_mt()[0] == 999999)
					{
						ImGui::Text("Unassociated formula");
					}
				}
				ImGui::PopTextWrapPos();
				ImGui::EndTooltip();
			}
			ImGui::NextColumn();
		}
		ImGui::PopStyleVar();
		ImGui::EndChild();


		ImGui::EndGroup();
		ImGui::SameLine();

		ImGui::BeginGroup();//1
		//
		ImGui::SetNextItemWidth(100.0f);
		if (Selected_item != -1)
		{
			//顶端显示物品
			ImGui::PushStyleColor(ImGuiCol_Text, PZ_color(Articles_found[Selected_item].get_ra()));

			if (ImGui::Button(langseting == 1 ? Articles_found[Selected_item].get_na().c_str() : Articles_found[Selected_item].get_nae().c_str(), ImVec2(-1, 0)))
			{
				ImGui::SetClipboardText(Articles_found[Selected_item].get_cl().c_str());
			}

			ImGui::PopStyleColor(1);
			//弹出提示
			if (ImGui::IsItemHovered())
			{
				ImGui::BeginTooltip();
				ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);
				ImGui::TextUnformatted(((langseting == 1 ? Articles_found[Selected_item].get_na() : Articles_found[Selected_item].get_nae()) + "\r\n" + alllang_[4]).c_str());
				ImGui::PopTextWrapPos();
				ImGui::EndTooltip();
			}

			if (langseting == 1)
			{
				ImGui::Text(u8"等级: %d   类型:%s   品质:%s", Articles_found[Selected_item].get_lv(), TP_yp(Articles_found[Selected_item].get_tp()), ZG_zg(Articles_found[Selected_item].get_ra()));
			}
			else
			{
				ImGui::Text("level: %d  Type:%s   quality:%s", Articles_found[Selected_item].get_lv(), TP_yp(Articles_found[Selected_item].get_tp()), ZG_zg(Articles_found[Selected_item].get_ra()));
			}

			ImGui::BeginGroup();//12
			ImGui::Text(alllang_[7].c_str());
			ImGui::PushStyleVar(ImGuiStyleVar_ChildRounding, 5.0f);
			ImGui::BeginChild("ChildR1", ImVec2(ImGui::GetWindowContentRegionWidth() * 0.3f, 250), true);


			if (selectedRecipes.size() > 0 || selectedMysticForge.size() > 0)
			{
				ImGui::Columns(1);

				if (selectedRecipes.size() > 0)
				{
					ImGui::Text(alllang_[5].c_str());
					for (size_t i = 0; i < selectedRecipes.size(); i++)
					{
						char buf[128];
						sprintf_s(buf, u8"%s##pf%03d", langseting == 1 ? selected_of_out_item_Recipes[i].get_na().c_str() : selected_of_out_item_Recipes[i].get_nae().c_str(), (int)i + 324);
						if (ImGui::Button(buf, ImVec2(-FLT_MIN, 0.0f)))
						{
							Selected_MysticForge = -1;
							Selected_Recipes = (int)i;
							//清空选定的配方
							selected_of_materials_item.clear();//材料储存
							for (size_t t = 0; t < data_Items.size(); t++)
							{
								if (selectedRecipes[i].get_itms()[0].get_a() != 0 && selectedRecipes[i].get_itms()[0].get_a() == data_Items[t].get_id())
								{
									selected_of_materials_item.push_back(data_Items[t]);
								}
								if (selectedRecipes[i].get_itms()[1].get_a() != 0 && selectedRecipes[i].get_itms()[1].get_a() == data_Items[t].get_id())
								{
									selected_of_materials_item.push_back(data_Items[t]);
								}
								if (selectedRecipes[i].get_itms()[2].get_a() != 0 && selectedRecipes[i].get_itms()[2].get_a() == data_Items[t].get_id())
								{
									selected_of_materials_item.push_back(data_Items[t]);
								}
								if (selectedRecipes[i].get_itms()[3].get_a() != 0 && selectedRecipes[i].get_itms()[3].get_a() == data_Items[t].get_id())
								{
									selected_of_materials_item.push_back(data_Items[t]);
								}
							}

						}
						//弹出提示
						if (ImGui::IsItemHovered())
						{
							ImGui::BeginTooltip();
							ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);
							ImGui::TextUnformatted(((langseting == 1 ? selected_of_out_item_Recipes[i].get_na() : selected_of_out_item_Recipes[i].get_nae()) + "\r\n" + alllang_[8]).c_str());
							ImGui::PopTextWrapPos();
							ImGui::EndTooltip();
						}
						ImGui::NextColumn();
					}
				}

				if (selectedMysticForge.size() > 0)
				{
					ImGui::Text(alllang_[6].c_str());
					for (size_t i = 0; i < selectedMysticForge.size(); i++)
					{
						char buf[128];
						sprintf_s(buf, u8"%s##mf%03d", langseting == 1 ? selected_of_out_item_MysticForge[i].get_na().c_str() : selected_of_out_item_MysticForge[i].get_nae().c_str(), (int)i);
						if (ImGui::Button(buf, ImVec2(-FLT_MIN, 0.0f)))
						{
							Selected_Recipes = -1;
							Selected_MysticForge = (int)i;

							selected_of_materials_item.clear();//材料储存
							for (size_t t = 0; t < data_Items.size(); t++)
							{
								if (selectedMysticForge[i].get_itms()[0].get_a() != 999999 && selectedMysticForge[i].get_itms()[0].get_a() == data_Items[t].get_id())
								{
									selected_of_materials_item.push_back(data_Items[t]);
								}
								if (selectedMysticForge[i].get_itms()[1].get_a() != 999999 && selectedMysticForge[i].get_itms()[1].get_a() == data_Items[t].get_id())
								{
									selected_of_materials_item.push_back(data_Items[t]);
								}
								if (selectedMysticForge[i].get_itms()[2].get_a() != 999999 && selectedMysticForge[i].get_itms()[2].get_a() == data_Items[t].get_id())
								{
									selected_of_materials_item.push_back(data_Items[t]);
								}
								if (selectedMysticForge[i].get_itms()[3].get_a() != 999999 && selectedMysticForge[i].get_itms()[3].get_a() == data_Items[t].get_id())
								{
									selected_of_materials_item.push_back(data_Items[t]);
								}
							}
						}
						//弹出提示
						if (ImGui::IsItemHovered())
						{
							ImGui::BeginTooltip();
							ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);
							ImGui::TextUnformatted(((langseting == 1 ? selected_of_out_item_MysticForge[i].get_na() : selected_of_out_item_MysticForge[i].get_nae()) + "\r\n" + alllang_[8]).c_str());
							ImGui::PopTextWrapPos();
							ImGui::EndTooltip();
						}
						ImGui::NextColumn();
					}
				}
			}


			ImGui::EndChild();
			ImGui::PopStyleVar();
			ImGui::EndGroup();//12

			ImGui::SameLine();

			ImGui::BeginGroup();//14

			//ImGui::BeginChild("ChildR2", ImVec2(ImGui::GetWindowContentRegionWidth() * 0.3f, 230), true);
			if (Selected_Recipes != -1)
			{
				std::string fuyejihe = langseting == 1 ? u8"制作:" : "Make:";

				if (std::count(selectedRecipes[Selected_Recipes].get_av().begin(), selectedRecipes[Selected_Recipes].get_av().end(), 1))
				{
					fuyejihe += langseting == 1 ? u8"工艺," : "Artificer,";
				}
				if (std::count(selectedRecipes[Selected_Recipes].get_av().begin(), selectedRecipes[Selected_Recipes].get_av().end(), 2))
				{
					fuyejihe += langseting == 1 ? u8"盔甲," : "Armorsmith,";
				}
				if (std::count(selectedRecipes[Selected_Recipes].get_av().begin(), selectedRecipes[Selected_Recipes].get_av().end(), 3))
				{
					fuyejihe += langseting == 1 ? u8"厨师," : "Chef,";
				}
				if (std::count(selectedRecipes[Selected_Recipes].get_av().begin(), selectedRecipes[Selected_Recipes].get_av().end(), 4))
				{
					fuyejihe += langseting == 1 ? u8"猎手," : "Huntsman,";
				}
				if (std::count(selectedRecipes[Selected_Recipes].get_av().begin(), selectedRecipes[Selected_Recipes].get_av().end(), 5))
				{
					fuyejihe += langseting == 1 ? u8"珠宝," : "Jeweler,";
				}
				if (std::count(selectedRecipes[Selected_Recipes].get_av().begin(), selectedRecipes[Selected_Recipes].get_av().end(), 6))
				{
					fuyejihe += langseting == 1 ? u8"皮革," : "Leatherworker,";
				}
				if (std::count(selectedRecipes[Selected_Recipes].get_av().begin(), selectedRecipes[Selected_Recipes].get_av().end(), 7))
				{
					fuyejihe += langseting == 1 ? u8"裁缝," : "Tailor,";
				}
				if (std::count(selectedRecipes[Selected_Recipes].get_av().begin(), selectedRecipes[Selected_Recipes].get_av().end(), 8))
				{
					fuyejihe += langseting == 1 ? u8"武器," : "Weaponsmith,";
				}
				if (std::count(selectedRecipes[Selected_Recipes].get_av().begin(), selectedRecipes[Selected_Recipes].get_av().end(), 9))
				{
					fuyejihe += langseting == 1 ? u8"抄写," : "Scribe,";
				}

				char buf2[128];
				sprintf_s(buf2, u8"->%s##pfotx", langseting == 1 ? selected_of_out_item_Recipes[Selected_Recipes].get_na().c_str() : selected_of_out_item_Recipes[Selected_Recipes].get_nae().c_str());
				ImGui::NewLine();
				ImGui::PushStyleColor(ImGuiCol_Text, PZ_color(selected_of_out_item_Recipes[Selected_Recipes].get_ra()));
				if (ImGui::Button(buf2, ImVec2(-FLT_MIN, 0.0f)))
				{
					std::string totxt = langseting == 1 ? selected_of_out_item_Recipes[Selected_Recipes].get_na().c_str() : selected_of_out_item_Recipes[Selected_Recipes].get_nae().c_str();
					totxt += " ->" + fuyejihe + " ->";
					for (size_t i = 0; i < selectedRecipes[Selected_Recipes].get_itms().size(); i++)
					{
						if (selectedRecipes[Selected_Recipes].get_itms()[i].get_a() != 0)
						{
							totxt += (langseting == 1 ? selected_of_materials_item[i].get_na() : selected_of_materials_item[i].get_nae()) + " * ";
							totxt += std::to_string(selectedRecipes[Selected_Recipes].get_itms()[i].get_b());
							totxt += " ";
						}
					}
					ImGui::SetClipboardText(totxt.c_str());
				}
				ImGui::PopStyleColor(1);
				if (ImGui::IsItemHovered())
				{
					ImGui::BeginTooltip();
					ImGui::Text(alllang_[9].c_str());
					ImGui::EndTooltip();
				}

				ImGui::TextWrapped(fuyejihe.c_str());

				for (size_t i = 0; i < selectedRecipes[Selected_Recipes].get_itms().size(); i++)
				{
					if (selectedRecipes[Selected_Recipes].get_itms()[i].get_a() != 0)
					{
						char buf[128];
						sprintf_s(buf, u8"%s * %d##pfot%03d", langseting == 1 ? selected_of_materials_item[i].get_na().c_str() : selected_of_materials_item[i].get_nae().c_str(), selectedRecipes[Selected_Recipes].get_itms()[i].get_b(), (int)i);
						
						ImGui::PushStyleColor(ImGuiCol_Text, PZ_color(selected_of_materials_item[i].get_ra()));
						if (ImGui::Button(buf, ImVec2(-FLT_MIN, 0.0f)))
						{
							ImGui::SetClipboardText(selected_of_materials_item[i].get_cl().c_str());
						}
						ImGui::PopStyleColor(1);

						if (ImGui::IsItemHovered())
						{
							ImGui::BeginTooltip();
							ImGui::PushTextWrapPos(ImGui::GetFontSize() * 25.0f);
							if (langseting == 1)
							{
								ImGui::TextUnformatted(selected_of_materials_item[i].get_na().c_str());
								ImGui::TextUnformatted((u8"描述:\n" + selected_of_materials_item[i].get_de()).c_str());
								ImGui::Text(u8"等级: %d    类型:%s", selected_of_materials_item[i].get_lv(), TP_yp(selected_of_materials_item[i].get_tp()));
								ImGui::Text(alllang_[4].c_str());
							}
							else
							{
								ImGui::TextUnformatted(selected_of_materials_item[i].get_nae().c_str());
								ImGui::TextUnformatted(("Description:\n" + selected_of_materials_item[i].get_dee()).c_str());
								ImGui::Text("level: %d   Type:%s", selected_of_materials_item[i].get_lv(), TP_yp(selected_of_materials_item[i].get_tp()));
								ImGui::Text(alllang_[4].c_str());
							}
							ImGui::PopTextWrapPos();
							ImGui::EndTooltip();
						}
					}
				}
			}
			if (Selected_MysticForge != -1)
			{

				char buf2[128];
				sprintf_s(buf2, u8"->%s##pfotx", langseting == 1 ? selected_of_out_item_MysticForge[Selected_MysticForge].get_na().c_str() : selected_of_out_item_MysticForge[Selected_MysticForge].get_nae().c_str());
				ImGui::NewLine();
				ImGui::PushStyleColor(ImGuiCol_Text, PZ_color(selected_of_out_item_MysticForge[Selected_MysticForge].get_ra()));
				if (ImGui::Button(buf2, ImVec2(-FLT_MIN, 0.0f)))
				{
					std::string totxt = langseting == 1 ? selected_of_out_item_MysticForge[Selected_MysticForge].get_na().c_str() : selected_of_out_item_MysticForge[Selected_MysticForge].get_nae().c_str();
					totxt += " -> " + alllang_[6];
					for (size_t i = 0; i < selectedMysticForge[Selected_MysticForge].get_itms().size(); i++)
					{
						if (selectedMysticForge[Selected_MysticForge].get_itms()[i].get_a() != 999999)
						{
							totxt += (langseting == 1 ? selected_of_materials_item[i].get_na() : selected_of_materials_item[i].get_nae()) + " * ";
							totxt += std::to_string(selectedMysticForge[Selected_MysticForge].get_itms()[i].get_b());
							totxt += " ";
						}
					}
					ImGui::SetClipboardText(totxt.c_str());
				}
				ImGui::PopStyleColor(1);

				if (ImGui::IsItemHovered())
				{
					ImGui::BeginTooltip();
					ImGui::Text(alllang_[9].c_str());
					ImGui::EndTooltip();
				}



				for (size_t i = 0; i < selectedMysticForge[Selected_MysticForge].get_itms().size(); i++)
				{
					if (selectedMysticForge[Selected_MysticForge].get_itms()[i].get_a() != 999999)
					{
						char buf[128];
						sprintf_s(buf, u8"%s * %d##pfot%03d", langseting == 1 ? selected_of_materials_item[i].get_na().c_str() : selected_of_materials_item[i].get_nae().c_str(), selectedMysticForge[Selected_MysticForge].get_itms()[i].get_b(), (int)i);
						ImGui::PushStyleColor(ImGuiCol_Text, PZ_color(selected_of_materials_item[i].get_ra()));
						if (ImGui::Button(buf, ImVec2(-FLT_MIN, 0.0f)))
						{
							ImGui::SetClipboardText(selected_of_materials_item[i].get_cl().c_str());
						}
						ImGui::PopStyleColor(1);
						if (ImGui::IsItemHovered())
						{
							ImGui::BeginTooltip();
							ImGui::PushTextWrapPos(ImGui::GetFontSize() * 25.0f);
							if (langseting == 1)
							{
								ImGui::TextUnformatted(selected_of_materials_item[i].get_na().c_str());
								ImGui::TextUnformatted((u8"描述:\n" + selected_of_materials_item[i].get_de()).c_str());
								ImGui::Text(u8"等级: %d    类型:%s", selected_of_materials_item[i].get_lv(), TP_yp(selected_of_materials_item[i].get_tp()));
								ImGui::Text(alllang_[4].c_str());
							}
							else
							{
								ImGui::TextUnformatted(selected_of_materials_item[i].get_nae().c_str());
								ImGui::TextUnformatted(("Description:\n" + selected_of_materials_item[i].get_dee()).c_str());
								ImGui::Text("level: %d   Type:%s", selected_of_materials_item[i].get_lv(), TP_yp(selected_of_materials_item[i].get_tp()));
								ImGui::Text(alllang_[4].c_str());
							}
							ImGui::PopTextWrapPos();
							ImGui::EndTooltip();
						}
					}
				}
			}



			ImGui::EndGroup();//14

			//ImGui::EndChild();


		}

		ImGui::EndGroup();//1
		if (loaditemdatestat == 2 && loadRecipesstat == 2 && loadMysticForgestat == 2)
		{
			ImGui::Text(alllang_[1].c_str());
		}
		else
		{
			ImGui::Text(alllang_[3].c_str());
		}
		//ImGui::Text("%d,%d,%d", loaditemdatestat, loadRecipesstat, loadMysticForgestat);
		ImGui::SameLine();
		if (langseting == 1)
		{
			if (ImGui::SmallButton("English"))
			{
				langseting = 0;
			}
		}
		else
		{
			if (ImGui::SmallButton("Chinese"))
			{
				langseting = 1;
			}
		}
		ImGui::End();














		

#pragma endregion

		//ImGui::BeginChild("ChildL", ImVec2(-1, 320));
		//{
		//	ImGui::BeginChild("ChildL1", ImVec2(ImGui::GetWindowContentRegionWidth() * 0.52f, 40));
		//	//static char str0[512] = "";
		//	static std::string str0 = "";
		//	ImGui::InputText(u8"", &str0);
		//	ImGui::SameLine();
		//	if (ImGui::Button(CTXEX2))
		//	//if (ImGui::Button(u8"搜索"))
		//	{
		//		cailiao_id.clear();
		//		cailiao_ct.clear();
		//		string tmp = str0.data();
		//		if (tmp != "")
		//		{
		//			//搜索数据库库
		//			Selected_item = -1;
		//			searchresults.clear();
		//			searchresults_id.clear();
		//			searchresults_zg.clear();
		//			searchresults_cl.clear();
		//			searchresults_tp.clear();
		//			for (size_t i = 0; i < data_it.size(); i++)
		//			{

		//				if (Lang == 0)
		//				{
		//					if (data_it[i].get_ne().find(tmp) != string::npos)
		//					{
		//						searchresults.push_back(data_it[i].get_ne());
		//						searchresults_id.push_back((int)i);
		//						searchresults_zg.push_back((int)data_it[i].get_zg());
		//						searchresults_cl.push_back(data_it[i].get_cl());
		//						searchresults_tp.push_back((int)data_it[i].get_tp());
		//					}
		//				}
		//				else
		//				{
		//					if (ci_find_substr(data_it[i].get_ne(), tmp) != -1)
		//					{
		//						searchresults.push_back(data_it[i].get_ne());
		//						searchresults_id.push_back((int)i);
		//						searchresults_zg.push_back((int)data_it[i].get_zg());
		//						searchresults_cl.push_back(data_it[i].get_cl());
		//						searchresults_tp.push_back((int)data_it[i].get_tp());
		//					}
		//				}

		//			}
		//		}
		//	}
		//	//if (ImGui::Button(u8"搜索"))
		//	//{
		//	//	websiye();
		//	//}
		//	ImGui::EndChild();
		//	ImGuiWindowFlags window_flags = ImGuiWindowFlags_HorizontalScrollbar;
		//	ImGui::PushStyleVar(ImGuiStyleVar_ChildRounding, 5.0f);
		//	ImGui::BeginChild("ChildL2", ImVec2(ImGui::GetWindowContentRegionWidth() * 0.5f, 270), false, window_flags);
		//	ImGui::Columns(1);
		//	for (int i = 0; i < searchresults.size(); i++)
		//	{
		//		//根据搜索值显示左边列表
		//		char buf[256];
		//		sprintf_s(buf, "%s##%03d", searchresults[i].c_str(), i);
		//		ImGui::PushStyleColor(ImGuiCol_Text, PZ_color(searchresults_zg[i]));
		//		if (ImGui::Button(buf, ImVec2(-FLT_MIN, 0.0f)))
		//		{
		//			Selected_item = i;
		//		}
		//		ImGui::PopStyleColor(1);
		//		ImGui::NextColumn();
		//	}
		//	ImGui::PopStyleVar();
		//	ImGui::EndChild();
		//}
		//ImGui::EndChild();


		//ImGui::SameLine(ImGui::GetWindowContentRegionWidth() * 0.52f);
		//ImGui::BeginChild("ChildR", ImVec2(0, 320));


		//ImGui::BeginChild("ChildL1", ImVec2(0, 80));

		////根据列表点击显示物品信息
		//if (Selected_item != -1)
		//{


		//	char buf[256];
		//	sprintf_s(buf, "%s##ll-%03d", searchresults[Selected_item].c_str(), Selected_item);
		//	ImGui::PushStyleColor(ImGuiCol_Text, PZ_color(searchresults_zg[Selected_item]));
		//	if (ImGui::Button(buf, ImVec2(-1, 32)))
		//	{
		//		ImGui::SetClipboardText(searchresults_cl[Selected_item].c_str());
		//	}
		//	ImGui::PopStyleColor(1);


		//	if (Lang == 0)
		//	{
		//		ImGui::Text(u8"物品ID: %d", data_it[searchresults_id[Selected_item]].get_id());

		//		ImGui::Text(u8"物品类型: "); ImGui::SameLine(); ImGui::Text(TP_yp(searchresults_tp[Selected_item])); ImGui::SameLine();

		//		ImGui::Text(u8"  物品品质: "); ImGui::SameLine();
		//	}
		//	else
		//	{
		//		ImGui::Text("Item ID: %d", data_it[searchresults_id[Selected_item]].get_id());

		//		ImGui::Text("Item type: "); ImGui::SameLine(); ImGui::Text(TP_yp(searchresults_tp[Selected_item])); ImGui::SameLine();

		//		ImGui::Text("  Item Quality: "); ImGui::SameLine();
		//	}





		//	
		//	ImGui::PushStyleColor(ImGuiCol_Text, PZ_color(searchresults_zg[Selected_item]));
		//	ImGui::Text(ZG_zg(searchresults_zg[Selected_item]));
		//	ImGui::PopStyleColor(1);
		//}


		//ImGui::EndChild();

		//ImGui::PushStyleVar(ImGuiStyleVar_ChildRounding, 5.0f);
		//ImGui::BeginChild("ChildR1", ImVec2(ImGui::GetWindowContentRegionWidth() * 0.5f, 230), true);
		//ImGui::Columns(1);

		//if (Selected_item != -1)
		//{
		//	if (data_it[searchresults_id[Selected_item]].get_pf().size() > 0)
		//	{
		//		for (size_t i = 0; i < data_it[searchresults_id[Selected_item]].get_pf().size(); i++)
		//		{
		//			char buf[128];
		//			//Recipes

		//			if (Lang == 0)
		//			{
		//				sprintf_s(buf, u8"配方 - %d##pf%03d", (int)data_it[searchresults_id[Selected_item]].get_pf()[i], (int)i);
		//			}
		//			else
		//			{
		//				sprintf_s(buf, "Recipes - %d##pf%03d", (int)data_it[searchresults_id[Selected_item]].get_pf()[i], (int)i);
		//			}

		//			
		//			//
		//			if (ImGui::Button(buf, ImVec2(-FLT_MIN, 0.0f)))
		//			{
		//				cailiao_id.clear();
		//				cailiao_ct.clear();
		//				for (size_t t = 0; t < data_re.size(); t++)
		//				{
		//					if (data_re[t].get_id() == data_it[searchresults_id[Selected_item]].get_pf()[i])
		//					{
		//						for (size_t k = 0; k < data_re[t].get_its().size(); k++)
		//						{
		//							cailiao_id.push_back((int)data_re[t].get_its()[k].get_it());
		//							cailiao_ct.push_back((int)data_re[t].get_its()[k].get_ct());
		//						}
		//					}
		//				}
		//			}
		//			ImGui::NextColumn();
		//		}
		//	}
		//	if (data_it[searchresults_id[Selected_item]].get_mc().size() > 0)
		//	{
		//		for (size_t i = 0; i < data_it[searchresults_id[Selected_item]].get_mc().size(); i++)
		//		{
		//			char buf[128];

		//			if (Lang == 0)
		//			{
		//				sprintf_s(buf, u8"熔炉  %s##mc%03d", data_it[searchresults_id[Selected_item]].get_mc()[i].c_str(), (int)i);
		//			}
		//			else
		//			{
		//				sprintf_s(buf, "MysticForge %s##mc%03d", data_it[searchresults_id[Selected_item]].get_mc()[i].c_str(), (int)i);
		//			}

		//			if (ImGui::Button(buf, ImVec2(-FLT_MIN, 0.0f)))
		//			{
		//				cailiao_id.clear();
		//				cailiao_ct.clear();
		//				for (size_t k = 0; k < data_my.size(); k++)
		//				{
		//					if (data_my[k].get_id() == data_it[searchresults_id[Selected_item]].get_mc()[i])
		//					{
		//						for (size_t t = 0; t < data_my[k].get_its().size(); t++)
		//						{
		//							cailiao_id.push_back((int)data_my[k].get_its()[t].get_it());
		//							cailiao_ct.push_back((int)data_my[k].get_its()[t].get_ct());
		//						}
		//					}
		//				}

		//			}
		//			ImGui::NextColumn();
		//		}
		//	}
		//}

		//ImGui::EndChild();
		//ImGui::PopStyleVar();

		//ImGui::SameLine();

		//ImGui::BeginChild("ChildR2", ImVec2(ImGui::GetWindowContentRegionWidth() * 0.46f, 230), true);
		////根据配方id显示存在的配方材料  神秘
		//

		//if (Selected_item != -1 && cailiao_id.size() > 0)
		//{


		//	//Copy the recipe text
		//	if (ImGui::Button(CTXEX1, ImVec2(-FLT_MIN, 0.0f)))
		//	//if (ImGui::Button(u8"复制配方文本", ImVec2(-FLT_MIN, 0.0f)))
		//	{
		//		tmptext = (searchresults[Selected_item] + u8"=").c_str();
		//		for (size_t i = 0; i < cailiao_id.size(); i++)
		//		{
		//			for (size_t t = 0; t < data_it.size(); t++)
		//			{
		//				if ((int)data_it[t].get_id() == cailiao_id[i])
		//				{
		//					tmptext +=  std::to_string(cailiao_ct[i]) + " " + data_it[t].get_ne() + ",";
		//				}
		//			}
		//		}
		//		///From SY recipe search plugin

		//		if (Lang == 0)
		//		{
		//			ImGui::SetClipboardText((tmptext + u8" 来自神油配方搜索插件").c_str());
		//		}
		//		else
		//		{
		//			ImGui::SetClipboardText((tmptext + " From SY recipe search plugin").c_str());
		//		}

		//		//
		//		
		//	}
		//	for (size_t i = 0; i < cailiao_id.size(); i++)
		//	{
		//		for (size_t t = 0; t < data_it.size(); t++)
		//		{
		//			if ((int)data_it[t].get_id() == cailiao_id[i])
		//			{
		//				char buf[128];
		//				sprintf_s(buf, "%d  %s##cailiao%03d", cailiao_ct[i], data_it[t].get_ne().c_str(), (int)i);
		//				ImGui::PushStyleColor(ImGuiCol_Text, PZ_color((int)data_it[t].get_zg()));
		//				if (ImGui::Button(buf, ImVec2(-FLT_MIN, 0.0f)))
		//				{

		//					ImGui::SetClipboardText(data_it[t].get_cl().c_str());
		//				}
		//				ImGui::PopStyleColor(1);
		//			}
		//		}
		//	}
		//}


		//ImGui::EndChild();

		//ImGui::EndChild();

		//if (isloaditemsOK == 2)
		//{
		//	if (Lang == 0)
		//	{
		//		ImGui::Text(u8"物品数据%d", data_it.size());
		//		//ImGui::Text("Item data %d", data_it.size());
		//		if (isloadrecisOK == 2)
		//		{
		//			ImGui::SameLine();
		//			ImGui::Text(u8"/配方数据%d", data_re.size());
		//			//ImGui::Text("/Recipe data %d", data_re.size());
		//			if (isloadmaysOK == 2)
		//			{
		//				ImGui::SameLine();
		//				//Mystic Forge
		//				//ImGui::Text("/Mystic Forge data %d", data_my.size());
		//				ImGui::Text(u8"/马桶数据%d", data_my.size());
		//				ImGui::SameLine();
		//				//Data not necessarily accurate database :2020-12-06 compile
		//				//ImGui::Text("Data not necessarily accurate,Database:2021-01-15 compile");
		//				ImGui::Text(u8"数据不一定准确 数据库:2020-12-06编译");
		//			}
		//		}
		//	}
		//	else
		//	{
		//		//ImGui::Text(u8"物品数据%d", data_it.size());
		//		ImGui::Text("Item data %d", data_it.size());
		//		if (isloadrecisOK == 2)
		//		{
		//			ImGui::SameLine();
		//			ImGui::Text("/Recipe data %d", data_re.size());
		//			if (isloadmaysOK == 2)
		//			{
		//				ImGui::SameLine();
		//				//Mystic Forge
		//				ImGui::Text("/Mystic Forge data %d", data_my.size());
		//				ImGui::SameLine();
		//				ImGui::Text("Data not necessarily accurate,Database:2021-01-15 compile");
		//			}
		//		}
		//	}

		//}

		//ImGui::End();
		ImGui::PopFont();
		//if (Selected_item != -1 && cailiao_id.size() > 0)
		//{
		//	for (size_t i = 0; i < cailiao_id.size(); i++)
		//	{
		//		ImGui::Text("%d %d", cailiao_id[i], cailiao_ct[i]);
		//	}
		//}
		
	}

	ImVec4 SY_settings::PZ_color(int initofpz)
	{
		//  no
		//  Junk
		//  Basic
		//	Fine
		//	Masterwork
		//	Rare
		//	Exotic
		//	Ascended
		//	Legendary
		switch (initofpz)
		{	
		case 0: return ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
		case 1: return ImVec4(0.631f, 0.631f, 0.604f, 1.00f);
		case 2: return ImVec4(0.984f, 0.984f, 0.984f, 1.00f);
		case 3: return ImVec4(0.322f, 0.690f, 1.00f, 1.00f);
		case 4: return ImVec4(0.137f, 0.855f, 0.00f, 1.00f);
		case 5: return ImVec4(1.00f, 0.895f, 0.106f, 1.00f);
		case 6: return ImVec4(1.00f, 0.714f, 0.00f, 1.00f);
		case 7: return ImVec4(1.00f, 0.271f, 0.592f, 1.00f);
		case 8: return ImVec4(0.655f, 0.196f, 1.00f, 1.00f);
		}
	}

	const char* SY_settings::ZG_zg(int initofcl)
	{


		if (langseting == 1)
		{
			switch (initofcl)
			{
			case 0: return u8"未知";
			case 1: return u8"垃圾";
			case 2: return u8"普通";
			case 3: return u8"优质";
			case 4: return u8"精制";
			case 5: return u8"稀有";
			case 6: return u8"特异";
			case 7: return u8"升华";
			case 8: return u8"传奇";
			}
		}
		else
		{
			switch (initofcl)
			{
			case 0: return "NULL";
			case 1: return "Junk";
			case 2: return "Basic";
			case 3: return "Fine";
			case 4: return "Masterwork";
			case 5: return "Rare";
			case 6: return "Exotic";
			case 7: return "Ascended";
			case 8: return "Legendary";
			}
		}
		return "";
	}

	const char* SY_settings::TP_yp(int inittotp)
	{
		//Armor – Armor 护甲1
		//	Back – Back item 背部2
		//	Bag – Bags 包包3
		//	Consumable – Consumables 消耗品4
		//	Container – Containers 容器箱5
		//	CraftingMaterial – Crafting materials 制作材料6
		//	Gathering – Gathering tools 采集工具7
		//	Gizmo – Gizmos 小发明8
		//	Key9
		//	MiniPet – Miniatures 迷你宠物10
		//	Tool – Salvage kits11
		//	Trait – Trait guides 12
		//	Trinket – Trinkets 首饰13
		//	Trophy – Trophies 战利品14
		//	UpgradeComponent – Upgrade components 升级组件15
		//	Weapon – Weapons 武器16



		if (langseting == 1)
		{
			switch (inittotp)
			{
			case 1: return u8"护甲";
			case 2: return u8"背部";
			case 3: return u8"容器";
			case 4: return u8"消耗品";
			case 5: return u8"容器箱";
			case 6: return u8"制作材料";
			case 7: return u8"采集工具";
			case 8: return u8"小发明";
			case 9: return u8"战利品(钥匙)";
			case 10: return u8"迷你宠物";
			case 11: return u8"工具";
			case 12: return u8"特征指导";
			case 13: return u8"首饰";
			case 14: return u8"战利品";
			case 15: return u8"升级组件";
			case 16: return u8"武器";
			case 0: return u8"未知";
			}
		}
		else
		{
			switch (inittotp)
			{
			case 1: return "Armor";
			case 2: return "Back";
			case 3: return "Bag";
			case 4: return "Consumable";
			case 5: return "Container";
			case 6: return "Crafting Material";
			case 7: return "Gathering";
			case 8: return "Gizmo";
			case 9: return "Key";
			case 10: return "MiniPet";
			case 11: return "Tool";
			case 12: return "Trait";
			case 13: return "Trinket";
			case 14: return "Trophy";
			case 15: return "Upgrade Component";
			case 16: return "Weapon";
			case 0: return "Unknown";
			}
		}

		return "";
	}





	
















//	void LoadMaysThread::LoadMaysDatas()
//	{
//		isloadmaysOK = 1;
//
//#pragma region 从文件加载
//		//filebuf* pbuf;
//		//ifstream filestr;
//		//long size;
//		//char* buffer;
//		//char buffe_r[MAX_PATH];
//		//GetModuleFileNameA(NULL, buffe_r, MAX_PATH);
//		//string::size_type pos = string(buffe_r).find_last_of("\\/");
//		//string currentPath = string(buffe_r).substr(0, pos);
//		//CHAR fullPath[MAX_PATH];
//		//sprintf_s(fullPath, "%s\\addons\\itemdata\\%s", currentPath.c_str(), "mysdata.json");
//		//filestr.open(fullPath, ios::binary);
//		//pbuf = filestr.rdbuf();
//		//size = pbuf->pubseekoff(0, ios::end, ios::in);
//		//pbuf->pubseekpos(0, ios::in);
//		//buffer = new char[size];
//		//pbuf->sgetn(buffer, size);
//		//filestr.close();
//		//std::string cur_str = buffer ;
//		//MysData data2 = nlohmann::json::parse(cur_str);
//		//data = data2;
//		//delete[]buffer;
//		//isloadmaysOK = 2;
//#pragma endregion
//
//		try
//		{
//			auto res = FindResource(dll_Module, MAKEINTRESOURCE(IDR_MYSDATA), RT_RCDATA);
//			if (res)
//			{
//				auto handle = LoadResource(dll_Module, res);
//				if (handle)
//				{
//					DWORD dataSize = SizeofResource(dll_Module, res);
//					LPVOID dataPtr = LockResource(handle);
//					BYTE* pByte = new BYTE[(int)dataSize + 1];
//					memcpy(pByte, dataPtr, dataSize);
//					char* p = new char[dataSize];
//					memcpy(p, pByte, dataSize);
//					p[dataSize] = 0;
//					std::string cur_s(p);
//					SYtool_MysData::MysData data2 = nlohmann::json::parse(cur_s);
//					data_my = data2;
//					isloadmaysOK = 2;
//				}
//				else
//				{
//					isloadmaysOK = 4;
//				}
//			}
//			else
//			{
//				isloadmaysOK = 3;
//			}
//		}
//		catch (...)
//		{
//			isloadmaysOK = 5;
//		}
//
//
//	}
//	void LoadMaysThread::LoaditemsDatas()
//	{
//		isloaditemsOK = 1;
//		try
//		{
//			auto res = FindResource(dll_Module, MAKEINTRESOURCE(IDR_ITEMDATA), RT_RCDATA);
//			if (res)
//			{
//				auto handle = LoadResource(dll_Module, res);
//				if (handle)
//				{
//					DWORD dataSize = SizeofResource(dll_Module, res);
//					LPVOID dataPtr = LockResource(handle);
//					BYTE* pByte = new BYTE[(int)dataSize + 1];
//					memcpy(pByte, dataPtr, dataSize);
//					char* p = new char[dataSize];
//					memcpy(p, pByte, dataSize);
//					p[dataSize] = 0;
//					std::string cur_s(p);
//					SYtool_item::Items data2 = nlohmann::json::parse(cur_s);
//					data_it = data2;
//					isloaditemsOK = 2;
//				}
//				else
//				{
//					isloaditemsOK = 4;
//				}
//			}
//			else
//			{
//				isloaditemsOK = 3;
//			}
//		}
//		catch (...)
//		{
//			isloaditemsOK = 5;
//		}
//
//	}
//	void LoadMaysThread::LoadrecisDatas()
//	{
//
//		isloadrecisOK = 1;
//		try
//		{
//			auto res = FindResource(dll_Module, MAKEINTRESOURCE(IDR_RECIDATA), RT_RCDATA);
//			if (res)
//			{
//				auto handle = LoadResource(dll_Module, res);
//				if (handle)
//				{
//					DWORD dataSize = SizeofResource(dll_Module, res);
//					LPVOID dataPtr = LockResource(handle);
//					BYTE* pByte = new BYTE[(int)dataSize + 1];
//					memcpy(pByte, dataPtr, dataSize);
//					char* p = new char[dataSize];
//					memcpy(p, pByte, dataSize);
//					p[dataSize] = 0;
//					std::string cur_s(p);
//					SYtool_recipes::Recipes data2 = nlohmann::json::parse(cur_s);
//					data_re = data2;
//					isloadrecisOK = 2;
//				}
//				else
//				{
//					isloadrecisOK = 4;
//				}
//			}
//			else
//			{
//				isloadrecisOK = 3;
//			}
//		}
//		catch (...)
//		{
//			isloadrecisOK = 5;
//		}
//
//	}

}
