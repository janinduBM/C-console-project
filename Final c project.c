#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

struct team {
    int  id;
    char name[50];
    int  count;
};

struct player {
    int  id;
    char name[50];
    struct team myteam;
};

struct game {
    int id;
    int map[16][16];
    int bombs[16][16];
};

struct match {
    int id;
    int teamone;
    int teamtwo;
    int done;
};

struct score {
    int teamid;
    int points;
    int wins;
};

struct team  teams[10];
struct player players[50];
struct game   games[5];
struct match  matches[10];
struct score  scores[10];

int numteams   = 0;
int numplayers = 0;
int nummatches = 0;
int numscores  = 0;

void addteam();
void showteams();
void findteam();
void fixteam();
void delteam();

void addplayer();
void showplayers();
void findplayer();
void fixplayer();
void delplayer();

void makegrid(int map[][16]);
void drawgrid(int map[][16]);
void putbombs(int map[][16], int bombs[][16]);
int  checkstep(int row, int col, int bombs[][16]);
int  playturn(int tid);
void playgame();

void makematch();
void showmatches();
void finishmatch();
void findmatch();
void dropmatch();

void startscore(int tid);
void addscore(int tid, int pts, int win);
void sortboard();
void showboard();
void clearscores();

int main() {
    srand(time(NULL));
    int pick;

    while (1) {
        printf("\n=========================================\n");
        printf(" E-SPORTS BOMB ARENA TOURNAMENT \n");
        printf("=========================================\n");
        printf("1. Team Management\n");
        printf("2. Player Management\n");
        printf("3. Match Scheduling\n");
        printf("4. Play Scheduled Bomb Arena Match\n");
        printf("5. Manual Match Override\n");
        printf("6. View All Players & Teams\n");
        printf("7. View Live Leaderboard\n");
        printf("0. Exit\n");
        printf("Enter your choice (Numbers only): ");

        if (scanf("%d", &pick) != 1) {
            printf("Invalid input! Please enter a number.\n");
            while (getchar() != '\n');
            continue;
        }

        switch (pick) {
            case 1: addteam();     break;
            case 2: addplayer();   break;
            case 3: makematch();   break;
            case 4: playgame();    break;
            case 5: finishmatch(); break;
            case 6: showplayers(); break;
            case 7: showboard();   break;
            case 0:
                printf("Exiting Tournament System...\n");
                exit(0);
            default:
                printf("Invalid choice! Try again.\n");
        }
    }
    return 0;
}

// Member 1: Team Management

void addteam() {
    if (numteams < 10) {
        char text[50];
        int  tid;
        int  sameid;
        int  samename;

        while (1) {
            sameid = 0;
            printf("Enter Team ID (Numbers only): ");

            if (scanf("%d", &tid) != 1) {
                printf("Error: Invalid input! Please enter numbers only.\n");
                while (getchar() != '\n');
                continue;
            }

            for (int i = 0; i < numteams; i++) {
                if (teams[i].id == tid) {
                    sameid = 1;
                    break;
                }
            }

            if (sameid == 1) {
                printf("Error: Team ID %d is already taken! Please try a different ID.\n", tid);
            } else {
                break;
            }
        }

        while (1) {
            samename = 0;
            printf("Enter Team Name (No spaces allowed): ");

            // Read entire line to check for spaces
            scanf(" %[^\n]", text);

            // Check if the input contains a space
            if (strchr(text, ' ') != NULL) {
                printf("Error: Team name cannot contain spaces! Please try again.\n");
                continue;
            }

            for (int i = 0; i < numteams; i++) {
                if (strcmp(teams[i].name, text) == 0) {
                    samename = 1;
                    break;
                }
            }

            if (samename == 1) {
                printf("Error: A team with the name '%s' already exists! Please try a different name.\n", text);
            } else {
                break;
            }
        }

        teams[numteams].id = tid;
        strcpy(teams[numteams].name, text);
        teams[numteams].count = 0;

        startscore(teams[numteams].id);
        numteams++;

        printf("Team added successfully!\n");
    } else {
        printf("Maximum team limit reached (10).\n");
    }
}

void showteams() {
    printf("\n--- Registered Teams ---\n");
    for (int i = 0; i < numteams; i++) {
        printf("ID: %d | Name: %s | Players: %d\n", teams[i].id, teams[i].name, teams[i].count);
    }
}

