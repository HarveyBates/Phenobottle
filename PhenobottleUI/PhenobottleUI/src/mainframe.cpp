#include "mainframe.h"

GrowLights growLights;
SerialComs serialComs;

BEGIN_EVENT_TABLE(MainFrame, wxFrame)
// File menu operations
EVT_MENU(wxID_OPEN, MainFrame::OnOpen)
EVT_MENU(wxID_EXIT, MainFrame::OnExit)
EVT_MENU(wxID_ABOUT, MainFrame::OnAbout)
EVT_MENU(ID_SETTINGS, MainFrame::SettingsDialog)

// Settings operations
EVT_BUTTON(ID_CALPH, MainFrame::pHCalibration)

// Scroll events
EVT_SLIDER(ID_REDSLIDER, MainFrame::RedLED)
EVT_SLIDER(ID_GREENSLIDER, MainFrame::GreenLED)
EVT_SLIDER(ID_BLUESLIDER, MainFrame::BlueLED)

// Checkbox events
EVT_CHECKBOX(ID_REDCB, MainFrame::RedLED)
EVT_CHECKBOX(ID_GREENCB, MainFrame::GreenLED)
EVT_CHECKBOX(ID_BLUECB, MainFrame::BlueLED)

// Button events
EVT_BUTTON(ID_REDFREQ, MainFrame::setRedLEDFrequency)
EVT_BUTTON(ID_GREENFREQ, MainFrame::setGreenLEDFrequency)
EVT_BUTTON(ID_BLUEFREQ, MainFrame::setBlueLEDFrequency)

EVT_MENU(ID_SEARCH_PORTS, MainFrame::list_serial_ports)
EVT_COMBOBOX(ID_SERIAL, MainFrame::set_port)

END_EVENT_TABLE()

MainFrame::MainFrame(const  wxString& title, const wxPoint& pos, const wxSize& size)  
: wxFrame(NULL, wxID_ANY, title, pos, size)
{
	CreateMenuBar();
	SetMenuBar(menuBar);
	CreateStatusBar();
	SetStatusText("Phenobottle Dashboard");
	ConstructToolBar();
	
	std::vector<float> values(100);
	std::iota(values.begin(), values.end(), 0);
	write_csv(dataDirectory, "Output", "Test Column", values);
	read_csv(dataDirectory, "Output", "Test Column");
	
	noteBook = new wxNotebook(this, -1, wxDefaultPosition, wxDefaultSize, wxNB_TOP);
	
	// Construct Graph Panel
	homePanel = new wxPanel(noteBook, wxID_ANY);
	DrawGraph(homePanel);
	ConstructGraphPage();
	noteBook->AddPage(homePanel, "Chart");
	
	ojipPanel = new wxPanel(noteBook, wxID_ANY);
	noteBook->AddPage(ojipPanel, "OJIP Chart");
	
	environmentalPanel = new wxPanel(noteBook, wxID_ANY);
	ConstructEnvironmentalControls();
	noteBook->AddPage(environmentalPanel, "Lights, Motors and Pumps");
	
	tempPanel = new wxPanel(noteBook, wxID_ANY);
	ConstructTempertureControls();
	noteBook->AddPage(tempPanel, "Temperture and pH");
	
	Centre();
}
	
void MainFrame::OnExit(wxCommandEvent& event){
	wxUnusedVar(event);
	Close(true);
}

void MainFrame::RedLED(wxCommandEvent& event){
	if(redCheckBox->GetValue()){
		int pos = redSlider->GetValue();
		if(pos == 0){
			growLights.lightOff("R");
		}
		else{
			growLights.lightOn("R", pos);
		}
	}
	else{
		growLights.lightOff("R");
	}
}

void MainFrame::GreenLED(wxCommandEvent& event){
	if(greenCheckBox->GetValue()){
		int pos = greenSlider->GetValue();
		if(pos == 0){
			growLights.lightOff("G");
		}
		else{
			growLights.lightOn("G", pos);
		}
	}
	else{
		growLights.lightOff("G");
	}
}

void MainFrame::BlueLED(wxCommandEvent& event){
	if(blueCheckBox->GetValue()){
		int pos = blueSlider->GetValue();
		if(pos == 0){
			growLights.lightOff("B");
		}
		else{
			growLights.lightOn("B", pos);
		}
	}
	else{
		growLights.lightOff("B");
	}
}

void MainFrame::setRedLEDFrequency(wxCommandEvent& event){
	int freq = wxAtoi(redFreqInput->GetValue());
	std::cout << freq << std::endl;
	if(freq > 1600){
		freq = 1600; // Max frequency
		redFreqInput->SetValue(wxString::Format(wxT("%i"), freq));
	}
	else if(freq < 40){
		freq = 40; // Min frequency
		redFreqInput->SetValue(wxString::Format(wxT("%i"), freq));
	}
	growLights.setFrequency("RF", freq);
}

void MainFrame::setGreenLEDFrequency(wxCommandEvent& event){
	int freq = wxAtoi(greenFreqInput->GetValue());
	std::cout << freq << std::endl;
	if(freq > 1600){
		freq = 1600; // Max frequency
		greenFreqInput->SetValue(wxString::Format(wxT("%i"), freq));
	}
	else if(freq < 40){
		freq = 40; // Min frequency
		greenFreqInput->SetValue(wxString::Format(wxT("%i"), freq));
	}
	growLights.setFrequency("GF", freq);
}

