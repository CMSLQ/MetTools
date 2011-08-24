import FWCore.ParameterSet.Config as cms

chargedPFMetProducer = cms.EDProducer('ChargedPFMetProducer',
                                      collectionTag = cms.InputTag("reducedPFCands")
                                      )