void findteam() {}
void fixteam()  {}
void delteam()  {}

// Member 2:Player Management

void addplayer() {
    if (numplayers >= 50) {
        printf("Maximum player limit reached (50).\n");
        return;
    }

    if (numteams == 0) {
        printf("Error: No teams exist! Please register a team in Team Management first.\n");
        return;
    }

    players[numplayers].id = numplayers + 1;
    char temp_name[50];
    int is_duplicate;

    // Duplicate and Space Check
    while (1) {
        is_duplicate = 0;
        printf("Enter Player Name (No spaces allowed): ");

        // Read entire line to catch spaces
        scanf(" %[^\n]", temp_name);

        // Check if there is a space in the input
        if (strchr(temp_name, ' ') != NULL) {
            printf("Error: Player name cannot contain spaces! Please try again.\n");
            continue;
        }

        for (int i = 0; i < numplayers; i++) {
            if (strcmp(players[i].name, temp_name) == 0) {
                is_duplicate = 1;
                break;
            }
        }

        if (is_duplicate == 1) {
            printf("Error: The name '%s' is already taken! Please choose a different name.\n", temp_name);
        } else {
            strcpy(players[numplayers].name, temp_name);
            break;
        }
    }

    printf("\n--- Available Teams to Join ---\n");
    for (int i = 0; i < numteams; i++) {
        printf("Team ID: %d | Name: %s | Current Players: %d\n", teams[i].id, teams[i].name, teams[i].count);
    }

    int pickid;
    int found = -1;

    while (1) {
        printf("\nEnter the Team ID this player will join (Numbers only): ");

        if (scanf("%d", &pickid) != 1) {
            printf("Error: Invalid input! Please enter numbers only.\n");
            while (getchar() != '\n');
            continue;
        }

        for (int i = 0; i < numteams; i++) {
            if (teams[i].id == pickid) {
                found = i;
                break;
            }
        }

        if (found == -1) {
            printf("Error: Team ID %d not found! Please enter a valid Team ID.\n", pickid);
        } else {
            break;
        }
    }

    teams[found].count++;
    players[numplayers].myteam = teams[found];
    numplayers++;

    printf("Player added to Team '%s' successfully!\n", teams[found].name);
}

void showplayers() {
    if (numplayers == 0) {
        printf("No players registered yet.\n");
        return;
    }

    printf("\n--- Registered Players (Total: %d) ---\n", numplayers);
    for (int i = 0; i < numplayers; i++) {
        printf("Player ID: %d | Name: %s | Team Name: %s (Team ID: %d)\n",
            players[i].id,
            players[i].name,
            players[i].myteam.name,
            players[i].myteam.id);
    }
}

void findplayer() {}
void fixplayer()  {}
void delplayer()  {}

// Member 3: Bomb Arena Gameplay

void makegrid(int map[][16]) {
    for (int i = 0; i < 16; i++) {
        map[0][i] = i;
    }
    for (int i = 0; i < 16; i++) {
        map[i][0] = i;
    }
    for (int i = 1; i < 16; i++) {
        for (int j = 1; j < 16; j++) {
            map[i][j] = 0;
        }
    }
}

void drawgrid(int map[][16]) {
    for (int i = 0; i < 16; i++) {
        for (int j = 0; j < 16; j++) {
            if (i == 0 && j == 0) {
                printf(" %02d", map[i][j]);
            } else if (i == 0) {
                printf("  %02d", map[i][j]);
            } else if (j == 0) {
                printf(" %02d", map[i][j]);
            } else {
                printf(" |%2d", map[i][j]);
            }
        }
        printf("\n");
    }
}

void putbombs(int map[][16], int bombs[][16]) {
    for (int i = 0; i < 16; i++) {
        for (int j = 0; j < 16; j++) {
            bombs[i][j] = map[i][j];
        }
    }

    int row;
    int col;

    for (int i = 0; i < 15; i++) {
        row = (rand() % 15) + 1;
        col = (rand() % 15) + 1;

        if (bombs[row][col] == 0) {
            bombs[row][col] = 1;
        } else {
            i--;
        }
    }
}

int checkstep(int row, int col, int bombs[][16]) {
    if (bombs[row][col] == 1) {
        printf("\nBOOM! You hit a bomb! GAME OVER!\n");
        return 1;
    } else {
        printf("\nSafe zone! Keep going.\n");
        return 0;
    }
}

int playturn(int tid) {
    struct game play;
    makegrid(play.map);
    putbombs(play.map, play.bombs);

    int row;
    int col;
    int dead = 0;
    int pts  = 0;

    printf("\n>>> TEAM %d IS NOW PLAYING <<<\n", tid);
    printf("--- BOMB ARENA GRID ---\n");
    drawgrid(play.map);

    while (dead == 0) {
        printf("\nTeam %d, enter row and col (Numbers only, 1-15, or 0 0 to retreat): ", tid);

        if (scanf("%d %d", &row, &col) != 2) {
            printf("Invalid input! Please enter two numbers separated by a space.\n");
            while (getchar() != '\n');
            continue;
        }

        if (row == 0 && col == 0) {
            printf("\nTeam %d chose to retreat safely! Smart move.\n", tid);

            for (int i = 1; i < 16; i++) {
                for (int j = 1; j < 16; j++) {
                    if (play.bombs[i][j] == 1) {
                        play.map[i][j] = 99;
                    }
                }
            }
            printf("\n--- FINAL MAP: ALL BOMBS REVEALED (99 = BOMB) ---\n");
            drawgrid(play.map);
            break;
        }

        if (row < 1 || row > 15 || col < 1 || col > 15) {
            printf("Invalid input! Please enter numbers between 1 and 15.\n");
            continue;
        }

        if (play.bombs[row][col] == 2) {
            printf("You already cleared spot [%d][%d]! Pick a new spot.\n", row, col);
            continue;
        }

        dead = checkstep(row, col, play.bombs);

        if (dead == 1) {
            for (int i = 1; i < 16; i++) {
                for (int j = 1; j < 16; j++) {
                    if (play.bombs[i][j] == 1) {
                        play.map[i][j] = 99;
                    }
                }
            }
            printf("\n--- FINAL MAP: ALL BOMBS REVEALED (99 = BOMB) ---\n");
            drawgrid(play.map);
            break;
        }

        if (dead == 0) {
            play.bombs[row][col] = 2;
            play.map[row][col]   = 88;
            pts += 10;

            printf("Team %d Current Points: %d\n", tid, pts);
            printf("\n--- ARENA MAP (88 = Cleared) ---\n");
            drawgrid(play.map);
        }
    }

    printf("\nTeam %d Turn Finished! Total Points Earned: %d\n", tid, pts);
    return pts;
}

void playgame() {
    if (nummatches == 0) {
        printf("Error: No matches scheduled yet! Go to Match Scheduling first.\n");
        return;
    }

    printf("\n--- PENDING SCHEDULED MATCHES ---\n");
    int ispending = 0;

    for (int i = 0; i < nummatches; i++) {
        if (matches[i].done == 0) {
            printf("Match ID: %d | Team %d VS Team %d\n", matches[i].id, matches[i].teamone, matches[i].teamtwo);
            ispending = 1;
        }
    }

    if (ispending == 0) {
        printf("All scheduled matches have already been played!\n");
        return;
    }

    int mid;
    int mindex = -1;

    while (1) {
        printf("\nEnter Match ID you want to play (Numbers only): ");
        if (scanf("%d", &mid) != 1) {
            printf("Error: Invalid input! Please enter numbers only.\n");
            while (getchar() != '\n');
            continue;
        }
        break;
    }

    for (int i = 0; i < nummatches; i++) {
        if (matches[i].id == mid) {
            mindex = i;
            break;
        }
    }

    if (mindex == -1) {
        printf("Error: Match ID not found.\n");
        return;
    }

    if (matches[mindex].done == 1) {
        printf("Error: This match is already finished!\n");
        return;
    }

    int idxone = -1;
    int idxtwo = -1;

    for (int i = 0; i < numteams; i++) {
        if (teams[i].id == matches[mindex].teamone) {
            idxone = i;
        }
        if (teams[i].id == matches[mindex].teamtwo) {
            idxtwo = i;
        }
    }

    if (teams[idxone].count == 0 || teams[idxtwo].count == 0) {
        printf("\nError: Cannot start match! One or both teams have no registered players.\n");
        printf("Team %d Players: %d | Team %d Players: %d\n",
            teams[idxone].id, teams[idxone].count,
            teams[idxtwo].id, teams[idxtwo].count);
        printf("Please go to Player Management to add players first.\n");
        return;
    }

    // Pre Match Coin Toss
    printf("\n=========================================\n");
    printf(" PRE-MATCH COIN TOSS \n");
    printf("=========================================\n");
    printf("Flipping the coin...\n");

    int toss = rand() % 2;
    int first_team, second_team;

    if (toss == 0) {
        printf(">>> Team %d won the toss! They will play FIRST. <<<\n", matches[mindex].teamone);
        first_team = matches[mindex].teamone;
        second_team = matches[mindex].teamtwo;
    } else {
        printf(">>> Team %d won the toss! They will play FIRST. <<<\n", matches[mindex].teamtwo);
        first_team = matches[mindex].teamtwo;
        second_team = matches[mindex].teamone;
    }

    printf("\n=========================================\n");
    printf(" MATCH STARTING: TEAM %d VS TEAM %d\n", first_team, second_team);
    printf("=========================================\n");

    int pts_first = playturn(first_team);

    printf("\nPress Enter to start Team %d's turn...", second_team);
    getchar();
    getchar();

    int pts_second = playturn(second_team);

    int ptsone, ptstwo;
    if (first_team == matches[mindex].teamone) {
        ptsone = pts_first;
        ptstwo = pts_second;
    } else {
        ptsone = pts_second;
        ptstwo = pts_first;
    }

    printf("\n=========================================\n");
    printf(" FINAL MATCH RESULTS\n");
    printf("=========================================\n");
    printf("Team %d Score: %d\n", matches[mindex].teamone, ptsone);
    printf("Team %d Score: %d\n", matches[mindex].teamtwo, ptstwo);

    matches[mindex].done = 1;

    if (ptsone > ptstwo) {
        printf("\n WINNER: Team %d!\n", matches[mindex].teamone);
        addscore(matches[mindex].teamone, 3, 1);
        addscore(matches[mindex].teamtwo, 0, 0);
    } else if (ptstwo > ptsone) {
        printf("\n WINNER: Team %d!\n", matches[mindex].teamtwo);
        addscore(matches[mindex].teamtwo, 3, 1);
        addscore(matches[mindex].teamone, 0, 0);
    } else {
        printf("\n IT IS A TIE!\n");
        addscore(matches[mindex].teamone, 1, 0);
        addscore(matches[mindex].teamtwo, 1, 0);
    }

    printf("Leaderboard and Match Status have been updated automatically.\n");
}

// Member 4 :Match Scheduling

void makematch() {
    if (numteams < 2) {
        printf("Error: Not enough teams registered! Please register at least 2 teams first.\n");
        return;
    }

    matches[nummatches].id = nummatches + 1;

    printf("\n--- Available Teams to Schedule ---\n");
    for (int i = 0; i < numteams; i++) {
        printf("Team ID: %d | Name: %s\n", teams[i].id, teams[i].name);
    }

    int okone = 0;
    int oktwo = 0;

    while (1) {
        okone = 0;
        printf("\nEnter Team 1 ID (Numbers only): ");

        if (scanf("%d", &matches[nummatches].teamone) != 1) {
            printf("Error: Invalid input! Please enter numbers only.\n");
            while (getchar() != '\n');
            continue;
        }

        for (int i = 0; i < numteams; i++) {
            if (teams[i].id == matches[nummatches].teamone) {
                okone = 1;
                break;
            }
        }

        if (okone == 0) {
            printf("Error: Team ID %d is not registered! Try again.\n", matches[nummatches].teamone);
        } else {
            break;
        }
    }

    while (1) {
        oktwo = 0;
        printf("Enter Team 2 ID (Numbers only): ");

        if (scanf("%d", &matches[nummatches].teamtwo) != 1) {
            printf("Error: Invalid input! Please enter numbers only.\n");
            while (getchar() != '\n');
            continue;
        }

        if (matches[nummatches].teamone == matches[nummatches].teamtwo) {
            printf("Error: A team cannot play against itself! Pick another team.\n");
            continue;
        }

        for (int i = 0; i < numteams; i++) {
            if (teams[i].id == matches[nummatches].teamtwo) {
                oktwo = 1;
                break;
            }
        }

        if (oktwo == 0) {
            printf("Error: Team ID %d is not registered! Try again.\n", matches[nummatches].teamtwo);
        } else {
            break;
        }
    }

    matches[nummatches].done = 0;
    nummatches++;

    printf("Match Scheduled successfully! Status is now PENDING.\n");
}

