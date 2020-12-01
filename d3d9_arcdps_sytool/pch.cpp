// pch.cpp: 与预编译标头对应的源文件

#include "pch.h"

// 当使用预编译的头时，需要使用此源文件，编译才能成功。
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
	void* get_init_addr(char* arcversionstr, void* imguicontext, IDirect3DDevice9* id3dd9)
	{
		arcvers = arcversionstr;
		//添加图片
		bgtx = CreateTextureFromResource(id3dd9, dll_Module, IDR_BG);
		//设置上下文
		ImGui::SetCurrentContext((ImGuiContext*)imguicontext);
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
		COM_RELEASE(bgtx);
		writeIni();//写配置
		return 0;
	}
	uintptr_t mod_wnd(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		if (lParam >> 31 == 0) //按下。
		{
			if (Show_settingwindows && wParam == VK_ESCAPE) {
				Show_settingwindows = false;
				return 0;
			}
		}
		auto const io = &ImGui::GetIO();
		if (shetingkey)
		{
			const int vkey = (int)wParam;
			if (vkey != VK_CONTROL && vkey != VK_MENU && vkey != VK_SHIFT && vkey != VK_TAB && vkey != VK_ESCAPE && vkey != VK_NUMLOCK && vkey != VK_CAPITAL)
			{
				BUFF = (int)wParam;
			}

		}
		switch (uMsg)
		{
		case WM_KEYUP:
		{
			const int vkey = (int)wParam;
			io->KeysDown[vkey] = 0;
			if (vkey == VK_CONTROL)
			{
				io->KeyCtrl = false;
			}
			else if (vkey == VK_MENU)
			{
				io->KeyAlt = false;
			}
			else if (vkey == VK_SHIFT)
			{
				io->KeyShift = false;
			}
			break;
		}
		case WM_KEYDOWN:
		{
			const int vkey = (int)wParam;
			io->KeysDown[vkey] = 1;
			if (vkey == VK_CONTROL)
			{
				io->KeyCtrl = true;
			}
			else if (vkey == VK_MENU)
			{
				io->KeyAlt = true;
			}
			else if (vkey == VK_SHIFT)
			{
				io->KeyShift = true;
			}
			break;
		}
		case WM_SYSKEYUP:
		{
			const int vkey = (int)wParam;
			io->KeysDown[vkey] = 0;
			if (vkey == VK_CONTROL)
			{
				io->KeyCtrl = false;
			}
			else if (vkey == VK_MENU)
			{
				io->KeyAlt = false;
			}
			else if (vkey == VK_SHIFT)
			{
				io->KeyShift = false;
			}
			break;
		}
		case WM_SYSKEYDOWN:
		{
			const int vkey = (int)wParam;
			io->KeysDown[vkey] = 1;
			if (vkey == VK_CONTROL)
			{
				io->KeyCtrl = true;
			}
			else if (vkey == VK_MENU)
			{
				io->KeyAlt = true;
			}
			else if (vkey == VK_SHIFT)
			{
				io->KeyShift = true;
			}
			break;
		}
		case WM_ACTIVATEAPP:
		{
			if (!wParam)
			{
				io->KeysDown[arc_global_mod1] = false;
				io->KeysDown[arc_global_mod2] = false;
			}
			break;
		}
		break;
		}



		if (io->KeysDown[arc_global_mod1] && io->KeysDown[arc_global_mod2])
		{
			if (io->KeysDown[setting_key]) return 0;
		}

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

		std::string pszValue = table_ini.GetValue("allkeys", "show_settingui_key", "75");
		setting_key = std::stoi(pszValue);//按键读取
	}
	void writeIni()
	{
		SI_Error rc = table_ini.SetValue("allkeys", "show_settingui_key", std::to_string(setting_key).c_str());//按键储存

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
		arc_exports.size = sizeof(arcdps_exports);
		arc_exports.out_name = u8"神油工具集";
		arc_exports.out_build = "0.1";
		arc_exports.wnd_nofilter = mod_wnd;
		//arc_exports.combat = mod_combat;
		arc_exports.imgui = mod_imgui;
		arc_exports.options_end = mod_options;
		return &arc_exports;
	}

	uintptr_t mod_options()
	{
		if (ImGui::Button(u8"设置神油工具集"))
		{
			Show_settingwindows = true;
		}
		return 0;
	}

	SY_settings::SY_settings()
	{
	}
	SY_settings::~SY_settings()
	{
	}

	LPCWSTR SY_settings::stringToLPCWSTR(std::string orig)
	{
		size_t origsize = orig.length() + 1;
		const size_t newsize = 100;
		size_t convertedChars = 0;
		wchar_t* wcstring = (wchar_t*)malloc(sizeof(wchar_t) * (orig.length() - 1));
		mbstowcs_s(&convertedChars, wcstring, origsize, orig.c_str(), _TRUNCATE);
		return wcstring;
	}

	void SY_settings::GET_json_toit()
	{
		wancheng = false;
		std::string retVal;
		std::string _a = "http://do.gw2.kongzhong.com/gw2task/completes?date=";

		time_t time_seconds = time(0);
		GetTime();
		std::string _b = datenow;
		std::string _tmp = _a + _b + "&_=" + std::to_string(time_seconds);

		DWORD response_length = 0;
		LPCWSTR ul = stringToLPCWSTR(_tmp);

		try
		{
			HINTERNET hSession = InternetOpen(ul, INTERNET_OPEN_TYPE_PRECONFIG, NULL, NULL, 0);
			if (hSession != NULL)
			{
				HINTERNET handle2 = InternetOpenUrl(hSession, ul, NULL, 0, INTERNET_FLAG_RELOAD | INTERNET_FLAG_PRAGMA_NOCACHE | INTERNET_FLAG_NO_CACHE_WRITE, 0);
				if (handle2 != NULL)
				{
					char response_data[8000];//缓冲区
					if (InternetReadFile(handle2, response_data, sizeof(response_data) - 1, &response_length) && response_length > 0)
					{
						response_data[response_length] = '\0';
						response_length++;
					}
					if (response_length > 0)
					{
						response_data[response_length] = '\0';
					}
					else
					{
						response_data[0] = '\0';
					}
					retVal = response_data;
					InternetCloseHandle(handle2);
					handle2 = NULL;
				}
				else
				{
					result[0] = "0";
					geting = false;
					wancheng = true;
					return;
				}
				InternetCloseHandle(hSession);
				hSession = NULL;
			}
			else
			{
				result[0] = "0";
				geting = false;
				wancheng = true;
				return;
			}
		}
		catch (...)
		{

		}

		if (!getjsonstring(retVal))result[0] = "0";
		geting = false;
		wancheng = true;

	}

	void SY_settings::GetTime()
	{
		time_t time_seconds = time(0);
		tm now;
		localtime_s(&now, &time_seconds);
		sprintf_s(datenow, u8"%d-%02d-%02d", now.tm_year + 1900, now.tm_mon + 1, now.tm_mday);
	}

	bool SY_settings::is_not_dateofstada() 
	{
		time_t time_seconds = time(0);
		tm now;
		char datenow2[64];
		localtime_s(&now, &time_seconds);
		sprintf_s(datenow2, u8"%d-%02d-%02d", now.tm_year + 1900, now.tm_mon + 1, now.tm_mday);
		return datenow == datenow2 ? true : false;
	}


	bool SY_settings::getjsonstring(std::string inits)
	{

		const auto rawJsonLength = static_cast<int>(inits.length());
		constexpr bool shouldUseOldWay = false;
		JSONCPP_STRING err;
		Json::Value root;

		Json::CharReaderBuilder builder;
		const std::unique_ptr<Json::CharReader> reader(builder.newCharReader());
		if (!reader->parse(inits.c_str(), inits.c_str() + rawJsonLength, &root,
			&err)) {
			return false;
		}
		if (!root["data"].isArray())
		{
			return false;
		}
		if (root["data"].isArray())
		{
			int nArraySize = root["data"].size();
			for (int i = 0; i < nArraySize; i++)
			{
				result[i] = root["data"][i]["task_name"].asString();
			}
		}
		return true;
	}

	void SY_settings::ShowUI(bool* INITbool)
	{
		ImGui::SetNextWindowPosCenter();
		ImGui::Begin(u8"神油工具集设置菜单", INITbool, ImGuiWindowFlags_AlwaysAutoResize);
		//ImGui::Image(bgtx,ImVec2(200,200));

		ImGui::InputInt(u8"按键", &BUFF, 0, 512);
		ImGui::SameLine();
		if (!shetingkey)
		{
			if (ImGui::Button(u8"设置##anjian1"))
			{
				shetingkey = true;
			}
		}
		else
		{
			if (ImGui::Button(u8"取消##anjian1"))
			{
				shetingkey = false;
			}
		}
		if (geting || !is_not_dateofstada())
		{
			geting = false;
			GET_json_toit();
		}
		else
		{
			if (wancheng)
			{
				ImGui::Text(datenow);
				if (result[0] != "0")
				{
					for (int i = 0; i < 5; i++)
					{
						ImGui::Text(result[i].c_str());
					}
				}
				else
				{
					ImGui::Text(u8"积分日常获取失败等待官网更新");
				}
			}
		}
		ImGui::End();
	}
}
