#include <stddef.h>

#define tableSize 10000
#define cityNameLength 101
#define tempLength 6

// know that using mmap will be useful when getting/opening file

int main(int argc, char *argv[])
{

    return 0;
    // open file
    
    // line by line

    // add temp
}
/// @brief stores all information related to the city
typedef struct City
{
    char *cityName;
    double sum;
    float max;
    float min;
    int tempsCount;
};

/// @brief Table of buckets with length 10000(tableSize)
typedef struct hTable
{
    // array of city structs
    struct City table[tableSize];
};

void addTemp(struct hTable table, char *cityName, float temp)
{
    // Check hash table for cityName
    int expectedLoc = hash(cityName);

    // search for city
    // 1. go to the expected location calculated above
    if (table.table[expectedLoc].cityName == cityName) // it exists
    { 
        
        table.table[expectedLoc].sum += temp; // add to sum
        table.table[expectedLoc].tempsCount += 1; // add 1 to count
    
        if (table.table[expectedLoc].min > temp) // check against min and max
        {
            table.table[expectedLoc].min = temp;
        }
        else if (table.table[expectedLoc].max < temp)
        {
            table.table[expectedLoc].max = temp;
        }

        return;
    }
    // make new var for loop condition
    int searchLoc = expectedLoc + 1;
    while (searchLoc != expectedLoc)
    {
        if (table.table[searchLoc].cityName == cityName)
        {
            // city found
            table.table[searchLoc].sum += temp;
            table.table[searchLoc].tempsCount += 1;
            if (table.table[searchLoc].min > temp)
            {
                table.table[searchLoc].min = temp;
            }
            else if (table.table[searchLoc].max < temp)
            {
                table.table[searchLoc].max = temp;
            }

            return;
        }
        else if (table.table[searchLoc].cityName == NULL) // unsure about value being NULL
        {
            // add city at this pos
            table.table[searchLoc].cityName = cityName;
            table.table[searchLoc].max = temp;
            table.table[searchLoc].min = temp;
            table.table[searchLoc].sum = temp;
            table.table[searchLoc].tempsCount = 1;
            // make max and min set to this value
            // set name
            // set sum to this value
            return;
        }
        // if the bucket is not the city and not NULL it is filled so go to next bucket due to linear searching
        // if we reach the end of the table, loop back to the front
        if (searchLoc == tableSize - 1)
        {
            searchLoc = 0;
        }
        // otherwise move to next bucket for searching
        else
        {
            searchLoc += 1;
        }
    }
};
/// @brief turns the name of the city into the index for storing, PsudoCode from https://en.wikipedia.org/wiki/Fowler%E2%80%93Noll%E2%80%93Vo_hash_function
/// @param city the name of the city upon which the hash func will create the index
/// @return The index in the array where the city should be placed
int hash(char *city)
{
    int hashVal = 2166136261;
    for (int i = 0; i < strlen(city); i++)
    {
        hashVal = hashVal ^ city[i];
        hashVal = hashVal * 1099511628211;
    }
    return;
}
