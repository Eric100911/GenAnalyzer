# GenAnalyzer

This program can be run under `CMSSW_10_2_5`.

## 1. Setup
In `CMSSW_10_2_5/src/` directory, git clone this repository via
```bash
git clone git@github.com:Alph-omeg/GenAnalyzer.git
```
or
```bash
git clone https://github.com/Alph-omeg/GenAnalyzer.git
```

## 2. Modify
Change input(PoolSource) and output file name in `test/GenAnalyzer_cfg.py`.
```python
# line 19
fileNames = cms.untracked.vstring("/path/fileName.root")
# line 26
fileName = cms.string("outputFile.root")
```
Change CRAB configuration in `test/multicrab_MC`.
```python
# line 70
config.General.workArea = 'workArea'
# line 80
config.Data.unitsPerJob = 100
# line 82
config.Data.outLFNDirBase = '/LFN_Path/'
# line 85
config.Site.storageSite = 'T2_CN_Beijing'
# line 88
inputDatasets = ['/A/user-B-hashcode/C']
# other configuration in real case...
```

## 3. Run
Compile the program after modification finishes.
```bash
scram b
```
Test the program via
```bash
cmsRun GenAnalyzer_cfg.py
```
The following step may be necessary before submitting CRAB job.
Try this command out if submission crashes.
```bash
source /cvmfs/cms.cern.ch/common/crab-setup.sh
```
Finally the program is ready to run.
```bash
./multicrab_MC --crabCmd=submit
```