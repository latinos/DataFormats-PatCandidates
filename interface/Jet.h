//
// $Id: Jet.h,v 1.30 2008/12/10 12:44:38 lowette Exp $
//

#ifndef DataFormats_PatCandidates_Jet_h
#define DataFormats_PatCandidates_Jet_h

/**
  \class    pat::Jet Jet.h "DataFormats/PatCandidates/interface/Jet.h"
  \brief    Analysis-level calorimeter jet class

   Jet implements the analysis-level calorimeter jet class within the
   'pat' namespace

  \author   Steven Lowette, Giovanni Petrucciani, Roger Wolf, Christian Autermann
  \version  $Id: Jet.h,v 1.30 2008/12/10 12:44:38 lowette Exp $
*/


#include "DataFormats/JetReco/interface/CaloJet.h"
#include "DataFormats/JetReco/interface/BasicJet.h"
#include "DataFormats/JetReco/interface/PFJet.h"
#include "DataFormats/CaloTowers/interface/CaloTower.h"
#include "DataFormats/JetReco/interface/GenJet.h"
#include "DataFormats/Candidate/interface/Particle.h"
#include "DataFormats/TrackReco/interface/Track.h"
#include "DataFormats/TrackReco/interface/TrackFwd.h"
#include "DataFormats/BTauReco/interface/JetTag.h"
#include "DataFormats/PatCandidates/interface/PATObject.h"
#include "DataFormats/BTauReco/interface/TrackIPTagInfo.h"
#include "DataFormats/BTauReco/interface/TrackProbabilityTagInfo.h"
#include "DataFormats/BTauReco/interface/TrackCountingTagInfo.h"
#include "DataFormats/BTauReco/interface/SoftLeptonTagInfo.h"

#include "DataFormats/BTauReco/interface/SecondaryVertexTagInfo.h"
#include "DataFormats/PatCandidates/interface/JetCorrFactors.h"

#include "DataFormats/Common/interface/Ptr.h"
#include "DataFormats/Common/interface/OwnVector.h"


// Define typedefs for convenience
namespace pat {
  class Jet;
  typedef std::vector<Jet>              JetCollection; 
  typedef edm::Ref<JetCollection>       JetRef; 
  typedef edm::RefVector<JetCollection> JetRefVector; 
}


// Class definition
namespace pat {

  typedef reco::CaloJet::Specific CaloSpecific;
  typedef reco::PFJet::Specific PFSpecific;

  class Jet : public PATObject<reco::Jet> {

    public:

      /// default constructor
      Jet();
      /// constructor from a reco::Jet
      Jet(const reco::Jet & aJet);
      /// constructor from ref to reco::Jet
      Jet(const edm::RefToBase<reco::Jet> & aJetRef);
      /// constructor from ref to reco::Jet
      Jet(const edm::Ptr<reco::Jet> & aJetRef);
      /// destructor
      virtual ~Jet();

      /// required reimplementation of the Candidate's clone method
      virtual Jet * clone() const { return new Jet(*this); }

      // ---- methods for MC matching ----
      /// return the matched generated parton
      const reco::GenParticle * genParton() const { return genParticle(); }
      /// return the matched generated jet
      const reco::GenJet * genJet() const;
      /// return the flavour of the parton underlying the jet
      int partonFlavour() const;

