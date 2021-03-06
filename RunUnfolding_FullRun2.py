#!/usr/bin/python
from datetime import datetime
import sys, multiprocessing, time, subprocess
from ROOT import *

n_jobs = 5


def main():
    global n_jobs                                                       # Needed to modify global copy of n_jobs
    pool_muon = multiprocessing.Pool(processes = n_jobs)
    job_list = [
        ['data', 'combine', 'FullRun2'],
        ['pseudo1695', 'combine', 'FullRun2'],
        ['pseudo1715', 'combine', 'FullRun2'],
        ['pseudo1735', 'combine', 'FullRun2'],
        ['pseudo1755', 'combine', 'FullRun2']
    ]
    result = pool_muon.map_async(submit_job, job_list)
    pool_muon.close()
    pool_muon.join()

def submit_job(arg):
    print 'Job', arg[0], arg[1], arg[2], 'submitted'
    subprocess.call(['./do_unfolding',arg[0],arg[1],arg[2]], stdout=open("/dev/null","w"), stderr=subprocess.STDOUT)
    print 'Job', arg[0], arg[1], arg[2], 'finished!'
    return 1


if __name__ == "__main__":
    main()
