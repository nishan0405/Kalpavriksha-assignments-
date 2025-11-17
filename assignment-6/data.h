#ifndef DATA_H
#define DATA_H

#define maxTeams 10
#define maxPlayers 200
#define maxNameLen 50


typedef struct {
    int playerId;
    char playerName[maxNameLen];
    char teamName[maxNameLen];
    int playerRole;
    int totalRuns;
    float battingAverage;
    float strikeRate;
    int wickets;
    float economyRate;
    float performanceIndex;
} MyPlayer;


typedef struct {
    int teamId;
    char teamName[maxNameLen];
    int totalPlayers;
    float averageBattingStrikeRate;
} Team;


extern MyPlayer playerList[maxPlayers];
extern Team teamList[maxTeams];
extern int teamPlayerCount[maxTeams];

int getRoleValue(const char *roleStr);
float calculatePerformanceIndex(const MyPlayer *player);
bool isBatsmanOrAllRounder(int role);
void initializeData();
int binarySearchTeam(const char *teamName);
void displayPlayersOfTeam(int teamId);
int compareTeamByAverageStrikeRate(const void *a, const void *b);
void displayTeamsByAverageStrikeRate();
int comparePlayerPerformanceDesc(const void *a, const void *b);
void displayTopKPlayers(int teamId, int roleVal, int K);
void displayAllPlayersOfRole(int roleVal);
void mergeSort(void *base, int num, int size, int (*compar)(const void*, const void*));
static void merge(void *base, int left, int mid, int right, int size, int (*compar)(const void*, const void*));

#endif