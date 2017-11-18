import FWCore.ParameterSet.Config as cms

import sys
import os

process = cms.Process("EdmToNtupleNoMask")

process.load("FWCore.MessageService.MessageLogger_cfi")
process.MessageLogger.cerr.threshold = 'INFO'
process.MessageLogger.cerr.FwkReport.reportEvery = 10
process.maxEvents = cms.untracked.PSet(input = cms.untracked.int32(-1))

#if len(sys.argv)>2:
#    inFile="file:"+sys.argv[2]
#else:
#    inFile = 'USC.00000478.0001.A.storageManager.00.0000.root'

process.source = cms.Source("PoolSource",
        fileNames = cms.untracked.vstring('file:/afs/cern.ch/work/s/sroychow/public/BeamTest2S/year2017/rawtodigi.root')
)

#mylist = FileUtils.loadListFromFile ('runInputList.txt') 
#readFiles = cms.untracked.vstring( *mylist)

#import FWCore.Utilities.FileUtils as FileUtils
#readFiles = cms.untracked.vstring( FileUtils.loadListFromFile ('///nfs/dust/cms/user/harbali/data/runInputList.txt') )
#process.source = cms.Source ("PoolSource",fileNames = readFiles)

#outFile = "RAW_%s"%(inFile)
#outFile = inFile.split("/")[-1].replace(".root","_RAW.root")
outFile = 'testNov17.root'

process.p = cms.Path(process.simple)

