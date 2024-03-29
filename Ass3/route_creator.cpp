﻿#include "route_creator.h"
#include <iostream>
#include <string>
#include <fstream>

using namespace std;

istream& operator>>(istream& is, Travel_Times& t) {
	string origin;
	string destination;
	string time;
	if (getline(is, origin, '\t') &&
		getline(is, destination, '\t') &&
		getline(is, time)) {
		t.add(origin, destination, time);
	}
	return is;
}

void RouteCreator::Create(const Reader& r, const Travel_Times& t) {
	ofstream route("route.txt");
	auto status = 111;
	int time = 0, ship, origin, destination = 0;
	string origin_name;
	string destination_name;

	while (status != 0) {
		if (status != 2) {
			time = 0;
			cout << "Ships:" << endl;
			r.dump_ships();
			cout << "Select ship" << endl;
			cin >> ship;
		}

		switch (ship) {
		case 1: route << "Andromeda Ascendant\t";
			break;
		case 2: route << "Galactica\t";
			break;
		case 3: route << "Heart of Gold\t";
			break;
		case 4: route << "Millenium Falcon\t";
			break;
		case 5: route << "Moya\t";
			break;
		case 6: route << "NSEA Protector\t";
			break;
		case 7: route << "Red Dwarf\t";
			break;
		case 8: route << "Serenity\t";
			break;
		case 9: route << "USS Enterprise\t";
			break;
		case 10: route << "USSC Discovery One\t";
			break;
		default: ;
		}

		if (status != 2) {
			cout << "\nSelect origin\nPlanets:" << endl;
			t.dump(0); // Show them origin planets
			cin >> origin; // Receive origin choice
			cout << endl;
			origin_name = t.dump(origin); // Save origin name planet and also print destinations from there
			route << origin_name << '\t' << time << '\t'; // Print origin name planet and current time
			cout << "Select destination" << endl;
			cin >> destination; // From previously printed destination list from that origin planet, pick choice
			const auto travel_pair = t.travel_time(origin, destination); // retrieve travel time
			destination_name = travel_pair.first; // get destination name
			time += travel_pair.second; // update time
			route << destination_name << '\t' << time << endl; // print destination name and arrival time
		}

		if (status == 2) {
			origin = destination; // Update origin digit with what we already know
			origin_name = destination_name; // Update origin name with what we already know
			time += 4; // Add 4 hour delay
			route << origin_name << '\t' << time << '\t'; // Print origin name and updated current time
			cout << "\nSelect Destination\nPlanets:" << endl;
			origin = t.obtain_origin_int(origin_name);
			t.dump(origin); // Print available destinations from new origin
			cin >> destination; // obtain new destination
			const auto travel_pair = t.travel_time(origin, destination);
			destination_name = travel_pair.first; // get destination name
			time += travel_pair.second; // update time
			route << destination_name << '\t' << time << endl; // print destination name and arrival time
		}

		cout << "type 0 to halt, type 1 for another ship, type 2 to continue with same ship" << endl;
		cin >> status;
	}
}

int main(int argc, char** argv) {
	const string filename = argv[1]; // argument passed. this is route.txt
	fstream in(filename); // fstream created w/ route.txt


	// TRAVEL TIMES
	fstream conduits("conduits.txt"); // fstream for conduits.txt
	Travel_Times t; // TravelTimes object created
	while (conduits >> t) {} // Fill TravelTimes w conduits.txt
	t.dump(); // dump it for validation (std::cerr)


	// READER
	const Reader r(in, &t); // create a reader w/ route.txt and Travel_Times which we filled with "conduits.txt"

	// ROUTE CREATOR
	RouteCreator::Create(r, t);

	return 0;
}