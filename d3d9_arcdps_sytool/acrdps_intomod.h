#pragma once
#include <cstdint>

/* arcdps export table */
typedef struct arcdps_exports {
	uintptr_t size; /* size of exports table */
	uintptr_t sig; /* pick a number between 0 and uint64_t max that isn't used by other modules */
	const char* out_name; /* name string */
	const char* out_build; /* build string */
	void* wnd_nofilter; /* wndproc callback, fn(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) */
	void* combat; /* combat event callback, fn(cbtevent* ev, ag* src, ag* dst, char* skillname, uint64_t id, uint64_t revision) */
	void* imgui; /* id3dd9::present callback, before imgui::render, fn(uint32_t not_charsel_or_loading) */
	void* options_end; /* id3dd9::present callback, appending to the end of options window in arcdps, fn() */
	void* combat_local;  /* combat event callback like area but from chat log, fn(cbtevent* ev, ag* src, ag* dst, char* skillname, uint64_t id, uint64_t revision) */
	void* wnd_filter; /* wndproc callback like above, input filered using modifiers */
	void* options_windows; /* called once per 'window' option checkbox, with null at the end, non-zero return disables drawing that option, fn(char* windowname) */
} arcdps_exports;

/* combat event - see evtc docs for details, revision param in combat cb is equivalent of revision byte header */
typedef struct cbtevent {
	uint64_t time;
	uintptr_t src_agent;
	uintptr_t dst_agent;
	int32_t value;
	int32_t buff_dmg;
	uint32_t overstack_value;
	uint32_t skillid;
	uint16_t src_instid;
	uint16_t dst_instid;
	uint16_t src_master_instid;
	uint16_t dst_master_instid;
	uint8_t iff;
	uint8_t buff;
	uint8_t result;
	uint8_t is_activation;
	uint8_t is_buffremove;
	uint8_t is_ninety;
	uint8_t is_fifty;
	uint8_t is_moving;
	uint8_t is_statechange;
	uint8_t is_flanking;
	uint8_t is_shields;
	uint8_t is_offcycle;
	uint8_t pad61;
	uint8_t pad62;
	uint8_t pad63;
	uint8_t pad64;
} cbtevent;

/* agent short */
typedef struct ag {
	char* name; /* agent name. may be null. valid only at time of event. utf8 */
	uintptr_t id; /* agent unique identifier */
	uint32_t prof; /* profession at time of event. refer to evtc notes for identification */
	uint32_t elite; /* elite spec at time of event. refer to evtc notes for identification */
	uint32_t self; /* 1 if self, 0 if not */
	uint16_t team; /* sep21+ */
} ag;



enum iff {
	IFF_FRIEND,
	IFF_FOE,
	IFF_UNKNOWN // or uncertain
};

/* combat result (physical) */
enum cbtresult {
	CBTR_NORMAL, // 物理打击
	CBTR_CRIT, // 物理打击-暴击
	CBTR_GLANCE, // 物理打击被glance
	CBTR_BLOCK, // 物理打击被阻挡 eg. mesmer shield 4
	CBTR_EVADE, // 物理打击被闪避, eg. dodge or mesmer sword 2
	CBTR_INTERRUPT, // 物理打击被打断
	CBTR_ABSORB, // 物理打击被“无效”或被吸收 eg. guardian elite
	CBTR_BLIND, // 物理命中错过
	CBTR_KILLINGBLOW, // 物理打击是致命一击
	CBTR_DOWNED, // 受到打击
};

/* 动作 */
enum cbtactivation {
	ACTV_NONE, // not used - not this kind of event
	ACTV_NORMAL, // 没有敏捷
	ACTV_QUICKNESS, // 有敏捷
	ACTV_CANCEL_FIRE, // 取消与到达频道时间
	ACTV_CANCEL_CANCEL, // 没有到达频道时间就取消
	ACTV_RESET // 动画完全完成
};

/* combat state change */
enum cbtstatechange {
	CBTS_NONE, // not used——不是这种场合
	CBTS_ENTERCOMBAT, // src_agent 进入战斗，dst_agent 是子组
	CBTS_EXITCOMBAT, // src_agent 离开战斗
	CBTS_CHANGEUP, // src_agent 是否存活
	CBTS_CHANGEDEAD, // src_agent 是否死亡
	CBTS_CHANGEDOWN, // src_agent 是否倒地
	CBTS_SPAWN, // src_agent 是否在游戏追踪范围内
	CBTS_DESPAWN, // src_agent 是否不再被追踪
	CBTS_HEALTHUPDATE, // src_agent 已经达到健康指标. dst_agent = percent * 10000 (eg. 99.5% will be 9950)
	CBTS_LOGSTART, // log start. value = server unix timestamp **uint32**. buff_dmg = local unix timestamp. src_agent = 0x637261 (arcdps id)
	CBTS_LOGEND, // log end. value = server unix timestamp **uint32**. buff_dmg = local unix timestamp. src_agent = 0x637261 (arcdps id)
	CBTS_WEAPSWAP, // src_agent 交换武器. dst_agent =当前集合id (0/1 水下, 4/5 陆地)
	CBTS_MAXHEALTHUPDATE, // src_agent 最大生命值改变了。dst_agent =新的最大运行状况
	CBTS_POINTOFVIEW, // src_agent 将成为“录音”播放器的代理人
	CBTS_LANGUAGE, // src_agent 将是文本语言
	CBTS_GWBUILD, // src_agent 将是游戏构建
	CBTS_SHARDID, // src_agent 将被切断碎片id
	CBTS_REWARD, // src_agent 是自我，dst_agent是奖励ID，值是奖励类型。 这些是你得到的摇摆不定的盒子
	CBTS_BUFFINITIAL, // 战斗事件，在记录开始时每个特工每个buff出现一次 (零时长, buff==18)
	CBTS_POSITION, // src_agent 位置改变, cast float* p = (float*)&dst_agent, 获取 x/y/z (float[3])
	CBTS_VELOCITY, // src_agent 位置改变, cast float* v = (float*)&dst_agent, 获取 x/y/z (float[3])
	CBTS_FACING, // src_agent 位置改变, cast float* f = (float*)&dst_agent,获取 x/y (float[2])
	CBTS_TEAMCHANGE, // src_agent 改变, dst_agent 新团队id
	CBTS_ATTACKTARGET, // src_agent 是攻击目标，dst_agent是父代理(小工具类型)，值是当前目标状态
	CBTS_TARGETABLE, // dst_agent 是新的目标状态(0 = no, 1 = yes)。默认是的)
	CBTS_MAPID, // src_agent 地图id
	CBTS_REPLINFO, // 内部使用，不会看到任何地方
	CBTS_STACKACTIVE // src_agent 代理与buff, dst_agent是堆栈的stackid被标记为活动
};

/* combat buff remove type */
enum cbtbuffremove {
	CBTB_NONE, // 不是buff事件
	CBTB_ALL, // 最后/所有堆栈被删除(由服务器发送)
	CBTB_SINGLE, // 删除单个堆栈(由服务器发送)。会发生在每个堆栈清理
	CBTB_MANUAL, // 单个堆栈被移除(在ooc上由arc自动或所有堆栈，忽略条带/清理calc，用于in/out卷)
};

/* custom skill ids */
enum cbtcustomskill {
	CSK_RESURRECT = 1066, // not custom but important and unnamed
	CSK_BANDAGE = 1175, // personal healing only
	CSK_DODGE = 65001 // will occur in is_activation==normal event
};
