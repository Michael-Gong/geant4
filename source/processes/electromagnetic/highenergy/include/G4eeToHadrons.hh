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
// $Id: G4eeToHadrons.hh,v 1.6 2006/06/29 19:32:24 gunter Exp $
// GEANT4 tag $Name: geant4-08-02 $
//
// -------------------------------------------------------------------
//
// GEANT4 Class header file
//
//
// File name:     G4eeToHadrons
//
// Author:        Vladimir Ivanchenko
//
// Creation date: 12.08.2004
//
// Modifications:
// 08-11-04 Migration to new interface of Store/Retrieve tables (V.Ivantchenko)
// 08-04-05 Major optimisation of internal interfaces (V.Ivantchenko)
//
//
// Class Description:
//
// This class manages the process of e+ annihilation into hadrons
//

// -------------------------------------------------------------------
//

#ifndef G4eeToHadrons_h
#define G4eeToHadrons_h 1

#include "G4VEmProcess.hh"
#include "G4Positron.hh"
#include "G4eeToHadronsMultiModel.hh"

class G4eeToHadrons : public G4VEmProcess
{

public:

  G4eeToHadrons(const G4String& name = "ee2hadr");

  virtual ~G4eeToHadrons();

  virtual G4bool IsApplicable(const G4ParticleDefinition& p);

  // Print out of the class parameters
  void PrintInfo();

  // Set the factor to artificially increase the crossSection (default 1)
  void SetCrossSecFactor(G4double fac);

protected:

  void InitialiseProcess(const G4ParticleDefinition*);

  std::vector<G4DynamicParticle*>* SecondariesPostStep(
                                   G4VEmModel*,
                             const G4MaterialCutsCouple*,
                             const G4DynamicParticle*);

private:

  std::vector<G4DynamicParticle*>* GenerateSecondaries(const G4DynamicParticle*);

  // hide assignment operator
  G4eeToHadrons & operator=(const G4eeToHadrons &right);
  G4eeToHadrons(const G4eeToHadrons&);

  G4eeToHadronsMultiModel*  multimodel;
  G4double                  csFactor;
  G4bool                    isInitialised;
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

inline G4bool G4eeToHadrons::IsApplicable(const G4ParticleDefinition& p)
{
  return (&p == G4Positron::Positron());
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

inline std::vector<G4DynamicParticle*>* G4eeToHadrons::SecondariesPostStep(
                                                  G4VEmModel*,
                                            const G4MaterialCutsCouple* couple,
                                            const G4DynamicParticle* dp)
{
  std::vector<G4DynamicParticle*>* newp = multimodel->SampleSecondaries(couple, dp);
  if(newp) fParticleChange.ProposeTrackStatus(fStopAndKill);
  return newp;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif
