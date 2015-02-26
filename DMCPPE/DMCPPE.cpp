// DMCPPE.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "DMCPPE.h"
#include <iostream>
#include <fstream>
#include <time.h>
#include <string>
#include <math.h>
#include <stdlib.h>
#include <iomanip>
#include <ctime>

using namespace std;


int _tmain(int argc, _TCHAR* argv[])	// main program
{
	bool exit = false;
	while (!exit)
	{	
		// Read the query person from the user
		m_CRecommendPeople.QueryPerson = m_CRecommendPeople.read_query_info();	// read the information of the user (the query person to whom we want to find good matches)
		clock_t begin = clock();
		m_CRecommendPeople.recommend_matches(m_CRecommendPeople.QueryPerson, m_CRecommendPeople.RecommendedPeople);		// recommend good matches for the query person
		clock_t end = clock();
		double elapsed_secs = double(end - begin) / CLOCKS_PER_SEC;
		cout << "Recommendation time: " << elapsed_secs << " [sec]" << endl << endl;

		cout << "Press ESC to exit\n";
		system("pause");
		if (GetAsyncKeyState(VK_ESCAPE))
			exit = true;
		cout << "\n\n";
	}

	return 0;
}

CRecommendPeople::CRecommendPeople()
{
	DatabaseSize = DATABASE_SIZE;
	NameListSize = NAME_LIST_SIZE;
	NumOfRecommendedPeople = NUM_OF_RECOMMENDED_PEOPLE;
	m_CRecommendPeople.create_database(DatabaseSize);	// creat a datastore of 'DatabaseSize' entries
}

CRecommendPeople::~CRecommendPeople()
{

}

void CRecommendPeople::create_database(int DatabaseSize)
{
	//srand(time(NULL)); // random seed to generate random number for each run of the code.

	// Read "names.txt" file
	ifstream myReadFile;
	string line;
	myReadFile.open("names.txt"); // a txt file of 4946 first-names taken from https://github.com/dominictarr/random-name/blob/master/first-names.txt (sorted alphabetically)
	int i = 0; 
	char tmp[LONGEST_NAME_POSSIBLE];
	if (myReadFile.is_open()) {
		while (!myReadFile.eof()) {
			getline(myReadFile, line);
			strcpy_s(NameList[i++], line.c_str());
		}
	}
	myReadFile.close();

	// Create the datastore
	for (int EntityIdx = 0; EntityIdx < DatabaseSize; EntityIdx++)
	{
		strcpy_s(Database[EntityIdx].PersonName, NameList[rand() % NameListSize]);	// randomly and uniformly select a name from the list and place it into the 'PersonName' cell of the entity. Actually the name selection should be based on the true distribution of names in the globe (or according to the population of users).
		Database[EntityIdx].PersonLatitude = (rand() % 180 - 90) + (float)rand() / (RAND_MAX + 1);		// latitude is between -90 [deg] (at the South Pole) to +90 [deg] (at the North Pole). A random number ('float' precision). Actually the latitude selection should be based on the true distribution of latitudes amongst the inhabited population in the globe (or according to the population of users). No reason to include random latitudes of deserted islands or the sea (which in fact covers most of the earth).
		Database[EntityIdx].PersonLongitude = (rand() % 360 - 180) + (float)rand() / (RAND_MAX + 1);	// longitude is between -180 [deg] to +180 [deg]. A random number ('float' precision). Actually the longitude selection should be based on the true distribution of longitudes amongst the inhabited population in the globe (or according to the population of users). No reason to include random longitudes of deserted islands or the sea (which in fact covers most of the earth).
		Database[EntityIdx].PersonAge = (rand() % 116 + 4);	// age is a random number between 4 to 120. I assume it's in a year precision since a month precision is irrelevant to recommending a similar people for someone who's above the age of 4. Here too, the best is to sample ages according to the true distribution of ages in the globes (or according to the population of users).

		Database[EntityIdx].PersonLatitude *= PI / 180.0;	// convert to radian (I can merge this line with the calculation above, but I wanted to explain here)
		Database[EntityIdx].PersonLongitude *= PI / 180.0;	// convert to radian (I can merge this line with the calculation above, but I wanted to explain here)
	}

}

