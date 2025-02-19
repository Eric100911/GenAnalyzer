import FWCore.ParameterSet.Config as cms
process = cms.Process("GenAnalyzer")                                                                                                                
process.load("FWCore.MessageService.MessageLogger_cfi")
process.load('Configuration/StandardSequences/Services_cff')
process.load('Configuration/StandardSequences/GeometryExtended_cff')
process.load('Configuration/StandardSequences/MagneticField_38T_cff')
process.load('Configuration/StandardSequences/FrontierConditions_GlobalTag_cff')

globalTag = '106X_upgrade2018_realistic_v4'
process.GlobalTag.globaltag = cms.string(globalTag)
from Configuration.AlCa.GlobalTag_condDBv2 import GlobalTag
process.GlobalTag = GlobalTag(process.GlobalTag, globalTag, '')

process.load("Configuration.EventContent.EventContent_cff")
process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(-1) )

process.source = cms.Source("PoolSource",
    duplicateCheckMode = cms.untracked.string('noDuplicateCheck'),
    fileNames = cms.untracked.vstring("file:/eos/home-c/chensh/JPsiPsi2s/GEN_nofilter/SPS/ULPythia2018/CMSSW_10_6_20_patch1/src/GenAnalyzer/GenAnalyzer/test/BPH-GEN-13TeV_1.root")
)

# ----------------------------------------------------------------------
# Output root file (monitoring histograms)
# ----------------------------------------------------------------------
process.TFileService=cms.Service('TFileService',
    fileName=cms.string('SPS_JJ_withGenFilter.root')
)

from FWCore.Modules.printContent_cfi import *
process.printDebug = cms.EDAnalyzer("EventContentAnalyzer")

process.GenAnalyzer = cms.EDAnalyzer('GenAnalyzer',
         genParticlesAOD = cms.InputTag('genParticles') 
)
process.p = cms.Path(process.GenAnalyzer)
