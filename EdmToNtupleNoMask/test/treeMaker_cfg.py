import FWCore.ParameterSet.Config as cms
import sys

process = cms.Process("treeMaker")
process.load("FWCore.MessageService.MessageLogger_cfi")
process.load("TestBeamAnalysis.EdmToNtupleNoMask.EdmToNtupleNoMask_cfi")
process.MessageLogger.cerr.FwkReport.reportEvery = 10000
process.treeMaker.verbosity = cms.untracked.int32(1)

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(10) )

process.source = cms.Source("PoolSource",
    # replace 'myfile.root' with the source file you want to use
    fileNames = cms.untracked.vstring(
     'file:/afs/cern.ch/work/s/sroychow/public/BeamTest2S/year2017/rawtodigi.root'
    )
)

process.TFileService = cms.Service("TFileService",
  fileName = cms.string('test2017.root')
)

process.p = cms.Path(process.treeMaker)
