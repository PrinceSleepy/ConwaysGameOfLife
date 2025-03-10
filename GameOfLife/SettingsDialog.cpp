#include "SettingsDialog.h"

wxBEGIN_EVENT_TABLE(SettingsDialog, wxDialog)
EVT_BUTTON(wxID_OK, SettingsDialog::OnOk)
EVT_BUTTON(wxID_CANCEL, SettingsDialog::OnCancel)
wxEND_EVENT_TABLE()

SettingsDialog::SettingsDialog(wxWindow& _Window, Settings& _settings): wxDialog(&_Window, wxID_ANY, "Settings"), mWindow(&_Window), mSettings(_settings)
{
	mSizer = new wxBoxSizer(wxVERTICAL);

	//Grid Size spin control
	wxBoxSizer* gridSizeSpinBox = new wxBoxSizer(wxHORIZONTAL);
	gridSizeSpinBox->Add(new wxStaticText(this, wxID_ANY, "Grid Size"));
	mGridSizeSpinCtrl = new wxSpinCtrl(this, wxID_ANY);
	mGridSizeSpinCtrl->SetValue(mSettings.gridSize);
	gridSizeSpinBox->Add(mGridSizeSpinCtrl, wxALL | wxEXPAND);
	mSizer->Add(gridSizeSpinBox, wxEXPAND);

	//Interval spin control
	wxBoxSizer* intervalSpinBox = new wxBoxSizer(wxHORIZONTAL);
	intervalSpinBox->Add(new wxStaticText(this, wxID_ANY, "Interval"));
	mIntervalSpinCtrl = new wxSpinCtrl(this, wxID_ANY);
	mIntervalSpinCtrl->SetValue(mSettings.interval);
	intervalSpinBox->Add(mIntervalSpinCtrl, wxALL | wxEXPAND);
	mSizer->Add(intervalSpinBox, wxEXPAND);

	//living color picker
	wxBoxSizer* livingColorSelection = new wxBoxSizer(wxHORIZONTAL);
	livingColorSelection->Add(new wxStaticText(this, wxID_ANY, "Living Cell Color Selection"), wxALL);
	mLivingColorPickerCtrl = new wxColourPickerCtrl(this, wxID_ANY);
	mLivingColorPickerCtrl->SetColour(mSettings.GetLivingColor());
	livingColorSelection->Add(mLivingColorPickerCtrl, wxALL | wxEXPAND);
	mSizer->Add(livingColorSelection, wxEXPAND);

	//dead color picker
	wxBoxSizer* deadColorSelection = new wxBoxSizer(wxHORIZONTAL);
	deadColorSelection->Add(new wxStaticText(this, wxID_ANY, "Dead Cell Color Selection"), wxALL);
	mDeadColorPickerCtrl = new wxColourPickerCtrl(this, wxID_ANY);
	mDeadColorPickerCtrl->SetColour(mSettings.GetDeadColor());
	deadColorSelection->Add(mDeadColorPickerCtrl, wxALL | wxEXPAND);
	mSizer->Add(deadColorSelection, wxEXPAND);

	//button sizer
	wxSizer* buttonSizer = CreateButtonSizer(wxOK | wxCANCEL);
	mSizer->Add(buttonSizer, wxEXPAND | wxALL);
	SetSizer(mSizer);

}

SettingsDialog::~SettingsDialog()
{
}

void SettingsDialog::OnOk(wxCommandEvent& _event)
{
	mSettings.gridSize = mGridSizeSpinCtrl->GetValue();
	mSettings.interval = mIntervalSpinCtrl->GetValue();
	mSettings.SetLivingColor(mLivingColorPickerCtrl->GetColour());
	mSettings.SetDeadColor(mDeadColorPickerCtrl->GetColour());
	EndModal(wxID_OK);
}

void SettingsDialog::OnCancel(wxCommandEvent& _event)
{
	EndModal(wxID_CANCEL);
}