      // ---- methods for jet corrections ----
      // Return true if this jet carries jet energy correction information
      bool  hasCorrFactors() const { return !jetEnergyCorrections_.empty(); }
      /// Return true if this jet carries the jet correction factors of a different set, for systematic studies
      bool  hasCorrFactorSet(const std::string &set) const;
      /// Return the label-name of the current set of jet energy corrections
      std::string corrFactorSetLabel() const { return corrFactors_()->getLabel(); }
      /// Return the all available label-names of all sets of jet energy corrections
      const std::vector<std::string> corrFactorSetLabels() const;
      /// Return the name of the current level of jet energy corrections
      std::string corrStep() const;
      /// Return flavour of the current level of jet energy corrections
      std::string corrFlavour() const;
      /// Total correction factor to target step, starting from jetCorrStep(),
      /// for the currently used set of correction factors
      float corrFactor(const std::string &step, const std::string &flavour="") const;
      /// Total correction factor to target step, starting from jetCorrStep(),
      /// for any available set of correction factors
      float corrFactor(const std::string &step, const std::string &flavour, const std::string &set) const;
      /// Copy of this jet with correction factor to target step
      /// for the currently used set of correction factors
      Jet correctedJet(const std::string &step, const std::string &flavour="") const ;
      /// Copy of this jet with correction factor to target step
      /// for any available set of correction factors
      Jet correctedJet(const std::string &step, const std::string &flavour, const std::string &set) const ;
      /// method to set the energy scale correction factors
      /// this will change the jet's momentum! To be used (only) by PATJetProducer.
      /// The default JetCorrFactor-Set is always the first element in 'jetEnergyCorrections_'
      void setCorrStep(JetCorrFactors::CorrStep step);
      /// to be used by PATJetProducer: method to set the energy scale correction factors
      void setCorrFactors(const JetCorrFactors & jetCorrF);
      /// to be used by PATJetProducer: method to add more sets of energy scale correction factors
      void addCorrFactors(const JetCorrFactors & jetCorrF);

      // ---- methods for accessing b-tagging info ----
      /// get b discriminant from label name
      float bDiscriminator(const std::string &theLabel) const;
      /// get vector of paire labelname-disciValue
      const std::vector<std::pair<std::string, float> > & getPairDiscri() const;
      /// Get a tagInfo with the given name, or NULL if none is found. 
      /// You should omit the 'TagInfos' part from the label
      const reco::BaseTagInfo            * tagInfo(const std::string &label) const;
      /// Get a tagInfo with the given name and type or NULL if none is found. 
      /// If the label is empty or not specified, it returns the first tagInfo of that type (if any one exists)
      /// You should omit the 'TagInfos' part from the label
      const reco::TrackIPTagInfo         * tagInfoTrackIP(const std::string &label="") const;
      /// Get a tagInfo with the given name and type or NULL if none is found. 
      /// If the label is empty or not specified, it returns the first tagInfo of that type (if any one exists)
      /// You should omit the 'TagInfos' part from the label
      const reco::SoftLeptonTagInfo      * tagInfoSoftLepton(const std::string &label="") const;
      /// Get a tagInfo with the given name and type or NULL if none is found. 
      /// If the label is empty or not specified, it returns the first tagInfo of that type (if any one exists)
      /// You should omit the 'TagInfos' part from the label
      const reco::SecondaryVertexTagInfo * tagInfoSecondaryVertex(const std::string &label="") const;
      /// method to add a algolabel-discriminator pair
      void addBDiscriminatorPair(const std::pair<std::string, float> & thePair);
      /// Sets a tagInfo with the given name from an edm::Ptr<T> to it. 
      /// If the label ends with 'TagInfos', the 'TagInfos' is stripped out.
      void  addTagInfo(const std::string &label, 
                       const edm::Ptr<reco::BaseTagInfo> &info) ;

      // ---- track related methods ----
      /// method to return the JetCharge computed when creating the Jet
      float jetCharge() const;
      /// method to return a vector of refs to the tracks associated to this jet
      const reco::TrackRefVector & associatedTracks() const;
      /// method to set the jet charge
      void setJetCharge(float jetCharge);
      /// method to set the vector of refs to the tracks associated to this jet
      void setAssociatedTracks(const reco::TrackRefVector &tracks);