void MainFrame::setBlueLEDFrequency(wxCommandEvent& event){
	int freq = wxAtoi(blueFreqInput->GetValue());
	std::cout << freq << std::endl;
	if(freq > 1600){
		freq = 1600; // Max frequency
		blueFreqInput->SetValue(wxString::Format(wxT("%i"), freq));
	}
	else if(freq < 40){
		freq = 40; // Min frequency
		blueFreqInput->SetValue(wxString::Format(wxT("%i"), freq));
	}
	growLights.setFrequency("BF", freq);
}

void MainFrame::ConstructGraphPage(){
	wxFlexGridSizer* graphBox = new wxFlexGridSizer(1, 3, 10, 10);
	
	wxBoxSizer* leftBox = new wxBoxSizer(wxVERTICAL);
	
	// Parameter parameters
	wxStaticBoxSizer* paraBox = new wxStaticBoxSizer(wxVERTICAL, homePanel, "Environmental Parameters");
	wxBoxSizer* enviroBox = new wxBoxSizer(wxHORIZONTAL);
	std::vector<std::string> parameters = {"Temperature", "pH"};
	for (unsigned int i = 0; i < parameters.size(); i++){
		enviroBox->Add(new wxCheckBox(homePanel, wxID_ANY, parameters[i]), 0, wxALIGN_CENTER_VERTICAL, 5);
		enviroBox->Add(new wxTextCtrl(homePanel, wxID_ANY, "Input"), 0, wxALIGN_CENTER_VERTICAL, 5);
		paraBox->Add(enviroBox, 0, wxEXPAND);
	}
	
	// Growth parameters
	wxStaticBoxSizer* growthBox = new wxStaticBoxSizer(wxVERTICAL, homePanel, "Growth Parameters");
	std::vector<std::string> growthParameters = {"Transmission", "Optical Density"};
	for (unsigned int i = 0; i < growthParameters.size(); i++){
		growthBox->Add(new wxCheckBox(homePanel, wxID_ANY, growthParameters[i]), 0, wxALIGN_CENTER_VERTICAL, 10);
	}

	// Photosynthetic parameters
	wxStaticBoxSizer* photoParaBox = new wxStaticBoxSizer(wxVERTICAL, homePanel, "Photosynthetic\nParameters");
	std::vector<std::string> photoParameters = {"Fo", "Fj", "Fi", "Fm", "Fv", "Fv/Fm", "FmQa", "Vj", "PIabs", "Area"};
	for(unsigned int i = 0; i < photoParameters.size(); i++){
		photoParaBox->Add(new wxCheckBox(homePanel, wxID_ANY, photoParameters[i]), 0, wxALIGN_CENTER_VERTICAL, 10);
	}
	
	// Graph toolbar
	wxBoxSizer* vToolbar = new wxBoxSizer(wxVERTICAL);
	wxToolBar* graphTools = new wxToolBar(homePanel, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTB_VERTICAL, "GraphTools");
	
	wxBitmap zoomImg = wxBitmap(wxT("../../graph_imgs/magnifier.png"), wxBITMAP_TYPE_PNG);
	wxBitmap expandImg = wxBitmap(wxT("../../graph_imgs/expand.png"), wxBITMAP_TYPE_PNG);
	wxBitmap crosshairImg = wxBitmap(wxT("../../graph_imgs/crosshair.png"), wxBITMAP_TYPE_PNG);
	
	graphTools->AddTool(wxID_ANY, wxT("Zoom"), zoomImg, "Zoom In", wxITEM_CHECK);
	graphTools->AddTool(wxID_ANY, wxT("Expand"), expandImg, "Reset Zoom", wxITEM_CHECK);
	graphTools->AddTool(wxID_ANY, wxT("CrossHair"), crosshairImg, "Enable CrossHair", wxITEM_CHECK);
	graphTools->Realize();
	vToolbar->Add(graphTools, 0, wxEXPAND);
	
	leftBox->Add(paraBox, 0, wxEXPAND);
	leftBox->AddStretchSpacer(1);
	leftBox->Add(growthBox, 0, wxEXPAND);
	leftBox->AddStretchSpacer(1);
	leftBox->Add(photoParaBox, 0, wxEXPAND);
	
	graphBox->Add(leftBox, 0, wxEXPAND);
	graphBox->Add(m_chartViewer, 0, wxCENTER);
	graphBox->Add(vToolbar, 0, wxEXPAND);
	graphBox->AddGrowableCol(0,0);
	homePanel->SetSizer(graphBox);
}

void MainFrame::DrawGraph(wxWindow* panel){
    m_chartViewer = new wxChartViewer(panel, wxID_ANY, wxDefaultPosition, wxDefaultSize);
        // The XY points for the scatter chart
    double dataX0[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11};
    double dataY0[] = {130, 150, 80, 110, 110, 105, 130, 115, 170, 125, 125};
	
    XYChart* c = new XYChart(850, 637, 0xffffff, 0, 1);
    c->setPlotArea(50, 20, 750, 562, Transparent, -1, -1, Transparent, Transparent);
    c->xAxis()->setTitle("Time (ms)");
    c->yAxis()->setTitle("Fluorescence (V)");
    c->addScatterLayer(DoubleArray(dataX0, (int)(sizeof(dataX0) / sizeof(dataX0[0]))), DoubleArray(
        dataY0, (int)(sizeof(dataY0) / sizeof(dataY0[0]))), "Genetically Engineered",
        Chart::DiamondSymbol, 13, 0xff9933);
    delete m_chartViewer->getChart();
    m_chartViewer->setChart(c);
}

