// pch.h: 这是预编译标头文件。
// 下方列出的文件仅编译一次，提高了将来生成的生成性能。
// 这还将影响 IntelliSense 性能，包括代码完成和许多代码浏览功能。
// 但是，如果此处列出的文件中的任何一个在生成之间有更新，它们全部都将被重新编译。
// 请勿在此处添加要频繁更新的文件，这将使得性能优势无效。

#ifndef PCH_H
#define PCH_H

// 添加要在此处预编译的标头

#define Lang 0
#if (Lang == 1)
#define   CTXEX1 "Copy text"  
#define   CTXEX2 "Search" 
#else
#define   CTXEX1 u8"复制文本"  
#define   CTXEX2 u8"搜索"   
#endif

#include "framework.h"
namespace SYtool {
	HMODULE dll_Module = nullptr;
	uint32_t cbtcount = 0;
	arcdps_exports arc_exports;
	char* arcvers;
	void dll_init(HMODULE hModule);
	void dll_exit();
	extern "C" __declspec(dllexport) void* get_init_addr(char* arcversionstr, void* imguicontext, IDirect3DDevice9 * id3dd9);
	extern "C" __declspec(dllexport) void* get_release_addr();
	arcdps_exports* mod_init();
	uintptr_t mod_imgui(uint32_t not_charsel_or_loading);
	uintptr_t mod_options();
	uintptr_t mod_release();
	uintptr_t mod_wnd(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	uintptr_t mod_combat(cbtevent* ev, ag* src, ag* dst, char* skillname, uint64_t id, uint64_t revision);
	bool LoadFontResource(HMODULE hModule, UINT resId, void*& dataPtr, size_t& dataSize);
	IDirect3DTexture9* CreateTextureFromResource(IDirect3DDevice9* pDev, HMODULE hModule, unsigned uResource);
	bool Show_settingwindows = false;
	IDirect3DTexture9* bgtx = nullptr;
	void readArcExports();
	typedef uint64_t(*arc_export_func_u64)();
	auto arc_dll = LoadLibraryA("d3d9.dll");
	auto arc_export_e6 = (arc_export_func_u64)GetProcAddress(arc_dll, "e6");
	auto arc_export_e7 = (arc_export_func_u64)GetProcAddress(arc_dll, "e7");
	WPARAM setting_key;
	WPARAM arc_global_mod1;
	WPARAM arc_global_mod2;
	WPARAM arc_global_mod_multi;
	bool arc_hide_all = false;
	bool arc_panel_always_draw = false;
	bool arc_movelock_altui = false;
	bool arc_clicklock_altui = false;
	bool arc_window_fastclose = false;
	int BUFF;
	bool shetingkey = false;
	//配置菜单
	CSimpleIniA table_ini(true);
	bool valid_table_ini = false;
	void parseIni();
	void writeIni();
	SYtool_item::Items data_it;
	SYtool_MysData::MysData data_my;
	SYtool_recipes::Recipes data_re;

	int first_use = 1;

	int isloadmaysOK = 0;
	int isloaditemsOK = 0;
	int isloadrecisOK = 0;
	class LoadMaysThread
	{
	public:
		LoadMaysThread() {}
		~LoadMaysThread() {}
		void LoadMaysDatas();
		void LoaditemsDatas();
		void LoadrecisDatas();
	private:
	};



	class SY_settings
	{
	public:
		SY_settings();
		~SY_settings();
		//LPCWSTR stringToLPCWSTR(std::string orig);
		//void GET_json_toit();
		//void GetTime();
		//bool is_not_dateofstada();
		//bool getjsonstring(std::string inits);
		void ShowUI(bool* INITbool);
	private:
		//std::string result[5];
		bool wancheng = false;
		bool geting = true;
		//char datenow[64];
		ImVec4 PZ_color (int initofpz);
		const char* ZG_zg(int initofcl);
		const char* TP_yp(int inittotp);
		std::string tmptext = "";
		::wkeWebView windowof;
		void websiye();
	};
	SY_settings seting;

}
#endif //PCH_H
