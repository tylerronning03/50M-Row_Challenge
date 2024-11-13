#include <stddef.h>

#define tableSize 10000
#define cityNameLength 101
#define tempLength 6

int main(int argc, char *argv[]) {

    return 0;
    // open file

    // line by line

    // add temp
}

typedef struct City
{
    /* data */
    char* cityName;
    float sum;
    float max;
    float min;
    int tempsCount;
};



typedef struct hTable {
    // array of city structs


};

void addTemp (char* cityName, float temp) {
    // Check hash table for cityName

    // If it doesn't exist:
        // add to Hash table
        // make max and min set to this value
        // set name
        // set sum to this value

    // else it exists:
        // add to sum
        // add 1 to count


};

int hash (char* city) {

}
// Hash table struct
