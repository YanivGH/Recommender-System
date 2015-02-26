#include <Windows.h>
#include <cstdint>

#define DATABASE_SIZE 2000000	// number of entries of database
#define NAME_LIST_SIZE 4946
#define LONGEST_NAME_POSSIBLE 30
#define NUM_OF_RECOMMENDED_PEOPLE 10	// the number of people on which to recommend
#define PI 3.14159265

typedef struct database	// A struct of an entity (a person)
{
	char	PersonName[LONGEST_NAME_POSSIBLE];
	float	PersonLatitude;
	float	PersonLongitude;
	uint8_t PersonAge;
} DATABASE_STRUCT;

class CRecommendPeople
{
	int DatabaseSize;
	int NameListSize;
	uint8_t NumOfRecommendedPeople;
	char NameList[NAME_LIST_SIZE][LONGEST_NAME_POSSIBLE];

	DATABASE_STRUCT Database[DATABASE_SIZE];	// The database that contains the info of all the existing persons

public:
	CRecommendPeople();
	~CRecommendPeople();

	void create_database(int DatabaseSize);
	DATABASE_STRUCT read_query_info();	// returns the info of the query person
	void recommend_matches(DATABASE_STRUCT QueryPerson, DATABASE_STRUCT RecommendedPeople[NUM_OF_RECOMMENDED_PEOPLE]);
	float hamming_dist(char QueryPersonName[LONGEST_NAME_POSSIBLE], char CandidatePersonName[LONGEST_NAME_POSSIBLE], int QueryNameLength);
	DATABASE_STRUCT QueryPerson;	// The info of the query user (the query person to whom we want to find good matches)
	DATABASE_STRUCT RecommendedPeople[NUM_OF_RECOMMENDED_PEOPLE];
	float calc_dist_between_persons(DATABASE_STRUCT QueryPerson, DATABASE_STRUCT CandidatePerson);
};

CRecommendPeople	m_CRecommendPeople;
