#include "main.h"
#include "mainframe.h"

wxIMPLEMENT_APP(PhenobottleApp);

bool PhenobottleApp::OnInit()
{
	MainFrame* frame = new MainFrame("Phenobottle Dashboard", wxPoint(50, 50), wxSize(1200, 850));
	frame->SetWindowStyle(frame->GetWindowStyle() | wxFULL_REPAINT_ON_RESIZE);
	frame->Show(true);
	
	wxImage::AddHandler(new wxPNGHandler);
	
	wxToolTip::Enable(true); // Enables tooltips for the toolbar
	
	return true;
}
