/**
 *
 * @file main.c
 *
 * @author Cardiño, Joemar J.
 *
 * @date 2022-10-19
 *
 * Title : CPU Scheduling  Implementation in C
 * Task  : Write a C Program to determine the average waiting time
 *         and average turnaround time given n processes and their
 *         burst times for the following CPU Scheduling algorithms:
 *          1. FCFS
 *          2. SJF (Non-preemptive)
 *          3. Priority (Non-preemptive)
 *          4. Round Robin
 *
 * @note
 *  - This program is made from scratch. I did my best to implement all algorithms for cpu scheduling.
 *    I hope that this works well...and user will enjoy this program... '_^
 */

#include <stdio.h>
#include <stdlib.h> // use malloc, free
#pragma pack(1)     // We can avoid the wastage of memory by simply writing #pragma pack(1) : https://iq.opengenus.org/size-of-struct-in-c/

/**
 * @note my_standard::Global Variables => will be used for iteration(s)
 *
 * ---------------------------------------------------------------------
 * variable | uses
 * ---------------------------------------------------------------------
 *   i      | for row, basically used to compare the size of the Process
 * ---------------------------------------------------------------------
 *   j      | for column, used for other loop inside a loop
 * ---------------------------------------------------------------------
 * current  | current time, when a process arrived at 'some time'
 *          | and execute it's burst time, it will save the values
 *          | and later be used for computations
 */
unsigned int i, j, current;
float att, awt;

typedef struct Process
{
    int id; // process id
    int pr; // priority
    int at; // arrival time
    int bt; // burst time
} process;

typedef struct Queue
{
    process process;
    int ct;             // completion time
    int wt;             // waiting time
    int tt;             // turnaround time
    struct Queue *next; // next in the queue
} queue;

void swap(queue *q1, queue *q2)
{
    /**
     * @note this function swap a queue process
     */
    queue temp = *q1;
    *q1 = *q2;
    *q2 = temp;
}

int compareID(const void *a, const void *b)
{
    return ((queue *)a)->process.id > ((queue *)b)->process.id;
}

int compareAT(const void *a, const void *b)
{
    return ((queue *)a)->process.at > ((queue *)b)->process.at;
}

int compareBT(const void *a, const void *b)
{
    return ((queue *)a)->process.bt > ((queue *)b)->process.bt;
}

int comparePR(const void *a, const void *b)
{
    return ((queue *)a)->process.pr > ((queue *)b)->process.pr;
}

/**
 * @brief test display
 *
 * @param q
 * @param n
 */
void testDisplayQ(queue *q, int n)
{
    printf("\n\n");
    printf("-----------------------------------------------\n");
    printf("| Process | PR  | AT  | BT  | CT  | TT  | WT  |\n");
    printf("-----------------------------------------------\n");
    for (i = 0; i < n; i++)
        printf("| %d       |%3d  |%3d  |%3d  |%3d  |%3d  |%3d  |\n", (q + i)->process.id, (q + i)->process.pr, (q + i)->process.at, (q + i)->process.bt, 0, 0, 0);
    printf("-----------------------------------------------\n");
}

int isSync(queue *q, int n)
{
    int result = 0;
    for (i = 0; i < n - 1; i++)
    {
        if ((q + i)->process.at != (q + i + 1)->process.at)
        {
            result = 1;
            break;
        }
    }
    return result;
}

int isPRSync(queue *q, int n)
{
    int result = 0;
    for (i = 0; i < n - 1; i++)
    {
        if ((q + i)->process.pr != (q + i + 1)->process.pr)
        {
            result = 1;
            break;
        }
    }
    return result;
}

/**
 * @note
 *  - We need to add (bt) per process and get its tunaround time and waiting time
 *  - Get Average tunaround time and Average Waiting time
 */