void MainFrame::ConstructTempertureControls(){
	wxBoxSizer* sensorBox = new wxBoxSizer(wxHORIZONTAL);
	
	// Temperture Settings
	wxStaticBoxSizer* tempStaticBox = new wxStaticBoxSizer(wxVERTICAL, tempPanel, "Setup Temperature Settings");
	wxFlexGridSizer* tempGrid = new wxFlexGridSizer(3, 1, 10, 20);
	
	tempGrid->Add(new wxStaticText(tempPanel, wxID_ANY, "Constant Temperature (Thermostat):" ), 1, wxALIGN_CENTER_VERTICAL);
	tempGrid->Add(new wxCheckBox(tempPanel, wxID_ANY, "Enable"), 0, wxALIGN_CENTER_VERTICAL);
	
	wxBoxSizer* tempSetSizer = new wxBoxSizer(wxHORIZONTAL);
	tempSetSizer->Add(new wxStaticText(tempPanel, wxID_ANY, "Set Temperture: " ), 1, wxALIGN_CENTER_VERTICAL);
	tempSetSizer->Add(new wxTextCtrl(tempPanel, wxID_ANY, "25", wxDefaultPosition, wxDefaultSize, wxTE_CENTER), 
		1, wxEXPAND);
		
	tempGrid->Add(tempSetSizer, 1, wxALL | wxEXPAND, 10);
	tempGrid->AddGrowableCol(0,1);
	tempStaticBox->Add(tempGrid, 1, wxALL | wxEXPAND, 10);
	sensorBox->Add(tempStaticBox, 1, wxALL | wxEXPAND, 10);
	
	// pH Settings
	wxStaticBoxSizer* phStaticBox = new wxStaticBoxSizer(wxVERTICAL, tempPanel, "Setup pH Settings");
	wxFlexGridSizer* phGrid = new wxFlexGridSizer(4, 1, 10, 20);

	phGrid->Add(new wxStaticText(tempPanel, wxID_ANY, "Constant pH (pH-Stat):" ), 1, wxALIGN_CENTER_VERTICAL);
	phGrid->Add(new wxCheckBox(tempPanel, wxID_ANY, "Enable"), 0, wxALIGN_CENTER_VERTICAL);
	
	wxBoxSizer* phSetSizer = new wxBoxSizer(wxHORIZONTAL);
	phSetSizer->Add(new wxStaticText(tempPanel, wxID_ANY, "Set pH: " ), 1, wxALIGN_CENTER_VERTICAL);
	phSetSizer->Add(new wxTextCtrl(tempPanel, wxID_ANY, "8.001", wxDefaultPosition, wxDefaultSize, wxTE_CENTER), 
		1, wxEXPAND);
	phGrid->Add(phSetSizer, 1, wxALL | wxEXPAND, 10);
	
	phGrid->Add(new wxButton(tempPanel,  ID_CALPH,  "Calibrate pH Probe", wxDefaultPosition, wxDefaultSize), 
		1, wxEXPAND);
	phGrid->AddGrowableCol(0,1);
	
	phStaticBox->Add(phGrid, 1, wxALL | wxEXPAND, 10);
	sensorBox->Add(phStaticBox, 1, wxALL | wxEXPAND, 10);
	
	// Density Settings
	wxStaticBoxSizer* odStaticBox = new wxStaticBoxSizer(wxVERTICAL, tempPanel, "Setup Density Settings");
	wxFlexGridSizer* odGrid = new wxFlexGridSizer(5, 1, 10, 20);
	
	odGrid->Add(new wxStaticText(tempPanel, wxID_ANY, "Constant Density (Turbidostat):" ), 1, wxALIGN_CENTER_VERTICAL);
	odGrid->Add(new wxCheckBox(tempPanel, wxID_ANY, "Enable"), 0, wxALIGN_CENTER_VERTICAL);
	
	wxBoxSizer* odSetSizer = new wxBoxSizer(wxHORIZONTAL);
	odSetSizer->Add(new wxStaticText(tempPanel, wxID_ANY, "Set Density: " ), 1, wxALIGN_CENTER_VERTICAL);
	odSetSizer->Add(new wxTextCtrl(tempPanel, wxID_ANY, "0.501", wxDefaultPosition, wxDefaultSize, wxTE_CENTER), 
		1, wxEXPAND);
	odGrid->Add(odSetSizer, 1, wxALL | wxEXPAND, 10);
	
	odGrid->Add(new wxStaticText(tempPanel, wxID_ANY, "Transmission Calibration (%):",  
		wxDefaultPosition, wxDefaultSize), 1, wxEXPAND);
	
	wxBoxSizer* odSetSizer2 = new wxBoxSizer(wxHORIZONTAL);
	odSetSizer2->Add(new wxButton(tempPanel, wxID_ANY,  "Calibrate %T"), 
		1, wxEXPAND); 
	odSetSizer2->Add(new wxTextCtrl(tempPanel, wxID_ANY, "0.005", wxDefaultPosition,
		wxDefaultSize, wxTE_READONLY | wxTE_CENTER), 1, wxEXPAND);
		
	odGrid->AddGrowableCol(0,1);
	
	odGrid->Add(odSetSizer2, 1, wxALL | wxEXPAND, 10);
	odStaticBox->Add(odGrid, 1, wxALL | wxEXPAND, 10);
	sensorBox->Add(odStaticBox, 1, wxALL | wxEXPAND, 10);

	tempPanel->SetSizer(sensorBox);
}

