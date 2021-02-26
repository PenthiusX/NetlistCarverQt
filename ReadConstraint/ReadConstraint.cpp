
#include<iostream>
#include <string.h>
#include <stdio.h>
#include <fstream>
#include <vector>

#include <externals/rapidXml/rapidxml_utils.hpp>
#include "ReadConstraint.h"

using namespace rapidxml;
using namespace std;


CReadConstraints CReadConstraints::parseLoadConstraintFile( string readConstraintFile )
{
	string readConstraintType ;
	string netName ;
	string pinName ;
	string getNetsString ;
	string tagName ;
	string tagValue ;


	string netPat = "NET" ;
	string pinPat = "PIN" ;

	// declare all the vectors
	vector<string> netNames;
	vector<string> pinNames;

	//Creating a object of class CReadConstraint
	CReadConstraints constraintList;

	xml_document<> doc;
	// The following code causes RapidXml to parse the xml input file
	//file<> xmlFile("readConstraint_new.xml");
	//file<> xmlFile("readConstraint_new1.xml");
	file<> xmlFile( readConstraintFile.c_str() );  //c_str() 
	doc.parse<0>(xmlFile.data());

	// Get root node
	xml_node<> *root_node = doc.first_node("GF_CONSTRAINT");


	// Checking if the root node is present or not.
	if(root_node == nullptr) 
	{
		cerr << "Element not found!" << endl;
	}
	else 
	{
		cout << "In else block" << endl ;


		// Looping all the constraint Block's and getting the next sibling.
		for (xml_node<> * constraint_node = root_node->first_node("CONSTRAINT"); constraint_node; constraint_node = constraint_node->next_sibling())
		{
			// Extract the constraint type.
			//cout << "I have visited Constraint. " <<  constraint_node->first_attribute("name")->value() << endl ;
			readConstraintType = constraint_node->first_attribute("name")->value();
			CNetAndPinData netPinObj;

			xml_node<> *child = constraint_node->first_node();
			// Looping all the childs inside CONSTRINTS
			while (child != 0)
			{
				//cout << child->name() << " " << child->value() << endl;
				string tagName =  child->name() ;
				string tagValue =  child->value() ;

				//cout << "Lenght of value " << tagValue.length() << endl;
				// Tag has child module. 
				// Loop all child's and extract data.
				// For now only child will GETNETS

				if ( (tagName.find(netPat) != std::string::npos) and ( tagValue.length() == 0 ) ) 
				{
					//cout << "found NETS..!" << '\n';
					xml_node<> *getNodeChild = child->first_node();

					if (getNodeChild !=0 )
					{
						while ( getNodeChild !=0 )
						{
							cout << getNodeChild->name() << " " << getNodeChild->value() << endl;
							netPinObj.AddGetNet( getNodeChild->value() );
							getNodeChild = getNodeChild->next_sibling() ; // Looping next sibling.
						}
					}
					else
					{
						cout << "Error - NetNode has no value" << endl ;
					}
				}
				// store the data here..	

				// checking for net tags
				if (tagName.find(netPat) != std::string::npos )
				{
					netPinObj.AddNet( tagValue );

				}
				// checking for pin tags
				else if (tagName.find(pinPat) != std::string::npos )
				{
					netPinObj.AddPin( tagValue );
				}

				child = child->next_sibling(); 	//Looping the next child.
			}

			// Store the data for each constraint.
			if (readConstraintType == "set_low")
			{
				constraintList.AddObjsIntoSetLow( netPinObj );
			}
			else if ( readConstraintType == "set_high")
			{
				constraintList.AddObjsIntoSetHigh( netPinObj );
			}
			else if ( readConstraintType == "release")
			{
				constraintList.AddObjsIntoRelease( netPinObj );
			}
			else if ( readConstraintType == "start_tracing")
			{
				constraintList.AddObjsIntoStartTracing( netPinObj );
			}
			else if ( readConstraintType == "end_tracing")
			{
				constraintList.AddObjsIntoEndTracing( netPinObj );
			}
			else if ( readConstraintType == "dump")
			{
				constraintList.AddObjsIntoDump( netPinObj );
			}
			else if ( readConstraintType == "path_tracing")
			{
				constraintList.AddObjsIntoPathTracing( netPinObj );
			}
			else
			{
				cerr << "Invalid Constrant" << endl ;
				exit(0);
			}
		}
	}
//	constraintList.print();

	return constraintList ;
}