void FCFS(queue *q, int n)
{
    // temp
    int tempCT = 0;
    att = 0, awt = 0;
    // set values to 0 for incrementation
    (q + i)->tt = 0;
    (q + i)->wt = 0;

    // default by id, if arrival time is not sync (a.k.a the same)
    qsort(q, n, sizeof(queue), compareID);
    if (isSync(q, n) == 1)
        qsort(q, n, sizeof(queue), compareAT);

    printf("\n\n===============================\n\n");
    printf("CPU SCHED := FCFS\n\n");
    printf("-----------------------------------\n");
    printf("| Process | AT  | BT  | TT  | WT  |\n");
    printf("-----------------------------------\n");

    for (i = 0; i < n; i++)
    {
        // if ((q + i)->process.at != 0)
        //     tempCT += (q + i)->process.at;
        tempCT += (q + i)->process.bt;
        (q + i)->ct = tempCT;
        (q + i)->tt = (q + i)->ct - (q + i)->process.at;
        (q + i)->wt = (q + i)->tt - (q + i)->process.bt;
        att += (q + i)->tt;
        awt += (q + i)->wt;

        printf("| %d       |%3d  |%3d  |%3d  |%3d  |\n", (q + i)->process.id, (q + i)->process.at, (q + i)->process.bt, (q + i)->tt, (q + i)->wt);
    }

    att /= n;
    awt /= n;
    printf("-----------------------------------\n");

    printf("ATT = %.1f ms\n", att);
    printf("AWT = %.1f ms\n", awt);
    qsort(q, n, sizeof(queue), compareID);
}

/**
 * @note
 *  - if arrival time 0, sort by burst time
 *  - must base on the arrival time if arrival time != 0, before sorting the their burst time
 */
void SJF(queue *q, int n)
{
    int tempCT = 0;
    int ttime = 0;
    att = 0, awt = 0;
    (q + i)->tt = 0;
    (q + i)->wt = 0;

    qsort(q, n, sizeof(queue), compareAT);
    for (i = 0; i < n; i++)
    {
        j = i;
        while ((q + j)->process.at <= ttime && j != n)
            j++;
        qsort((q + j), n - j, sizeof(queue), compareBT);
        ttime += (q + i + 1)->process.at;
    }

    printf("\n\n===============================\n\n");
    printf("CPU SCHED := SJF\n\n");
    printf("-----------------------------------\n");
    printf("| Process | AT  | BT  | TT  | WT  |\n");
    printf("-----------------------------------\n");

    for (i = 1; i < n - 1; i++)
    {
        if ((q + i)->process.bt > (q + i + 1)->process.at)
            qsort((q + j), n - j, sizeof(queue), compareBT);
    }

    if (isSync(q, n) != 1)
        qsort(q, n, sizeof(queue), compareBT);

    for (i = 0; i < n; i++)
    {
        // if ((q + i)->process.at != 0)
        //     tempCT += (q + i)->process.at;
        tempCT += (q + i)->process.bt;
        (q + i)->ct = tempCT;
        (q + i)->tt = (q + i)->ct - (q + i)->process.at;
        (q + i)->wt = (q + i)->tt - (q + i)->process.bt;
        att += (q + i)->tt;
        awt += (q + i)->wt;

        printf("| %d       |%3d  |%3d  |%3d  |%3d  |\n", (q + i)->process.id, (q + i)->process.at, (q + i)->process.bt, (q + i)->tt, (q + i)->wt);
    }

    att /= n;
    awt /= n;
    printf("-----------------------------------\n");

    printf("ATT = %.1f ms\n", att);
    printf("AWT = %.1f ms\n", awt);
    qsort(q, n, sizeof(queue), compareID);
}

void PRIO(queue *q, int n)
{
    int tempCT = 0;
    int ttime = 0;
    att = 0, awt = 0;
    (q + i)->tt = 0;
    (q + i)->wt = 0;

    if (isSync(q, n) != 1)
    {
        qsort(q, n, sizeof(queue), compareID);
    }
    if (isPRSync(q, n) != 1)
    {
        qsort(q, n, sizeof(queue), compareAT);
    }
    else
    {
        for (i = 0; i < n; i++)
        {
            j = i;
            while ((q + j)->process.at <= ttime && j != n)
                j++;
            qsort((q + j), n - j, sizeof(queue), comparePR);
            ttime += (q + i + 1)->process.at;
        }
    }

    printf("\n\n=====================================\n\n");
    printf("CPU SCHED := PRIORITY\n\n");
    printf("-----------------------------------------\n");
    printf("| Process | PR  | AT  | BT  | TT  | WT  |\n");
    printf("-----------------------------------------\n");

    for (i = 0; i < n; i++)
    {
        // if ((q + i)->process.at != 0)
        //     tempCT += (q + i)->process.at;
        tempCT += (q + i)->process.bt;
        (q + i)->ct = tempCT;
        (q + i)->tt = (q + i)->ct - (q + i)->process.at;
        (q + i)->wt = (q + i)->tt - (q + i)->process.bt;
        att += (q + i)->tt;
        awt += (q + i)->wt;

        printf("| %d       |%3d  |%3d  |%3d  |%3d  |%3d  |\n", (q + i)->process.id, (q + i)->process.pr, (q + i)->process.at, (q + i)->process.bt, (q + i)->tt, (q + i)->wt);
    }

    att /= n;
    awt /= n;
    printf("-----------------------------------------\n");

    printf("ATT = %.1f ms\n", att);
    printf("AWT = %.1f ms\n", awt);
    qsort(q, n, sizeof(queue), compareID);
}

