/**
 * OS Coord: A Simple OS Coordinate Transformation Library for C
 *
 * This is a port of a the Javascript library produced by Chris Veness available
 * from http://www.movable-type.co.uk/scripts/latlong-gridref.html.
 */

#ifndef OS_COORD_ORDINANCE_SURVEY_H
#define OS_COORD_ORDINANCE_SURVEY_H

#include "os_coord.h"


/**
 * Transform a set of eastings and northings into a Ordinance Survey style grid
 * reference. If the point does not lie within the grid then an os_grid_ref_t
 * with an empty-string grid code (and all other fields undefined) is returned.
 */
os_grid_ref_t os_eas_nor_to_grid_ref(os_eas_nor_t point, os_grid_t grid);

#endif
