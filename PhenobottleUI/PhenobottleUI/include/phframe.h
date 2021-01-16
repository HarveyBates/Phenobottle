#pragma once
// wx Imports
#include <wx/wx.h>

class PHFrame : public wxFrame{
public:
	PHFrame(const wxString& title, const wxPoint& pos, const wxSize& size);
	
	// pH Parameters
	wxStaticText* pHLabel;
	wxBoxSizer* pHSizer;
	wxGridSizer* pHGrid;

};