#!/usr/bin/python
from datetime import datetime
import sys, multiprocessing, time, subprocess
from ROOT import *

n_jobs = 10


def main():
    global n_jobs                                                       # Needed to modify global copy of n_jobs
    pool_muon = multiprocessing.Pool(processes = n_jobs)
    job_list = [
        ['data', 'muon_elecJMS', '2016'],
        ['data', 'muon_elecJMS', '2017'],
        ['data', 'muon_elecJMS', '2018'],
        ['data', 'elec_muonJMS', '2016'],
        ['data', 'elec_muonJMS', '2017'],
        ['data', 'elec_muonJMS', '2018'],
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
