#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "Players_data.h"
#include "data.h"

MyPlayer playerList[maxPlayers];
Team teamList[maxTeams] = {0};
int teamPlayerCount[maxTeams] = {0};

int main()
{
    int choice;
    initializeData();
    do
    {
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
        switch(choice)
        {
            case 1:
            {
                int teamId;
                printf("Enter Team ID: ");
                scanf("%d", &teamId);
                displayPlayersOfTeam(teamId);
                break;
            }
            case 2:
            {
                displayTeamsByAverageStrikeRate();
                break;
            }
            case 3:
            {
                int teamId, roleVal, k;
                printf("Enter Team ID: ");
                scanf("%d", &teamId);

                printf("Enter Role (1-Batsman, 2-Bowler, 3-All-rounder): ");
                scanf("%d", &roleVal);

                if(roleVal < 1 || roleVal > 3)
                {
                    printf("Invalid Role\n");
                    break;
                }

                printf("Enter number of players (K): ");
                scanf("%d", &k);

                displayTopKPlayers(teamId, roleVal, k);
                break;
            }
            case 4:
            {
                int roleVal;
                printf("Enter Role (1-Batsman, 2-Bowler, 3-All-rounder): ");
                scanf("%d", &roleVal);

                if(roleVal < 1 || roleVal > 3)
                {
                    printf("Invalid Role\n");
                    break;
                }

                displayAllPlayersOfRole(roleVal);
                break;
            }
            case 5:
            {
                printf("Exiting...\n");
                break;
            }

            default:
            {
                printf("Invalid Choice!\n");
                break;
            }
        }
    }
    while(choice != 5);

    return 0;
}

void printPlayerInfo(const MyPlayer *player)
{
    printf("%-5d %-18s %-11s %-5d %-5.1f %-5.1f %-5d %-5.1f %-10.2f\n",
           player->playerId,
           player->playerName,
           (player->playerRole == ROLE_BATSMAN) ? "Batsman" :
           (player->playerRole == ROLE_BOWLER) ? "Bowler" : "All-rounder",
           player->totalRuns,
           player->battingAverage,
           player->strikeRate,
           player->wickets,
           player->economyRate,
           player->performanceIndex);
}

int getRoleValue(const char *roleStr)
{
    if(strcmp(roleStr, "Batsman") == 0)
    {
        return ROLE_BATSMAN;
    }
    if(strcmp(roleStr, "Bowler") == 0)
    {
        return ROLE_BOWLER;
    }
    return ROLE_ALLROUNDER;
}

float calculatePerformanceIndex(const MyPlayer *player)
{
    if(player->playerRole == ROLE_BATSMAN)
    {
        return (player->battingAverage * player->strikeRate) / 100.0f;
    }
    else if(player->playerRole == ROLE_BOWLER)
    {
        return (player->wickets * 2) + (100.0f - player->economyRate);
    }
    else
    {
        return ((player->battingAverage * player->strikeRate) / 100.0f) + (player->wickets * 2);
    }
}

bool isBatsmanOrAllRounder(int role)
{
    if(role == ROLE_BATSMAN || role == ROLE_ALLROUNDER)
    {
        return true;
    }
    return false;
}

void initializeData()
{
    for(int i = 0; i < maxTeams; i++)
    {
        teamList[i].playersHead = NULL;
    }
    for(int i = 0; i < playerCount; i++)
    {
        MyPlayer *player = &playerList[i];
        player->playerId = players[i].id;
        strncpy(player->playerName, players[i].name, maxNameLen);
        player->playerName[maxNameLen-1] = '\0';
        strncpy(player->teamName, players[i].team, maxNameLen);
        player->teamName[maxNameLen-1] = '\0';
        player->playerRole = (Role)getRoleValue(players[i].role);
        player->totalRuns = players[i].totalRuns;
        player->battingAverage = players[i].battingAverage;
        player->strikeRate = players[i].strikeRate;
        player->wickets = players[i].wickets;
        player->economyRate = players[i].economyRate;
        player->performanceIndex = calculatePerformanceIndex(player);
        player->next = NULL;
        for(int j = 0; j < teamCount; j++)
        {
            if(strcmp(player->teamName, teams[j]) == 0)
            {
                player->next = teamList[j].playersHead;
                teamList[j].playersHead = player;
                teamPlayerCount[j]++;
                break;
            }
        }
    }
    for(int i = 0; i < teamCount; i++)
    {
        teamList[i].teamId = i + 1;
        strncpy(teamList[i].teamName, teams[i], maxNameLen);
        teamList[i].teamName[maxNameLen-1] = '\0';
        teamList[i].totalPlayers = teamPlayerCount[i];
        float sumSR = 0.0f;
        int count = 0;
        MyPlayer *cur = teamList[i].playersHead;
        while(cur != NULL)
        {
            if(isBatsmanOrAllRounder(cur->playerRole))
            {
                sumSR += cur->strikeRate;
                count++;
            }
            cur = cur->next;
        }
        if(count > 0)
        {
            teamList[i].averageBattingStrikeRate = sumSR / count;
        }
    }
}

int binarySearchTeam(const char *teamName)
{
    int left = 0;
    int right = teamCount - 1;
    while(left <= right)
    {
        int mid = (left + right) / 2;
        int cmp = strcmp(teamList[mid].teamName, teamName);

        if(cmp == 0)
        {
            return mid;
        }
        else if(cmp < 0)
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

void displayPlayersOfTeam(int teamId)
{
    if(teamId < 1 || teamId > teamCount)
    {
        printf("Invalid Team ID.\n");
        return;
    }
    Team *team = &teamList[teamId - 1];
    printf("Players of Team %s:\n", team->teamName);
    printLayout();
    int total = 0;
    float srSum = 0;
    MyPlayer *cur = team->playersHead;
    while(cur != NULL)
    {
        printPlayerInfo(cur);
        if(isBatsmanOrAllRounder(cur->playerRole))
        {
            srSum += cur->strikeRate;
        }
        total++;
        cur = cur->next;
    }
    printf("====================================================================================\n");
    printf("Total Players: %d\n", total);
    printf("Average Batting Strike Rate: %.2f\n", (total > 0) ? srSum / total : 0);
}

int compareTeamByAverageStrikeRate(const void *a, const void *b)
{
    Team *x = (Team*)a;
    Team *y = (Team*)b;

    if(y->averageBattingStrikeRate > x->averageBattingStrikeRate)
    {
        return 1;
    }
    else if(y->averageBattingStrikeRate < x->averageBattingStrikeRate)
    {
        return -1;
    }
    return 0;
}

void displayTeamsByAverageStrikeRate()
{
    Team sortedTeams[maxTeams];

    for(int i = 0; i < teamCount; i++)
    {
        sortedTeams[i] = teamList[i];
    }
    mergeSort(sortedTeams, teamCount, sizeof(Team), compareTeamByAverageStrikeRate);
    printf("Teams Sorted by Average Batting Strike Rate\n");
    printf("=========================================================\n");
    printf("ID   Team Name         Avg Bat SR   Total Players\n");
    printf("=========================================================\n");
    for(int i = 0; i < teamCount; i++)
    {
        printf("%-4d %-18s %-13.2f %-5d\n",
               sortedTeams[i].teamId,
               sortedTeams[i].teamName,
               sortedTeams[i].averageBattingStrikeRate,
               sortedTeams[i].totalPlayers);
    }
}

int comparePlayerPerformanceDesc(const void *a, const void *b)
{
    MyPlayer *x = *(MyPlayer**)a;
    MyPlayer *y = *(MyPlayer**)b;
    if(y->performanceIndex > x->performanceIndex)
    {
        return 1;
    }
    else if(y->performanceIndex < x->performanceIndex)
    {
        return -1;
    }
    return 0;
}

void printLayout()
{
    printf("====================================================================================\n");
    printf("ID    Name                Role         Runs  Avg   SR    Wkts  ER    Perf.Index\n");
    printf("====================================================================================\n");
}

void displayTopKPlayers(int teamId, int roleVal, int k)
{
    if(teamId < 1 || teamId > teamCount || k < 1)
    {
        printf("Invalid Input\n");
        return;
    }
    Team *team = &teamList[teamId - 1];
    MyPlayer *rolePlayers[maxPlayers];
    int count = 0;
    MyPlayer *cur = team->playersHead;
    while(cur != NULL)
    {
        if(cur->playerRole == roleVal)
        {
            rolePlayers[count++] = cur;
        }
        cur = cur->next;
    }
    if(count == 0)
    {
        printf("No players of that role in %s\n", team->teamName);
        return;
    }
    mergeSort(rolePlayers, count, sizeof(MyPlayer*), comparePlayerPerformanceDesc);
    printf("Top %d Players of Team %s:\n", k, team->teamName);
    printLayout();
    for(int i = 0; i < k && i < count; i++)
    {
        printPlayerInfo(rolePlayers[i]);
    }
}

void displayAllPlayersOfRole(int roleVal)
{
    MyPlayer *arr[maxPlayers];
    int count = 0;

    for(int t = 0; t < teamCount; t++)
    {
        MyPlayer *cur = teamList[t].playersHead;

        while(cur != NULL)
        {
            if(cur->playerRole == roleVal)
            {
                arr[count++] = cur;
            }
            cur = cur->next;
        }
    }
    if(count == 0)
    {
        printf("No Players Found\n");
        return;
    }
    mergeSort(arr, count, sizeof(MyPlayer*), comparePlayerPerformanceDesc);
    printf("All Players with role %d Sorted\n", roleVal);
    printLayout();
    for(int i = 0; i < count; i++)
    {
        printPlayerInfo(arr[i]);
    }
}

void merge(void *array, int leftIndex, int middleIndex, int rightIndex, int elementSize,int (*compare)(const void*, const void*))
{
    int leftArraySize  = middleIndex - leftIndex + 1;
    int rightArraySize = rightIndex - middleIndex;
    char *leftTempArray  = malloc(leftArraySize * elementSize);
    char *rightTempArray = malloc(rightArraySize * elementSize);
    for(int i = 0; i < leftArraySize; i++)
    {
        memcpy(leftTempArray + i*elementSize,(char*)array + (leftIndex+i)*elementSize,elementSize);
    }
    for(int j = 0; j < rightArraySize; j++)
    {
        memcpy(rightTempArray + j*elementSize,(char*)array + (middleIndex+1+j)*elementSize,elementSize);
    }
    int leftPos = 0;
    int rightPos = 0;
    int mergePos = leftIndex;
    while(leftPos < leftArraySize && rightPos < rightArraySize)
    {
        if(compare(leftTempArray + leftPos*elementSize,rightTempArray + rightPos*elementSize) <= 0)
        {
            memcpy((char*)array + mergePos*elementSize,leftTempArray + leftPos*elementSize,elementSize);
            leftPos++;
        }
        else
        {
            memcpy((char*)array + mergePos*elementSize,rightTempArray + rightPos*elementSize, elementSize);
            rightPos++;
        }
        mergePos++;
    }
    while(leftPos < leftArraySize)
    {
        memcpy((char*)array + mergePos*elementSize,leftTempArray + leftPos*elementSize,elementSize);
        leftPos++;
        mergePos++;
    }
    while(rightPos < rightArraySize)
    {
        memcpy((char*)array + mergePos*elementSize,rightTempArray + rightPos*elementSize,elementSize);
        rightPos++;
        mergePos++;
    }
    free(leftTempArray);
    free(rightTempArray);
}

void mergeSort(void *array,int totalElements,int elementSize,int (*compare)(const void*,const void*))
{
    if(totalElements<2) return;

    int mid=totalElements/2;

    mergeSort(array,mid,elementSize,compare);
    mergeSort((char*)array+mid*elementSize,totalElements-mid,elementSize,compare);
    merge(array,0,mid-1,totalElements-1,elementSize,compare);
}

