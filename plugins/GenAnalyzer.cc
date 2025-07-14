// -*- C++ -*-
/*
    Package:    GenAnalyzer/GenAnalyzer
    Class:      GenAnalyzer

    Description: GEN level MC sample analyzer
    Implementation:
        This plugin is designed for pp->J/psi+psi(2S)->mu+mu-mu+mu- DPS/SPS MC sample.
        Definition of 'deve' serves as a switch to allow more detailed information.
        For SPS samples GEN_pair_id specify which one of J/psi and psi(2S) are paired.

    Original Author:  Candan Dozen
    Created:          Sat, 11 Jul 2020 11:06:05 GMT

    Modified by: Shiyang Chen
    ////////////////////////////////
    //
    //            ／＞      フ
    //            |    _   _|
    //           ／` ミ＿꒳ノ
    //          /          |
    //         /   ヽ     ﾉ
    //        │    |  |  |
    //    ／￣|     |  |  |
    //    | (￣ヽ＿_ヽ_)__)
    //    ＼二つ
    ////////////////////////////////
*/

// system include files
#include <memory>
// user include files
#include <vector>
#include <iostream>
#include <string>
#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include <DataFormats/HepMCCandidate/interface/GenParticle.h>
#include "DataFormats/HepMCCandidate/interface/GenParticleFwd.h"
#include "DataFormats/EgammaCandidates/interface/GsfElectron.h"
#include "DataFormats/HepMCCandidate/interface/GenStatusFlags.h"
#include "DataFormats/Candidate/interface/CompositeRefCandidateT.h"
#include "DataFormats/EgammaCandidates/interface/Conversion.h"
#include "DataFormats/VertexReco/interface/Vertex.h"
#include "DataFormats/VertexReco/interface/VertexFwd.h"
#include "DataFormats/METReco/interface/PFMETCollection.h"
#include "SimDataFormats/PileupSummaryInfo/interface/PileupSummaryInfo.h"
#include "DataFormats/Math/interface/LorentzVector.h"
#include <FWCore/Framework/interface/ESHandle.h>
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/Framework/interface/ESHandle.h"
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "CommonTools/UtilAlgos/interface/TFileService.h"
#include <TString.h>
#include <stdio.h>
#include <TCanvas.h>
#include <TROOT.h>
#include <TNtuple.h>
#include <TTree.h>
#include "TTree.h"
#include "TClonesArray.h"
#include "TParticle.h"
#include "TVector3.h"
#include "TH1F.h"
#include "TH2F.h"

// Development mode
#define deve

using namespace std;
using namespace edm;
using namespace reco;

class GenAnalyzer : public edm::EDAnalyzer  {
    public:
        explicit GenAnalyzer(const edm::ParameterSet&);
        ~GenAnalyzer();
        static void fillDescriptions(edm::ConfigurationDescriptions & descriptions);
        void FillTruth(const edm::Event&, const edm::EventSetup&);
        void FillEvent(const edm::Event&, const edm::EventSetup&);
   
