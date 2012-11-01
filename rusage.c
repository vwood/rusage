#include <sys/resource.h>
#include <sys/wait.h>
#include <stdio.h>
#include <stddef.h>
#include <unistd.h>

int main(int argc, char **argv) {
    struct rusage r;
    int status = 0;
    pid_t pid;

    if (argc <= 1) {
        fprintf(stderr, "Rusage requires a command as an argument.\n");
        fprintf(stderr, "(Further arguments are given to that command.)\n");
        return 1;
    }

    pid = fork();
    if (pid == 0) {
        execv(argv[1], argv + 1);
        return 1;
    } else if (pid < 0) {
        return 1;
    } else {
        if (waitpid(pid, &status, 0) != pid) {
            return 1;
        } else {
            getrusage(RUSAGE_CHILDREN, &r);
            printf("Process used:\n");
            printf("\t%ld.%06ld secs usr time\n", r.ru_utime.tv_sec, r.ru_utime.tv_usec);
            printf("\t%ld.%06ld secs sys time\n", r.ru_stime.tv_sec, r.ru_stime.tv_usec);
            printf("\t%ld kb max rss\n", r.ru_maxrss);
            printf("\t%ld kb shared text\n", r.ru_ixrss);
            printf("\t%ld kb unshared data\n", r.ru_idrss);
            printf("\t%ld kb unshared stack\n", r.ru_isrss);
            printf("\t%ld minor page faults (no i/o)\n", r.ru_minflt);
            printf("\t%ld major page faults (with i/o)\n", r.ru_majflt);
            printf("\t%ld times swapped out\n", r.ru_nswap);
            printf("\t%ld disk reads\n", r.ru_inblock);
            printf("\t%ld disk writes\n", r.ru_oublock);
        }
    }

    return status;
}
