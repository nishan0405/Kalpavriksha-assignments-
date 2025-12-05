#ifndef DATA_H
#define DATA_H
#define maxPlayers 200
#define maxTeams 10
#define maxNameLen 50

typedef enum {
    ROLE_BATSMAN = 1,
    ROLE_BOWLER = 2,
    ROLE_ALLROUNDER = 3
} Role;

typedef struct MyPlayer {
    int playerId;
    char playerName[maxNameLen];
    char teamName[maxNameLen];
    Role playerRole;
    int totalRuns;
    float battingAverage;
    float strikeRate;
    int wickets;
    float economyRate;
    float performanceIndex;
    struct MyPlayer *next;  
} MyPlayer;

typedef struct {
    int teamId;
    char teamName[maxNameLen];
    int totalPlayers;
    float averageBattingStrikeRate;
    MyPlayer *playersHead;  
} Team;

extern MyPlayer playerList[maxPlayers];
extern Team teamList[maxTeams];
extern int teamPlayerCount[maxTeams];

int getRoleValue(const char *roleStr);
float calculatePerformanceIndex(const MyPlayer *player);
bool isBatsmanOrAllRounder(int role);
void initializeData();
void printLayout();
int binarySearchTeam(const char *teamName);
void displayPlayersOfTeam(int teamId);
void displayTeamsByAverageStrikeRate();
void displayTopKPlayers(int teamId, int roleVal, int k);
void displayAllPlayersOfRole(int roleVal);
void printPlayerInfo(const MyPlayer *player);
int compareTeamByAverageStrikeRate(const void *a, const void *b);
int comparePlayerPerformanceDesc(const void *a, const void *b);
void merge(void *array, int leftIndex, int middleIndex, int rightIndex, int elementSize,int (*compare)(const void*, const void*));
void mergeSort(void *array, int totalElements, int elementSize,int (*compare)(const void*, const void*));

#endif
