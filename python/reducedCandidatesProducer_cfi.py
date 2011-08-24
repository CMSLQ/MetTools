import FWCore.ParameterSet.Config as cms

reducedPFCands = cms.EDProducer('ReducedCandidatesProducer',
                                srcCands = cms.InputTag('particleFlow'),
                                srcVertices = cms.InputTag('offlinePrimaryVertices'),
                                dz = cms.double(0.1)
                                )
