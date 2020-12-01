// pch.cpp: 与预编译标头对应的源文件

#include "pch.h"
#include <thread>
#include <iostream>
#include <fstream>
#include "imgui/imgui_internal.h"
#include <boost/thread.hpp>
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
		//bgtx = CreateTextureFromResource(id3dd9, dll_Module, IDR_BG);
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
		//COM_RELEASE(bgtx);
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

		pszValue = table_ini.GetValue("setting", "first_time_to_use", "1");

		first_use = std::stoi(pszValue);//按键读取
	}
	void writeIni()
	{
		SI_Error rc = table_ini.SetValue("allkeys", "show_settingui_key", std::to_string(setting_key).c_str());//按键储存
		rc = table_ini.SetValue("setting", "first_time_to_use", std::to_string(first_use).c_str());//按键储存
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
		arc_exports.out_name = u8"神油工具-配方搜索插件";
		arc_exports.out_build = "0.1";
		arc_exports.wnd_nofilter = mod_wnd;
		//arc_exports.combat = mod_combat;
		arc_exports.imgui = mod_imgui;
		arc_exports.options_end = mod_options;
		return &arc_exports;
	}

	uintptr_t mod_options()
	{
		if (ImGui::Button(u8"神油工具-配方搜索插件"))
		{
			Show_settingwindows = true;
		}
		return 0;
	}

	void LoadMays_Datas()
	{
		isloadmaysOK = 1;

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
					SYtool_MysData::MysData data2 = nlohmann::json::parse(cur_s);
					data_my = data2;
					isloadmaysOK = 2;
				}
				else
				{
					isloadmaysOK = 4;
				}
			}
			else
			{
				isloadmaysOK = 3;
			}
		}
		catch (...)
		{
			isloadmaysOK = 5;
		}


	}
	void Loaditems_Datas()
	{
		isloaditemsOK = 1;
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
					SYtool_item::Items data2 = nlohmann::json::parse(cur_s);
					data_it = data2;
					isloaditemsOK = 2;
				}
				else
				{
					isloaditemsOK = 4;
				}
			}
			else
			{
				isloaditemsOK = 3;
			}
		}
		catch (...)
		{
			isloaditemsOK = 5;
		}

	}
	void Loadrecis_Datas()
	{

		isloadrecisOK = 1;
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
					SYtool_recipes::Recipes data2 = nlohmann::json::parse(cur_s);
					data_re = data2;
					isloadrecisOK = 2;
				}
				else
				{
					isloadrecisOK = 4;
				}
			}
			else
			{
				isloadrecisOK = 3;
			}
		}
		catch (...)
		{
			isloadrecisOK = 5;
		}

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

	vector<string> searchresults;
	vector<string> searchresults_cl;
	vector<int> searchresults_id;
	vector<int> searchresults_zg;
	vector<int> searchresults_tp;

	vector<int> cailiao_id;
	vector<int> cailiao_ct;
	int Selected_item = -1;

	void SY_settings::ShowUI(bool* INITbool)
	{
		if (INITbool && isloadmaysOK == 0)
		{
			//LoadMaysThread test;
			//std::thread mutexTestThread1(&LoadMaysThread::LoadMaysDatas, &test);
			//std::thread mutexTestThread2(&LoadMaysThread::LoaditemsDatas, &test);
			//std::thread mutexTestThread3(&LoadMaysThread::LoadrecisDatas, &test);
			//mutexTestThread1.join();
			//mutexTestThread2.join();
			//mutexTestThread3.join();

			//boost::thread thServer1(Loaditems_Datas);
			//boost::thread thServer2(LoadMays_Datas);
			//boost::thread thServer3(Loadrecis_Datas);

			isloadmaysOK = 1;
			boost::thread thServer2(LoadMays_Datas);

		}
		if (INITbool && isloadrecisOK == 0)
		{
			isloadrecisOK = 1;
			boost::thread thServer3(Loadrecis_Datas);
		}
		if (INITbool && isloaditemsOK == 0)
		{
			isloaditemsOK = 1;
			boost::thread thServer1(Loaditems_Datas);
		}
		if (first_use == 1)
		{
			first_use = 0;
			ImGui::SetNextWindowPosCenter();
			ImGui::SetNextWindowSize(ImVec2(500.0f, 380.0f));
		}
		//ImGui::SetNextWindowPosCenter();
		ImGui::PushFont(ImGui::GetIO().Fonts->Fonts.back());
		ImGui::Begin(u8"神油工具_配方搜索插件", INITbool);

		//if (isloaditemsOK == 0)
		//{
		//	if (ImGui::Button(u8"点击加载物品数据库"))
		//	{
		//		isloaditemsOK = 1;
		//		boost::thread thServer1(Loaditems_Datas);
		//	}
		//}
		//
		//if (isloadmaysOK == 0)
		//{
		//	if (ImGui::Button(u8"点击加载马桶数据库"))
		//	{
		//		isloadmaysOK = 1;
		//		boost::thread thServer2(LoadMays_Datas);
		//	}
		//}
		//if (isloadrecisOK == 0)
		//{
		//	if (ImGui::Button(u8"点击加载配方数据库"))
		//	{
		//		isloadrecisOK = 1;
		//		boost::thread thServer3(Loadrecis_Datas);
		//	}
		//}

		//ImGui::Image(bgtx,ImVec2(200,200));
		//ImGui::InputInt(u8"按键", &BUFF, 0, 512);
		//ImGui::SameLine();
		//if (!shetingkey)
		//{
		//	if (ImGui::Button(u8"设置##anjian1"))
		//	{
		//		shetingkey = true;
		//	}
		//}
		//else
		//{
		//	if (ImGui::Button(u8"取消##anjian1"))
		//	{
		//		shetingkey = false;
		//	}
		//}
		//if (geting || !is_not_dateofstada())
		//{
		//	geting = false;
		//	GET_json_toit();
		//}
		//else
		//{
		//	if (wancheng)
		//	{
		//		ImGui::Text(datenow);
		//		if (result[0] != "0")
		//		{
		//			for (int i = 0; i < 5; i++)
		//			{
		//				ImGui::Text(result[i].c_str());
		//			}
		//		}
		//		else
		//		{
		//			ImGui::Text(u8"积分日常获取失败等待官网更新");
		//		}
		//	}
		//}
		//} 

#pragma region 数据库加载反馈
		if (isloaditemsOK == 1)ImGui::Text(u8"正在加载物品数据库");
		if (isloaditemsOK > 2)
		{
			ImGui::Text(u8"加载物品数据库失败:错误代码=%d", isloaditemsOK);
			ImGui::SameLine();
			if (ImGui::Button(u8"重新加载物品数据库"))
			{
				isloaditemsOK = 0;
			}
		}
		if (isloadrecisOK == 1)ImGui::Text(u8"正在加载配方数据库");
		if (isloadrecisOK > 2)
		{
			ImGui::Text(u8"加载配方数据库失败:错误代码=%d", isloadrecisOK);
			ImGui::SameLine();
			if (ImGui::Button(u8"重新加载配方数据库"))
			{
				isloadrecisOK = 0;
			}
		}
		if (isloadmaysOK == 1)ImGui::Text(u8"正在加载马桶数据库");
		if (isloadmaysOK > 2)
		{
			ImGui::Text(u8"加载数据库失败:错误代码=%d", isloadmaysOK);
			ImGui::SameLine();
			if (ImGui::Button(u8"重新加载马桶数据库"))
			{
				isloadmaysOK = 0;
			}
		}

#pragma endregion

		ImGui::BeginChild("ChildL", ImVec2(-1, 320));
		{
			ImGui::BeginChild("ChildL1", ImVec2(ImGui::GetWindowContentRegionWidth() * 0.52f, 40));
			static char str0[512] = "";
			ImGui::InputText("", str0, IM_ARRAYSIZE(str0));
			ImGui::SameLine();
			if (ImGui::Button(u8"搜索"))
			{
				cailiao_id.clear();
				cailiao_ct.clear();
				string tmp = str0;
				if (tmp != "")
				{
					//搜索数据库
					Selected_item = -1;
					searchresults.clear();
					searchresults_id.clear();
					searchresults_zg.clear();
					searchresults_cl.clear();
					searchresults_tp.clear();
					for (size_t i = 0; i < data_it.size(); i++)
					{
						if (data_it[i].get_ne().find(tmp) != string::npos)
						{
							searchresults.push_back(data_it[i].get_ne());
							searchresults_id.push_back((int)i);
							searchresults_zg.push_back((int)data_it[i].get_zg());
							searchresults_cl.push_back(data_it[i].get_cl());
							searchresults_tp.push_back((int)data_it[i].get_tp());
							
						}
					}
				}
			}
			ImGui::EndChild();
			ImGuiWindowFlags window_flags = ImGuiWindowFlags_HorizontalScrollbar;
			ImGui::PushStyleVar(ImGuiStyleVar_ChildWindowRounding, 5.0f);
			ImGui::BeginChild("ChildL2", ImVec2(ImGui::GetWindowContentRegionWidth() * 0.5f, 270), false, window_flags);
			ImGui::Columns(1);
			for (int i = 0; i < searchresults.size(); i++)
			{
				//根据搜索值显示左边列表
				char buf[256];
				sprintf_s(buf, "%s##%03d", searchresults[i].c_str(), i);
				ImGui::PushStyleColor(ImGuiCol_Text, PZ_color(searchresults_zg[i]));
				if (ImGui::Button(buf, ImVec2(-FLT_MIN, 0.0f)))
				{
					Selected_item = i;
				}
				ImGui::PopStyleColor(1);
				ImGui::NextColumn();
			}
			ImGui::PopStyleVar();
			ImGui::EndChild();
		}
		ImGui::EndChild();


		ImGui::SameLine(ImGui::GetWindowContentRegionWidth() * 0.52f);
		ImGui::BeginChild("ChildR", ImVec2(0, 320));


		ImGui::BeginChild("ChildL1", ImVec2(0, 80));

		//根据列表点击显示物品信息
		if (Selected_item != -1)
		{


			char buf[256];
			sprintf_s(buf, "%s##ll-%03d", searchresults[Selected_item].c_str(), Selected_item);
			ImGui::PushStyleColor(ImGuiCol_Text, PZ_color(searchresults_zg[Selected_item]));
			if (ImGui::Button(buf, ImVec2(-1, 32)))
			{
				ImGui::SetClipboardText(searchresults_cl[Selected_item].c_str());
			}
			ImGui::PopStyleColor(1);

			ImGui::Text(u8"物品ID: %d", data_it[searchresults_id[Selected_item]].get_id() );

			ImGui::Text(u8"物品类型: "); ImGui::SameLine(); ImGui::Text(TP_yp(searchresults_tp[Selected_item])); ImGui::SameLine();

			ImGui::Text(u8"  物品品质: "); ImGui::SameLine(); 
			
			ImGui::PushStyleColor(ImGuiCol_Text, PZ_color(searchresults_zg[Selected_item]));
			ImGui::Text(ZG_zg(searchresults_zg[Selected_item]));
			ImGui::PopStyleColor(1);
		}


		ImGui::EndChild();

		ImGui::PushStyleVar(ImGuiStyleVar_ChildWindowRounding, 5.0f);
		ImGui::BeginChild("ChildR1", ImVec2(ImGui::GetWindowContentRegionWidth() * 0.5f, 230), true);
		ImGui::Columns(1);

		if (Selected_item != -1)
		{
			if (data_it[searchresults_id[Selected_item]].get_pf().size() > 0)
			{
				for (size_t i = 0; i < data_it[searchresults_id[Selected_item]].get_pf().size(); i++)
				{
					char buf[128];
					sprintf_s(buf, u8"配方 - %d##pf%03d", (int)data_it[searchresults_id[Selected_item]].get_pf()[i], (int)i);
					if (ImGui::Button(buf, ImVec2(-FLT_MIN, 0.0f)))
					{
						cailiao_id.clear();
						cailiao_ct.clear();
						for (size_t t = 0; t < data_re.size(); t++)
						{
							if (data_re[t].get_id() == data_it[searchresults_id[Selected_item]].get_pf()[i])
							{
								for (size_t k = 0; k < data_re[t].get_its().size(); k++)
								{
									cailiao_id.push_back((int)data_re[t].get_its()[k].get_it());
									cailiao_ct.push_back((int)data_re[t].get_its()[k].get_ct());
								}
							}
						}
					}
					ImGui::NextColumn();
				}
			}
			if (data_it[searchresults_id[Selected_item]].get_mc().size() > 0)
			{
				for (size_t i = 0; i < data_it[searchresults_id[Selected_item]].get_mc().size(); i++)
				{
					char buf[128];
					sprintf_s(buf, u8"熔炉  %s##mc%03d", data_it[searchresults_id[Selected_item]].get_mc()[i].c_str(), (int)i);
					if (ImGui::Button(buf, ImVec2(-FLT_MIN, 0.0f)))
					{
						cailiao_id.clear();
						cailiao_ct.clear();
						for (size_t k = 0; k < data_my.size(); k++)
						{
							if (data_my[k].get_id() == data_it[searchresults_id[Selected_item]].get_mc()[i])
							{
								for (size_t t = 0; t < data_my[k].get_its().size(); t++)
								{
									cailiao_id.push_back((int)data_my[k].get_its()[t].get_it());
									cailiao_ct.push_back((int)data_my[k].get_its()[t].get_ct());
								}
							}
						}

					}
					ImGui::NextColumn();
				}
			}
		}

		ImGui::EndChild();
		ImGui::PopStyleVar();

		ImGui::SameLine();

		ImGui::BeginChild("ChildR2", ImVec2(ImGui::GetWindowContentRegionWidth() * 0.46f, 230), true);
		//根据配方id显示存在的配方材料  神秘
		

		if (Selected_item != -1 && cailiao_id.size() > 0)
		{

			if (ImGui::Button(u8"复制配方文本", ImVec2(-FLT_MIN, 0.0f)))
			{
				tmptext = (searchresults[Selected_item] + u8"=").c_str();
				for (size_t i = 0; i < cailiao_id.size(); i++)
				{
					for (size_t t = 0; t < data_it.size(); t++)
					{
						if ((int)data_it[t].get_id() == cailiao_id[i])
						{
							tmptext +=  std::to_string(cailiao_ct[i]) + " " + data_it[t].get_ne();
						}
					}
				}
				ImGui::SetClipboardText((tmptext +u8" 来自神油配方搜索插件").c_str());
			}
			for (size_t i = 0; i < cailiao_id.size(); i++)
			{
				for (size_t t = 0; t < data_it.size(); t++)
				{
					if ((int)data_it[t].get_id() == cailiao_id[i])
					{
						char buf[128];
						sprintf_s(buf, "%d  %s##cailiao%03d", cailiao_ct[i], data_it[t].get_ne().c_str(), (int)i);
						ImGui::PushStyleColor(ImGuiCol_Text, PZ_color((int)data_it[t].get_zg()));
						if (ImGui::Button(buf, ImVec2(-FLT_MIN, 0.0f)))
						{

							ImGui::SetClipboardText(data_it[t].get_cl().c_str());
						}
						ImGui::PopStyleColor(1);
					}
				}
			}
		}


		ImGui::EndChild();

		ImGui::EndChild();

		if (isloaditemsOK == 2)
		{
			ImGui::Text(u8"物品数据%d", data_it.size());
			if (isloadrecisOK == 2)
			{
				ImGui::SameLine();
				ImGui::Text(u8"/配方数据%d", data_re.size());
				if (isloadmaysOK == 2)
				{
					ImGui::SameLine();
					ImGui::Text(u8"/马桶数据%d", data_my.size());
					ImGui::SameLine();
					ImGui::Text(u8"数据不一定准确 数据库:2020-12-06编译");
				}
			}
		}

		ImGui::End();
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





	
















	void LoadMaysThread::LoadMaysDatas()
	{
		isloadmaysOK = 1;

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
					SYtool_MysData::MysData data2 = nlohmann::json::parse(cur_s);
					data_my = data2;
					isloadmaysOK = 2;
				}
				else
				{
					isloadmaysOK = 4;
				}
			}
			else
			{
				isloadmaysOK = 3;
			}
		}
		catch (...)
		{
			isloadmaysOK = 5;
		}


	}
	void LoadMaysThread::LoaditemsDatas()
	{
		isloaditemsOK = 1;
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
					SYtool_item::Items data2 = nlohmann::json::parse(cur_s);
					data_it = data2;
					isloaditemsOK = 2;
				}
				else
				{
					isloaditemsOK = 4;
				}
			}
			else
			{
				isloaditemsOK = 3;
			}
		}
		catch (...)
		{
			isloaditemsOK = 5;
		}

	}
	void LoadMaysThread::LoadrecisDatas()
	{

		isloadrecisOK = 1;
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
					SYtool_recipes::Recipes data2 = nlohmann::json::parse(cur_s);
					data_re = data2;
					isloadrecisOK = 2;
				}
				else
				{
					isloadrecisOK = 4;
				}
			}
			else
			{
				isloadrecisOK = 3;
			}
		}
		catch (...)
		{
			isloadrecisOK = 5;
		}

	}

}
