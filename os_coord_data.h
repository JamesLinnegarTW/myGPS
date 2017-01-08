/**
 * OS Coord: A Simple OS Coordinate Transformation Library for C
 *
 * This is a port of a the Javascript library produced by Chris Veness available
 * from http://www.movable-type.co.uk/scripts/latlong-gridref.html.
 *
 * Useful data/constants for the UK.
 */

#ifndef OS_COORD_DATA_H
#define OS_COORD_DATA_H

/******************************************************************************
 * Ellipsoid definitions. Values taken from "A guide to coordinate systems in
 * Great Britain"
 ******************************************************************************/

// Airy 1830
const os_ellipsoid_t OS_EL_AIRY_1830 = {.a=6377563.396, .b=6356256.910};

// GRS80 (aka WGS84 ellipsoid)
const os_ellipsoid_t OS_EL_WGS84 = {.a=6378137.000, .b=6356752.3141};


/******************************************************************************
 * Helmert transformations
 ******************************************************************************/

// WGS84 -> OSGB36. Produces heights "similar to" ODN heights. From from "A
// guide to coordinate systems in Great Britain".
const os_helmert_t OS_HE_WGS84_TO_OSGB36 = {
	.tx= -446.448,  .ty=  125.157,   .tz= -542.060,
	.rx=   -0.1502, .ry=   -0.2470,  .rz=   -0.8421,
	 .s=   20.4894
};


/******************************************************************************
 * Transverse Mercator Projections. Values taken from "A guide to coordinate
 * systems in Great Britain".
 ******************************************************************************/


// Ordnance Survey National Grid
const os_tm_projection_t OS_TM_NATIONAL_GRID = {
	.e0=400000.0, .n0=-100000.0,
	.f0=0.9996012717,
	.lat0=49.0, .lon0=-2.0,
	.ellipsoid={.a=6377563.396, .b=6356256.910} // Airy 1830
};


/******************************************************************************
 * National grid system grids.
 ******************************************************************************/

// Ordinance Survey National Grid over England, Scotland and Wales.
const os_grid_t OS_GR_NATIONAL_GRID = {
	.num_digits=2,
	.bottom_left_first_char='S',
	.width=7,
	.height=13
};

#endif

