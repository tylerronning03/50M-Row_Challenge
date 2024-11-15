#include <stddef.h>   // defines NULL
#include <sys/mman.h> // allows for mmap
#include <sys/stat.h> // allows for getting metadata from file including the size of the file for mmap
#include <string.h>   // allows for strlen()
#include <stdio.h>
#include <fcntl.h>
#include <errno.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>

#define tableSize 10000
#define rowLength 107
#define cityLength 101

/// @brief stores all information related to the city
typedef struct
{
    char cityName[cityLength];
    double sum;
    float max;
    float min;
    int tempsCount;
} City;

/// @brief turns the name of the city into the index for storing, PsudoCode from https://en.wikipedia.org/wiki/Fowler%E2%80%93Noll%E2%80%93Vo_hash_function
/// @param city the name of the city upon which the hash func will create the index
/// @return The index in the array where the city should be placed
int hash(char *city)
{
    unsigned int hashVal = 2166136261;
    for (int i = 0; i < strlen(city); i++)
    {
        hashVal ^= (unsigned)city[i];
        hashVal = hashVal * (uint64_t)1099511628211;
    }
    return hashVal % tableSize;
}

void addTemp(City *table, char *cityName, float temp)
{
    // Check hash table for cityName
    int expectedLoc = hash(cityName);

    // search for city
    // 1. go to the expected location calculated above
    if (strcmp(table[expectedLoc].cityName, cityName) == 0) // it exists
    {
        //printf("City Found at expected Location");
        table[expectedLoc].sum += temp;     // add to sum
        table[expectedLoc].tempsCount += 1; // add 1 to count

        if (table[expectedLoc].min > temp) // check against min and max
        {
            table[expectedLoc].min = temp;
        }
        else if (table[expectedLoc].max < temp)
        {
            table[expectedLoc].max = temp;
        }

        return;
    }
    else if (!table[expectedLoc].cityName[0]) // Location == NULL
    {
        // add city at this pos
            // set name
            strcpy(table[expectedLoc].cityName, cityName);
            // make max and min set to this value
            table[expectedLoc].max = temp;
            table[expectedLoc].min = temp;
            // set sum to this value
            table[expectedLoc].sum = temp;
            table[expectedLoc].tempsCount = 1;

            return;
    }
    // make new var for loop condition
    int searchLoc = expectedLoc + 1;
    while (searchLoc != expectedLoc)
    {
        if (strcmp(table[searchLoc].cityName, cityName) == 0)
        {
            // city found
            table[searchLoc].sum += temp;
            table[searchLoc].tempsCount += 1;
            if (table[searchLoc].min > temp)
            {
                table[searchLoc].min = temp;
            }
            else if (table[searchLoc].max < temp)
            {
                table[searchLoc].max = temp;
            }

            return;
        }
        else if (!table[searchLoc].cityName[0]) // unsure about value being NULL
        {
            // add city at this pos
            // set name
            strcpy(table[searchLoc].cityName, cityName);
            // make max and min set to this value
            table[searchLoc].max = temp;
            table[searchLoc].min = temp;
            // set sum to this value
            table[searchLoc].sum = temp;
            table[searchLoc].tempsCount = 1;

            return;
        }
        // if the bucket is not the city and not NULL it is filled so go to next bucket due to linear searching
        // if we reach the end of the table, loop back to the front
        if (searchLoc == tableSize - 1)
        {
            searchLoc = (searchLoc + 1)%tableSize;
        }
        // otherwise move to next bucket for searching
        else
        {
            searchLoc += 1;
        }
    }
};

void print(City *table) 
{
    FILE* file = fopen("output.txt", "w");
    for (int i = 0; i < tableSize - 1; i += 1)
    {
        if (table[i].cityName[0])
        {
            fprintf(file, "%s, %.1f, %.1f, %.1f\n", table[i].cityName, table[i].min, (float)table[i].sum / table[i].tempsCount, table[i].max);
        }
        
    }
    fclose(file);
};

int main(int argc, char *argv[])
{
    City hashTable[tableSize]; // create hash table
    memset(hashTable, 0, tableSize * sizeof(City));
    
    char *file = argv[1];
    //char file[] = "measurements.txt";
    // open file
    int fd = open(file, O_RDONLY);
    if (fd == -1)
    {
        perror("open");
        return 1;
    }

    struct stat sb; // used to get file size
    if (fstat(fd, &sb) == -1)
    {
        perror("fstat");
        return 1;
    }

    char *data = mmap(NULL, sb.st_size, PROT_READ, MAP_SHARED, fd, 0);
    if (data == MAP_FAILED)
    {
        fprintf(stderr, "Mmap failed, errno: %d\n", errno); // from powerpoint
    }
    // line by line
    char* currLine;
    currLine = data;
    char* eof = data + sb.st_size;

    while (currLine < eof)
    {
        char* newLineEnd;
        newLineEnd = strpbrk(currLine, "\n");
        if (newLineEnd == NULL) {//Last Line
        newLineEnd = eof;
        }
        size_t lineLength = newLineEnd - currLine;
        char fullLine[lineLength + 1];
        memcpy(fullLine, currLine, lineLength);

        char* endOfCity = strpbrk(currLine, ";");
        if (endOfCity == NULL) {fprintf(stderr, "No Simicolin present"); currLine = newLineEnd + 1; continue;}
        
        size_t n = endOfCity - currLine;
        char cityName[n + 1];
        
        memcpy(cityName, fullLine, n);
        cityName[n] = '\0';
        float temp = strtof(endOfCity + 1, NULL);
        // add temp
        addTemp(hashTable, cityName, temp);
        currLine = newLineEnd + 1;
    }
    print(hashTable);
    return 0;
}