void finishmatch() {
    if (nummatches == 0) {
        printf("No matches scheduled yet!\n");
        return;
    }

    showmatches();

    int mid;
    int winner;

    while (1) {
        printf("\nEnter the Match ID you want to manually finish (Numbers only): ");
        if (scanf("%d", &mid) != 1) {
            printf("Error: Invalid input! Please enter numbers only.\n");
            while (getchar() != '\n');
            continue;
        }
        break;
    }

    for (int i = 0; i < nummatches; i++) {
        if (matches[i].id == mid) {
            if (matches[i].done == 1) {
                printf("This match is already finished!\n");
                return;
            }

            printf("\nMatch: Team %d VS Team %d\n", matches[i].teamone, matches[i].teamtwo);

            while (1) {
                printf("Who won? (1 for Team %d | 2 for Team %d | 3 for Draw): ", matches[i].teamone, matches[i].teamtwo);

                if (scanf("%d", &winner) != 1) {
                    printf("Error: Invalid input! Please enter a number (1, 2, or 3).\n");
                    while (getchar() != '\n');
                    continue;
                }
                if (winner >= 1 && winner <= 3) {
                    break;
                } else {
                    printf("Invalid choice. Please enter 1, 2, or 3.\n");
                }
            }

            matches[i].done = 1;

            if (winner == 1) {
                addscore(matches[i].teamone, 3, 1);
                addscore(matches[i].teamtwo, 0, 0);
                printf("Manual override complete. Team %d wins.\n", matches[i].teamone);
            } else if (winner == 2) {
                addscore(matches[i].teamtwo, 3, 1);
                addscore(matches[i].teamone, 0, 0);
                printf("Manual override complete. Team %d wins.\n", matches[i].teamtwo);
            } else {
                addscore(matches[i].teamone, 1, 0);
                addscore(matches[i].teamtwo, 1, 0);
                printf("Manual override complete. Match is a Draw!\n");
            }
            return;
        }
    }
    printf("Match ID not found.\n");
}

void showmatches() {
    printf("\n--- Scheduled Matches ---\n");
    for (int i = 0; i < nummatches; i++) {
        printf("Match ID: %d | Team %d VS Team %d | Status: %s\n",
            matches[i].id,
            matches[i].teamone,
            matches[i].teamtwo,
            (matches[i].done == 0) ? "PENDING" : "FINISHED");
    }
}

void findmatch() {}
void dropmatch() {}
// Member 5: Leaderboard and Scoring

void startscore(int tid) {
    scores[numscores].teamid = tid;
    scores[numscores].points = 0;
    scores[numscores].wins   = 0;
    numscores++;
}

void addscore(int tid, int pts, int win) {
    for (int i = 0; i < numscores; i++) {
        if (scores[i].teamid == tid) {
            scores[i].points += pts;

            if (win == 1) {
                scores[i].wins += 1;
            }
            return;
        }
    }
}

void sortboard() {
// Bubble sort ranks teams from highest points to lowest points
    for (int i = 0; i < numscores - 1; i++) {
        for (int j = 0; j < numscores - i - 1; j++) {
            if (scores[j].points < scores[j+1].points) {
                struct score temp = scores[j];
                scores[j] = scores[j+1];
                scores[j+1] = temp;
            }
        }
    }
}

void showboard() {
// Call the sorting function before printing the board
    sortboard();

    printf("\n TOURNAMENT LEADERBOARD \n");
    printf("------------------------------------------------------------------\n");
    printf("Team ID | Total Points | Matches Won | Pending Games | Finished \n");
    printf("------------------------------------------------------------------\n");

    for (int i = 0; i < numscores; i++) {
        int wait = 0;
        int done = 0;

        for (int j = 0; j < nummatches; j++) {
            if (matches[j].teamone == scores[i].teamid || matches[j].teamtwo == scores[i].teamid) {
                if (matches[j].done == 0) {
                    wait++;
                } else {
                    done++;
                }
            }
        }

        printf("   %2d   |      %3d      |      %2d      |       %2d      |    %2d\n",
            scores[i].teamid,
            scores[i].points,
            scores[i].wins,
            wait,
            done);
    }
    printf("------------------------------------------------------------------\n");
}

void clearscores() {}
