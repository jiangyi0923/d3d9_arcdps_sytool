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
	CBTR_NORMAL, // ������
	CBTR_CRIT, // ������-����
	CBTR_GLANCE, // ��������glance
	CBTR_BLOCK, // ���������赲 eg. mesmer shield 4
	CBTR_EVADE, // ������������, eg. dodge or mesmer sword 2
	CBTR_INTERRUPT, // �����������
	CBTR_ABSORB, // ������������Ч�������� eg. guardian elite
	CBTR_BLIND, // �������д��
	CBTR_KILLINGBLOW, // ������������һ��
	CBTR_DOWNED, // �ܵ����
};

/* ���� */
enum cbtactivation {
	ACTV_NONE, // not used - not this kind of event
	ACTV_NORMAL, // û������
	ACTV_QUICKNESS, // ������
	ACTV_CANCEL_FIRE, // ȡ���뵽��Ƶ��ʱ��
	ACTV_CANCEL_CANCEL, // û�е���Ƶ��ʱ���ȡ��
	ACTV_RESET // ������ȫ���
};

/* combat state change */
enum cbtstatechange {
	CBTS_NONE, // not used�����������ֳ���
	CBTS_ENTERCOMBAT, // src_agent ����ս����dst_agent ������
	CBTS_EXITCOMBAT, // src_agent �뿪ս��
	CBTS_CHANGEUP, // src_agent �Ƿ���
	CBTS_CHANGEDEAD, // src_agent �Ƿ�����
	CBTS_CHANGEDOWN, // src_agent �Ƿ񵹵�
	CBTS_SPAWN, // src_agent �Ƿ�����Ϸ׷�ٷ�Χ��
	CBTS_DESPAWN, // src_agent �Ƿ��ٱ�׷��
	CBTS_HEALTHUPDATE, // src_agent �Ѿ��ﵽ����ָ��. dst_agent = percent * 10000 (eg. 99.5% will be 9950)
	CBTS_LOGSTART, // log start. value = server unix timestamp **uint32**. buff_dmg = local unix timestamp. src_agent = 0x637261 (arcdps id)
	CBTS_LOGEND, // log end. value = server unix timestamp **uint32**. buff_dmg = local unix timestamp. src_agent = 0x637261 (arcdps id)
	CBTS_WEAPSWAP, // src_agent ��������. dst_agent =��ǰ����id (0/1 ˮ��, 4/5 ½��)
	CBTS_MAXHEALTHUPDATE, // src_agent �������ֵ�ı��ˡ�dst_agent =�µ��������״��
	CBTS_POINTOFVIEW, // src_agent ����Ϊ��¼�����������Ĵ�����
	CBTS_LANGUAGE, // src_agent �����ı�����
	CBTS_GWBUILD, // src_agent ������Ϸ����
	CBTS_SHARDID, // src_agent �����ж���Ƭid
	CBTS_REWARD, // src_agent �����ң�dst_agent�ǽ���ID��ֵ�ǽ������͡� ��Щ����õ���ҡ�ڲ����ĺ���
	CBTS_BUFFINITIAL, // ս���¼����ڼ�¼��ʼʱÿ���ع�ÿ��buff����һ�� (��ʱ��, buff==18)
	CBTS_POSITION, // src_agent λ�øı�, cast float* p = (float*)&dst_agent, ��ȡ x/y/z (float[3])
	CBTS_VELOCITY, // src_agent λ�øı�, cast float* v = (float*)&dst_agent, ��ȡ x/y/z (float[3])
	CBTS_FACING, // src_agent λ�øı�, cast float* f = (float*)&dst_agent,��ȡ x/y (float[2])
	CBTS_TEAMCHANGE, // src_agent �ı�, dst_agent ���Ŷ�id
	CBTS_ATTACKTARGET, // src_agent �ǹ���Ŀ�꣬dst_agent�Ǹ�����(С��������)��ֵ�ǵ�ǰĿ��״̬
	CBTS_TARGETABLE, // dst_agent ���µ�Ŀ��״̬(0 = no, 1 = yes)��Ĭ���ǵ�)
	CBTS_MAPID, // src_agent ��ͼid
	CBTS_REPLINFO, // �ڲ�ʹ�ã����ῴ���κεط�
	CBTS_STACKACTIVE // src_agent ������buff, dst_agent�Ƕ�ջ��stackid�����Ϊ�
};

/* combat buff remove type */
enum cbtbuffremove {
	CBTB_NONE, // ����buff�¼�
	CBTB_ALL, // ���/���ж�ջ��ɾ��(�ɷ���������)
	CBTB_SINGLE, // ɾ��������ջ(�ɷ���������)���ᷢ����ÿ����ջ����
	CBTB_MANUAL, // ������ջ���Ƴ�(��ooc����arc�Զ������ж�ջ����������/����calc������in/out��)
};

/* custom skill ids */
enum cbtcustomskill {
	CSK_RESURRECT = 1066, // not custom but important and unnamed
	CSK_BANDAGE = 1175, // personal healing only
	CSK_DODGE = 65001 // will occur in is_activation==normal event
};
