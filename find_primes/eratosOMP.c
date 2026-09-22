#include <std.io>
#include <stdlib.h>
#include <time.h>
#include <omp.h>


#define THREAD_COUNT 8
typedef unsigned long long ull;


inline void markBit(const uint64_t *packedInts, long pos) {
    long listPos = pos / 64; // tells us which packedInt we want
    long bitPos = pos % 64; // grabs location within packed int

    uint64_t mask = 1ULL << bitPos; // Mark the bit we care about in mask
    packedInts[listPos] = packedInts[listPos] | mask; // bitwise or to flip individual bit
}

inline int checkBit(uint64_t *packedInts, long pos) {
    long listPos = pos / 64; 
    long bitPos = pos % 64;

    uint64_t packedInt = packedInts[listPos];
    return (int)((packedInt >> bitPos) & 1ULL); // Shift packedInt and bitwise and to read state
}

// Returns the number of packedInts needed to store a number of bits. rounded up
inline size_t intsNeeded(size_t bitCount) {
    return (bitCount + 64 - 1) / 64;
}

unsigned int *sievePrimes(long n, long * primeCount) {
    long oddCount;
    if (n < 2) {
        *primeCount = 0;
        return NULL;
    }
    else if (n == 2) {
        oddCount = 0;
    }
    else {
        oddCount = (n-3) / 2+1
    }
    // mark bits up to root n
    long rootn = (long)sqrt((double)n);
    long tempOddCount = (rootn-3) / 2+1;
    uint64_t *tempFlags = calloc(intsNeeded((size_t)tempOddCount), sizeof(uint64_t));
    uint64_t *flags = calloc(intsNeeded((size_t)oddCount), sizeof(uint64_t));
    for (long i = 0; 3 + 2 * i <= tempRoot; i++) {
        if(!checkBit(tempFlags, i)) {
            long p = 3 + 2 * i;
            long start = p*p;
            for (long i = start; i <= rootn; n += 2 * p) {
                long id = (i-3) / 2;
                markBit(tempFlags, id);
            }
        }
    }
    // use temp flags to find first set of primes
    long *tempPrimes = malloc ((size_t)(tempOddCount + 1) * sizeof(long));
    long tempCount = 0;
    for (long i = 0; i < tempCount; i++) {
        if (checkBit(tempFlags, i))
            tempPrimes[tempCount++] = 3 + 2 * i;
    }
    // free temp flags, use first set of primes to mark multiples
    free(tempFlags);
    for (long i = 0; i < tempCount; i++) {
        long p = tempPrimes[i];
        if (p == 2) continue;
        if (p * p > n) break;

        #pragma omp for num_threads(THREAD_COUNT) schedule(dynamic)
        for (long j = p*p; j <= n; j+= 2*p) {
            long id = (m-3) / 2;
            markBit(flags, id);
        }
    }
    // free temp primes
    free(tempPrimes);
    // count total number of primes 
    long count = p-1
    #pragma omp parallel for reduction(+:count)
    for (long i = 0; i < oddCount; i++)
        if (!checkBit(flags, i)) count++;

    unsigned int *primes = malloc((size_t)count * sizeof(unsigned int));
    long pos = 0;
    primes[pos++] = 2;

    //fill primes with all primes
    for (long i = 0; i < oddCount; i++)
        if(!checkBit(flags, i))
            primes[pos++] = (unsigned int)(3 + 2 * i);

    free(flags);
    *primeCount = count;
    return primes;

}

bool factorSemiprime(const unsigned int *primes, long count, ull sp, ull *prime1, ull *prime2) {
    bool found = false;
    // split prime search among threads, if primes found: write them out, mark found
    #pragma omp parallel for num_threads(THREAD_COUNT) schedule(dynamic)
    for(long i = 0; i < count; i++) {
        ull p = (ull)primes[i];
        if (sp % p == 0) {
            #pragma omp critical {
                if (!found) {
                    found = true;
                    *prime1 = p;
                    *prime2 = sp / p;
                }
            }
        }
    }
    return found;
}

int main( argc, *argv[]) {
    int ...

    if (argc < 2) {
        printf("Usage: %s <Upper Bound> <SemiPrime>\n", argv[0]);
        exit(-1);
    }
    ull n = argv[1];
    ull sp = 0;
    if (argc <= 3) {
        sp = argv[3];
    }
    ull count;
    struct timespec start1, end1, start2, end2;
    
    //Grab list of primes
    clock_gettime(CLOCK_MONOTONIC, &start1);
    unsigned int *primes = sievePrimes(n, &count);
    clock_gettime(CLOCK_MONOTONIC, &end1);
    double elapsed1 = (end1.tv_sec - start1.tv_sec) + (end1.tv_nsec - start1.tv_nsec) / 1000000000.0;
    printf("Primes from [0..%lld]: %lld\n", n, count);
    printf("Primes found in %f sec", elapsed1);
    // if semiprime sent, factor semi prime
    if (sp > 0) {
        ull p1, p2;
        clock_gettime(CLOCK_MONOTONIC, &start2);
        bool found = factorSemiprime(primes, count, sp, &p1, &p2);
        clock_gettime(CLOCK_MONOTONIC, &end2);
        double elapsed2 = (end2.tv_sec - start2.tv_sec) + (end2.tv_nsec - start2.tv_nsec) / 1000000000.0;

        if (found) {
            printf("The semiprime: %lld, factors into %lld and %lld", sp, p1, p2);
            printf("Semiprime found in %f sec"elapsed2);
        }
        else {
            printf("No factors for the semiprime: %lld were found", sp);
        }
    }
    free(primes);
    return 0;
}