DATABASE_STRUCT CRecommendPeople::read_query_info()
{
	DATABASE_STRUCT QueryPerson;
	string line;

	//Read query user's name (any character is legit) 
	while (line.empty())
	{
		cout << "Please enter user's name: ";
		getline(cin, line);
		char tmp[LONGEST_NAME_POSSIBLE];
		strcpy_s(QueryPerson.PersonName, line.c_str());
	}
		
	//Read query user's age {4, 120}
	line.clear();
	while (line.empty())
	{
		cout << "Please enter user's age {4, 120}: ";
		getline(cin, line);
		for (int i = 0; i < line.length(); i++)
		{
			if ((atoi(line.substr(i, 1).c_str()) == 0) && (line.substr(i, 1).compare("0") != 0))	// 1st condition: NaN; 2nd condition: not a zero
				line.clear();
		}

		// age needs to be in the range
		int age = atoi(line.c_str());
		if ((age < 4) || (age > 120))
			line.clear();
	}
	QueryPerson.PersonAge = (uint8_t)atoi(line.c_str());


	//Read query user's latitude {-90, +90}
	line.clear();
	while (line.empty())
	{
		cout << "Please enter user's latitude {-90, +90}: ";
		getline(cin, line);
		for (int i = 0; i < line.length(); i++)
		{
			if ((atoi(line.substr(i, 1).c_str()) == 0) && (line.substr(i, 1).compare("0") != 0) && (line.substr(i, 1).compare(".") != 0) && (line.substr(i, 1).compare("-") != 0))	// 1st condition: NaN; 2nd condition: not a zero
				line.clear();
		}

		if (line.find(".", line.find(".") + 1) != -1)	// check if more than one decimal points
			line.clear();
		
		if (line.find("-", 1) != -1)	// check if more than one minuses and if the minus is in the legit location
			line.clear();

		// latitude needs to be in the range
		float latitude = atof(line.c_str());
		if ((latitude < -90) || (latitude > 90))
			line.clear();
	}
	QueryPerson.PersonLatitude = atof(line.c_str()) * PI / 180.0;	// convert to radian


	//Read query user's longitude {-180, +180}
	line.clear();
	while (line.empty())
	{
		cout << "Please enter user's longitude {-180, +180}: ";
		getline(cin, line);
		for (int i = 0; i < line.length(); i++)
		{
			if ((atoi(line.substr(i, 1).c_str()) == 0) && (line.substr(i, 1).compare("0") != 0) && (line.substr(i, 1).compare(".") != 0) && (line.substr(i, 1).compare("-") != 0))	// 1st condition: NaN; 2nd condition: not a zero
				line.clear();
		}

		if (line.find(".", line.find(".") + 1) != -1)	// check if more than two decimal points
			line.clear();

		if (line.find("-", 1) != -1)	// check if more than one minuses and if the minus is in the legit location
			line.clear();

		// longitude needs to be in the range
		float longitude = atof(line.c_str());
		if ((longitude < -180) || (longitude > 180))
			line.clear();
	}
	QueryPerson.PersonLongitude = atof(line.c_str()) * PI/180.0;	// convert to radian

	return(QueryPerson);
}

void CRecommendPeople::recommend_matches(DATABASE_STRUCT QueryPerson, DATABASE_STRUCT RecommendedPeople[NUM_OF_RECOMMENDED_PEOPLE])
{
	float TotalDistance;	// distance between two entries (distance is the opposite of similarity; higher distances means less similarity)
	int	  TempTopBestIdx[NUM_OF_RECOMMENDED_PEOPLE];		// This is the temporary list of indices of the top best people
	float TempTopBestDist[NUM_OF_RECOMMENDED_PEOPLE];		// This is the temporary list of distances of the top best people

	BOOL flDoNextSort = TRUE;
	float maxval; int maxidx;

	for (int i = 0; i < DatabaseSize; i++)
	{
		// calculates the distance between the query to a database entry
		TotalDistance = calc_dist_between_persons(QueryPerson, Database[i]);
																								// In fact in my example, the weight are equal so I can even take them out of the equation.
		//~~~~~~~~ Create a list of the Top Best entries (entries that are mostly similar to the query entry)
		if (i < NumOfRecommendedPeople) // in the beggining for the 1st 'NumOfRecommendedPeople' people in the database, just write them in the top lists
		{
			TempTopBestIdx[i] = i;
			TempTopBestDist[i] = TotalDistance;
		}
		else
		{
			//static int cnt = 0;
			if (flDoNextSort)	// NOTICE: the proceedure below is a sort fo the top best (only 10 in this example), but it is executed only when enabled (when there is a new top 10 entry). 
			{					// So in time there will be less and less sorts since the list will contain small distances already - and this part becomes very negligible in the run-time
								// for example, for a 'DatabaseSize' = 2M, the sort was called for only 0.007% of the times
				//cnt++; 
				maxval = TempTopBestDist[0]; maxidx = 0;
				for (int j = 1; j < NumOfRecommendedPeople; j++)
				if (TempTopBestDist[j] > maxval) { maxval = TempTopBestDist[j]; maxidx = j; }
			}

			if (TotalDistance < maxval)
			{
				TempTopBestDist[maxidx] = TotalDistance;
				TempTopBestIdx[maxidx] = i;
				flDoNextSort = TRUE;	// the list was changed so do sort the next time
			}
			else
				flDoNextSort = FALSE;	// No change in the list so don't sort the next time
		}
	}

	// Copy the most similar
	for (int j = 0; j < NumOfRecommendedPeople; j++)
		memcpy(&RecommendedPeople[j], &Database[TempTopBestIdx[j]], sizeof(DATABASE_STRUCT));

	// Print results
	cout << "\nMost similar persons recommended: \n\n" ;
	cout << setw(3) << "Num" << setw(10) << "Name" << setw(12) << "Age" << setw(20) << "Latitude [deg]" << setw(20) << "Longitude [deg]" << endl;
	cout << setw(3) << "---" << setw(10) << "----" << setw(12) << "---" << setw(20) << "--------------" << setw(20) << "---------------" << endl;
	for (int j = 0; j < NumOfRecommendedPeople; j++)
		cout << left << setw(2) << j + 1 << setw(5) << ": " << setw(15) << RecommendedPeople[j].PersonName << setw(12) << (int)RecommendedPeople[j].PersonAge << setw(19) << RecommendedPeople[j].PersonLatitude * (180 / PI) << setw(12) << RecommendedPeople[j].PersonLongitude  * (180 / PI) << endl;
	cout << endl;
}

