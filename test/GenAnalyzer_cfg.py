import FWCore.ParameterSet.Config as cms
import os
process = cms.Process("GenAnalyzer")                                                                                                                
process.load("FWCore.MessageService.MessageLogger_cfi")
process.load('Configuration/StandardSequences/Services_cff')
process.load('Configuration/StandardSequences/FrontierConditions_GlobalTag_cff')
### Set Geometry/GlobalTag/BField
process.load('Configuration.StandardSequences.GeometryRecoDB_cff')
process.load("Configuration.StandardSequences.Reconstruction_cff")

from Configuration.AlCa.GlobalTag import GlobalTag
globalTag = '130X_mcRun3_2022_realistic_v5'  # for Run3 MC
process.GlobalTag.globaltag = cms.string(globalTag)
process.GlobalTag = GlobalTag(process.GlobalTag, globalTag, '')

process.load("Configuration.EventContent.EventContent_cff")
process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(-1) )

process.source = cms.Source("PoolSource",
    duplicateCheckMode = cms.untracked.string('noDuplicateCheck'),
    # fileNames = cms.untracked.vstring("file:/eos/user/c/chiw/JpsiJpsiPhi/tryHelac/CMSSW_12_4_14_patch3_mc_demo/src/test_JpsiPhi_Run3Summer22_GENonly.root")
    # fileNames = cms.untracked.vstring("file:/eos/user/c/chiw/JpsiJpsiPhi/MC_samples/GENSIM/DPS-JpsiJpsi-Phi/filter_JPsi_PtMin6p0_Phi_PtMin6p0/DPS-JpsiJpsi-Phi1020_JJPhi_4Mu2K_13p6TeV_TuneCP5_pythia8_Run3Summer22_GENSIM_20.root")
    # Read file names with "*" matching, with python os module
    fileNames = cms.untracked.vstring( 
        'file:' + os.path.join('/eos/user/c/chiw/JpsiJpsiPhi/MC_samples/GENSIM/DPS-JpsiJpsi-Phi/filter_JPsi_PtMin6p0_Phi_PtMin6p0/', fname) 
        for fname in os.listdir('/eos/user/c/chiw/JpsiJpsiPhi/MC_samples/GENSIM/DPS-JpsiJpsi-Phi/filter_JPsi_PtMin6p0_Phi_PtMin6p0/') 
        if fname.endswith('.root') and fname.startswith('DPS-JpsiJpsi-Phi1020_JJPhi_4Mu2K_13p6TeV_TuneCP5_pythia8_Run3Summer22_GENSIM_')
    )
    # fileNames = cms.untracked.vstring( 
    #     'file:' + os.path.join('/eos/user/x/xcheng/learn_MC/JJP_DPS_MC_output/MINIAOD/', fname) 
    #     for fname in os.listdir('/eos/user/x/xcheng/learn_MC/JJP_DPS_MC_output/MINIAOD/') 
    #     if fname.endswith('20_MINIAOD.root')
    # )
    # fileNames = cms.untracked.vstring( 
    #     'file:/eos/user/c/chiw/JpsiJpsiPhi/MC_samples/GENSIM/DPS-JpsiJpsi-Phi/filter_JPsi_PtMin6p0_Phi_PtMin6p0/deprecated-pseudo-DPS-JpsiY2S-Phi-4Mu2K_13p6TeV_TuneCP5_pythia8_Run3Summer22_GENSIM.root'
    # )
)

# ----------------------------------------------------------------------
# Output root file (monitoring histograms)
# ----------------------------------------------------------------------
process.TFileService=cms.Service('TFileService',
    fileName=cms.string('DPS_shuffled_JpsiJpsi-Phi_GEN_level.root')
)

from FWCore.Modules.printContent_cfi import *
process.printDebug = cms.EDAnalyzer("EventContentAnalyzer")

process.GenAnalyzer = cms.EDAnalyzer('GenAnalyzer',
         genParticlesAOD = cms.InputTag('genParticles') 
)
process.p = cms.Path(process.GenAnalyzer)
