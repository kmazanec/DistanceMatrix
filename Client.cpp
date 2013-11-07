#include <fstream.h>
#include "matrix.h"
#include "mystring.h"
#include "DiM_classes.cpp"

void main ()
{
	cout << "Welcome to the testing program client file\n";

	vector <string> firstCutraStopId;
	matrix <int> dist(447,447);

	inputData( firstCutraStopId, dist );

	int choice, x, c, r;

//	cin >> x;


// Displays entire list of stop ID's, 5 per line

/*	for ( c = 0; c < firstCutraStopId.size(); c++ )
	{
		cout << convertDistance(firstCutraStopId[c]) << '\t';

		if (c % 5 == 0) cout << endl;
	}
*/


//	Displays entire distance matrix

/*	for ( r = 0; r < dist.numrows(); r++ )
	{
		for ( c = 0; c < dist.numcols(); c++ )
		{
			cout << convertDistance( dist[r][c] ) << '\t';
		}
		cout << "-------------------------------------------------------------" << endl;
	}
*/


	string stopId;

	vector <ID> num2;

//	vector < vector <ID> > num3;
	matrix <ID> num3(0,0);

	string id1 = "2003722WC49F   ";
	string id2 = "2002010W       ";

	while (true)
	{
		cout << "Welcome to the program\n";
		cout << "What would you like to do?\n";
		cout << " (1) Print all ID's\n";
		cout << " (2) Distance btw two points\n";
		cout << " (3) Closest x stops to a given ID\n";
		cout << " (4) Closest x stops to all ID's\n";
		cout << " (5) All stops within a radius for one\n";
		cout << " (6) All stops within a radius for all\n";
		cout << " (0) QUIT\n";
		cin >> choice;
		
		switch (choice)
		{
		case 1:
			// Displays entire list of stop ID's, 5 per line
	
			for ( c = 0; c < firstCutraStopId.size(); c++ )
			{
				cout << firstCutraStopId[c] << '\t';
		
				if (c % 5 == 0) cout << endl;
			}
			break;

		case 2:
			cout << distAtoB( firstCutraStopId, dist, id1, id2 ) << endl; 
			break;

		case 3:
			cout << "Enter the number of points needed: ";
			cin >> x;

			stopId = "2051051WB007   ";

			num2 = closestXtoID( firstCutraStopId, dist, x, stopId );

			for ( c = 0; c < num2.size(); c++ )								//note that the first distance is 0
			{
				cout << num2[c].distance << "\t" << num2[c].stopID << endl;
			}

			break;

		case 4:
			cout << "Enter the number of points needed: ";
			cin >> x;

			num3 = closestXtoALL( firstCutraStopId, dist, x );

			for ( r = 0; r < firstCutraStopId.size(); r++ )
			{
				for ( c = 0; c < num3.numcols(); c++ )
				{
					cout << ( ( num3[r] )[c] ).distance << "\t";
				}
				cout << "\n--------------------------------------------------------\n";
			}
			break;

		case 5:
			num2 = withinOneRadius( firstCutraStopId, dist, 100, id1 );
			for( c = 0; c < num2.size()-1; c++ )
			{
				cout << num2[c].distance << "\t"<< num2[c].stopID << endl;		//last element is garbage
			}
			break;

		case 6:
			cout << "Enter the radius desired: ";
			cin >> x;

			num3 = withinAllRadius( firstCutraStopId, dist, 100 );

			for ( r = 0; r < firstCutraStopId.size(); r++ )
			{
				for ( c = 0; c < num3.numrows(); c++ )
				{
					cout << num3[r][c].distance << "\t";
				}
				cout << "--------------------------------------------------------\n";
			}
			break;

		case 0:
			return;
		}
	}

}