void MainFrame::ConstructEnvironmentalControls(){
	// Irradiance controls for RGB LED's
	wxBoxSizer* enviroBox = new wxBoxSizer(wxVERTICAL);
	wxStaticBoxSizer* enviroStaticBox = new wxStaticBoxSizer(wxVERTICAL, environmentalPanel, "Setup Irradiance Settings");
	wxFlexGridSizer* lightGrid = new wxFlexGridSizer(3, 6, 5, 20);
	
	// Red light controls
	lightGrid->Add(new wxStaticText(environmentalPanel, wxID_ANY, "Red Intensity (%)"), 1, 
		wxALIGN_CENTER);
	redSlider = new wxSlider(environmentalPanel, ID_REDSLIDER, 200,  0, 4096, wxDefaultPosition, 
		wxDefaultSize, wxSL_VALUE_LABEL);
	lightGrid->Add(redSlider, 1, wxHORIZONTAL | wxEXPAND);
	redCheckBox = new wxCheckBox(environmentalPanel, ID_REDCB, "ON");
	lightGrid->Add(redCheckBox, 0, wxALIGN_CENTER);
	lightGrid->Add(new wxStaticText(environmentalPanel, wxID_ANY, "Red Frequency (40 - 1600 Hz):"), 1, 
		wxALIGN_CENTER);
	redFreqInput = new wxTextCtrl(environmentalPanel, wxID_ANY, "120", wxDefaultPosition, wxDefaultSize, 
		 wxTE_CENTRE);
	lightGrid->Add(redFreqInput, 0,  wxALIGN_CENTER);
	redFreqSet = new wxButton(environmentalPanel,  ID_REDFREQ,  "Set");
	lightGrid->Add(redFreqSet, 0, wxALIGN_CENTER);
	
	// Green light controls
	lightGrid->Add(new wxStaticText(environmentalPanel, wxID_ANY, "Green Intensity (%)"), 1, 
		wxALIGN_CENTER);
	greenSlider = new wxSlider(environmentalPanel, ID_GREENSLIDER, 200,  0, 4096, wxDefaultPosition, 
		wxDefaultSize, wxSL_VALUE_LABEL);
	lightGrid->Add(greenSlider, 1, wxHORIZONTAL | wxEXPAND);
	greenCheckBox = new wxCheckBox(environmentalPanel, ID_GREENCB, "ON");
	lightGrid->Add(greenCheckBox, 1, wxALIGN_CENTER);
	lightGrid->Add(new wxStaticText(environmentalPanel, wxID_ANY, "Green Frequency (40 - 1600 Hz):"), 1, 
		wxALIGN_CENTER);
	greenFreqInput = new wxTextCtrl(environmentalPanel, wxID_ANY, "120", wxDefaultPosition, wxDefaultSize, 
		 wxTE_CENTRE);
	lightGrid->Add(greenFreqInput, 0, wxALIGN_CENTER);
	greenFreqSet = new wxButton(environmentalPanel,  ID_GREENFREQ,  "Set");
	lightGrid->Add(greenFreqSet, 0, wxALIGN_CENTER);
	
	// Blue light controls
	lightGrid->Add(new wxStaticText(environmentalPanel, wxID_ANY, "Blue Intensity (%)"), 1, 
		wxALIGN_CENTER);
	blueSlider = new wxSlider(environmentalPanel, ID_BLUESLIDER, 200,  0, 4096, wxDefaultPosition, 
		wxDefaultSize, wxSL_VALUE_LABEL);
	lightGrid->Add(blueSlider, 1, wxHORIZONTAL | wxEXPAND);
	blueCheckBox = new wxCheckBox(environmentalPanel, ID_BLUECB, "ON");
	lightGrid->Add(blueCheckBox, 1, wxALIGN_CENTER);
	lightGrid->Add(new wxStaticText(environmentalPanel, wxID_ANY, "Blue Frequency (40 - 1600 Hz):"), 1, 
		wxALIGN_CENTER);
	blueFreqInput = new wxTextCtrl(environmentalPanel, wxID_ANY, "120", wxDefaultPosition, wxDefaultSize, 
		 wxTE_CENTRE);
	lightGrid->Add(blueFreqInput, 0, wxALIGN_CENTER);
	blueFreqSet = new wxButton(environmentalPanel,  ID_BLUEFREQ,  "Set");
	lightGrid->Add(blueFreqSet, 0, wxALIGN_CENTER);
	
	lightGrid->AddGrowableCol(1, 1);
	lightGrid->AddGrowableRow(0, 1);
	lightGrid->AddGrowableRow(1, 1);
	lightGrid->AddGrowableRow(2, 1);
	lightGrid->SetFlexibleDirection(wxBOTH);
	enviroStaticBox->Add(lightGrid, 1, wxALL | wxEXPAND, 10);
	enviroBox->Add(enviroStaticBox, 1, wxALL | wxEXPAND, 10);
	
	wxStaticBoxSizer* motorStaticBox = new wxStaticBoxSizer(wxVERTICAL, environmentalPanel, "Setup Motors and Pumps");
	wxFlexGridSizer* motorGrid = new wxFlexGridSizer(5, 4, 5, 20);
	
	motorGrid->Add(new wxStaticText(environmentalPanel, wxID_ANY, "Mixing Motor:"), 1, 
		wxEXPAND | wxALIGN_CENTER_HORIZONTAL);
	motorGrid->Add(new wxSlider(environmentalPanel, wxID_ANY, 10,  0, 100, wxDefaultPosition, 
		wxDefaultSize, wxSL_VALUE_LABEL), 1, wxEXPAND);
	motorGrid->Add(new wxCheckBox(environmentalPanel, wxID_ANY, "ON"), 1, 
		wxEXPAND | wxALIGN_CENTER_HORIZONTAL);
	motorGrid->Add(new wxCheckBox(environmentalPanel, wxID_ANY, "Reverse"), 1, 
		wxEXPAND | wxALIGN_CENTER_HORIZONTAL);
		
	motorGrid->Add(new wxStaticText(environmentalPanel, wxID_ANY, "Bubbling Motor:"), 1, 
		wxEXPAND | wxALIGN_CENTER_HORIZONTAL);
	motorGrid->Add(new wxSlider(environmentalPanel, wxID_ANY, 20,  0, 100, wxDefaultPosition, 
		wxDefaultSize, wxSL_VALUE_LABEL), 1, wxEXPAND);
	motorGrid->Add(new wxCheckBox(environmentalPanel, wxID_ANY, "ON"), 1, 
		wxEXPAND | wxALIGN_CENTER_HORIZONTAL);
	motorGrid->Add(new wxCheckBox(environmentalPanel, wxID_ANY, "Reverse"), 1, 
		wxEXPAND | wxALIGN_CENTER_HORIZONTAL);
	
	motorGrid->Add(new wxStaticText(environmentalPanel, wxID_ANY, "Peristaltic Pump (Culture Out):"), 1, 
		wxEXPAND | wxALIGN_CENTER_HORIZONTAL);
	motorGrid->Add(new wxSlider(environmentalPanel, wxID_ANY, 30,  0, 100, wxDefaultPosition, 
		wxDefaultSize, wxSL_VALUE_LABEL), 1, wxEXPAND);
	motorGrid->Add(new wxCheckBox(environmentalPanel, wxID_ANY, "ON"), 1, 
		wxEXPAND | wxALIGN_CENTER_HORIZONTAL);
	motorGrid->Add(new wxCheckBox(environmentalPanel, wxID_ANY, "Reverse"), 1, 
		wxEXPAND | wxALIGN_CENTER_HORIZONTAL);
		
	motorGrid->Add(new wxStaticText(environmentalPanel, wxID_ANY, "Peristaltic Pump (Media In):"), 1, 
		wxEXPAND | wxALIGN_CENTER_HORIZONTAL);
	motorGrid->Add(new wxSlider(environmentalPanel, wxID_ANY, 40,  0, 100, wxDefaultPosition, 
		wxDefaultSize, wxSL_VALUE_LABEL), 1, wxEXPAND);
	motorGrid->Add(new wxCheckBox(environmentalPanel, wxID_ANY, "ON"), 1, 
		wxEXPAND | wxALIGN_CENTER_HORIZONTAL);
	motorGrid->Add(new wxCheckBox(environmentalPanel, wxID_ANY, "Reverse"), 1, 
		wxEXPAND | wxALIGN_CENTER_HORIZONTAL);
	
	motorGrid->Add(new wxStaticText(environmentalPanel, wxID_ANY, "Peristaltic Pump (Fluorometer):"), 1, 
		wxEXPAND | wxALIGN_CENTER_HORIZONTAL);
	motorGrid->Add(new wxSlider(environmentalPanel, wxID_ANY, 50,  0, 100, wxDefaultPosition, 
		wxDefaultSize, wxSL_VALUE_LABEL), 1, wxEXPAND);
	motorGrid->Add(new wxCheckBox(environmentalPanel, wxID_ANY, "ON"), 1, 
		wxEXPAND | wxALIGN_CENTER_HORIZONTAL);
	motorGrid->Add(new wxCheckBox(environmentalPanel, wxID_ANY, "Reverse"), 1, 
		wxEXPAND | wxALIGN_CENTER_HORIZONTAL);
		
	motorGrid->AddGrowableCol(1, 1);
	motorGrid->AddGrowableRow(0, 1);
	motorGrid->AddGrowableRow(1, 1);
	motorGrid->AddGrowableRow(2, 1);
	motorGrid->AddGrowableRow(3, 1);
	motorGrid->AddGrowableRow(4, 1);
	motorGrid->SetFlexibleDirection(wxBOTH);
	
	motorStaticBox->Add(motorGrid, 1, wxALL | wxEXPAND, 10);
	enviroBox->Add(motorStaticBox, 1, wxALL | wxEXPAND, 10);
	
	environmentalPanel->SetSizer(enviroBox);
}

