#pragma once

#include<iostream>
#include<vector>

using namespace std;


/*
// ReadConstraint.h.
// Parses the ReadConstraint file in XML format and reads all the input constraints and 
// creates the object model for the design redyness module.
*/


class CDuumpconstraint
{
	private:
		int value;
};


// Holds the list of net and pin Names for each constraint type.
class CNetAndPinData
{
    public:
		// Holds the list of net names
        vector<string> nets;
		// Holds the list of pin names
        vector<string> pins;
		// Holds the string for GetNet.
        vector<string> getNet;


		// Add's netname to nets vector
        void AddNet(string netName)
		{
			nets.push_back(netName);
		}
		// Add's pinname to pins vector
        void AddPin(string pinName)
		{
			pins.push_back(pinName);
		}
		// Add's the getNet vector.
        void AddGetNet(string pinName)
		{
			getNet.push_back(pinName);
        }
};

// CReadConstraints class holds all types of Constraint types.
class CReadConstraints
{
    public:
		// Sets the all input value as 1 for all nets and pins.
		vector<CNetAndPinData> setHigh;
		// Sets the all input value as 0 for all nets and pins.
		vector<CNetAndPinData> setLow;
		// List of pin's and net's to be removed from the object model.
		vector<CNetAndPinData> release;
		// List of pin's or Net's from which the starting of begin points for carving.
		vector<CNetAndPinData> startTracing;
		// List of pin's or Net's where the carving or path tracing should end.
		vector<CNetAndPinData> endTracing;
		// List of all paths with both start & end points given.
		vector<CNetAndPinData> path_tracing;
		// Dump's the paths.
		vector<CNetAndPinData> dumpPath;



        CReadConstraints parseLoadConstraintFile( string readConstraintFile );

        // Add the object into setHigh
        void AddObjsIntoSetHigh( CNetAndPinData obj)
        {
            cout << "Adding into SetHigh" << endl ;
            setHigh.push_back(obj);
        }

        // Add the object into setLow
        void AddObjsIntoSetLow( CNetAndPinData obj)
        {
            setLow.push_back(obj);
        }

        // Add the object into Release
        void AddObjsIntoRelease( CNetAndPinData obj)
        {
            release.push_back(obj);
        }

        // Add the object into start Tracing
        void AddObjsIntoStartTracing( CNetAndPinData obj)
        {
            startTracing.push_back(obj);
        }

        // Add the object into end Tracing
        void AddObjsIntoEndTracing( CNetAndPinData obj)
        {
            endTracing.push_back(obj);
        }

        // Add the object into dumpPath
        void AddObjsIntoDump( CNetAndPinData obj)
        {
            dumpPath.push_back(obj);
        }

        // Add the object into path_tracing
        void AddObjsIntoPathTracing( CNetAndPinData obj)
        {
            path_tracing.push_back(obj);
        }
};
