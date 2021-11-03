# webserv (@42Paris)

"This is when you finally understand why a URL starts with HTTP"

![Alt text](https://github.com/llefranc/42_webserv/blob/main/webserv_example.png)

## About



Philosophers is a project of the mandatory part of the cursus.
It's only made in C and the main goal was to understand how threads works in language C and use them to solve a logical problem.
The simulation will stop if all philosophers have eaten enough or if one philosopher die.

- There are three different version of the algorithm :
    - Philo_one is about using threads and mutexs
    - Philo_two is about using threads and semaphores
    - Philo_three is about using processes and semaphores

- Here is the [subject][1]

`This project was code for MACOS`

### Building and running the project

1. Download/Clone this repo

        git clone https://github.com/llefranc/42_philosophers.git
2. `cd` into the root directory, then `cd` into philo_x (one, two or three) and run `make`

        cd 42_philosophers/philo_one
        make

3.  run `./philo_x arg1 arg2 arg3 arg4 (arg5)` from the directory philo_x.
    - arg1 is the number of philosopher
    - arg2 is the time to die in ms
    - arg3 is the time to eat in ms
    - arg4 is the time to sleep in ms
    - arg5 (optional) is the number of time each philosopher needs to eat to end the simulation
    
## Sources

- [Programmation système en C sous UNIX (FR)][5]
- [Processus et threads (FR)][2]
- [Cours sur les threads (FR)][3]
- [Optimizing philo algo][4]
- [Comprendre les sémaphores (FR)][6]
- [How to use sempahores on fork processes][7]

[1]: https://github.com/llefranc/42_webserv/blob/main/webserv.en.subject.pdf
[2]: https://zestedesavoir.com/tutoriels/607/les-systemes-dexploitation/processus-et-threads/
[3]: https://cours.polymtl.ca/inf2610/documentation/notes/chap4.pdf
[4]: https://www.notion.so/philosophers-VM-c60be9c836084edfbcd9c07e29b429c4
[5]: https://pub.phyks.me/sdz/sdz/la-programmation-systeme-en-c-sous-unix.html#Lesthreads
[6]: https://sites.uclouvain.be/SystInfo/notes/Theorie/html/Threads/coordination.html
[7]: https://stackoverflow.com/questions/16400820/how-to-use-posix-semaphores-on-forked-processes-in-c