void MainFrame::CreateMenuBar(){
	// File Menu
	wxMenu* menuFile = new wxMenu;
	menuFile->Append(wxID_ANY, "&New \tCtrl+N", "Open new file with default values");
	
	wxMenu* templateFiles = new wxMenu;
	templateFiles->Append(wxID_ANY, "Themo-Stat Mode");
	templateFiles->Append(wxID_ANY, "pH-Stat Mode");
	templateFiles->Append(wxID_ANY, "Turbido-Stat Mode");
	menuFile->AppendSubMenu(templateFiles, "&New From Template", "Open a predefined program template");

	menuFile->AppendSeparator();
	menuFile->Append(wxID_OPEN, "&Open \tCtrl+O", "Open a new file");
	menuFile->AppendSeparator();
	menuFile->Append(wxID_SAVE, "&Save \tCtrl+S", "Save experimental settings");
	menuFile->Append(wxID_SAVEAS, "&Save As \tShift+Ctrl+S", "Save experimental settings as...");
	menuFile->AppendSeparator();
	menuFile->Append(wxID_EXIT, "&Close Window \tAlt+F4", "Exit program");
	
	// Settings Menu
	wxMenu* menuSettings = new wxMenu;
	menuSettings->Append(ID_SETTINGS, "&Preferences", "Adjust advanced phenobottle settings");
	
	// Help Menu
	wxMenu* menuHelp = new wxMenu;
	menuHelp->Append(wxID_ANY, "&Check for updates...", 
		"Check if software or hardware updates have been released");
	menuHelp->Append(wxID_ABOUT, "&About", "About the phenobottle");
	
	// Create MenuBar
	menuBar = new wxMenuBar;
	menuBar->Append(menuFile, "&File");
	menuBar->Append(menuSettings, "&Settings");
	menuBar->Append(menuHelp, "&Help");
}

