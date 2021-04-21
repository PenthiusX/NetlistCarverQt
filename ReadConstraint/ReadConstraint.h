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
	private:
		// Holds the list of net names
		vector<string> nets;
		// Holds the list of pin names
		vector<string> pins;
		// Holds the string for GetNet.
		vector<string> getNet;
	public:

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


//		void printall()
//		{
//			cout << "*************************" << endl ;
//			if ( nets.size() > 0 )
//			{
//				cout << "Nets: " ;
//				 for (auto i = nets.begin(); i != nets.end(); ++i)
//					         cout << *i << " ";
//				 cout << endl;
//			}
//			if ( pins.size() > 0 )
//			{
//				cout << "Pins: " ;
//				 for (auto i = pins.begin(); i != pins.end(); ++i)
//					         cout << *i << " ";
//				 cout << endl;
//			}
//			if ( getNet.size() > 0 )
//			{
//				cout << "GetNets: " ;
//				 for (auto i = getNet.begin(); i != getNet.end(); ++i)
//					         cout << *i << " ";
//				 cout << endl;
//			}
//		}
};

// CReadConstraints class holds all types of Constraint types.
class CReadConstraints
{
	private:
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

	public:

        CReadConstraints parseLoadConstraintFile( string readConstraintFile );

//        void print()
//        {
//            cout << "Printing all the data" << endl ;
//            if ( setHigh.size() > 0)
//            {
//                for (int i = 0; i < setHigh.size(); i++)
//                {
//                    setHigh[i].printall() ;
//                }
//            }
//            else
//            {
//                cout << "sizeof setHigh  = 0" << endl ;
//            }

//            if ( setLow.size() > 0)
//            {
//                for (int i = 0; i < setLow.size(); i++)
//                {
//                    setLow[i].printall() ;
//                }
//            }
//            else
//            {
//                cout << "sizeof setLow  = 0" << endl ;
//            }

//            if ( release.size() > 0)
//            {
//                for (int i = 0; i < release.size(); i++)
//                {
//                    release[i].printall() ;
//                }
//            }
//            else
//            {
//                cout << "sizeof release  = 0" << endl ;
//            }

//            if ( startTracing.size() > 0)
//            {
//                for (int i = 0; i < startTracing.size(); i++)
//                {
//                    startTracing[i].printall() ;
//                }
//            }
//            else
//            {
//                cout << "sizeof startTracing  = 0" << endl ;
//            }

//            if ( endTracing.size() > 0)
//            {
//                for (int i = 0; i < endTracing.size(); i++)
//                {
//                    endTracing[i].printall() ;
//                }
//            }
//            else
//            {
//                cout << "sizeof endTracing  = 0" << endl ;
//            }
//        }
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
