#include <set>

#include "MapSurfaceVisualisation.h"

#include "wx/log.h"

typedef std::vector<Map::Edge*, std::allocator<Map::Edge*> > BORDERLIST;
typedef std::set<Map::Corner*, std::less<Map::Corner*>, std::allocator<Map::Corner*> > CORNERSET;
typedef std::set<Map::Center*, std::less<Map::Center*>, std::allocator<Map::Center*> > NEIGHBOURSET;

#define rgb(r,g,b) r/255.0f, g/255.0f, b/255.0f

MapSurfaceCellVertices::MapSurfaceCellVertices(Map::Center* center, int size) : Attribute(size * 3 * 3) {
	Attribute vertices = Attribute(0, 0);
	
	glm::vec3 c(center->point, center->elevation);
	//wxLogError(wxT("elevation: %f, size: %i"), center->elevation, size);

	for(BORDERLIST::iterator it = center->borders.begin(); it != center->borders.end(); ++it) {
		glm::vec3 a((*it)->v0->point, (*it)->v0->elevation);
		glm::vec3 b((*it)->v1->point, (*it)->v1->elevation);
		
		//The correctly ordered face will have a normal with positive z
		if (glm::cross(a-c, b-c).z < 0) {
			glm::vec3 temp = a;
			a = b;
			b = temp;
		}

		float maxheight = 80.0f;

		GLfloat triangle[] = {
			a.x, a.y, a.z * maxheight,
			b.x, b.y, b.z * maxheight,
			c.x, c.y, c.z * maxheight
		};

		vertices = vertices + Attribute(3 * 3, &triangle[0]);
	}

	vertices.scale(1/15.0f);

/*
	CORNERSET::iterator itb = --center->corners.end();
	for (CORNERSET::iterator ita = center->corners.begin(); ita != center->corners.end(); ++ita) {
		wxLogError(wxT("add a triangle at (%f, %f, %f)"), c.x, c.y, c.z);
		
		glm::vec3 b((*ita)->point, (*ita)->elevation);
		glm::vec3 a((*itb)->point, (*itb)->elevation);
		
		GLfloat triangle[] = {
			a.x, a.y, a.z,
			b.x, b.y, b.z,
			c.x, c.y, c.z
		};

		vertices = vertices + Attribute(3 * 3, &triangle[0]);

		itb = ita;
	}
*/

	values = new GLfloat[length];
	for (int i = 0; i < length; i++) {
		values[i] = vertices[i];
	}
}

MapSurfaceCellVertices::~MapSurfaceCellVertices() {
	length = 0;
	wxDELETEA(values);
}

MapSurface::MapSurface(std::vector<Map::Center*> centers) {
	cellcount = centers.size();

	cells = new ColoredObject*[cellcount];

	MapSurfaceCellVertices** vertices = new MapSurfaceCellVertices*[cellcount];
	Normals** normals = new Normals*[cellcount];
	SolidColor** color = new SolidColor*[cellcount];

	//Calculate initial object attributes

	int i = 0;
	for (std::vector<Map::Center*>::iterator it = centers.begin(); it != centers.end(); ++it) {
		vertices[i] = new MapSurfaceCellVertices((*it), (*it)->borders.size());
		normals[i] = new Normals(vertices[i]->size()/3, *vertices[i], false);
		
		glm::vec3 c(0.2f, 0.8f, 0.2f);
		switch((*it)->biome) {
			case Map::LAKE:
				c = glm::vec3(rgb(91, 132, 173));
				break;
			case Map::BEACH:
				c = glm::vec3(rgb(172,159,139));
				break;
			case Map::ICE:
				c = glm::vec3(rgb(222, 230, 245));
				break;
			case Map::MARSH:
				c = glm::vec3(rgb(64, 108, 86));
				break;
			case Map::SNOW:
				c = glm::vec3(rgb(248, 248, 248));
				break;
			case Map::TUNDRA:
				c = glm::vec3(rgb(221, 221, 187));
				break;
			case Map::BARE:
				c = glm::vec3(rgb(187, 187, 187));
				break;
			case Map::SCORCHED:
				c = glm::vec3(rgb(153, 153, 153));
				break;
			case Map::TAIGA:
				c = glm::vec3(rgb(204, 212, 187));
				break;
			case Map::SHRUBLAND:
				c = glm::vec3(rgb(196, 204, 187));
				break;
			case Map::TEMPERATE_DESERT:
				c = glm::vec3(rgb(228, 232, 202));
				break;
			case Map::TEMPERATE_RAIN_FOREST:
				c = glm::vec3(rgb(164, 196, 168));
				break;
			case Map::TEMPERATE_DECIDUOUS_FOREST:
				c = glm::vec3(rgb(180, 201, 169));
				break;
			case Map::GRASSLAND:
				c = glm::vec3(rgb(196, 212, 170));
				break;
			case Map::TROPICAL_RAIN_FOREST:
				c = glm::vec3(rgb(156, 187, 169));
				break;
			case Map::TROPICAL_SEASONAL_FOREST:
				c = glm::vec3(rgb(169, 204, 164));
				break;
			case Map::SUBTROPICAL_DESERT:
				c = glm::vec3(rgb(233, 221, 199));
				break;
			case Map::OCEAN:
			default:
				c = glm::vec3(rgb(54, 54, 97));
		}

		color[i] = new SolidColor(vertices[i]->size()/3, c.r, c.g, c.b);
		i++;
	}
	wxLogError(wxT("Assigned %i biome based colors"), i);

	//Smooth normals
	i = 0;
	for (std::vector<Map::Center*>::iterator it = centers.begin(); it != centers.end(); ++it) {
		std::vector<Attribute*> neighbourVertices;
		std::vector<Normals*> neighbourNormals;

		//Find neighbouring data
		int j = 0;
		for (std::vector<Map::Center*>::iterator it2 = centers.begin(); it2 != centers.end(); ++it2) {
			for (NEIGHBOURSET::iterator it3 = (*it)->neighbours.begin(); it3 != (*it)->neighbours.end(); ++it3) {
				if ((*it2)->index == (*it3)->index) {
					//it2 refers to a neighbour of it
					neighbourVertices.push_back(vertices[j]);
					neighbourNormals.push_back(normals[j]);
				}
			}
			j++;
		}

		//Smoothing process
		normals[i]->smooth(vertices[i], neighbourVertices, neighbourNormals);

		i++;
	}

	wxLogError(wxT("Smoothed %i objects' normals"), i);

	//Construct objects
	i = 0;
	for (std::vector<Map::Center*>::iterator it = centers.begin(); it != centers.end(); ++it) {
		const GLfloat* attributes[] = {*vertices[i], *normals[i], *color[i]};
		cells[i] = new ColoredObject(vertices[i]->size()/3, &attributes[0]);
		i++;
	}
	wxLogError(wxT("Constructed %i objects"), i);
}

void MapSurface::draw() {
	for (int i = 0; i < cellcount; i++) {
		cells[i]->draw();
	}
}