void MainFrame::OnAbout(wxCommandEvent& event){
	//Displays about and contact information
	wxDialog* aboutDialog = new wxDialog(this, wxID_ANY, "About | Contact", wxDefaultPosition, wxDefaultSize, 
	wxDEFAULT_DIALOG_STYLE, "About");
	aboutNotebook = new wxNotebook(aboutDialog, -1, wxDefaultPosition, wxSize(300, 200), wxNB_TOP);
	
	// ** Needs work to reference the right file here **
	wxBitmap companyLogo(wxT("../../imgs/companyLogo.bmp"), wxBITMAP_TYPE_BMP);
	wxStaticBitmap* sb = new wxStaticBitmap(aboutDialog, -1, companyLogo);
	
	// About Information
	wxPanel* aboutPanel = new wxPanel(aboutNotebook, wxID_ANY);
	aboutNotebook->AddPage(aboutPanel, "About");
	wxBoxSizer* aboutBox = new wxBoxSizer(wxVERTICAL);
	aboutBox->Add(sb, 0, wxEXPAND | wxALL, 10);
	wxStaticText* aboutInfo = new wxStaticText(aboutPanel, wxID_ANY, 
	"The Phenobottle is an open-source photobioreactor\ndesgined for microalgal physiological experiments.");
	aboutBox->Add(aboutInfo, 0, wxEXPAND | wxALL, 10);
	wxHyperlinkCtrl* aboutLink = new wxHyperlinkCtrl(aboutPanel, wxID_ANY, "https://github.com/HarveyBates/Phenobottle-v0.2", 
	"https://github.com/HarveyBates/Phenobottle-v0.2", wxDefaultPosition, wxDefaultSize, 
		wxHL_DEFAULT_STYLE, "AboutLink");
	aboutBox->Add(aboutLink, 0, wxEXPAND | wxALL, 10);
	aboutPanel->SetSizer(aboutBox);

	// Contact Information
	wxPanel* contactPanel = new wxPanel(aboutNotebook, wxID_ANY);
	aboutNotebook->AddPage(contactPanel, "Contact");
	wxBoxSizer* contactBox = new wxBoxSizer(wxVERTICAL);
	wxStaticText* contactName = new wxStaticText(contactPanel, wxID_ANY, "Harvey Bates");
	contactBox->Add(contactName, 0, wxALIGN_CENTER, 10);
	wxBoxSizer* emailBox = new wxBoxSizer(wxHORIZONTAL);
	wxStaticText* contactInfo = new wxStaticText(contactPanel, wxID_ANY, "Email:");
	emailBox->Add(contactInfo, 0, wxALIGN_CENTER, 10);
	wxHyperlinkCtrl* contactEmail = new wxHyperlinkCtrl(contactPanel, wxID_ANY, "harvey_bates@hotmail.com", 
		"harvey_bates@hotmail.com", 
	wxDefaultPosition, wxDefaultSize, wxHL_DEFAULT_STYLE, "EmailLink");
	emailBox->Add(contactEmail, 0, wxALIGN_CENTER, 10);
	contactBox->Add(emailBox, 0, wxALIGN_CENTER, 10);
	wxStaticText* contactPhone = new wxStaticText(contactPanel, wxID_ANY, "Phone: +61 478 515 336");
	contactBox->Add(contactPhone, 0, wxALIGN_CENTER, 10);
	contactPanel->SetSizer(contactBox);
	aboutDialog->ShowModal();
	aboutDialog->Destroy();
}

