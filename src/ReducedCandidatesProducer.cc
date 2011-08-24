// -*- C++ -*-
//
// Package:    ReducedCandidatesProducer
// Class:      ReducedCandidatesProducer
// 
/**\class ReducedCandidatesProducer ReducedCandidatesProducer.cc Leptoquarks/MetTools/src/ReducedCandidatesProducer.cc

 Description: [one line class summary]

 Implementation:
     [Notes on implementation]
*/
//
// Original Author:  Francesco Santanastasio,8 R-019,+41227675765,
//         Created:  Tue Aug 23 17:50:50 CEST 2011
// $Id$
//
//


// system include files
#include <memory>

// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDProducer.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"

//

#include "DataFormats/Candidate/interface/Candidate.h"
#include "DataFormats/ParticleFlowCandidate/interface/PFCandidate.h"
#include "DataFormats/ParticleFlowCandidate/interface/PFCandidateFwd.h"
#include "DataFormats/Candidate/interface/LeafCandidate.h"

#include "DataFormats/VertexReco/interface/Vertex.h"
#include "DataFormats/VertexReco/interface/VertexFwd.h"
#include <DataFormats/MuonReco/interface/Muon.h>
#include <DataFormats/GsfTrackReco/interface/GsfTrack.h>

//
// class declaration
//

class ReducedCandidatesProducer : public edm::EDProducer {
   public:
      explicit ReducedCandidatesProducer(const edm::ParameterSet&);
      ~ReducedCandidatesProducer();

      static void fillDescriptions(edm::ConfigurationDescriptions& descriptions);

   private:
      virtual void beginJob() ;
      virtual void produce(edm::Event&, const edm::EventSetup&);
      virtual void endJob() ;
      
      virtual void beginRun(edm::Run&, edm::EventSetup const&);
      virtual void endRun(edm::Run&, edm::EventSetup const&);
      virtual void beginLuminosityBlock(edm::LuminosityBlock&, edm::EventSetup const&);
      virtual void endLuminosityBlock(edm::LuminosityBlock&, edm::EventSetup const&);

      edm::InputTag srcCands_;
      edm::InputTag srcVertices_;
      double dz_;

      // ----------member data ---------------------------
};

//
// constants, enums and typedefs
//


//
// static data member definitions
//

//
// constructors and destructor
//
ReducedCandidatesProducer::ReducedCandidatesProducer(const edm::ParameterSet& iConfig):
  srcCands_( iConfig.getParameter<edm::InputTag>("srcCands") ),
  srcVertices_( iConfig.getParameter<edm::InputTag>("srcVertices") ),
  dz_( iConfig.getParameter<double>("dz") )
{
   //register your products
  /* Examples
     produces<ExampleData2>();
     
     //if do put with a label
     produces<ExampleData2>("label");
     
     //if you want to put into the Run
     produces<ExampleData2,InRun>();
  */

  using namespace reco;
  produces<CandidateCollection>(); 
  //now do what ever other initialization is needed
  
}


ReducedCandidatesProducer::~ReducedCandidatesProducer()
{
 
   // do anything here that needs to be done at desctruction time
   // (e.g. close files, deallocate resources etc.)

}


//
// member functions
//

// ------------ method called to produce the data  ------------
void
ReducedCandidatesProducer::produce(edm::Event& iEvent, const edm::EventSetup& iSetup)
{
  using namespace edm;
  using namespace reco;
  using namespace std;
  /* This is an event example
  //Read 'ExampleData' from the Event
  Handle<ExampleData> pIn;
  iEvent.getByLabel("example",pIn);
  
  //Use the ExampleData to create an ExampleData2 which 
  // is put into the Event
  std::auto_ptr<ExampleData2> pOut(new ExampleData2(*pIn));
  iEvent.put(pOut);
  */
  
  /* this is an EventSetup example
  //Read SetupData from the SetupRecord in the EventSetup
  ESHandle<SetupData> pSetup;
  iSetup.get<SetupRecord>().get(pSetup);
  */
  
  std::auto_ptr<CandidateCollection> comp( new CandidateCollection );

  Handle<PFCandidateCollection>   cands;
  iEvent.getByLabel( srcCands_, cands );
  
  edm::Handle<vector<reco::Vertex> >  vtxH;
  iEvent.getByLabel( srcVertices_ ,vtxH);
  
  reco::Vertex vtx = vtxH->front();

  for( PFCandidateCollection::const_iterator c = cands->begin(); c != cands->end(); ++c ) {
    if(c->charge()==0) continue;
    double dz(99999.);
    if(c->trackRef().isNonnull()) 
      dz = fabs(c->trackRef()->dz(vtx.position()));
    else if(c->gsfTrackRef().isNonnull())
      dz = fabs(c->gsfTrackRef()->dz(vtx.position()));
    else if(c->muonRef().isNonnull() && c->muonRef()->innerTrack().isNonnull())
      dz = fabs(c->muonRef()->innerTrack()->dz(vtx.position()));
    else {
      cout << "WARNING: found charged PF candidate without any track ref" << endl;
      continue;
    }
    if(dz>dz_) continue;
    std::auto_ptr<Candidate> cand( new LeafCandidate( * c ) );
    comp->push_back( cand.release() );
  }
  
  iEvent.put( comp );
  
}

// ------------ method called once each job just before starting event loop  ------------
void 
ReducedCandidatesProducer::beginJob()
{
}

// ------------ method called once each job just after ending the event loop  ------------
void 
ReducedCandidatesProducer::endJob() {
}

// ------------ method called when starting to processes a run  ------------
void 
ReducedCandidatesProducer::beginRun(edm::Run&, edm::EventSetup const&)
{
}

// ------------ method called when ending the processing of a run  ------------
void 
ReducedCandidatesProducer::endRun(edm::Run&, edm::EventSetup const&)
{
}

// ------------ method called when starting to processes a luminosity block  ------------
void 
ReducedCandidatesProducer::beginLuminosityBlock(edm::LuminosityBlock&, edm::EventSetup const&)
{
}

// ------------ method called when ending the processing of a luminosity block  ------------
void 
ReducedCandidatesProducer::endLuminosityBlock(edm::LuminosityBlock&, edm::EventSetup const&)
{
}

// ------------ method fills 'descriptions' with the allowed parameters for the module  ------------
void
ReducedCandidatesProducer::fillDescriptions(edm::ConfigurationDescriptions& descriptions) {
  //The following says we do not know what parameters are allowed so do no validation
  // Please change this to state exactly what you do use, even if it is no parameters
  edm::ParameterSetDescription desc;
  desc.setUnknown();
  descriptions.addDefault(desc);
}

//define this as a plug-in
DEFINE_FWK_MODULE(ReducedCandidatesProducer);
