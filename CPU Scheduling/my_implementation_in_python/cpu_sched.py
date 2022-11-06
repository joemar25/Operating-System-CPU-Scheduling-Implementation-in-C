class Process:

    def __init__(self, id=0, pr=0, at=0, bt=0):
        # need to fill
        self.id = id
        self.pr = pr
        self.at = at
        self.bt = bt
        # to process
        self.ct = 0
        self.tt = 0
        self.wt = 0


class Scheduler:

    def __init__(self, queue):
        self.queue = queue

    def FCFS(self):

        # first come first serve, means we first finish the burst time of the process that arrives first
        # assumes that all arrived at 0
        """
            test arrival time vulnerability

            _1__345_7
        """
        self.queue.sort(key=lambda p: p.at)

        print("\n=========================================")
        print("\nCPU SCHED := FCFS\n")
        print("-----------------------------------------")
        print("| Process | AT  | BT  | CT  | TT  | WT  |")
        print("-----------------------------------------")
        counter = ct = att = awt = 0
        for p in queue:
            if counter == 0 and int(p.at) > 0:
                ct += int(p.at) + int(p.bt)
            else:
                ct += int(p.bt)

            p.ct = ct
            p.tt = int(p.ct) - int(p.at)
            p.wt = int(p.tt) - int(p.bt)
            att += p.tt
            awt += p.wt

            print(
                f"| {p.id : <3}     | {p.at : <3} | {p.bt : <3} | {p.ct:<3} | {p.tt:<3} | {p.wt:<3} |")

        att /= len(self.queue)
        awt /= len(self.queue)

        print("-----------------------------------------")

        print("ATT = %.1f ms" % att)
        print("AWT = %.1f ms" % awt)

    # use for sjf preemtive
    def isATSync(self):
        if len(self.queue) > 1:
            for i in range(len(self.queue)):
                for j in range(len(self.queue)):
                    if self.queue[i].at is not self.queue[j].at:
                        return False
        return True

    def SJF(self):

        if self.isATSync():
            """
                if all of the queue process arrived at 0, sort by bt
            """
            self.queue.sort(key=lambda p: p.bt)
            queue = self.queue
        else:
            # this would be the temp queue
            queue = []
            # sort by arrival time
            self.queue.sort(key=lambda p: p.at)
            # get the first element
            # firstElem = self.queue[0]
            queue.insert(0, self.queue[0])
            # delete first element
            del self.queue[0]
            # sort old list by burst time
            self.queue.sort(key=lambda p: p.bt)
            # list checking
            for i in range(len(self.queue)):
                queue.insert((i+1), self.queue[i])

        print("\n=========================================")
        print("\nCPU SCHED := SJF\n")
        print("-----------------------------------------")
        print("| Process | AT  | BT  | CT  | TT  | WT  |")
        print("-----------------------------------------")

        counter = ct = att = awt = 0
        for p in queue:
            if counter == 0 and int(p.at) > 0:
                ct += int(p.at) + int(p.bt)
            else:
                ct += int(p.bt)

            p.ct = ct
            p.tt = int(p.ct) - int(p.at)
            p.wt = int(p.tt) - int(p.bt)
            att += p.tt
            awt += p.wt

            self.display(p)

            counter += 1

        att /= len(queue)
        awt /= len(queue)

        print("-----------------------------------------")

        print("ATT = %.1f ms" % att)
        print("AWT = %.1f ms" % awt)

    def Process(self):
        self.FCFS()
        self.SJF()

    # test display
    def display(self, p):
        print(
            f"| {p.id : <3}     | {p.at : <3} | {p.bt : <3} | {p.ct:<3} | {p.tt:<3} | {p.wt:<3} |"
        )


# main
queue = []

for i in range(int(input("input number of processes: "))):

    id = input("input process id: ")
    pr = input("input process pr: ")
    at = input("input process at: ")
    bt = input("input process bt: ")
    print()

    queue.append(Process(id, pr, at, bt))

# cpu schedling
Scheduler(queue).Process()


"""
FCFC test
3
1
0
0
5
2
0
0
4
3
0
0
1


id pr at bt
5
P1
0
0
4
P2
0
1
3
P3
0
2
1
P4
0
3
2
P5
0
4
5

SJF test

Process ID  Arrival Time  Burst Time  Waiting Time  Turnaround Time
2              0              4              0              4
3              4              2              0              2
1              2              3              4              7
4              5              4              4              8
5              6              6              7             13
6              7              7             12             19

6
1
0
2
3
2
0
0
4
3
0
4
2
4
0
5
4
5
0
6
6
6
0
7
7


"""
