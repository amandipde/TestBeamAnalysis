import FWCore.ParameterSet.Config as cms

treeMaker = cms.EDAnalyzer("EdmToNtupleNoMask",
   verbosity = cms.untracked.int32(0),
   detIdBottomVec = cms.vstring( "50000", "51011", "50025" ),
   detIdTopVec = cms.vstring( "50004", "51012", "50026" ),
   isSparsified = cms.bool(False),
   fedRawData = cms.InputTag("rawDataCollector"),
   unsparisifiedDigiColl = cms.InputTag("Phase2TrackerDigiProducerTestBeam","Unsparsified"),
   cluster1DColl = cms.InputTag("Phase2TrackerDigiProducerTestBeam", "Sparsified"),
   stubColl = cms.InputTag("Phase2TrackerStubProducer", "Stubs" ),
   ####Not used till now
   detNamesVec = cms.vstring( "det0","det1","det2","det3" ),
   tdcAddress = cms.string("0x03000000"),
   hvAddress = cms.string("0x05000000"),
   dutAngAddress = cms.string("0x04000000"),
   stubAddress = cms.string("0x0B0000FF"),
   cwdAddress = cms.string("0x01180100"),
   offsetAddress = cms.string("0x80000000"),
   windowAddress = cms.string("0x01190100"),
   tiltAddress = cms.string("0x83000000"),
   vcthAddress = cms.string("0x010c0100"),
   stubLatencyAddress = cms.string("0x09000000"),
   triggerLatencyAddress = cms.string("0x01010100"),
   numCBC = cms.untracked.int32(2),
)

