#include <stdio.h>

// THE CACHE
struct LINE{
    char valid;
    int tag;
    char data[32];
};

struct SET{
    struct LINE line[1];
};

struct CACHE {
    struct SET set[8];
} cache;

// GLOBALDATA
// increment these in your function
unsigned hit_count  = 0;
unsigned miss_count = 0;
unsigned read_data_count = 0;

// SYSTEM BUS
// call Read_Data_From_Ram to update this
char system_bus[32];



// READ DATA FROM RAM
// this function copies 32 character from the text file to
// the system_bus array aligned with the system bus
// (we will not test this with input larger than the text file)
void Read_Data_From_Ram(unsigned address) {
    address >>= 5; address <<= 5; // get align the data
    read_data_count++;
    FILE *file;
    file = fopen("alice_in_wonderland.txt", "r");
    fseek(file, address, SEEK_SET);
    for (int i = 0; i<32; i++) system_bus[i] = fgetc(file);
    return;
}


// COMPLETE THE FOLLOWING FUNCTION
//
// this code should check to see if the data in the cache is valid and the tag matches
// if so it should return the char matching the address and increment the hit count
//
// if not it should read the data from the system bus copy it into the cache and
// increment the miss count and return the char matching the address
//
char Read_Data_From_Cache(unsigned address)
{
    unsigned setBit = (address >> 5) % 8;          // Store bits 5:7 (set bits)
    unsigned tag = address >> 8;                   // Store bits 32:8 (tag)
    struct LINE* line = cache.set[setBit].line;    // Pointer to correct line in cache to check

    if (line->valid == 1 && line->tag == tag) {    // If line is valid and its tag matches that of address
        hit_count++;                               // Increase hit count, do not copy data

    } else {                                       // If line isn't valid or tag doesn't match
        Read_Data_From_Ram(address);               // Load data from main memory
        int a = address % 32;
        int b = address - address % 32;
        for (int i = b; i < b + 32; i++)           // Copy data from system_bus into line
            line->data[i%32] = system_bus[i%32];
        line->valid = 1;                           // Update line metadata
        line->tag = tag;
        miss_count++;                              // Increase miss count
    }

    return line->data[address % 32];               // Return data stored in the line from address
}


int main() {
    // INITIALIZE CACHE
    for (int i = 0; i<8; i++) cache.set[i].line[0].valid = 0;

    // READ SOME DATA
    char c;
    for (int i = 0; i < 3; i++) {
    for (int j = 0; j < 1000; j++) {
        c = Read_Data_From_Cache(j);
        printf("data = %c : hit count = %-3u : miss count = %-3u : read data count = %-3u\n", c, hit_count, miss_count, read_data_count);
    }
    }

    return 0;
}
