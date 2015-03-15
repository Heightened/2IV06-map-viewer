#pragma once

#include <GL/glew.h>

#include <wx/glcanvas.h>


#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <string>
#include <fstream>
#include <vector>
#include <iostream>

#include "Canvas.h"
//TODO: Seperate data structure definition from generator

class SimpleCanvas : public Canvas {
	std::vector<Map::Center*> centers;
	wxDECLARE_EVENT_TABLE();
public:
	SimpleCanvas(wxWindow* parent, wxSize size): Canvas(parent, size) {};
	virtual void GenerateGeometry(std::vector<Map::Center*>);
	virtual void Paint(wxPaintEvent& WXUNUSED(event));
	virtual void Initialize();
};
