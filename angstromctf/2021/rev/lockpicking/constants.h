int outerpos[][2] = {
    {9, 20},
    {8, 20},
    {7, 20},
    {6, 19},
    {5, 19},
    {4, 18},
    {3, 17},
    {2, 16},
    {1, 15},
    {1, 14},
    {0, 13},
    {0, 12},
    {0, 11},
    {0, 10},
    {0, 9},
    {0, 8},
    {0, 7},
    {1, 6},
    {1, 5},
    {2, 4},
    {3, 3},
    {4, 2},
    {5, 1},
    {6, 1},
    {7, 0},
    {8, 0},
    {9, 0},
    {10, 0},
    {11, 0},
    {12, 0},
    {13, 0},
    {14, 1},
    {15, 1},
    {16, 2},
    {17, 3},
    {18, 4},
    {19, 5},
    {19, 6},
    {20, 7},
    {20, 8},
    {20, 9},
    {20, 10},
    {20, 11},
    {20, 12},
    {20, 13},
    {19, 14},
    {19, 15},
    {18, 16},
    {17, 17},
    {16, 18},
    {15, 19},
    {14, 19},
    {13, 20},
    {12, 20},
    {11, 20},
    {10, 20}};

int middlepos[][2] = {
    {9, 18},
    {8, 18},
    {7, 17},
    {6, 17},
    {5, 16},
    {4, 15},
    {3, 14},
    {3, 13},
    {2, 12},
    {2, 11},
    {2, 10},
    {2, 9},
    {2, 8},
    {3, 7},
    {3, 6},
    {4, 5},
    {5, 4},
    {6, 3},
    {7, 3},
    {8, 2},
    {9, 2},
    {10, 2},
    {11, 2},
    {12, 2},
    {13, 3},
    {14, 3},
    {15, 4},
    {16, 5},
    {17, 6},
    {17, 7},
    {18, 8},
    {18, 9},
    {18, 10},
    {18, 11},
    {18, 12},
    {17, 13},
    {17, 14},
    {16, 15},
    {15, 16},
    {14, 17},
    {13, 17},
    {12, 18},
    {11, 18},
    {10, 18}};

int innerpos[][2] = {
    {9, 15},
    {8, 15},
    {7, 14},
    {6, 13},
    {5, 12},
    {5, 11},
    {5, 10},
    {5, 9},
    {5, 8},
    {6, 7},
    {7, 6},
    {8, 5},
    {9, 5},
    {10, 5},
    {11, 5},
    {12, 5},
    {13, 6},
    {14, 7},
    {15, 8},
    {15, 9},
    {15, 10},
    {15, 11},
    {15, 12},
    {14, 13},
    {13, 14},
    {12, 15},
    {11, 15},
    {10, 15}};

const int outerlen = sizeof(outerpos) / sizeof(*outerpos);
const int middlelen = sizeof(middlepos) / sizeof(*middlepos);
const int innerlen = sizeof(innerpos) / sizeof(*innerpos);

#define OUTBUFSIZE 21
#define PINS 5