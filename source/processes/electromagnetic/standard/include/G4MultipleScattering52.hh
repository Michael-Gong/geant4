//
// ********************************************************************
// * License and Disclaimer                                           *
// *                                                                  *
// * The  Geant4 software  is  copyright of the Copyright Holders  of *
// * the Geant4 Collaboration.  It is provided  under  the terms  and *
// * conditions of the Geant4 Software License,  included in the file *
// * LICENSE and available at  http://cern.ch/geant4/license .  These *
// * include a list of copyright holders.                             *
// *                                                                  *
// * Neither the authors of this software system, nor their employing *
// * institutes,nor the agencies providing financial support for this *
// * work  make  any representation or  warranty, express or implied, *
// * regarding  this  software system or assume any liability for its *
// * use.  Please see the license in the file  LICENSE  and URL above *
// * for the full disclaimer and the limitation of liability.         *
// *                                                                  *
// * This  code  implementation is the result of  the  scientific and *
// * technical work of the GEANT4 collaboration.                      *
// * By using,  copying,  modifying or  distributing the software (or *
// * any work based  on the software)  you  agree  to acknowledge its *
// * use  in  resulting  scientific  publications,  and indicate your *
// * acceptance of all terms of the Geant4 Software license.          *
// ********************************************************************
//
// $Id: G4MultipleScattering52.hh,v 1.3 2006/06/29 19:50:34 gunter Exp $
// GEANT4 tag $Name: geant4-08-02 $
//
//------------- G4MultipleScattering52 physics process --------------------------
//               by Laszlo Urban, March 2001
//
// 07-08-01 new methods Store/Retrieve PhysicsTable
// 23-08-01 new angle and z distribution,energy dependence reduced,
//          Store,Retrieve methods commented out temporarily, L.Urban
// 11-09-01 G4MultipleScatteringx put as default: G4MultipleScattering
//          Store,Retrieve methods reactived (mma)
// 13-09-01 Unused TrueToGeomTransformation method deleted,
//          class description (L.Urban)
// 19-09-01 come back to previous process name msc
// 17-04-02 NEW angle distribution + boundary algorithm modified, L.Urban
// 22-04-02 boundary algorithm modified -> important improvement in timing !!!!
//          (L.Urban)
// 24-04-02 some minor changes in boundary algorithm, L.Urban
// 24-05-02 changes in data members, L.Urban
// 30-10-02 changes in data members, L.Urban
// 05-02-03 changes in data members, L.Urban
// 18-04-03 Change signature of  GetTransportMeanFreePath (V.Ivanchenko)
// 08-08-03 This class is frozen at the release 5.2 (V.Ivanchenko)
// 08-11-04 Remove Store/Retrieve tables (V.Ivantchenko)

//------------------------------------------------------------------------------

// class description
//
//  The class simulates the multiple scattering for any kind
//  of charged particle.
//
// class description - end

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#ifndef G4MultipleScattering52_h
#define G4MultipleScattering52_h 1
#include "G4VContinuousDiscreteProcess.hh"
#include "G4EnergyLossTables.hh"
#include "G4GPILSelection.hh"
#include "G4PhysicsLogVector.hh"
#include "G4VPhysicalVolume.hh"
#include "G4ParticleChangeForMSC.hh"
#include "G4UnitsTable.hh"
#include "G4MaterialCutsCouple.hh"
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class G4MultipleScattering52 : public G4VContinuousDiscreteProcess

