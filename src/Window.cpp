#include "Window.h"

#include "MapIO.h"

#ifdef __WINDOWS__
#define CANVAS ShaderCanvas
#else
#include "SimpleCanvas.h"
#define CANVAS SimpleCanvas
#endif

ViewerFrame::ViewerFrame(const wxString& title, const wxPoint& pos, const wxSize& size) : wxFrame(NULL, wxID_ANY, title, pos, size) {
	
	//Initialize Menu
    wxMenu* menuFile = new wxMenu;
	menuFile->Append(ID_Open, "&Open...\tCtrl-O", "Load previously saved parameters");

    menuFile->AppendSeparator();
    menuFile->Append(wxID_EXIT);

    wxMenu* menuHelp = new wxMenu;
    menuHelp->Append(wxID_ABOUT);

    wxMenuBar* menuBar = new wxMenuBar;
    menuBar->Append( menuFile, "&File" );
    menuBar->Append( menuHelp, "&Help" );

    SetMenuBar(menuBar);

	//Initialize Status bar

    CreateStatusBar();
    SetStatusText("Welcome to View-an-Isle!");

	//Initialize Map preview

	wxLogStream(NULL);

	mapPreview = new CANVAS(this, wxSize(this->GetClientSize().GetWidth(), this->GetClientSize().GetHeight()));
}

void ViewerFrame::InitializeGL() {
	glContext = new wxGLContext(mapPreview, NULL);
	mapPreview->Initialize(glContext);
}

void ViewerFrame::OnExit(wxCommandEvent& event) {
    Close(true);
}

void ViewerFrame::OnAbout(wxCommandEvent& event) {
    wxMessageBox("This program is a viewing application for islands generated by the terrain generator expanding on a concept published by Amit Patel.", "About View-an-Isle", wxOK | wxICON_INFORMATION);
}

void ViewerFrame::OnOpen(wxCommandEvent& event) {
	wxFileDialog* ImportDialog = new wxFileDialog(this, _("Open map"), wxEmptyString, wxEmptyString, _("Map files|*.map"), wxFD_OPEN, wxDefaultPosition);

	// Creates a "open file" dialog with 4 file types
	if (ImportDialog->ShowModal() == wxID_OK) // if the user click "Open" instead of "Cancel"
	{
		wxString CurrentDocPath = ImportDialog->GetPath();
		FILE * file= fopen(CurrentDocPath.c_str().AsChar(), "rb");
		if (file != NULL) {
			mapPreview->GenerateGeometry(IO::importMap(file));
			mapPreview->Refresh(false);
		} else {
			wxLogMessage("Failed to open file");
		}
	}

	// Clean up after ourselves
	ImportDialog->Destroy();
}

wxBEGIN_EVENT_TABLE(ViewerFrame, wxFrame)
	EVT_MENU(ID_Open,   ViewerFrame::OnOpen)
    EVT_MENU(wxID_EXIT,  ViewerFrame::OnExit)
    EVT_MENU(wxID_ABOUT, ViewerFrame::OnAbout)
wxEND_EVENT_TABLE()

wxIMPLEMENT_APP(ViewerApp);

bool ViewerApp::OnInit() {
    ViewerFrame* frame = new ViewerFrame("View-an-Isle", wxPoint(100, 100), wxSize(800, 600));
	bool init = frame->Show(true);
	frame->InitializeGL();
	return init;
}