void RR(queue *q, int n)
{
    int ttime = 0, tempCT = 0, quantum = 0;
    att = 0, awt = 0;
    (q + i)->tt = 0;
    (q + i)->wt = 0;

    printf("\n=================================\n\n");

    printf("quantum time : ");
    scanf("%d", &quantum);

    printf("\n=================================\n\n");
    printf("CPU SCHED := RR\n");
    printf("Quantum   := %d\n\n", quantum);
    printf("-----------------------------------\n");
    printf("| Process | AT  | BT  | TT  | WT  |\n");
    printf("-----------------------------------\n");

    int time, remain = n, reachedQT = 0;
    int tempRT[n];

    for (i = 0; i < n; i++)
        tempRT[i] = (q + i)->process.bt;

    for (time = 0, i = 0; remain != 0;)
    {
        if (tempRT[i] <= quantum && tempRT[i] > 0)
        {
            time += tempRT[i];
            tempRT[i] = 0;
            reachedQT = 1;
        }

        else if (tempRT[i] > 0)
        {
            tempRT[i] -= quantum;
            time += quantum;
        }

        if (tempRT[i] == 0 && reachedQT == 1)
        {
            (q + i)->tt = time - (q + i)->process.at;
            (q + i)->wt = time - (q + i)->process.at - (q + i)->process.bt;
            att += (q + i)->tt;
            awt += (q + i)->wt;

            reachedQT = 0;
            remain--;
        }

        if (i == n - 1)
            i = 0;
        else if ((q + i + 1)->process.at <= time)
            i++;
        else
            i = 0;
    }

    if (n > 3)
    {
        if ((q + 1)->process.at > (q + 2)->process.at)
            qsort(q + 1, n - 1, sizeof(queue), compareAT);
    }

    for (i = 0; i < n; i++)
        printf("| %d       |%3d  |%3d  |%3d  |%3d  |\n", (q + i)->process.id, (q + i)->process.at, (q + i)->process.bt, (q + i)->tt, (q + i)->wt);

    att /= n;
    awt /= n;
    printf("-----------------------------------\n");

    printf("ATT = %.1f ms\n", att);
    printf("AWT = %.1f ms\n", awt);
}

void input(queue *q, int n)
{
    for (i = 0; i < n; ++i)
    {
        printf("\nCount : %d\n", (i + 1));

        printf("enter process id  : ");
        scanf("%d", &(q + i)->process.id);

        printf("enter priority     : ");
        scanf("%d", &(q + i)->process.pr);

        printf("enter arrival time : ");
        scanf("%d", &(q + i)->process.at);

        printf("enter burst time  : ");
        scanf("%d", &(q + i)->process.bt);

        q->next = q;
    }
    q->next = NULL;
}

void CPU_SCHED_ALGO(queue *q, int n)
{
    // input the processes inside queue
    input(q, n);

    // use cpu scheduling algorithms
    FCFS(q, n);
    SJF(q, n);
    PRIO(q, n);
    RR(q, n);
}

int main()
{
    int size;
    queue *q;

    printf("Enter the number of processes: ");
    scanf("%d", &size);

    // Memory allocation for process structures
    q = (queue *)malloc(size * sizeof(queue));

    CPU_SCHED_ALGO(q, size);

    free(q);

    system("pause");
    return 0;
}
