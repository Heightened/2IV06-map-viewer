#include "Map.h"
#include "Objects.h"
#include "StatusBar.h"

class MapSurfaceCellVertices : public Attribute {
public:
	MapSurfaceCellVertices(Map::Center* center, int size);
	~MapSurfaceCellVertices();
};

class MapSurface {
	int cellcount;
	ColoredObject** cells;
public:
	MapSurface(std::vector<Map::Center*> centers, StatusProgressBar* statusBar);
	void draw();
};
