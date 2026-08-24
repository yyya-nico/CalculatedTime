/*
	TVTest plugin sample

	Displays TVTest's calculated broadcast time in the status bar.
	The value comes from the %tot-*% variable string family. TVTest uses an
	interpolated TOT time, or the time in SIT when TOT is not present.
*/

#define WIN32_LEAN_AND_MEAN
#define NOMINMAX

#include <windows.h>

#define TVTEST_PLUGIN_CLASS_IMPLEMENT
#include "TVTestPlugin.h"


class CCalculatedTime : public TVTest::CTVTestPlugin
{
	static constexpr int STATUS_ITEM_ID = 1;

	static LRESULT CALLBACK EventCallback(
		UINT Event, LPARAM lParam1, LPARAM lParam2, void *pClientData);
	void ShowItem(bool fShow);
	void DrawTime(const TVTest::StatusItemDrawInfo *pInfo);

public:
	bool GetPluginInfo(TVTest::PluginInfo *pInfo) override;
	bool Initialize() override;
};


bool CCalculatedTime::GetPluginInfo(TVTest::PluginInfo *pInfo)
{
	pInfo->Type = TVTest::PLUGIN_TYPE_NORMAL;
	pInfo->Flags = 0;
	pInfo->pszPluginName = L"Calculated Time";
	pInfo->pszCopyright = L"Public Domain";
	pInfo->pszDescription = L"TVTestが計算したTOT/SIT時刻をステータスバーに表示します。";
	return true;
}


bool CCalculatedTime::Initialize()
{
	TVTest::StatusItemInfo Item{};
	Item.Size = sizeof(Item);
	Item.Flags = TVTest::STATUS_ITEM_FLAG_TIMERUPDATE;
	Item.ID = STATUS_ITEM_ID;
	Item.pszIDText = L"CalculatedTime";
	Item.pszName = L"計算済みTOT/SIT時刻";
	Item.MinWidth = 0;
	Item.MaxWidth = -1;
	Item.DefaultWidth = TVTest::StatusItemWidthByFontSize(19);
	Item.MinHeight = 0;
	if (!m_pApp->RegisterStatusItem(&Item)) {
		m_pApp->AddLog(L"時刻表示用ステータス項目を登録できません。", TVTest::LOG_TYPE_ERROR);
		return false;
	}

	m_pApp->SetEventCallback(EventCallback, this);
	return true;
}


LRESULT CALLBACK CCalculatedTime::EventCallback(
	UINT Event, LPARAM lParam1, LPARAM lParam2, void *pClientData)
{
	CCalculatedTime *pThis = static_cast<CCalculatedTime *>(pClientData);

	switch (Event) {
	case TVTest::EVENT_PLUGINENABLE:
		pThis->ShowItem(lParam1 != 0);
		return TRUE;

	case TVTest::EVENT_STATUSITEM_DRAW:
		pThis->DrawTime(reinterpret_cast<const TVTest::StatusItemDrawInfo *>(lParam1));
		return TRUE;

	case TVTest::EVENT_STATUSITEM_NOTIFY:
		{
			const auto *pInfo =
				reinterpret_cast<const TVTest::StatusItemEventInfo *>(lParam1);
			switch (pInfo->Event) {
			case TVTest::STATUS_ITEM_EVENT_CREATED:
				pThis->ShowItem(pThis->m_pApp->IsPluginEnabled());
				return TRUE;
			case TVTest::STATUS_ITEM_EVENT_VISIBILITYCHANGED:
				pThis->m_pApp->EnablePlugin(pInfo->Param != 0);
				return TRUE;
			case TVTest::STATUS_ITEM_EVENT_UPDATETIMER:
				return TRUE;
			}
		}
		break;
	}

	return FALSE;
}


void CCalculatedTime::DrawTime(const TVTest::StatusItemDrawInfo *pInfo)
{
	LPCWSTR pszText = L"----/--/-- --:--:--";
	TVTest::VarStringFormatInfo Format{};
	Format.Size = sizeof(Format);
	Format.Flags = TVTest::VAR_STRING_FORMAT_FLAG_NONE;
	Format.pszFormat =
		L"%tot-year%/%tot-month%/%tot-day%(%tot-day-of-week%) "
		L"%tot-hour%:%tot-minute2%:%tot-second2%";

	if ((pInfo->Flags & TVTest::STATUS_ITEM_DRAW_FLAG_PREVIEW) != 0) {
		pszText = L"2026/8/24(月) 12:34:56";
	} else if (m_pApp->FormatVarString(&Format)) {
		pszText = Format.pszResult;
	}

	m_pApp->ThemeDrawText(
		pInfo->pszStyle, pInfo->hdc, pszText, pInfo->DrawRect,
		DT_LEFT | DT_VCENTER | DT_SINGLELINE | DT_END_ELLIPSIS,
		pInfo->Color);

	if (Format.pszResult != nullptr)
		m_pApp->MemoryFree(Format.pszResult);
}


void CCalculatedTime::ShowItem(bool fShow)
{
	TVTest::StatusItemSetInfo Info{};
	Info.Size = sizeof(Info);
	Info.Mask = TVTest::STATUS_ITEM_SET_INFO_MASK_STATE;
	Info.ID = STATUS_ITEM_ID;
	Info.StateMask = TVTest::STATUS_ITEM_STATE_VISIBLE;
	Info.State = fShow ? TVTest::STATUS_ITEM_STATE_VISIBLE : 0;
	m_pApp->SetStatusItem(&Info);
}


TVTest::CTVTestPlugin *CreatePluginClass()
{
	return new CCalculatedTime;
}
