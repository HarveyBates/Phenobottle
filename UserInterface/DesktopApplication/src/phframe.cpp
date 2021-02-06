#include "phframe.h"

PHFrame::PHFrame(const  wxString& title, const wxPoint& pos, const wxSize& size)  
: wxFrame(NULL, wxID_ANY, title, pos, size)
{
	// Calibration procedure for pH Probe
	pHSizer = new wxBoxSizer(wxVERTICAL);
	pHLabel = new wxStaticText(this, wxID_ANY, "Calibrate pH Probe");
	pHSizer->Add(pHLabel, 0, wxEXPAND | wxTOP | wxBOTTOM, 10);
	pHGrid = new wxGridSizer(4, 3, 5, 5);
	
	pHGrid->Add(new wxStaticText(this, wxID_ANY, "Select to Calibrate"), 0, wxEXPAND | wxALIGN_CENTRE_HORIZONTAL);
	pHGrid->Add(new wxStaticText(this, wxID_ANY, "Current Calibration"), 0, wxEXPAND | wxALIGN_CENTRE_HORIZONTAL);
	pHGrid->Add(new wxStaticText(this, wxID_ANY, "Set New Calibration"), 0, wxEXPAND | wxALIGN_CENTRE_HORIZONTAL);
	
	pHGrid->Add(new wxCheckBox(this, wxID_ANY, "Calibrate Low (pH 4)"), 0, wxEXPAND);
	pHGrid->Add(new wxTextCtrl(this, wxID_ANY, "4.001", wxDefaultPosition, wxDefaultSize, 
		wxTE_READONLY | wxTE_CENTRE), 0, wxEXPAND);
	pHGrid->Add(new wxButton(this, wxID_ANY, "Set"), 0, wxEXPAND);
	
	pHGrid->Add(new wxCheckBox(this, wxID_ANY, "Calibrate Mid (pH 7)"), 0, wxEXPAND);
	pHGrid->Add(new wxTextCtrl(this, wxID_ANY, "7.001", wxDefaultPosition, wxDefaultSize, 
		wxTE_READONLY | wxTE_CENTRE), 0, wxEXPAND);
	pHGrid->Add(new wxButton(this, wxID_ANY, "Set"), 0, wxEXPAND);
	
	pHGrid->Add(new wxCheckBox(this, wxID_ANY, "Calibrate High (pH 10)"), 0, wxEXPAND);
	pHGrid->Add(new wxTextCtrl(this, wxID_ANY, "10.001", wxDefaultPosition, wxDefaultSize, 
		wxTE_READONLY | wxTE_CENTRE), 0, wxEXPAND);
	pHGrid->Add(new wxButton(this, wxID_ANY, "Set"), 0, wxEXPAND);
	
	pHSizer->Add(pHGrid, 1, wxEXPAND);
	SetSizer(pHSizer);
	SetMinSize(wxSize(500, 180));
	Centre();
}