    private:
        virtual void beginJob() override;
        virtual void analyze(const edm::Event&, const edm::EventSetup&) override;
        virtual void endJob() override;
        bool verbose = false;
        TTree *_mytree;
        int _nEvent, _nRun, _nLumi;
        // -------------------------
        // GEN level information
        // -------------------------
        // Event variables
        vector<pair<int, int>> GEN_pair_id;
        //// J/psi
        #ifdef deve
        int GENjpsi_ins;
        vector<int> GENjpsi_id;
        vector<int> GENjpsi_status;
        vector<int> GENjpsi_nMom;
        vector<double> GENjpsi_nDau;
        vector<vector<int> > GENjpsi_anc;
        vector<vector<int> > GENjpsi_sib;
        #endif
        vector<double> GENjpsi_pt;
        vector<double> GENjpsi_eta;
        vector<double> GENjpsi_mass;
        // vector<vector<double> > GENjpsi_mu_eta;
        // vector<vector<double> > GENjpsi_mu_pt;
        vector <double> GENjpsi_mu_eta;
        vector <double> GENjpsi_mu_pt;
        //// psi(2S)
        #ifdef deve
        int GENpsi2s_ins;
        vector<int> GENpsi2s_id;
        vector<int> GENpsi2s_status;
        vector<int> GENpsi2s_momId;
        vector<double> GENpsi2s_nDau;
        vector<vector<int>> GENpsi2s_anc;
        vector<vector<int>> GENpsi2s_sib;
        #endif
        vector<double> GENpsi2s_pt;
        vector<double> GENpsi2s_eta;
        vector<double> GENpsi2s_mass;
        vector<vector<double> > GENpsi2s_mu_eta;
        vector<vector<double> > GENpsi2s_mu_pt;
        //// muon
        #ifdef deve
        int GENmu_ins;
        vector<int> GENmu_id;
        vector<int> GENmu_status;
        vector<double> GENmu_pt;
        vector<double> GENmu_eta;
        #endif
        /// Phi -> KK verification
        int GEN_phi_ins;
        vector<int> GEN_phi_status;
        vector<int> GEN_phi_momId;
        vector<int> GEN_phi_nDau;
        vector<vector<int>> GEN_phi_anc;
        vector<vector<int>> GEN_phi_sib;
        vector<int> GEN_phi_id;
        vector<double> GEN_phi_pt;
        vector<double> GEN_phi_eta;
        vector<double> GEN_phi_mass;
        // vector< vector<double> > GEN_phi_K_eta;
        // vector< vector<double> > GEN_phi_K_pt;
        vector<double> GEN_phi_K_eta;
        vector<double> GEN_phi_K_pt;
        // Look into all kaons
        int GEN_K_ins;
        vector<int>             GEN_K_status;
        vector<int>             GEN_K_id;
        vector<double>          GEN_K_pt;
        vector<double>          GEN_K_eta;
        vector<int>             GEN_K_momId;
        vector<int>             GEN_K_nDau;
        vector<vector<int> >    GEN_K_anc;
        vector<vector<int> >    GEN_K_sib;
        // -------------------------
  
        const edm::ParameterSet& conf;
        edm::EDGetTokenT<vector<reco::GenParticle> > genParticleToken_;
};

GenAnalyzer::GenAnalyzer(const edm::ParameterSet& iConfig): conf(iConfig) {
    genParticleToken_ = mayConsume<vector<reco::GenParticle>>(iConfig.getParameter<edm::InputTag>("genParticlesAOD"));
    edm::Service<TFileService> fs ;
    _mytree  = fs->make < TTree >("gen_tree","gen tree");
}

GenAnalyzer::~GenAnalyzer() {}

