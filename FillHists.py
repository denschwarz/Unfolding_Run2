#!/usr/bin/python
from datetime import datetime
import sys, multiprocessing, time, subprocess
from ROOT import *

n_jobs = 3


def main():
    global n_jobs                                                       # Needed to modify global copy of n_jobs
    pool_muon = multiprocessing.Pool(processes = n_jobs)
    job_list = [
        # ['muon','FullRun2'],
        ['muon','2016'],
        ['muon','2017'],
        ['muon','2018'],
        ['elec','2016'],
        ['elec','2017'],
        ['elec','2018'],
        ['muon_elecJMS','2016'],
        ['muon_elecJMS','2017'],
        ['muon_elecJMS','2018'],
        ['elec_muonJMS','2016'],
        ['elec_muonJMS','2017'],
        ['elec_muonJMS','2018']
    ]
    result = pool_muon.map_async(submit_job, job_list)
    pool_muon.close()
    pool_muon.join()
    # print 'now combine...'
    # subprocess.call(['./combine.sh'])

def submit_job(args):
    print 'fill hists', args[0], args[1], 'submitted'
    subprocess.call(['./hist_filler',args[0], args[1]], stdout=open("/dev/null","w"), stderr=subprocess.STDOUT)
    print 'fill hists', args[0], args[1], 'finished!'
    return 1


if __name__ == "__main__":
    main()
