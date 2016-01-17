/*
 * CS 70, Spring 2004, Assignment 7
 *
 * Implementation of the "City" and "Cities" classes.
 */

#include "city.hh"
#include <cassert>
#include <cmath>
#include <iostream>

using namespace std;

/*
 * Public functions defined in this file for class Cities:
 *
 *   Constructors and destructors:
 *	Cities::Cities(std::istream& stream);
 *					// Create cities from a stream
 *	Cities::~Cities();
 *
 *   Informational functions:
 *	int Cities::size() const;	// Return number of cities in array
 *
 *   Overloaded operators:
 *	City& Cities::operator[](int which) const;
 *
 * Public functions defined in this file for class City:
 *
 *   Constructors and destructors:
 *	City::City();			// Create a blank city
 *	City::City(const City& source);
 *	City::~City();
 *
 *   Informational functions:
 *	double City::distanceFrom(const City& other) const;
 *					// Find distance from another city
 *
 *   Overloaded operators:
 *	City& City::operator=(const City& source);
 *	istream& operator>>(istream& stream, City& city);
 *					// Read a city from a stream
 *	ostream& operator<<(ostream& stream, City& city);
 *					// Write a city to a stream
 */

/*
 * The following constants are used in the City class, but the stupid
 * language won't let us define them there.  So they'll be global to this
 * file.  Only one of these values is actually used in the code; to
 * change the program's units requires recompilation after changing
 * EARTH_RADIUS.
 */
const double EARTH_RADIUS_KM = 6378.137;
const double EARTH_RADIUS_NM = 3443.918;
const double EARTH_RADIUS_MILES = EARTH_RADIUS_KM / 1.609344;
const double EARTH_RADIUS = EARTH_RADIUS_KM;
					// Produce results in kilometers

/*
 * Read a variable-length list of cities from a stream.  Uses the City
 * input operator to read each city.
 *
 * BUGS: error checking is minimal.
 */
Cities::Cities(
    istream& stream)
  : nCities(0),
    cities(new City[1])
{
    int maxSize = 1;			// Current size of cities array

    while (1) {
	if (nCities >= maxSize) {	// Need to expand?
	    maxSize *= 2;		// ..if so, expand by doubling
	    City* newCities = new City[maxSize];

	    for (int i = 0;  i < nCities;  ++i)
		newCities[i] = cities[i];

	    delete[] cities;
	    cities = newCities;
	}

	stream >> cities[nCities];	// Read a city
	if (!stream)			// Done?
	    break;

	++nCities;			// Count a city
    }
}

Cities::~Cities()
{
    delete[] cities;
}

/*
 * Return the number of City objects in the array.
 */
int Cities::size() const
{
    return nCities;
}

/*
 * Access operator.  This returns an actual City.
 */
City& Cities::operator[](
    int which)
  const
{
    assert(which < nCities);
    return cities[which];
}

/*
 * Create a blank city.  This default constructor is needed because we
 * create arrays of cities.
 */
City::City()
  : name(),
    lat(0.0),
    lon(0.0)
{
}

City::City(
    const City& source)
  : name(source.name),
    lat(source.lat),
    lon(source.lon)
{
}

City::~City()
{
}

/*
 * Calculate the (great-circle) distance between two cities.  The
 * formula is adapted from Bowditch, The American Practical Navigator,
 * U.S. Defense Mapping Agency Publication No. 9, 1995, p. 342.
 * Bowditch is "the" reference on navigation and is readily available
 * in PDF form.
 *
 * The Bowditch formula produces the distance in degrees, which
 * happens to be handy for navigation (a degree is 60 nautical miles).
 * We instead calculate the distance in radians (acos) and then
 * multiply by the earth's radius, which gives us a surface distance
 * in whichever unit we prefer.
 */
double City::distanceFrom(
    const City& other)			// Other city to find distance from
  const
{
    double sinLatProd = sin(lat) * sin(other.lat);
    double lonDiff = lon - other.lon;
    double cosProd = cos(lat) * cos(other.lat) * cos(lonDiff);
    return EARTH_RADIUS * acos(sinLatProd + cosProd);
}

City& City::operator=(
    const City& source)
{
    name = source.name;
    lat = source.lat;
    lon = source.lon;

    return *this;
}

/*
 * Read a city name and latitude/longitude from a stream.  The
 * latitude and longitude are converted to radians as part of reading.
 * M_PI is a constant defined in <cmath>.
 */
istream& operator>>(
    istream& stream,
    City& city)
{
    stream >> city.name >> city.lat >> city.lon;
    city.lat *= M_PI / 180.0;
    city.lon *= M_PI / 180.0;
    return stream;
}

/*
 * Write a city name and latitude/longitude to a stream.  The
 * latitude and longitude are converted to degrees as part of writing.
 * M_PI is a constant defined in <cmath>.
 */
ostream& operator<<(
    ostream& stream,
    City& city)
{
    return stream << city.name << '\t' << city.lat * 180.0 / M_PI
      << ' ' << city.lon * 180.0 / M_PI;
}