//
// member functions
//
// ------------ method called for each event  ------------
void GenAnalyzer::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup) {
    using namespace edm;

    FillEvent(iEvent, iSetup);
    LogDebug("")<<"After FillEvent"; 

    FillTruth(iEvent, iSetup);
    LogDebug("")<<"After FillTruth"; 
    
    _mytree->Fill();

    //Clear vectors
    GEN_pair_id.clear();

    GENjpsi_pt.clear();
    GENjpsi_eta.clear();
    GENjpsi_mass.clear();
    GENjpsi_mu_eta.clear();
    GENjpsi_mu_pt.clear();

    GENpsi2s_pt.clear();
    GENpsi2s_eta.clear();
    GENpsi2s_mass.clear();
    GENpsi2s_mu_eta.clear();
    GENpsi2s_mu_pt.clear();

    #ifdef deve
    GENjpsi_id.clear();
    GENjpsi_status.clear();
    GENjpsi_nMom.clear();
    GENjpsi_nDau.clear();
    GENjpsi_anc.clear();
    GENjpsi_sib.clear();

    GENpsi2s_id.clear();
    GENpsi2s_status.clear();
    GENpsi2s_momId.clear();
    GENpsi2s_nDau.clear();
    GENpsi2s_anc.clear();
    GENpsi2s_sib.clear();

    GENmu_id.clear();
    GENmu_status.clear();
    GENmu_pt.clear();
    GENmu_eta.clear();
    #endif
    // Phi
    GEN_phi_ins = 0;
    GEN_phi_status.clear();
    GEN_phi_momId.clear();
    GEN_phi_nDau.clear();
    GEN_phi_anc.clear();
    GEN_phi_sib.clear();
    GEN_phi_id.clear();
    GEN_phi_pt.clear();
    GEN_phi_eta.clear();
    GEN_phi_mass.clear();
    GEN_phi_K_eta.clear();
    GEN_phi_K_pt.clear();
    // Kaon
    GEN_K_ins = 0;
    GEN_K_status.clear();
    GEN_K_id.clear();
    GEN_K_pt.clear();
    GEN_K_eta.clear();
    GEN_K_momId.clear();
    GEN_K_nDau.clear();
    GEN_K_anc.clear();
    GEN_K_sib.clear();
    // Clear temporary vectors
}
// ------------ method called once each job just before starting event loop  ------------
void GenAnalyzer::beginJob() {
    _mytree->Branch("GEN_pair_id", &GEN_pair_id);

    _mytree->Branch("GENjpsi_pt", &GENjpsi_pt);
    _mytree->Branch("GENjpsi_eta", &GENjpsi_eta);
    _mytree->Branch("GENjpsi_mass", &GENjpsi_mass);
    _mytree->Branch("GENjpsi_mu_eta", &GENjpsi_mu_eta);
    _mytree->Branch("GENjpsi_mu_pt", &GENjpsi_mu_pt);

    _mytree->Branch("GENpsi2s_pt", &GENpsi2s_pt);
    _mytree->Branch("GENpsi2s_eta", &GENpsi2s_eta);
    _mytree->Branch("GENpsi2s_mass", &GENpsi2s_mass);
    _mytree->Branch("GENpsi2s_mu_eta", &GENpsi2s_mu_eta);
    _mytree->Branch("GENpsi2s_mu_pt", &GENpsi2s_mu_pt);

    #ifdef deve
    _mytree->Branch("GENjpsi_ins", &GENjpsi_ins, "GENjpsi_ins/I");
    _mytree->Branch("GENjpsi_id", &GENjpsi_id);
    _mytree->Branch("GENjpsi_status", &GENjpsi_status);
    _mytree->Branch("GENjpsi_nMom", &GENjpsi_nMom);
    _mytree->Branch("GENjpsi_nDau", &GENjpsi_nDau);
    _mytree->Branch("GENjpsi_anc", &GENjpsi_anc);
    _mytree->Branch("GENjpsi_sib", &GENjpsi_sib);
    
    _mytree->Branch("GENpsi2s_ins", &GENpsi2s_ins, "GENpsi2s_ins/I");
    _mytree->Branch("GENpsi2s_id", &GENpsi2s_id);
    _mytree->Branch("GENpsi2s_status", &GENpsi2s_status);
    _mytree->Branch("GENpsi2s_momId", &GENpsi2s_momId);
    _mytree->Branch("GENpsi2s_nDau", &GENpsi2s_nDau);
    _mytree->Branch("GENpsi2s_anc", &GENpsi2s_anc);
    _mytree->Branch("GENpsi2s_sib", &GENpsi2s_sib);

    _mytree->Branch("GENmu_ins", &GENmu_ins, "GENmu_ins/I");
    _mytree->Branch("GENmu_id", &GENmu_id);
    _mytree->Branch("GENmu_status", &GENmu_status);
    _mytree->Branch("GENmu_pt", &GENmu_pt);
    _mytree->Branch("GENmu_eta", &GENmu_eta);
    #endif
    // Phi
    _mytree->Branch("GEN_phi_ins", &GEN_phi_ins);
    _mytree->Branch("GEN_phi_status", &GEN_phi_status);
    _mytree->Branch("GEN_phi_momId", &GEN_phi_momId);
    _mytree->Branch("GEN_phi_nDau", &GEN_phi_nDau);
    _mytree->Branch("GEN_phi_anc", &GEN_phi_anc);
    _mytree->Branch("GEN_phi_sib", &GEN_phi_sib);
    _mytree->Branch("GEN_phi_id", &GEN_phi_id);
    _mytree->Branch("GEN_phi_pt", &GEN_phi_pt);
    _mytree->Branch("GEN_phi_eta", &GEN_phi_eta);
    _mytree->Branch("GEN_phi_mass", &GEN_phi_mass);
    _mytree->Branch("GEN_phi_K_eta", &GEN_phi_K_eta);
    _mytree->Branch("GEN_phi_K_pt", &GEN_phi_K_pt);
    // Kaon
    _mytree->Branch("GEN_K_ins", &GEN_K_ins);
    _mytree->Branch("GEN_K_status", &GEN_K_status);
    _mytree->Branch("GEN_K_id", &GEN_K_id);
    _mytree->Branch("GEN_K_pt", &GEN_K_pt);
    _mytree->Branch("GEN_K_eta", &GEN_K_eta);
    _mytree->Branch("GEN_K_momId", &GEN_K_momId);
    _mytree->Branch("GEN_K_nDau", &GEN_K_nDau);
    _mytree->Branch("GEN_K_anc", &GEN_K_anc);
    _mytree->Branch("GEN_K_sib", &GEN_K_sib);
    // -------------------------
    // Event variables
    _mytree->Branch("_nEvent", &_nEvent, "_nEvent/I");
    _mytree->Branch("_nRun", &_nRun, "_nRun/I");
    _mytree->Branch("_nLumi", &_nLumi, "_nLumi/I");
    return;
}
// ------------ method called once each job just after ending the event loop  ------------
void GenAnalyzer::endJob() {return;}
// ------------ method fills 'descriptions' with the allowed parameters for the module  ------------
void GenAnalyzer::fillDescriptions(edm::ConfigurationDescriptions& descriptions) {
    edm::ParameterSetDescription desc;
    desc.setUnknown();
    descriptions.addDefault(desc);
}

