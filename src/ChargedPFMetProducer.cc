// -*- C++ -*-
//
// Package:    ChargedPFMetProducer
// Class:      ChargedPFMetProducer
// 
/**\class ChargedPFMetProducer ChargedPFMetProducer.cc Leptoquarks/MetTools/src/ChargedPFMetProducer.cc

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

#include "DataFormats/RecoCandidate/interface/RecoCandidate.h"
#include "DataFormats/METReco/interface/CommonMETData.h"
#include "DataFormats/METReco/interface/PFMETFwd.h"
#include "DataFormats/METReco/interface/PFMETCollection.h"
#include "RecoMET/METAlgorithms/interface/PFSpecificAlgo.h"

//
// class declaration
//

class ChargedPFMetProducer : public edm::EDProducer {
   public:
      explicit ChargedPFMetProducer(const edm::ParameterSet&);
      ~ChargedPFMetProducer();

      static void fillDescriptions(edm::ConfigurationDescriptions& descriptions);

   private:
      virtual void beginJob() ;
      virtual void produce(edm::Event&, const edm::EventSetup&);
      virtual void endJob() ;
      
      virtual void beginRun(edm::Run&, edm::EventSetup const&);
      virtual void endRun(edm::Run&, edm::EventSetup const&);
      virtual void beginLuminosityBlock(edm::LuminosityBlock&, edm::EventSetup const&);
      virtual void endLuminosityBlock(edm::LuminosityBlock&, edm::EventSetup const&);

      edm::InputTag collectionTag_;    
      edm::InputTag vertexTag_;
      double dzCut_;

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
ChargedPFMetProducer::ChargedPFMetProducer(const edm::ParameterSet& iConfig):
  collectionTag_(iConfig.getParameter<edm::InputTag>("collectionTag"))
{
   //register your products
  /* Examples
     produces<ExampleData2>();
     
     //if do put with a label
     produces<ExampleData2>("label");
     
     //if you want to put into the Run
     produces<ExampleData2,InRun>();
  */
  
  produces<reco::PFMETCollection>();
  //now do what ever other initialization is needed
  
}


ChargedPFMetProducer::~ChargedPFMetProducer()
{
 
   // do anything here that needs to be done at desctruction time
   // (e.g. close files, deallocate resources etc.)

}


//
// member functions
//

// ------------ method called to produce the data  ------------
void
ChargedPFMetProducer::produce(edm::Event& iEvent, const edm::EventSetup& iSetup)
{
  using namespace edm;
  using namespace reco;
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
  
  edm::Handle<edm::View<reco::Candidate> > reducedPFCands;
  iEvent.getByLabel(collectionTag_,  reducedPFCands);
  
  reco::Candidate::LorentzVector totalP4;
  float sumet = 0.0;
  edm::View<reco::Candidate>::const_iterator it;
  
  for(it= reducedPFCands->begin(); it!=reducedPFCands->end(); ++it) {
    totalP4 += it->p4();
    sumet += it->pt();
  }
  
  reco::Candidate::LorentzVector invertedP4(-totalP4);
  
  CommonMETData output;
  output.mex = invertedP4.px();
  output.mey = invertedP4.py();
  output.mez = invertedP4.pz();
  output.met = invertedP4.pt();
  output.sumet = sumet;
  output.phi = atan2(invertedP4.py(),invertedP4.px());
  PFSpecificAlgo pf;
  std::auto_ptr<reco::PFMETCollection> pfmetcoll;
  pfmetcoll.reset (new reco::PFMETCollection);
  pfmetcoll->push_back( pf.addInfo(reducedPFCands, output) );
  
  // and finally put it in the event
  iEvent.put( pfmetcoll );      
}

// ------------ method called once each job just before starting event loop  ------------
void 
ChargedPFMetProducer::beginJob()
{
}

// ------------ method called once each job just after ending the event loop  ------------
void 
ChargedPFMetProducer::endJob() {
}

// ------------ method called when starting to processes a run  ------------
void 
ChargedPFMetProducer::beginRun(edm::Run&, edm::EventSetup const&)
{
}

// ------------ method called when ending the processing of a run  ------------
void 
ChargedPFMetProducer::endRun(edm::Run&, edm::EventSetup const&)
{
}

// ------------ method called when starting to processes a luminosity block  ------------
void 
ChargedPFMetProducer::beginLuminosityBlock(edm::LuminosityBlock&, edm::EventSetup const&)
{
}

// ------------ method called when ending the processing of a luminosity block  ------------
void 
ChargedPFMetProducer::endLuminosityBlock(edm::LuminosityBlock&, edm::EventSetup const&)
{
}

// ------------ method fills 'descriptions' with the allowed parameters for the module  ------------
void
ChargedPFMetProducer::fillDescriptions(edm::ConfigurationDescriptions& descriptions) {
  //The following says we do not know what parameters are allowed so do no validation
  // Please change this to state exactly what you do use, even if it is no parameters
  edm::ParameterSetDescription desc;
  desc.setUnknown();
  descriptions.addDefault(desc);
}

//define this as a plug-in
DEFINE_FWK_MODULE(ChargedPFMetProducer);
