#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "Players_data.h"
#include "data.h"


MyPlayer playerList[maxPlayers];
Team teamList[maxTeams];
int teamPlayerCount[maxTeams] = {0};


int main() {
    int choice;
    initializeData();
    do {
        printf("==============================================================================\n");
        printf("ICC ODI Player Performance Analyzer\n");
        printf("==============================================================================\n");
        printf("1. Display Players of a Specific Team\n");
        printf("2. Display Teams by Average Batting Strike Rate\n");
        printf("3. Display Top K Players of a Specific Team by Role\n");
        printf("4. Display all Players of specific role Across All Teams by performance index\n");
        printf("5. Exit\n");
        printf("==============================================================================\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);
        switch (choice) {
            case 1:
                {
                    int teamId;
                    printf("Enter Team ID: ");
                    scanf("%d", &teamId);
                    displayPlayersOfTeam(teamId);
                }
                break;
            case 2:
                displayTeamsByAverageStrikeRate();
                break;
            case 3:
                {
                    int teamId, roleVal, k;
                    printf("Enter Team ID: ");
                    scanf("%d", &teamId);
                    printf("Enter Role (1-Batsman, 2-Bowler, 3-All-rounder): ");
                    scanf("%d", &roleVal);
                    if(roleVal>3)
                    {
                        printf("invalid Role\n");
                    }
                    printf("Enter number of players (K): ");
                    scanf("%d", &k);
                    displayTopKPlayers(teamId, roleVal, k);
                }
                break;
            case 4:
                {
                    int roleVal;
                    printf("Enter Role (1-Batsman, 2-Bowler, 3-All-rounder): ");
                    scanf("%d", &roleVal);
                    if(roleVal>3)
                    {
                        printf("invalid Role\n");
                    }
                    displayAllPlayersOfRole(roleVal);
                }
                break;
            case 5:
                printf("Exiting...\n");
                break;
            default:
                printf("Invalid Choice!\n");
                break;
        }
    } while (choice != 5);
    return 0;
}

void printPlayerInfo(const MyPlayer *player) {
    printf("%-5d %-18s %-11s %-5d %-5.1f %-5.1f %-5d %-5.1f %-10.2f\n",
           player->playerId,
           player->playerName,
           player->playerRole == 1 ? "Batsman" : player->playerRole == 2 ? "Bowler" : "All-rounder",
           player->totalRuns,
           player->battingAverage,
           player->strikeRate,
           player->wickets,
           player->economyRate,
           player->performanceIndex);
}

int getRoleValue(const char *roleStr) {
    if (strcmp(roleStr, "Batsman") == 0)
    {
        return 1;
    }
    if (strcmp(roleStr, "Bowler") == 0)
    {
        return 2;
    }
    return 3;
}

float calculatePerformanceIndex(const MyPlayer *player) {
    if (player->playerRole == 1)
    {
        return (player->battingAverage * player->strikeRate) / 100.0f;
    }
    else if (player->playerRole == 2)
    {
        return (player->wickets * 2) + (100.0f - player->economyRate);
    }
    else
    {
        return ((player->battingAverage * player->strikeRate) / 100.0f) + (player->wickets * 2);
    }
}

bool isBatsmanOrAllRounder(int role) {
    return (role == 1) || (role == 3);
}

void initializeData() {
    memset(teamPlayerCount, 0, sizeof(teamPlayerCount));
    for (int i = 0; i < playerCount; ++i) {
        MyPlayer *player = &playerList[i];
        player->playerId = players[i].id;
        strncpy(player->playerName, players[i].name, maxNameLen);
        strncpy(player->teamName, players[i].team, maxNameLen);
        player->playerRole = getRoleValue(players[i].role);
        player->totalRuns = players[i].totalRuns;
        player->battingAverage = players[i].battingAverage;
        player->strikeRate = players[i].strikeRate;
        player->wickets = players[i].wickets;
        player->economyRate = players[i].economyRate;
        player->performanceIndex = calculatePerformanceIndex(player);


        for (int j = 0; j < teamCount; ++j) {
            if (strcmp(player->teamName, teams[j]) == 0) {
                teamPlayerCount[j]++;
                break;
            }
        }
    }
    for (int i = 0; i < teamCount; ++i) {
        teamList[i].teamId = i + 1;
        strncpy(teamList[i].teamName, teams[i], maxNameLen);
        teamList[i].totalPlayers = teamPlayerCount[i];
        float sumStrikeRate = 0.0f;
        int count = 0;
        for (int j = 0; j < playerCount; ++j) {
            if (strcmp(playerList[j].teamName, teamList[i].teamName) == 0) {
                if (isBatsmanOrAllRounder(playerList[j].playerRole)) {
                    sumStrikeRate += playerList[j].strikeRate;
                    count++;
                }
            }
        }
        teamList[i].averageBattingStrikeRate = count ? (sumStrikeRate / count) : 0;
    }
}

int binarySearchTeam(const char *teamName) {
    int left = 0;
    int right = teamCount - 1;
    while (left <= right) {
        int mid = (left + right) / 2;
        int cmp = strcmp(teamList[mid].teamName, teamName);
        if (cmp == 0) 
        {
            return mid;
        }
        if (cmp < 0)
        {
         left = mid + 1;
        }
        else 
        {
            right = mid - 1;
        }
    }
    return -1;
}

void displayPlayersOfTeam(int teamId) {
    if (teamId < 1 || teamId > teamCount) {
        printf("Invalid Team ID.\n");
        return;
    }
    printf("Players of Team %s:\n", teamList[teamId - 1].teamName);
    printLayout();
    int totalPlayers = 0;
    float sumStrikeRate = 0.0f;
    for (int i = 0; i < playerCount; ++i) {
        if (strcmp(playerList[i].teamName, teamList[teamId - 1].teamName) == 0) {
            MyPlayer *player = &playerList[i];
            printPlayerInfo(player);


            if (isBatsmanOrAllRounder(player->playerRole)) {
                sumStrikeRate += player->strikeRate;
            }
            totalPlayers++;
        }
    }
    printf("====================================================================================\n");
    printf("Total Players: %d\n", totalPlayers);
    printf("Average Batting Strike Rate: %.2f\n", totalPlayers ? sumStrikeRate / totalPlayers : 0);
}

int compareTeamByAverageStrikeRate(const void *a, const void *b) {
    Team *teamA = (Team *)a;
    Team *teamB = (Team *)b;


    if (teamB->averageBattingStrikeRate > teamA->averageBattingStrikeRate)
        return 1;


    if (teamB->averageBattingStrikeRate < teamA->averageBattingStrikeRate)
        return -1;


    return 0;
}

void displayTeamsByAverageStrikeRate() {
    Team sortedTeams[maxTeams];
    memcpy(sortedTeams, teamList, sizeof(teamList));
    mergeSort(sortedTeams, teamCount, sizeof(Team), compareTeamByAverageStrikeRate);
    printf("Teams Sorted by Average Batting Strike Rate\n");
    printf("=========================================================\n");
    printf("ID   Team Name         Avg Bat SR   Total Players\n");
    printf("=========================================================\n");
    for (int i = 0; i < teamCount; i++) {
        printf("%-4d %-18s %-13.2f %-5d\n",
               sortedTeams[i].teamId,
               sortedTeams[i].teamName,
               sortedTeams[i].averageBattingStrikeRate,
               sortedTeams[i].totalPlayers);
    }
}

int comparePlayerPerformanceDesc(const void *a, const void *b) {
    MyPlayer *playerA = *(MyPlayer **)a;
    MyPlayer *playerB = *(MyPlayer **)b;
    if (playerB->performanceIndex > playerA->performanceIndex)
        return 1;


    if (playerB->performanceIndex < playerA->performanceIndex)
        return -1;


    return 0;
}

 void printLayout()
 {
    printf("====================================================================================\n");
    printf("ID    Name                Role         Runs  Avg   SR    Wkts  ER    Perf.Index\n");
    printf("====================================================================================\n");
 }

void displayTopKPlayers(int teamId, int roleVal, int k) {
    if (teamId < 1 || teamId > teamCount || k < 1) {
        printf("Invalid input.\n");
        return;
    }
    MyPlayer *rolePlayers[maxPlayers];
    int playerCountForRole = 0;
    for (int i = 0; i < playerCount; ++i) {
        if (strcmp(playerList[i].teamName, teamList[teamId - 1].teamName) == 0 &&
            playerList[i].playerRole == roleVal) {
            rolePlayers[playerCountForRole++] = &playerList[i];
        }
    }
    mergeSort(rolePlayers, playerCountForRole, sizeof(MyPlayer *), comparePlayerPerformanceDesc);
    printf("Top %d %s of Team %s:\n",
           k,
           (roleVal == 1) ? "Batsmen" : (roleVal == 2) ? "Bowlers" : "All-rounders",
           teamList[teamId - 1].teamName);

     printLayout();
    for (int i = 0; i < k && i < playerCountForRole; ++i) {
        MyPlayer *player = rolePlayers[i];
        printPlayerInfo(player);
    }
}

void displayAllPlayersOfRole(int roleVal) {
    MyPlayer *rolePlayers[maxPlayers];
    int playerCountForRole = 0;
    for (int i = 0; i < playerCount; ++i) {
        if (playerList[i].playerRole == roleVal) {
            rolePlayers[playerCountForRole++] = &playerList[i];
        }
    }
    mergeSort(rolePlayers, playerCountForRole, sizeof(MyPlayer *), comparePlayerPerformanceDesc);
    printf("Players of all teams with role %s by Performance Index (Desc)\n",
           (roleVal == 1) ? "Batsman" : (roleVal == 2) ? "Bowler" : "All-rounder");
   printLayout();
    for (int i = 0; i < playerCountForRole; i++) {
        MyPlayer *player = rolePlayers[i];
        printPlayerInfo(player);
    }
}

void merge(void *base, int leftIndex, int middleIndex, int rightIndex, int size, int (*compare)(const void*, const void*)) {
    int leftSubarraySize = middleIndex - leftIndex + 1;
    int rightSubarraySize = rightIndex - middleIndex;
    char *leftArray = (char *)malloc(leftSubarraySize * size);
    char *rightArray = (char *)malloc(rightSubarraySize * size);
    if (!leftArray || !rightArray) {
        fprintf(stderr, "Memory allocation failed in merge\n");
        exit(EXIT_FAILURE);
    }
    memcpy(leftArray, (char *)base + leftIndex * size, leftSubarraySize * size);
    memcpy(rightArray, (char *)base + (middleIndex + 1) * size, rightSubarraySize * size);
    int i = 0, j = 0, k = leftIndex;
    while (i < leftSubarraySize && j < rightSubarraySize) {
        if (compare(leftArray + i * size, rightArray + j * size) <= 0) {
            memcpy((char *)base + k * size, leftArray + i * size, size);
            i++;
        } else {
            memcpy((char *)base + k * size, rightArray + j * size, size);
            j++;
        }
        k++;
    }
    while (i < leftSubarraySize) {
        memcpy((char *)base + k * size, leftArray + i * size, size);
        i++;
        k++;
    }
    while (j < rightSubarraySize) {
        memcpy((char *)base + k * size, rightArray + j * size, size);
        j++;
        k++;
    }
    free(leftArray);
    free(rightArray);
}


void mergeSort(void *base, int numElements, int size, int (*compare)(const void*, const void*)) {
    if (numElements < 2) {
        return;
    }
    int middleIndex = numElements / 2;
    mergeSort(base, middleIndex, size, compare);
    mergeSort((char *)base + middleIndex * size, numElements - middleIndex, size, compare);
    merge(base, 0, middleIndex - 1, numElements - 1, size, compare);
}
