#pragma once
// wx Imports
#include <wx/wx.h>
#include <wx/notebook.h>
#include <wx/textfile.h>
#include <wx/hyperlink.h>
#include <wx/textctrl.h>
#include <wx/statbmp.h>
#include <wx/toolbar.h>
#include <wx/tooltip.h>
// wxChartDir Import
#include "wxchartviewer.h"
//System Imports
#include <iostream>
#include <string>
#include <vector>
#include <numeric>
// Local Imports
#include "SerialComs.h"
#include "growlights.h"
#include "phframe.h"
#include "csvreader.h"

#define ID_SETTINGS 10000


class MainFrame : public wxFrame{
public:
	MainFrame(const wxString& title, const wxPoint& pos, const wxSize& size);
	// Notebook pages and panels
	wxNotebook* noteBook;
	
	wxPanel* ojipPanel;
	
	// Home Panel
	wxPanel* homePanel;

	//Environmental Controls
	wxPanel* environmentalPanel;
	
	//Sliders
	wxSlider* blueSlider;
	wxSlider* greenSlider;
	wxSlider* redSlider;
	
	//CheckBoxes
	wxCheckBox* redCheckBox;
	wxCheckBox* greenCheckBox;
	wxCheckBox* blueCheckBox;
	
	// Buttons
	wxButton* redFreqSet;
	wxButton* greenFreqSet;
	wxButton* blueFreqSet;
	
	// Freqency Inputs
	wxTextCtrl* redFreqInput;
	wxTextCtrl* greenFreqInput;
	wxTextCtrl* blueFreqInput;
	
	// Sensor Panel
	wxPanel* tempPanel;

	//Test Chart
	wxChartViewer* m_chartViewer;
    RanSeries* m_ranSeries;
    DoubleArray m_timeStamps;
    wxColour m_bgColour;
    DoubleArray m_dataSeriesA;
	
	//Top Bar Menu
	wxMenuBar* menuBar; // Leave
	wxNotebook* aboutNotebook;
	wxImage companyLogo;
	wxComboBox* serialCombo;
	const char * port;
	
	// ToolBar
	wxToolBar* toolBar;
	wxImageList* toolBarIcons;
	
	// Settings
	std::string dataDirectory = "/home/pi/Documents/DataOutput/";
	std::string settingsDirectory = "/home/pi/Documents/DataOutput/";
	wxNotebook* settingsNotebook;
	wxDialog* settingsDialog;
	
	// pH Calibration
	wxFrame* pHFrame;
	
	// Hard motor limits
	int maxMixingSpeed = 50;  // Just a placeholder
		
private:
	// MenuBar Functions
	void CreateMenuBar();
	void OnExit(wxCommandEvent& event);
	void OnHello(wxCommandEvent& event);
	void OnAbout(wxCommandEvent& event);
	void OnContact(wxCommandEvent& event);
	void OnOpen(wxCommandEvent& event);
	void list_serial_ports(wxCommandEvent& event);
	void set_port(wxCommandEvent& event);
	
	// MotorPumpLight Controls
	void ConstructEnvironmentalControls();
	
	// Temperture Controls
	void ConstructTempertureControls();
	
	// ToolBar Functions
	void ConstructToolBar();
	
	//Chart
	void DrawGraph(wxWindow* panel);
	void ConstructGraphPage();
	
	//Settings
	void SettingsDialog(wxCommandEvent& event);
	void pHCalibration(wxCommandEvent& event);

	// Scroll events
	void BlueLED(wxCommandEvent& event);
	void RedLED(wxCommandEvent& event);
	void GreenLED(wxCommandEvent& event);

	// LED Frequency events
	void setRedLEDFrequency(wxCommandEvent& event);
	void setGreenLEDFrequency(wxCommandEvent& event);
	void setBlueLEDFrequency(wxCommandEvent& event);
	
	DECLARE_EVENT_TABLE();
};

enum
{
		ID_TestMix = wxID_HIGHEST + 1, ID_MaxMix, ID_CALPH, ID_BLUESLIDER, ID_GREENSLIDER, ID_REDSLIDER, 
		ID_BLUECB, ID_GREENCB, ID_REDCB, ID_BLUEFREQ, ID_GREENFREQ, ID_REDFREQ, ID_SERIAL, ID_SEARCH_PORTS
};


	