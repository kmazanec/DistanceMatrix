// v0.81
// 4/29/04

#include <fstream.h>
#include <stdlib.h>
#include "mystring.h"
#include "vector.h"
#include "matrix.h"

double convertDistance(int dist)
{
	if (dist < 0) dist *= -1;
	return double(dist) / 100.0;
}

struct ID	//for sorting purposes
{
	string stopID;
	double distance;
};

//Function to determine the partitions
//partitions input_array, returning middle index. Used by procedure quicksort
int partition(vector <ID> &input_array, int top, int bottom)
{
     double x = input_array[top].distance;
     int i = top - 1;
     int j = bottom + 1;
     ID temp;
     do
     {
           do      //move j towards top to the next element less than or equal to x.
           {
                  j--;
           }while (x > input_array[j].distance);

          do      //move j towards bottom to the next element greater than or equal to x.
         {
                 i++;
          } while (x < input_array[i].distance);

          if (i < j)
         { 
                 temp.distance = input_array[i].distance;    //switch elements at positions i and j
                 input_array[i].distance = input_array[j].distance;
                 input_array[j].distance = temp.distance;

				 temp.stopID = input_array[i].stopID;
				 input_array[i].stopID = input_array[j].stopID;
				 input_array[j].stopID = temp.stopID;
         }
     }while (i < j);     //loop ends when i and j have met in the middle
     return j;           // j and above represents top partition, below j is bottom partition
}


void quicksort(vector <ID> &input_array, int top, int bottom) // uses recursion-calling itself
{
     int middle;
     if(top<bottom)
    {
          middle = partition(input_array, top, bottom);
          quicksort(input_array, top, middle);   //sort top partition
          quicksort(input_array, middle+1, bottom);    //sort bottom partition
     }
     return;
}



void inputData(vector <string> & firstCutraStopId, matrix <int> & dist)
{
	// must be changed
	const int N = 447;
	// must be changed


	firstCutraStopId.resize(N); // a vector of the stop ID names
	char temp[15];

	ifstream stopIdFile; // stream for the IdFile
	stopIdFile.open("CR0923.id"); // opening the stopID file
	int c = 0; // counter for the loop
	while ( ! stopIdFile.eof() ) // loop reads in every element from the file and puts it in the array
	{
		stopIdFile.getline(temp, 17, '\n'); // get the next line and store it to the temporary char array
		firstCutraStopId[c] = temp; // put the char array into the vector of strings
		c++; // incriment
	}

	int oriDestcount = N; // number of stopIDs
	int *inrecord=0; // pointer for an array of ints (i think)
	inrecord= new int[oriDestcount]; // point the pointer to a new array

	ifstream inMatrix; // stream for reading in the matrix data
	inMatrix.open("CR0923.mt2", ios::binary); // open the matrix file for binary reading


	// matrix //
	matrix <int> dist1(N,N); // create a matrix for storing all the distances
	dist = dist1;


	for (int kkn=0; kkn < N; kkn++) // a loop that will go through each line of the file
	{
		inMatrix.seekg(kkn*(oriDestcount)*sizeof(int)); // not sure what this does

		inMatrix.read(reinterpret_cast<char *> (inrecord), oriDestcount*sizeof(int)); // not sure what this does

		for (int nnk=0; nnk<oriDestcount; nnk++) // nested loop which reads in each data point from the line
		{
			dist[kkn][nnk] = inrecord[nnk]; // puts numbers into matrix
		}
	}
	inMatrix.close(); // close the strean
	delete [] inrecord; // delete the temporary array
}

// Query the distance matrix btw two points
double distAtoB(vector <string> & id, matrix <int> & dist, string id1, string id2) 
{
//	id1 = "1001634        ";
//	id2 = "P975W          ";

	// now should search the stopID file for the value of the node in id
	int c = 0;
	while ( id1 != id[c] ) // assumes that id1 was entered correctly
	{
//		cout << id[c];
		c++;
	}
	int d = 0;
	while ( id2 != id[d] ) // assumes that id1 was entered correctly
	{
		d++;
	}

	return convertDistance(dist[c][d]);
}


