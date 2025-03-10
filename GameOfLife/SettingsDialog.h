#pragma once
#include "wx/wx.h"
#include "wx/spinctrl.h"
#include "wx/clrpicker.h"
#include "Settings.h"

class SettingsDialog : public wxDialog
{
	wxWindow* mWindow = nullptr;
	wxBoxSizer* mSizer = nullptr;
	wxSpinCtrl* mGridSizeSpinCtrl = nullptr;
	wxSpinCtrl* mIntervalSpinCtrl = nullptr;
	wxColourPickerCtrl* mLivingColorPickerCtrl = nullptr;
	wxColourPickerCtrl* mDeadColorPickerCtrl = nullptr;
	Settings& mSettings;

public:
	SettingsDialog(wxWindow& _Window, Settings& _settings);
	~SettingsDialog();
	
	void OnOk(wxCommandEvent& _event);
	void OnCancel(wxCommandEvent& _event);

	wxDECLARE_EVENT_TABLE();

};

