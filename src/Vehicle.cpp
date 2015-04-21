#include "Vehicle.h"

#include <cfloat>
#include <cstdio>

//TODO: Make this dependent on the resolution?
#define MIN_DISTANCE 0.01f

void Vehicle::step() {
	// Check if we've reached the target
	if (glm::distance(position, target->point) < MIN_DISTANCE) {
		target = getNextTarget();
	}
	// Move to the next target
	// TODO: move in smaller steps, 3D
	position = target->point;
}

Map::Corner* PropertyVehicle::getNextTarget() {
	Map::Corner* newTarget;
	//TODO: Custom comparator that enables min and max versions of this class
	float maxvalue = -FLT_MAX;
	float value;

	// Find all corners of all centers that the current target touches
	// (Since the current target is also among those, that means that this is stable)
	for (std::set<Map::Center*>::iterator it = target->touches.begin(); it != target->touches.end(); it++) {
		for (std::set<Map::Corner*>::iterator cit = (*it)->corners.begin(); cit != (*it)->corners.end(); cit++) {
			value = getProperty(*cit);
			if (value > maxvalue) {
				maxvalue = value;
				newTarget = *cit;
			}
		}
	}

	return newTarget;
}