      // ---- methods for content embedding ----
      /// method to store the CaloJet constituents internally
      void setCaloTowers(const std::vector<CaloTowerPtr> & caloTowers);
      /// method to set the matched parton
      void setGenParton(const reco::GenParticleRef & gp, bool embed=false) { setGenParticleRef(gp, embed); }
      /// method to set the matched generated jet
      void setGenJet(const reco::GenJet & gj);
      /// method to set the flavour of the parton underlying the jet
      void setPartonFlavour(int partonFl);
//      /// method to set the energy scale correction factors
//      void setJetCorrFactors(const JetCorrFactors & jetCorrF);
//      /// method to set correction factor to go back to an uncorrected jet
//      void setNoCorrFactor(float noCorrF);

      // ---- jet specific methods ----
      /// check to see if the jet is a reco::CaloJet
      bool isCaloJet()  const { return !specificCalo_.empty(); }
      /// check to see if the jet is a reco::PFJet
      bool isPFJet()    const { return !specificPF_.empty(); }
      /// check to see if the jet is no more than a reco::BasicJet
      bool isBasicJet() const { return !(isCaloJet() || isPFJet()); }
      /// retrieve the calo specific part of the jet
      const CaloSpecific & caloSpecific() const { 
          if (specificCalo_.empty()) throw cms::Exception("Type Mismatch") << "This PAT jet was not made from a CaloJet.\n";
          return specificCalo_[0];
      }
      /// retrieve the pf specific part of the jet
      const PFSpecific & pfSpecific() const { 
          if (specificPF_.empty()) throw cms::Exception("Type Mismatch") << "This PAT jet was not made from a PFJet.\n";
          return specificPF_[0];
      }

      // ---- Calo Jet specific information ----
      /// Returns the maximum energy deposited in ECAL towers
      float maxEInEmTowers() const {return caloSpecific().mMaxEInEmTowers;}
      /// Returns the maximum energy deposited in HCAL towers
      float maxEInHadTowers() const {return caloSpecific().mMaxEInHadTowers;}
      /// Returns the jet hadronic energy fraction
      float energyFractionHadronic () const {return caloSpecific().mEnergyFractionHadronic;}
      /// Returns the jet electromagnetic energy fraction
      float emEnergyFraction() const {return caloSpecific().mEnergyFractionEm;}
      /// Returns the jet hadronic energy in HB
      float hadEnergyInHB() const {return caloSpecific().mHadEnergyInHB;}
      /// Returns the jet hadronic energy in HO
      float hadEnergyInHO() const {return caloSpecific().mHadEnergyInHO;}
      /// Returns the jet hadronic energy in HE
      float hadEnergyInHE() const {return caloSpecific().mHadEnergyInHE;}
      /// Returns the jet hadronic energy in HF
      float hadEnergyInHF() const {return caloSpecific().mHadEnergyInHF;}
      /// Returns the jet electromagnetic energy in EB
      float emEnergyInEB() const {return caloSpecific().mEmEnergyInEB;}
      /// Returns the jet electromagnetic energy in EE
      float emEnergyInEE() const {return caloSpecific().mEmEnergyInEE;}
      /// Returns the jet electromagnetic energy extracted from HF
      float emEnergyInHF() const {return caloSpecific().mEmEnergyInHF;}
      /// Returns area of contributing towers
      float towersArea() const {return caloSpecific().mTowersArea;}
      /// Returns the number of constituents carrying a 90% of the total Jet energy*/
      int n90() const {return nCarrying (0.9);}
      /// Returns the number of constituents carrying a 60% of the total Jet energy*/
      int n60() const {return nCarrying (0.6);}

      /// convert generic constituent to specific type
      //      static CaloTowerPtr caloTower (const reco::Candidate* fConstituent);
      /// Get specific constituent of the CaloJet. 
      /// If the caloTowers were embedded, this reference is transient only and must not be persisted
      CaloTowerPtr getCaloConstituent (unsigned fIndex) const;
      /// Get the constituents of the CaloJet. 
      /// If the caloTowers were embedded, these reference are transient only and must not be persisted
      std::vector<CaloTowerPtr> getCaloConstituents () const;

