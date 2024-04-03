#ifndef __CONSTANTS__
#define __CONSTANTS__

#define NUM_CHILDREN 12
#define LEAD_A 0
#define LEAD_B 6
#define LAST_A 5
#define LAST_B 11

const int MSG_SIZE = 512;

struct lead_vars {
    int energy;
    int fd_pipe[2];
    int player_num;
    int num_balls_team;
    pid_t next_pid;
    pid_t other_team_lead;
};

struct player_vars {
    int energy;
    int fd_pipe[2];
    int player_num;
    pid_t next_pid;
};

// int RANDOM_NUMBER;
// float THE_PRICE;
// char THE_LEADER_NAME[25];

#endif