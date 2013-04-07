#ifndef BPRAY_JOB_MANAGER_H
#define BPRAY_JOB_MANAGER_H

#include <semaphore.h>

class jobmanager;

class job {
public:
        job(unsigned id, jobmanager &manager)
                : _id (id)
                , _manager(manager) {
                sem_init(&_sem, 0, 0);
        }
        void run();
        int wait() { return sem_wait(&_sem); }
        void notify() { sem_post(&_sem); }
        unsigned getId() const { return _id; }
        jobmanager &getManager() const { return _manager; } 
        void setArg(void *arg) { _arg = arg; }
        void *getArg() const { return _arg; }
private:
        unsigned _id;
        sem_t _sem;
        jobmanager &_manager;
        void *_arg;
};

class jobmanager {
public:
        jobmanager();
        unsigned getNumThreads() const { return _numThreads; }

        void registerThread(job *job);
        void threadDone(int tid);
        job& getJob(int tid) const { return _jobs[tid]; }
        void runJobs();
private:
        sem_t _sem;
        unsigned long long _runningMask;
        unsigned long long _schedMask;
        unsigned _numThreads;
        job* _jobs;
};

#endif