      // ---- PF Jet specific information ----
      /// chargedHadronEnergy
      float chargedHadronEnergy () const {return pfSpecific().mChargedHadronEnergy;}
      ///  chargedHadronEnergyFraction
      float  chargedHadronEnergyFraction () const {return chargedHadronEnergy () / energy ();}
      /// neutralHadronEnergy
      float neutralHadronEnergy () const {return pfSpecific().mNeutralHadronEnergy;}
      /// neutralHadronEnergyFraction
      float neutralHadronEnergyFraction () const {return neutralHadronEnergy () / energy ();}
      /// chargedEmEnergy
      float chargedEmEnergy () const {return pfSpecific().mChargedEmEnergy;}
      /// chargedEmEnergyFraction
      float chargedEmEnergyFraction () const {return chargedEmEnergy () / energy ();}
      /// chargedMuEnergy
      float chargedMuEnergy () const {return pfSpecific().mChargedMuEnergy;}
      /// chargedMuEnergyFraction
      float chargedMuEnergyFraction () const {return chargedMuEnergy () / energy ();}
      /// neutralEmEnergy
      float neutralEmEnergy () const {return pfSpecific().mNeutralEmEnergy;}
      /// neutralEmEnergyFraction
      float neutralEmEnergyFraction () const {return neutralEmEnergy () / energy ();}
      /// chargedMultiplicity
      float chargedMultiplicity () const {return pfSpecific().mChargedMultiplicity;}
      /// neutralMultiplicity
      float neutralMultiplicity () const {return pfSpecific().mNeutralMultiplicity;}
      /// muonMultiplicity
      float muonMultiplicity () const {return pfSpecific().mMuonMultiplicity;}

      /// convert generic constituent to specific type
      static const reco::PFCandidate* getPFCandidate (const reco::Candidate* fConstituent);
      /// get specific constituent
      const reco::PFCandidate* getPFConstituent (unsigned fIndex) const;
      /// get all constituents
      std::vector <const reco::PFCandidate*> getPFConstituents () const;

      /// Get a pointer to a Candididate constituent of the jet 
      /// Needs to be re-implemented because of CaloTower embedding
      using reco::Jet::daughter; // needed to avoid a warning in ACLiC compilation
      virtual const reco::Candidate * daughter(size_t i) const {
          return (embeddedCaloTowers_ ?  &caloTowers_[i] : reco::Jet::daughter(i));
      }
      /// Get the number of constituents 
      /// Needs to be re-implemented because of CaloTower embedding
      virtual size_t numberOfDaughters() const {
          return (embeddedCaloTowers_ ? caloTowers_.size() : reco::Jet::numberOfDaughters() );
      }
 
    protected:

      // ---- for content embedding ----
      bool embeddedCaloTowers_;
      CaloTowerCollection caloTowers_;
      // ---- MC info ----
      std::vector<reco::GenJet> genJet_;
      int partonFlavour_;
      // ---- energy scale correction factors ----
      pat::JetCorrFactors::CorrStep    jetEnergyCorrectionStep_;
      std::vector<pat::JetCorrFactors> jetEnergyCorrections_;
      // index in 'jetEnergyCorrections_' of the currently applied correction factor set
      unsigned activeJetCorrIndex_;
      /// Return the jet correction factors of a different set, for systematic studies
      const JetCorrFactors * corrFactors_(const std::string &set) const ;
      /// return the correction factor for this jet. Throws if they're not available.
      const JetCorrFactors * corrFactors_() const;

      // ---- b-tag related members ----
      std::vector<std::pair<std::string, float> >           pairDiscriVector_;
      std::vector<std::string>          tagInfoLabels_;
      edm::OwnVector<reco::BaseTagInfo> tagInfos_;  
      // ---- track related members ----
      float jetCharge_;
      reco::TrackRefVector associatedTracks_;
      // ---- specific members ----
      std::vector<CaloSpecific> specificCalo_;
      std::vector<PFSpecific>   specificPF_;

    private:

      // ---- helper functions ----
      void tryImportSpecific(const reco::Jet &source);
      template<typename T> const T * tagInfoByType() const;

  };


}

#endif
