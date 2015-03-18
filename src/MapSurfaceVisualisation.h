#include "Map.h"
#include "Objects.h"

class MapSurfaceCellVertices : public Attribute {
public:
	MapSurfaceCellVertices(Map::Center* center, int size);
	~MapSurfaceCellVertices();
};

class MapSurface {
	int cellcount;
	ColoredObject** cells;
public:
	MapSurface(std::vector<Map::Center*> centers);
	void draw();
};
