#include "jobmanager.h"
#include <sched.h>
#include <iomanip>
#include "utils.h"
#include <unistd.h>

jobmanager::jobmanager() {
        sem_init(&_sem, 0, 0);
        cpu_set_t set;
        CPU_ZERO(&set);
        
        int err = sched_getaffinity(0, sizeof(set), &set);
        if (err) {
                perror("could not get affinity");
                exit(0);
        }
        
        _schedMask = 0;
        _numThreads = CPU_COUNT(&set);
        for (unsigned i = 0; i < sizeof(set); ++i)
                if (CPU_ISSET(i, &set))
                        _schedMask |= (1 <<i);
        _jobs = (job*)calloc(_numThreads, sizeof(job));
        for (unsigned i = 0; i < _numThreads; ++i)
                new (_jobs+i) job(i, *this);
}

void jobmanager::threadDone(int tid) {
        uint64_t newmask = __sync_and_and_fetch(&_runningMask, ~(1 << tid));
        if (newmask == 0)
                sem_post(&_sem);
}

void jobmanager::runJobs() {
        _runningMask = (1 << _numThreads) - 1;
        for (unsigned i =0; i < _numThreads; ++i)
                _jobs[i].notify();
        sem_wait(&_sem);
}

void jobmanager::registerThread(job* job) {
        cpu_set_t set;
        CPU_ZERO(&set);

        int cnt = job->getId();
        int res;
        for (int i = 0; i < 64; ++i) {
                if (_schedMask & (1<<i)) {
                        if (cnt == 0) {
                                CPU_SET(i, &set);
                                //schedule this job to run on this cpu
                                res = sched_setaffinity(0, sizeof(set), &set);
                                if (res) {
                                        perror("could not set affinity");
                                        exit(1);
                                }
                                return;
                        }
                        --cnt;
                }
        }
        
        INFO("Could not set affinity for thread " << job->getId());
        exit(1);
}