void MainFrame::OnOpen(wxCommandEvent& event){
	wxFileDialog* openDialog = new wxFileDialog(this, _("Open Text File"), "", "", _("Text files (*.txt)|*.txt"),
		wxFD_OPEN, wxDefaultPosition); // Define open file type
	if (openDialog->ShowModal() == wxID_OK) // if the user click "Open" instead of "Cancel"
	{
		wxString currentDocPath = openDialog->GetPath(); // Get pathname of file
		wxTextFile openTextFile;
		openTextFile.Open(wxString(currentDocPath));
		SetTitle(wxString("Phenobottle Dashboard - ") <<
			openDialog->GetFilename()); // Set the Title to reflect the file open
	}
	openDialog->Destroy();
}

void MainFrame::ConstructToolBar(){
	toolBar = CreateToolBar(wxTB_TOP); // Build a toolbar within the MainFrame
	
	wxBitmap newImg = wxBitmap(wxT("../../toolbar_imgs/new.png"), wxBITMAP_TYPE_PNG);
	wxBitmap templateImg = wxBitmap(wxT("../../toolbar_imgs/template.png"), wxBITMAP_TYPE_PNG);
	wxBitmap openImg = wxBitmap(wxT("../../toolbar_imgs/open.png"), wxBITMAP_TYPE_PNG);
	wxBitmap saveImg = wxBitmap(wxT("../../toolbar_imgs/save.png"), wxBITMAP_TYPE_PNG);
	wxBitmap playImg = wxBitmap(wxT("../../toolbar_imgs/play.png"), wxBITMAP_TYPE_PNG);
	wxBitmap pauseImg = wxBitmap(wxT("../../toolbar_imgs/pause-button.png"), wxBITMAP_TYPE_PNG);
	wxBitmap stopImg = wxBitmap(wxT("../../toolbar_imgs/stop.png"), wxBITMAP_TYPE_PNG);
	wxBitmap settingsImg = wxBitmap(wxT("../../toolbar_imgs/settings.png"), wxBITMAP_TYPE_PNG);
	wxBitmap serialImg = wxBitmap(wxT("../../toolbar_imgs/pendrive.png"), wxBITMAP_TYPE_PNG);

	toolBar->AddTool(wxID_ANY,  "", newImg, "Open new file with default values", wxITEM_NORMAL);
	toolBar->AddTool(wxID_ANY,  "", templateImg, "Open a program from a predefined template", 
		wxITEM_NORMAL);
	toolBar->AddSeparator();
	toolBar->AddTool(wxID_OPEN,  "", openImg, "Open experimental setup", wxITEM_NORMAL);
	toolBar->AddSeparator();
	toolBar->AddTool(wxID_SAVE,  "", saveImg, "Save experimental setup", wxITEM_NORMAL);
	toolBar->AddSeparator();
	toolBar->AddTool(wxID_ANY,  "", playImg, "Run experiment", wxITEM_CHECK);
	toolBar->AddTool(wxID_ANY,  "", pauseImg, "Pause experiment", wxITEM_CHECK);
	toolBar->AddTool(wxID_ANY,  "", stopImg, "Stop / abandon experiment", wxITEM_CHECK);
	toolBar->AddSeparator();
	toolBar->AddTool(ID_SETTINGS,  "", settingsImg, "Advanced experimental settings", wxITEM_NORMAL);
	toolBar->AddSeparator();
	toolBar->AddTool(ID_SEARCH_PORTS,  "Search ports", serialImg, "Search for connected USB devices", wxITEM_NORMAL);
	serialCombo = new wxComboBox(toolBar, ID_SERIAL, "None");
	toolBar->AddControl(serialCombo, wxString("Select serial port"));
	
	toolBar->Realize();
}

void MainFrame::list_serial_ports(wxCommandEvent& event){
	// When the toolbar list ports button has been pressed, list all avalible serial ports and remove duplicates
	std::cout << "List ports\n";
	std::vector<std::string> ports;
	ports = serialComs.list_ports();
	if(ports.size() == 0){
		serialCombo->Append("No open ports found...");
	}
	else{
		for(unsigned int i = 0; i < ports.size(); i++){
			bool found = false;
			for(unsigned int x = 0; x < serialCombo->GetCount(); x++){
				if(ports[i] == serialCombo->GetString(x)){
					found = true;
				}
			}
			if(!found){
				serialCombo->Append(ports[i]);
			}
		}
	}
}

void MainFrame::set_port(wxCommandEvent& event){
	// Once the serial port combo box has been selected serial communications are initalised 
	wxString selectedPort = serialCombo->GetStringSelection();
	selectedPort = "/dev/" + selectedPort;
	serialComs.setSerialAttributes(selectedPort.c_str());
}