// Show closest X stops to a given ID
vector <ID> closestXtoID(vector <string> & id, matrix <int> & dist, int x, string stopID) //absolute value? divide by 100?
																	//which is the starting point? (r or c?)
{
//	string stopID;

//	stopID = "453933W        ";

//	int x;	// the variable which will hold how many data points are to be found
//	cout<<"Please enter x (between 1 and 446, inclusive): ";
//	cin>>x;

	int n = id.size();

	int r=0;
	while(id[r]!=stopID)	//locate the corresponding row of distances in the matrix
		r++;
		
	vector <ID> rowOfInterest(n + 1);

//	int n=447;
	for(int c=0; c<n; c++)	//copy the row of distances as well as the vector of stopIDs correspondingly
	{
		rowOfInterest[c].distance=dist[r][c];
		rowOfInterest[c].stopID=id[c];
	}

	quicksort(rowOfInterest, 0, n);

	for(int b=1; b<=x; b++)	//starts from 1 because rowOfInterest[0] contains 0
	{
		rowOfInterest[b].distance = convertDistance(rowOfInterest[b].distance);
	}

	rowOfInterest.resize(x+1); // resize to only include the desired number of elements

	return rowOfInterest;
}

//Show closest X stops and their distance to all stops
matrix <ID> closestXtoALL(vector <string> & id, matrix <int> & dist, int x)
{

	int n = id.size();

	int k, c, b;

//	int x; // again, the number of data points needed
//	cout<<"please enter x (between 1 and 446, inclusive): ";
//	cin>>x;
	vector <ID> rowOfInterest(n + 1); // will be changed each iteration of k, old one will be added to allrows

	matrix <ID> allrows( n+1, x+1 ); // work on being able to return all the different vectors to the GUI


	for ( k = 0; k < n; k++ )	//each iteration of the loop shows closest X stopIDs to 1 ID
								//unfortunately the function closestXtoID cannot be called here
								//because vector <ID> cannot be passed as a parameter
	{

		rowOfInterest.resize(n+1); // needs to be done because was shrunk before


//		int n=447;
//		double cdist;
		for( c = 0; c < n; c++ )
		{
			rowOfInterest[c].distance = dist[k][c];
			rowOfInterest[c].stopID = id[c];
		}

		quicksort( rowOfInterest, 0, n );

		for( b = 1; b <= x; b++ )	
		{
			rowOfInterest[b].distance = convertDistance (rowOfInterest[b].distance );
		}

		rowOfInterest.resize( x+1 );

// **** change **** //

		for ( c = 0; c < x+1; c++ )
		{
			allrows[k][c] = rowOfInterest[c];
		}

// **** change **** //

	}

	return allrows;
}

// Show all stops inside a certain radius
vector <ID> withinOneRadius(vector <string> & id, matrix <int> & dist, double rad, string theid)
{

	int n = id.size();

//	double rad;

//	cout << "Enter the desired radius to be searched in: "; // ?? should radius be *100 or not??
//	cin >> rad;

//	string theid;

//	we hate getline
//	cout << "Enter the stop ID for which you would like the radii: ";
//	getline(cin, theid);

//	theid = "2051051WB007   ";

	// now should search the stopID file for the value of the node in id
	int row = 0;
	while ( theid != id[row] ) // assumes that id1 was entered correctly
	{
//		cout << id[row];
		row++;
	}

	vector <ID> withinRad(n + 1);

	ID temp;

	int col, count = 0;
	for ( col = 0; col < dist.numcols(); col++)
	{
		if ( convertDistance(dist[row][col]) <= rad )
		{
			temp.stopID = id[col];
			temp.distance = convertDistance(dist[row][col]);
			// the distance is dist[row][col], will eventually be added to a data structure?
//			cout << id[col] << " is within the radius with distance of " << convertDistance(dist[row][col]) << endl;
			
			withinRad[count] = temp;
			count ++;
		}
	}

	withinRad.resize(count + 1); // resize the array to be smaller and not wasteful
	return withinRad;
} 

// Show all stops inside a certain radius for all stops
matrix <ID> withinAllRadius(vector <string> & id, matrix <int> & dist, double rad)
{

	int n = id.size();

//	double rad;

//	cout << "Enter the desired radius to be searched in: "; // ?? should radius be *100 or not??
//	cin >> rad;

	int row = 0;
	int col;
	int count = 0;

	vector <ID> withinRad(n + 1);
	ID temp;
	matrix <ID> allRadii( n+1, x+1 );

	for ( row = 0; row < dist.numrows(); row++)
	{
		for ( col = 0; col < dist.numcols(); col++)
		{
			if ( convertDistance(dist[row][col]) <= rad && row != col)
			{
				temp.stopID = id[col];
				temp.distance = convertDistance(dist[row][col]);
				// the distance is dist[row][col], will eventually be added to a data structure?
//				cout << convertDistance(dist[row][col]) << '\t';
				withinRad[count] = temp;
				count ++;
			}
		}
		allRadii[row] = withinRad;
//		cout << endl << "--------------------------------------------" << endl;
	}

	return allRadii;
}

