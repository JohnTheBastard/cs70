#ifndef CITY_HH
#define CITY_HH

/*
 * Support classes for Cities
 *
 * A City is the basic object used in the traveling-salesrep problem.
 * Each city consists of a name and a position.  The names are used
 * only when producing output.  The position is given in latitude
 * (north is positive) and longitude (east is positive).  The class
 * only supports reading and writing of a City, plus a way to
 * calculate the distance between a City and any other City.
 *
 * The TSP also needs to be able to handle arrays of Cities.  The
 * Cities class implements a self-expanding array of City objects.
 */

#include <iostream>
#include <string>

/*
 * The following classes are defined in this file:
 */
class Cities;
class City;

/*
 * The Cities class defines an array of cities.
 */
class Cities {
    public:
	explicit Cities(std::istream& stream);
					// Create cities from a stream
	~Cities();

	/*
	 * Informational functions
	 */
	int size() const;		// Return number of cities in array

	/*
	 * Overloaded operators.
	 */
	City& operator[](int which) const;

    private:
	/*
	 * Disable copy and assignment.
	 */
	Cities(const Cities& source);
	Cities& operator=(const Cities& source);

	/*
	 * Private data.
	 */
	int nCities;			// Number of cities
	City* cities;			// Array of cities
};

/*
 * The City class defines a single city.
 */
class City {
    public:
	City();				// Create a blank city
	City(const City& source);
	~City();

	double distanceFrom(const City& other) const;
					// Find distance from another city

	/*
	 * Overloaded operators.
	 */
	City& operator=(const City& source);
	friend std::istream& operator>>(std::istream& stream, City& city);
					// Read a city from a stream
	friend std::ostream& operator<<(std::ostream& stream, City& city);
					// Write a city to a stream

    private:
	/*
	 * Private data
	 */
	std::string name;		// City name
	double lat;			// Latitude
	double lon;			// Longitude
};

#endif // CITY_HH
