#!/usr/bin/python
from datetime import datetime
import sys, multiprocessing, time, subprocess
from ROOT import *

n_jobs = 4


def main():

    if len(sys.argv) < 3:
        print "[ERROR] wrong usage:"
        print "    ./RunUnfolding_masses <channel> <year>"
        print "                     - channel: muon/elec/combine"
        print "                     - year   : 2016/2017/2018"
        return

    print('Channel: ', sys.argv[1])
    print('Year: ', sys.argv[2])

    global n_jobs                                                       # Needed to modify global copy of n_jobs
    pool = multiprocessing.Pool(processes = n_jobs)
    job_list = ['pseudo1695', 'pseudo1715', 'pseudo1735', 'pseudo1755'] # Create a List with [1,2,...,n_jobs]
    result = pool.map_async(submit_job,job_list)
    pool.close()
    pool.join()

def submit_job(arg):
    print 'Job', arg, 'submitted'
    subprocess.call(['./do_unfolding',arg,sys.argv[1],sys.argv[2]], stdout=open("/dev/null","w"), stderr=subprocess.STDOUT)
    print 'Job', arg, 'finished!'
    return 1


if __name__ == "__main__":
    main()
