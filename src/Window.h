#include <wx/wxprec.h>

#ifndef WX_PRECOMP
    #include <wx/wx.h>
#endif

#include "Canvas.h"
#include "StatusBar.h"

class ViewerApp : public wxApp {
public:
	virtual bool OnInit();
};

class ViewerFrame : public wxFrame {
	void OnNew(wxCommandEvent& event);
	void OnOpen(wxCommandEvent& event);
	void OnSave(wxCommandEvent& event);
	void OnExportMap(wxCommandEvent& event);
    void OnExit(wxCommandEvent& event);
    void OnAbout(wxCommandEvent& event);
	void OnGenerate(wxCommandEvent& event);
	void OnPointRandom(wxCommandEvent& event);
	void OnPointHex(wxCommandEvent& event);
	void OnPointPoisson(wxCommandEvent& event);
	void OnShapeRadial(wxCommandEvent& event);
	void OnShapeSquare(wxCommandEvent& event);
	void OnShapeBlob(wxCommandEvent& event);
    wxDECLARE_EVENT_TABLE();

	Canvas* mapPreview;
	wxGLContext* glContext;
	StatusProgressBar* statusBar;

public:
	ViewerFrame(const wxString& title, const wxPoint& pos, const wxSize& size);

	void InitializeGL();
};

enum {
    ID_New = wxID_HIGHEST + 1,
	ID_Open = wxID_HIGHEST + 2,
	ID_Save = wxID_HIGHEST + 3,
	ID_Export = wxID_HIGHEST + 4,
	ID_BtnGenerate = wxID_HIGHEST + 5,
	ID_RadioPointSelectorRandom = wxID_HIGHEST + 6,
	ID_RadioPointSelectorHex = wxID_HIGHEST + 7,
	ID_RadioPointSelectorPoisson = wxID_HIGHEST + 8,
	ID_RadioMapShaperRadial = wxID_HIGHEST + 9,
	ID_RadioMapShaperSquare = wxID_HIGHEST + 10,
	ID_RadioMapShaperBlob = wxID_HIGHEST + 11,
	ID_BtnExport = wxID_HIGHEST + 12
};