void MainFrame::SettingsDialog(wxCommandEvent& event){
	//Settings dialog box
	settingsDialog = new wxDialog(this, wxID_ANY, "Settings", wxDefaultPosition, wxSize(700, 500), 
					wxDEFAULT_DIALOG_STYLE, "Settings");
	settingsNotebook = new wxNotebook(settingsDialog, -1, wxDefaultPosition, wxDefaultSize, wxNB_TOP);

	wxPanel* fileSettings = new wxPanel(settingsNotebook, wxID_ANY);
	wxStaticBoxSizer* fileBox = new wxStaticBoxSizer(wxVERTICAL, fileSettings, "Output Directories");
	fileBox->Add(new wxStaticText(fileSettings, wxID_ANY, "Data Output Directory:", 
				wxDefaultPosition, wxDefaultSize), 0, wxEXPAND | wxALL, 5); 
	fileBox->Add(new wxTextCtrl(fileSettings, wxID_ANY, wxString(dataDirectory), wxDefaultPosition, 
				wxDefaultSize, wxTE_LEFT), 0, wxEXPAND | wxALL, 5);
	fileBox->Add(new wxStaticText(fileSettings, wxID_ANY, "Experimental Settings Save Directory:", 
				wxDefaultPosition, wxDefaultSize), 0, wxEXPAND | wxALL, 5); 
	fileBox->Add(new wxTextCtrl(fileSettings, wxID_ANY, wxString(settingsDirectory), wxDefaultPosition, 
				wxDefaultSize, wxTE_LEFT), 0, wxEXPAND | wxALL, 5);
				
	fileSettings->SetSizer(fileBox);
	settingsNotebook->AddPage(fileSettings, "File");
	
	// Motors hard limits
	wxPanel* limitSettings = new wxPanel(settingsNotebook, wxID_ANY);
	wxBoxSizer* limitHBox = new wxBoxSizer(wxHORIZONTAL);
	wxStaticBoxSizer* limitBox = new wxStaticBoxSizer(wxVERTICAL, limitSettings, "Motors and Lights Limits");
	limitBox->Add(new wxStaticText(limitSettings, wxID_ANY, "Limit Mixing Speed (0 - 4096):", 
				wxDefaultPosition, wxDefaultSize), 0, wxEXPAND | wxALL, 5);
	limitBox->Add(new wxTextCtrl(limitSettings, ID_MaxMix, std::to_string(maxMixingSpeed), wxDefaultPosition, 
				wxDefaultSize, wxTE_RIGHT), 0, wxEXPAND | wxALL, 5);
	limitBox->Add(new wxStaticText(limitSettings, wxID_ANY, "Limit Bubbling Intensity (0 - 4096):", 
				wxDefaultPosition, wxDefaultSize), 0, wxEXPAND | wxALL, 5);
	limitBox->Add(new wxTextCtrl(limitSettings, wxID_ANY, std::to_string(maxMixingSpeed), wxDefaultPosition, 
				wxDefaultSize, wxTE_RIGHT), 0, wxEXPAND | wxALL, 5);
	limitBox->Add(new wxStaticText(limitSettings, wxID_ANY, "Limit Peristaltic Pump Speed (0 - 4096):", 
				wxDefaultPosition, wxDefaultSize), 0, wxEXPAND | wxALL, 5);
	limitBox->Add(new wxTextCtrl(limitSettings, wxID_ANY, std::to_string(maxMixingSpeed), wxDefaultPosition, 
				wxDefaultSize, wxTE_RIGHT), 0, wxEXPAND | wxALL, 5);
	limitBox->Add(new wxStaticText(limitSettings, wxID_ANY, "Limit Light Intensity (0 - 4096):", 
				wxDefaultPosition, wxDefaultSize), 0, wxEXPAND | wxALL, 5);
	limitBox->Add(new wxTextCtrl(limitSettings, ID_MaxMix, std::to_string(maxMixingSpeed), wxDefaultPosition, 
				wxDefaultSize, wxTE_RIGHT), 0, wxEXPAND | wxALL, 5);
	limitHBox->Add(limitBox, 0, wxEXPAND | wxALL, 5);

	// Temperature limits
	wxStaticBoxSizer* tempBox = new wxStaticBoxSizer(wxVERTICAL, limitSettings, "Temperature Limits");
	tempBox->Add(new wxStaticText(limitSettings, wxID_ANY, "Max Culture Temperature (0 - 40):", 
				wxDefaultPosition, wxDefaultSize), 0, wxEXPAND | wxALL, 5);
	tempBox->Add(new wxTextCtrl(limitSettings, ID_MaxMix, "25", wxDefaultPosition, 
				wxDefaultSize, wxTE_RIGHT), 0, wxEXPAND | wxALL, 5);

	limitHBox->Add(tempBox, 0, wxEXPAND | wxALL, 5);
	limitSettings->SetSizer(limitHBox);
	settingsNotebook->AddPage(limitSettings, "Limits");
	settingsDialog->ShowModal();
	settingsDialog->Destroy();
}

void MainFrame::pHCalibration(wxCommandEvent& event){
	// Creates pH frame after a button has been clicked
	PHFrame* pHFrame = new PHFrame("pH Calibration", wxDefaultPosition, wxSize(570, 200));
	pHFrame->SetWindowStyle(pHFrame->GetWindowStyle() | wxFULL_REPAINT_ON_RESIZE);
	pHFrame->Show(true);
}


