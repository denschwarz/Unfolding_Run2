#!/usr/bin/python
from datetime import datetime
import sys, multiprocessing, time, subprocess
from ROOT import *

n_jobs = 10


def main():
    global n_jobs                                                       # Needed to modify global copy of n_jobs
    pool_muon = multiprocessing.Pool(processes = n_jobs)
    job_list = [
        ['data', 'muon', '2016'],
        ['pseudo1695', 'muon', '2016'],
        ['pseudo1715', 'muon', '2016'],
        ['pseudo1735', 'muon', '2016'],
        ['pseudo1755', 'muon', '2016'],
        ['data', 'elec', '2016'],
        ['pseudo1695', 'elec', '2016'],
        ['pseudo1715', 'elec', '2016'],
        ['pseudo1735', 'elec', '2016'],
        ['pseudo1755', 'elec', '2016'],
        ['data', 'combine', '2016'],
        ['pseudo1695', 'combine', '2016'],
        ['pseudo1715', 'combine', '2016'],
        ['pseudo1735', 'combine', '2016'],
        ['pseudo1755', 'combine', '2016'],
        ['data', 'muon', '2017'],
        ['pseudo1695', 'muon', '2017'],
        ['pseudo1715', 'muon', '2017'],
        ['pseudo1735', 'muon', '2017'],
        ['pseudo1755', 'muon', '2017'],
        ['data', 'elec', '2017'],
        ['pseudo1695', 'elec', '2017'],
        ['pseudo1715', 'elec', '2017'],
        ['pseudo1735', 'elec', '2017'],
        ['pseudo1755', 'elec', '2017'],
        ['data', 'combine', '2017'],
        ['pseudo1695', 'combine', '2017'],
        ['pseudo1715', 'combine', '2017'],
        ['pseudo1735', 'combine', '2017'],
        ['pseudo1755', 'combine', '2017'],
        ['data', 'muon', '2018'],
        ['pseudo1695', 'muon', '2018'],
        ['pseudo1715', 'muon', '2018'],
        ['pseudo1735', 'muon', '2018'],
        ['pseudo1755', 'muon', '2018'],
        ['data', 'elec', '2018'],
        ['pseudo1695', 'elec', '2018'],
        ['pseudo1715', 'elec', '2018'],
        ['pseudo1735', 'elec', '2018'],
        ['pseudo1755', 'elec', '2018'],
        ['data', 'combine', '2018'],
        ['pseudo1695', 'combine', '2018'],
        ['pseudo1715', 'combine', '2018'],
        ['pseudo1735', 'combine', '2018'],
        ['pseudo1755', 'combine', '2018'],
        ['data', 'combine', 'FullRun2'],
        ['pseudo1695', 'combine', 'FullRun2'],
        ['pseudo1715', 'combine', 'FullRun2'],
        ['pseudo1735', 'combine', 'FullRun2'],
        ['pseudo1755', 'combine', 'FullRun2'],
        ['data', 'muon_elecJMS', '2016'],
        ['data', 'muon_elecJMS', '2017'],
        ['data', 'muon_elecJMS', '2018'],
        ['data', 'elec_muonJMS', '2016'],
        ['data', 'elec_muonJMS', '2017'],
        ['data', 'elec_muonJMS', '2018'],
        ['pseudo1715', 'muon_elecJMS', '2016'],
        ['pseudo1715', 'muon_elecJMS', '2017'],
        ['pseudo1715', 'muon_elecJMS', '2018'],
        ['pseudo1735', 'muon_elecJMS', '2016'],
        ['pseudo1735', 'muon_elecJMS', '2017'],
        ['pseudo1735', 'muon_elecJMS', '2018'],
        ['pseudo1715', 'elec_muonJMS', '2016'],
        ['pseudo1715', 'elec_muonJMS', '2017'],
        ['pseudo1715', 'elec_muonJMS', '2018'],
        ['pseudo1735', 'elec_muonJMS', '2016'],
        ['pseudo1735', 'elec_muonJMS', '2017'],
        ['pseudo1735', 'elec_muonJMS', '2018'],
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