void GenAnalyzer::FillEvent(const edm::Event& iEvent, const edm::EventSetup& iSetup) {
    _nEvent = iEvent.id().event();
    _nRun   = iEvent.id().run();
    _nLumi  = iEvent.luminosityBlock();
}

void GenAnalyzer::FillTruth(const edm::Event& iEvent, const edm::EventSetup& iSetup) {
    LogDebug("")<<"GenAnalyzer::FillTruth"; 

    edm::Handle<vector<reco::GenParticle> > genCandidatesCollection;
    iEvent.getByToken(genParticleToken_, genCandidatesCollection);

    // ----------------------------
    //      Loop on particles
    // ----------------------------
    int barcode=0;
    #ifdef deve
    GENmu_ins = 0;
    GENjpsi_ins = 0;
    GENpsi2s_ins = 0;
    #endif
    vector<const reco::Candidate *> _temp_GENjpsi;
    vector<const reco::Candidate *> _temp_GENpsi2s;
    vector<const reco::Candidate *> _temp_GENphi;

    for(auto p = genCandidatesCollection->begin(); p != genCandidatesCollection->end(); p++) {
        barcode++;

        if(fabs(p->pdgId()) == 13) {//if the particle is muon
            if(p->status() != 1) continue;
            #ifdef deve
            GENmu_ins++;
            GENmu_id.push_back(p->pdgId());
            GENmu_status.push_back(p->status());
            GENmu_pt.push_back(p->pt());
            GENmu_eta.push_back(p->eta());
            #endif
            continue;
        }

        // Kaon
        if(fabs(p->pdgId()) == 321) {//if the particle is kaon
            if(p->status() != 1) continue;
            #ifdef deve
            GEN_K_ins++;
            GEN_K_id.push_back(p->pdgId());
            GEN_K_status.push_back(p->status());
            GEN_K_pt.push_back(p->pt());
            GEN_K_eta.push_back(p->eta());
            if(p->mother(0)) GEN_K_momId.push_back(p->mother(0)->pdgId());
            GEN_K_nDau.push_back(p->numberOfDaughters());
            vector<int> _temp_GEN_K_anc;
            vector<int> _temp_GEN_K_sib;
            const reco::Candidate *kaon_anc = p->mother(0);
            const reco::Candidate *kaon_cur = &*p;
            while(kaon_anc->mother(0)) {
                for(unsigned int i = 0; i < kaon_anc->numberOfDaughters(); i++) {
                    if(kaon_anc->daughter(i)->pdgId() == kaon_cur->pdgId()) continue;
                    _temp_GEN_K_sib.push_back(kaon_anc->daughter(i)->pdgId());
                }
                _temp_GEN_K_anc.push_back(kaon_anc->pdgId());
                _temp_GEN_K_sib.push_back(0);
                kaon_cur = kaon_anc;
                kaon_anc = kaon_anc->mother(0);
            }
            _temp_GEN_K_anc.push_back(0);
            _temp_GEN_K_sib.push_back(0);
            GEN_K_anc.push_back(_temp_GEN_K_anc);
            GEN_K_sib.push_back(_temp_GEN_K_sib);
            #endif
            continue;
        }

        vector<double> _temp_GENmu_pt;
        vector<double> _temp_GENmu_eta;
        // kaons from decay of phi meson
        vector<double> _temp_GENphi_K_eta;
        vector<double> _temp_GENphi_K_pt;

        if(p->pdgId() == 443) {// if the particle is J/psi
            if(p->status() != 2) continue;
            #ifdef deve
            puts("Found J/psi");
            #endif
            int nDau = p->numberOfDaughters();
            for(int i = 0; i < nDau; i++) {
                const reco::Candidate *jpsi_dau = p->daughter(i);
                while(jpsi_dau->status() != 1 && jpsi_dau->daughter(0)) jpsi_dau = jpsi_dau->daughter(0);
                if(fabs(jpsi_dau->pdgId()) == 13 && jpsi_dau->status() == 1) {//if daughter of J/psi is muon
                    _temp_GENmu_pt.push_back(jpsi_dau->pt());
                    _temp_GENmu_eta.push_back(jpsi_dau->eta());
                }
                #ifdef deve
                else {// check why muons are not recorded
                    _temp_GENmu_pt.push_back((double)jpsi_dau->pdgId());
                    _temp_GENmu_eta.push_back((double)jpsi_dau->status());
                }
                #endif
            }

            GENjpsi_pt.push_back(p->pt());
            GENjpsi_eta.push_back(p->eta());
            GENjpsi_mass.push_back(p->mass());
            // GENjpsi_mu_eta.push_back(_temp_GENmu_eta);
            // GENjpsi_mu_pt.push_back(_temp_GENmu_pt);
            GENjpsi_mu_eta.insert(GENjpsi_mu_eta.end(), _temp_GENmu_eta.begin(), _temp_GENmu_eta.end());
            GENjpsi_mu_pt.insert(GENjpsi_mu_pt.end(), _temp_GENmu_pt.begin(), _temp_GENmu_pt.end());
            //
            const reco::Candidate *jpsi_self = &*p;
            while(jpsi_self->mother(0)->pdgId() == 443) jpsi_self = jpsi_self->mother(0);
            _temp_GENjpsi.push_back(jpsi_self);
            //

            #ifdef deve
            GENjpsi_ins++;
            GENjpsi_id.push_back(p->pdgId());
            GENjpsi_status.push_back(p->status());
            GENjpsi_nMom.push_back(p->numberOfMothers());
            GENjpsi_nDau.push_back(nDau);
            // find J/psi ancestors and siblings recursively
            vector<int> _temp_GENjpsi_anc;
            vector<int> _temp_GENjpsi_sib;
            const reco::Candidate *jpsi_anc = p->mother(0);
            const reco::Candidate *jpsi_cur = &*p;
            while(jpsi_anc->mother(0)) {
                for(unsigned int i = 0; i < jpsi_anc->numberOfDaughters(); i++) {
                    if(jpsi_anc->daughter(i)->pdgId() == jpsi_cur->pdgId()) continue;
                    _temp_GENjpsi_sib.push_back(jpsi_anc->daughter(i)->pdgId());
                }
                _temp_GENjpsi_anc.push_back(jpsi_anc->pdgId());
                _temp_GENjpsi_sib.push_back(0);
                jpsi_cur = jpsi_anc;
                jpsi_anc = jpsi_anc->mother(0);
            }
            _temp_GENjpsi_anc.push_back(0);
            _temp_GENjpsi_sib.push_back(0);
            GENjpsi_anc.push_back(_temp_GENjpsi_anc);
            GENjpsi_sib.push_back(_temp_GENjpsi_sib);
            //
            #endif
        }
        if(p->pdgId() == 100443) {// if the particle is psi(2S)
            if(p->status() != 2) continue;
            int nDau = p->numberOfDaughters();
            for(int i = 0; i < nDau; i++) {
                const reco::Candidate *psi2s_dau = p->daughter(i);
                while(psi2s_dau->status() != 1 && psi2s_dau->daughter(0)) psi2s_dau = psi2s_dau->daughter(0);
                if(fabs(psi2s_dau->pdgId()) == 13 && psi2s_dau->status() == 1) {//if daughter of J/psi is muon
                    _temp_GENmu_pt.push_back(psi2s_dau->pt());
                    _temp_GENmu_eta.push_back(psi2s_dau->eta());
                }
            }

            GENpsi2s_pt.push_back(p->pt());
            GENpsi2s_eta.push_back(p->eta());
            GENpsi2s_mass.push_back(p->mass());
            GENpsi2s_mu_eta.push_back(_temp_GENmu_eta);
            GENpsi2s_mu_pt.push_back(_temp_GENmu_pt);
            //
            const reco::Candidate *psi2s_self = &*p;
            while(psi2s_self->mother(0)->pdgId() == 100443) psi2s_self = psi2s_self->mother(0);
            _temp_GENpsi2s.push_back(psi2s_self);
            //

            #ifdef deve
            GENpsi2s_ins++;
            GENpsi2s_id.push_back(p->pdgId());
            GENpsi2s_status.push_back(p->status());
            if(p->mother(0)) GENpsi2s_momId.push_back(p->mother(0)->pdgId());
            GENpsi2s_nDau.push_back(nDau);
            // find psi(2S) ancestors and siblings recursively
            vector<int> _temp_GENpsi2s_anc;
            vector<int> _temp_GENpsi2s_sib;
            const reco::Candidate *psi2s_anc = p->mother(0);
            const reco::Candidate *psi2s_cur = &*p;
            while(psi2s_anc->mother(0)) {
                for(unsigned int i = 0; i < psi2s_anc->numberOfDaughters(); i++) {
                    if(psi2s_anc->daughter(i)->pdgId() == psi2s_cur->pdgId()) continue;
                    _temp_GENpsi2s_sib.push_back(psi2s_anc->daughter(i)->pdgId());
                }
                _temp_GENpsi2s_anc.push_back(psi2s_anc->pdgId());
                _temp_GENpsi2s_sib.push_back(0);
                psi2s_cur = psi2s_anc;
                psi2s_anc = psi2s_anc->mother(0);
            }
            _temp_GENpsi2s_anc.push_back(0);
            _temp_GENpsi2s_sib.push_back(0);
            GENpsi2s_anc.push_back(_temp_GENpsi2s_anc);
            GENpsi2s_sib.push_back(_temp_GENpsi2s_sib);
            //
            #endif
        }
        // Look for phi meson (PDG ID = 333)
        if(p->pdgId() == 333) {
            if(p->status() != 2) continue;
            #ifdef deve
            puts("Found phi");
            #endif
            int nDau = p->numberOfDaughters();
            for(int i = 0; i < nDau; i++) {
                const reco::Candidate *phi_dau = p->daughter(i);
                while(phi_dau->status() != 1 && phi_dau->daughter(0)) phi_dau = phi_dau->daughter(0);
                if(fabs(phi_dau->pdgId()) == 321 && phi_dau->status() == 1) {//if daughter of phi is kaon
                    _temp_GENphi_K_eta.push_back(phi_dau->eta());
                    _temp_GENphi_K_pt.push_back(phi_dau->pt());
                }
                #ifdef deve
                else {// check why kaons are not recorded
                    _temp_GENphi_K_eta.push_back((double)phi_dau->pdgId());
                    _temp_GENphi_K_pt.push_back((double)phi_dau->status());
                }
                #endif
            }
            GEN_phi_ins++;
            GEN_phi_status.push_back(p->status());
            GEN_phi_K_eta.insert(GEN_phi_K_eta.end(), _temp_GENphi_K_eta.begin(), _temp_GENphi_K_eta.end());
            GEN_phi_K_pt.insert(GEN_phi_K_pt.end(), _temp_GENphi_K_pt.begin(), _temp_GENphi_K_pt.end());
            // GEN_phi_K_eta.push_back(_temp_GENphi_K_eta);
            // GEN_phi_K_pt.push_back(_temp_GENphi_K_pt);
            #ifdef deve
            puts(">> _temp_GENphi_K content:");
            for(unsigned int i = 0; i < _temp_GENphi_K_eta.size(); i++) {
                printf(">> %d: eta = %f, pt = %f\n", i, _temp_GENphi_K_eta[i], _temp_GENphi_K_pt[i]);
            }
            puts("<< _temp_GENphi_K content");
            #endif

            if(p->mother(0)) GEN_phi_momId.push_back(p->mother(0)->pdgId());
            GEN_phi_nDau.push_back(nDau);
            vector<int> _temp_GENphi_anc;
            vector<int> _temp_GENphi_sib;
            const reco::Candidate *phi_anc = p->mother(0);
            const reco::Candidate *phi_cur = &*p;
            while(phi_anc->mother(0)) {
                for(unsigned int i = 0; i < phi_anc->numberOfDaughters(); i++) {
                    if(phi_anc->daughter(i)->pdgId() == phi_cur->pdgId()) continue;
                    _temp_GENphi_sib.push_back(phi_anc->daughter(i)->pdgId());
                }
                _temp_GENphi_anc.push_back(phi_anc->pdgId());
                _temp_GENphi_sib.push_back(0);
                phi_cur = phi_anc;
                phi_anc = phi_anc->mother(0);
            }
            _temp_GENphi_anc.push_back(0);
            _temp_GENphi_sib.push_back(0);
            GEN_phi_anc.push_back(_temp_GENphi_anc);
            GEN_phi_sib.push_back(_temp_GENphi_sib);

            GEN_phi_id.push_back(p->pdgId());
            GEN_phi_pt.push_back(p->pt());
            GEN_phi_eta.push_back(p->eta());
            GEN_phi_mass.push_back(p->mass());
        }
    }

    // for(unsigned int i = 0; i < _temp_GENjpsi.size(); i++) {
    //     for(unsigned int j = 0; j < _temp_GENpsi2s.size(); j++) {
    //         if(_temp_GENjpsi[i]->mother(0) != _temp_GENpsi2s[j]->mother(0)) continue;
    //         GEN_pair_id.push_back(pair<int, int>(i, j));
    //     }
    // }
    // Look for associated J/psi and Phi meson pairs.
    for(unsigned int i = 0; i < _temp_GENjpsi.size(); i++) {
        for(unsigned int j = 0; j < _temp_GENphi.size(); j++) {
            if(_temp_GENjpsi[i]->mother(0) != _temp_GENphi[j]->mother(0)) continue;
            GEN_pair_id.push_back(pair<int, int>(i, j));
        }
    }
}
//define this as a plug-in
DEFINE_FWK_MODULE(GenAnalyzer);