float CRecommendPeople::hamming_dist(char QueryPersonName[LONGEST_NAME_POSSIBLE], char CandidatePersonName[LONGEST_NAME_POSSIBLE], int QueryNameLength)
{
	// hamming distance counts the number of characters that are different in two equal sized strings. In our case, the strings might be of different sizes.
	  
	int HammingDist = 0;
	int MinLen = min(QueryNameLength, strlen(CandidatePersonName));
	for (int j = 0; j < MinLen; j++)
	{
		HammingDist += CandidatePersonName[j] != QueryPersonName[j];	// comparing the ascii code the char instead of the char (same effect)
	}
	return(HammingDist);
}

float CRecommendPeople::calc_dist_between_persons(DATABASE_STRUCT QueryPerson, DATABASE_STRUCT CandidatePerson)
{
	// This function calculates the distance between the query to a database entry

	float AgeDist, NearbyDist, TotalDistance;
	float WeightAgeDist = 0.5, WeightNearbyDist = 0.5;
	int EarthRadius = 6371; // in [km]

	float MaxDistanceBetween2Points = PI * EarthRadius; // The distance between any two point on earth can go from '0' (they are at the same place) up to half of earth's perimeter at the equator
	float MaxAgeDistance = 120 - 4;	// The age range is [4,120] so the difference can go from '0' (they are of same age) up to 120-4. Later I will use it for normalization

	//float NameDist;
	//int QueryNameLength = strlen(QueryPerson.PersonName);


	// Calculates age distance
	AgeDist = QueryPerson.PersonAge - CandidatePerson.PersonAge;	// Currently I don't care about the sign since further it will be positive

	// Calculate the distance between two point on the earth using the Haversine formula (http://en.wikipedia.org/wiki/Haversine_formula) [km]
	NearbyDist = 2 * EarthRadius * asin(sqrt(pow(sin((QueryPerson.PersonLatitude - CandidatePerson.PersonLatitude) / 2), 2) + cos(QueryPerson.PersonLatitude) * cos(CandidatePerson.PersonLatitude) * pow(sin((QueryPerson.PersonLongitude - CandidatePerson.PersonLongitude) / 2), 2)));




	// ~~~~ NOTICE ~~~~~ : 
	// I initially thought you wanted that the similarity would be based also on the distance between names of the people (though it doesn't make sense, but still). 
	// Therefore I implemented below also the part that measure distances between names as part of the similarity. When I relized you aren't looking for that - I put it in comment and don't use it 

	//NameDist = hamming_dist(QueryPerson.PersonName, Database[i].PersonName, QueryNameLength);	// I'm using hamming distance to compare the names, but there are beter algorithms (Levenshtein distance) but they are more time consuming
	// ~~~~		   ~~~~~




	// Normalizations: we wish to normalize each distanse (Nearby and Age) to be in the range of [0,1] so we could further combine them:
	AgeDist /= MaxAgeDistance;
	NearbyDist /= MaxDistanceBetween2Points;	// 'NearbyDist' can go from '0' (they are at the same place) up to half of earth's perimeter at the equator ('MaxDistanceBetween2Points')

	// Calculate total distance using a weighted Euclidian distance
	TotalDistance = WeightAgeDist*AgeDist*AgeDist + WeightNearbyDist*NearbyDist*NearbyDist; // This is the weighted distance. Mathematically I should extract the square-root of it and normalize it by the sum of weights, but it wouldn't change the order of the distances between the candidates to the query and I'm trying to minimize the run-time.

	return (TotalDistance);
}