{
 public:    // with description

   G4MultipleScattering52(const G4String& processName="msc");

  ~G4MultipleScattering52();
          
   G4bool IsApplicable ( const G4ParticleDefinition& );
     // returns true for charged particles, false otherwise

   void BuildPhysicsTable(const G4ParticleDefinition& aParticleType);
     // This function overloads the corresponding virtual function
     // of the base class G4VContinuousDiscreteProcess.
     // It is invoked by the G4ParticleWithCuts()::SetCut() method.
     // It prepares the table of the transport mean free paths 
     // for every material.

   void PrintInfoDefinition();
     // Print few lines of informations about the process: validity range,
     // origine ..etc..
     // Invoked by BuildPhysicsTable().

   G4double AlongStepGetPhysicalInteractionLength(const G4Track&,
                                                  G4double  previousStepSize,
                                                  G4double  currentMinimumStep,
                                                  G4double& currentSafety,
                                                  G4GPILSelection* selection);
     // The function overloads the corresponding function of the base
     // class.It limits the step near to boundaries only 
     // and invokes the method GetContinuousStepLimit at every step. 
						  
   G4double GetContinuousStepLimit(const G4Track& aTrack,
                                   G4double previousStepSize,
                                   G4double currentMinimumStep,
                                   G4double& currentSafety); 
     // It performs the true step length --> geometrical step length
     // transformation. It is invoked by the
     // AlongStepGetPhysicalInteractionLength method.  

   G4double GetMeanFreePath(const G4Track& aTrack,
                            G4double previousStepSize,
                            G4ForceCondition* condition);
     // It sets the force condition to true only
     // in order to have the PostStepDoIt called at every step.
     // This function overloads a virtual function of the base class.
     // It is invoked by the ProcessManager of the Particle.


   G4double GetTransportMeanFreePath(
                          G4double KineticEnergy,const G4MaterialCutsCouple* couple);
     // Just a utility method to get the values of the transport
     //  mean free path . (It is not used inside the class.)

   G4VParticleChange* AlongStepDoIt(const G4Track& aTrack,const G4Step& aStep);
     // The geometrical step length --> true path length transformation
     // is performed here (the inverse of the transformation done
     // by GetContinuousStepLimit).  

   G4VParticleChange* PostStepDoIt(const G4Track& aTrack,const G4Step& aStep);
     // It computes the final state of the particle: samples the 
     // scattering angle and computes the lateral displacement.
     // The final state is returned as a ParticleChange object.
     // This function overloads a virtual function of the base class.
     // It is invoked by the ProcessManager of the Particle.

   void Setsamplez(G4bool value)               {samplez = value;};
     // geom. step length distribution should be sampled or not

   void Setdtrl(G4double value)                 {dtrl = value;};
     // to reduce the energy/step dependence

   void SetBoundary(G4bool value)  {boundary = value;};
   void Setfacxsi(G4double value)      {facxsi = value;
                                    G4cout << " facxsi=" << facxsi << G4endl;};
   void SetFacrange(G4double val)  {facrange=val;
                                    nsmallstep = G4int(std::log((cf+facrange-1.)/
                                                 facrange)/std::log(cf))+1 ;
                                    G4cout << " fr=" << facrange 
                                           << "  nsmall=" << nsmallstep << G4endl ;};
     // Steplimit after boundary crossing = facrange*range
     // estimated nb of steps at boundary nsmallstep = 1/facrange


   void SetLateralDisplacementFlag(G4bool flag) {fLatDisplFlag = flag;};
     // lateral displacement to be/not to be computed
   
   void SetNuclCorrPar(G4double val)            {NuclCorrPar = val;};
   void SetFactPar(G4double val)                {FactPar = val;};

 protected:    // with description

   virtual G4double ComputeTransportCrossSection(
                             const G4ParticleDefinition& aParticleType,
                                   G4double KineticEnergy,
                                   G4double AtomicNumber,
                                   G4double AtomicWeight);
     // It computes the transport cross section.
     // The transport mean free path is 1/(transport cross section). 

 private:

 //  hide assignment operator as  private
   G4MultipleScattering52 & operator = (const G4MultipleScattering52 &right);
   G4MultipleScattering52 ( const G4MultipleScattering52 &);

 private:        // data members

   G4PhysicsTable* theTransportMeanFreePathTable;

   G4double taubig,tausmall,taulim;

   G4double LowestKineticEnergy;
   G4double HighestKineticEnergy;
   G4int    TotBin;

   G4int       materialIndex;
  
   G4double tLast;
   G4double zLast;

   // model parameters
   G4bool   boundary;                         // spec. handling near boundaries
   G4double facrange,tlimit,tlimitmin,cf;
   G4int stepno,stepnolastmsc,nsmallstep ;
   G4double laststep ;
   G4GPILSelection  valueGPILSelectionMSC;

   G4double zmean;                            // z(geom.step length)
   G4bool samplez ;                           //  distribution 

   G4double range,T0,T1,lambda0,lambda1,      // used to reduce the energy
            Tlow,alam,blam,dtrl,lambdam,      // (or step length) dependence
            clam,zm,cthm;

   // with/without lateral displacement
   G4bool fLatDisplFlag;

   // nuclear size effect correction
   G4double NuclCorrPar;
   G4double FactPar;

   G4ParticleChangeForMSC fParticleChange; 

   G4double alfa1,alfa2,alfa3,b,xsi,c0,facxsi ;    // angle distr. parameters
                                                 // facxsi : some tuning 
                                                 // possibility in the tail 
};

#include "G4MultipleScattering52.icc"

#endif
 

 
