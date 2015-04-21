#pragma once

#include <glm/glm.hpp>

#include "Map.h"

/**
 * An autonomous vehicle that moves around the map
 */
class Vehicle {
	// The current position of this Vehicle
	glm::vec2 position;

	protected:
		// The current target of the vehicle
		Map::Corner* target;
		// The next target of the vehicle
		virtual Map::Corner* getNextTarget() = 0;

	public:
		Vehicle(Map::Corner* start): target(start), position(start->point) {}
		// Make the vehicle take the next step
		void step();
		// Get the current vehicle position
		glm::vec2 getPosition() {
			return position;
		}
};

/**
 * A vehicle that is attracted to a corner property
 */
class PropertyVehicle: public Vehicle {
	protected:
		// The property that this vehicle is attracted to
		virtual float getProperty(Map::Corner*) = 0;
		virtual Map::Corner* getNextTarget();

	public:
		PropertyVehicle(Map::Corner* start): Vehicle(start) {}
};

/**
 * A vehicle that is attracted to elevation
 */
class ElevationPropertyVehicle: public PropertyVehicle {
	protected:
		virtual float getProperty(Map::Corner* c) {
			return c->elevation;
		}

	public:
		ElevationPropertyVehicle(Map::Corner* start): PropertyVehicle(start) {}
};

/**
 * A vehicle that is attracted to moisture
 */
class MoisturePropertyVehicle: public PropertyVehicle {
	protected:
		virtual float getProperty(Map::Corner* c) {
			return c->moisture;
		}

	public:
		MoisturePropertyVehicle(Map::Corner* start): PropertyVehicle(start) {}
};
