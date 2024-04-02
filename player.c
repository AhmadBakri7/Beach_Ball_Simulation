#include "headers.h"

pid_t fetch_next_pid (int, int*);
unsigned int get_sleep_duration();
void initialize(int);


int energy = 100;
pid_t next_pid;
pid_t other_team_lead;
int player_num;


int main(int argc, char *argv[]) {

    void catch_ball_player(int);
    void catch_ball_teamlead (int sig);

    if (argc < 4) {
        perror("Not enough arguments\n");
        exit(-1);
    }

    int pipe[2] = {atoi(argv[1]), atoi(argv[2])};
    player_num = atoi(argv[3]);

    initialize(pipe[0]);

    while (1) {
        // printf("energy: %d\t", energy);
        // fflush(NULL);
        // decreament energy.
        pause();
    }

    // close(pipe[0]);
    // close(pipe[1]); // Close write end

    return 0;
}


void initialize(int r_pipe) {
    if (player_num == 6 || player_num == 0) {
        next_pid = fetch_next_pid(r_pipe, &other_team_lead);

        if ( sigset(SIGUSR2, catch_ball_teamlead) == SIG_ERR ) {
            perror("Sigset can not set SIGQUIT");
            exit(SIGQUIT);
        }
    }
    else
        next_pid = fetch_next_pid(r_pipe, NULL);

    if ( sigset(SIGUSR1, catch_ball_player) == SIG_ERR ) {
        perror("Sigset can not set SIGQUIT");
        exit(SIGQUIT);
    }

    // initialize energy
    srand(getpid());
    energy = (rand() % 51) + 50; // 50 - 100
}


pid_t fetch_next_pid(int r_pipe, int* other_team_lead) {
    char message[512];

    read(r_pipe, message, sizeof(message));

    if (other_team_lead) {
        char* temp = strtok(message, ",");
        int next_pid = atoi(temp);
        
        temp = strtok('\0', ",");
        *other_team_lead = atoi(temp);

        return next_pid;
    }

    return atoi(message);
}


void catch_ball_player(int sig) {
    // unsigned int duration = get_sleep_duration();
    sleep(1);

    // critical section
    // block signals
    // update energy level
    // sighold(SIGUSR1);
    // sighold(SIGUSR2);

    // if (energy > 10)
    //     energy--;
    
    // sigrelse(SIGUSR1);
    // sigrelse(SIGUSR2);


    if (player_num == 0 || player_num == 6) {
        kill(other_team_lead, SIGUSR2);
        printf("team lead (%d) passing ball to: %d\n", player_num, other_team_lead);
        fflush(NULL);
    } else {
        // signal next child
        kill(next_pid, SIGUSR1);
        printf("player (%d) passing ball to: %d\n", player_num, next_pid);
        fflush(NULL);
    }
}


void catch_ball_teamlead(int sig) {
    // unsigned int duration = get_sleep_duration();
    sleep(1);

    // sighold(SIGUSR1);
    // sighold(SIGUSR2);

    // if (energy > 10)
    //     energy--;
    
    kill(next_pid, SIGUSR1);
    printf("team lead (%d) passing ball to: %d\n", player_num, next_pid);
    fflush(NULL);
    // sigrelse(SIGUSR1);
    // sigrelse(SIGUSR2);

}


unsigned int get_sleep_duration() {
    srand(time(NULL));

    int random = (rand() % 10) + 1;
    float duration = (float) ((100 + random) / energy);

    printf("child (%d) sleep duration: %f\n", player_num, duration);
    fflush(NULL);

    return (unsigned int) (duration);
}
