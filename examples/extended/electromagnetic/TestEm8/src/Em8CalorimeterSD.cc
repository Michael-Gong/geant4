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
//
// $Id: Em8CalorimeterSD.cc,v 1.7 2006/06/29 17:00:05 gunter Exp $
// GEANT4 tag $Name: geant4-08-02 $
//
// 

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

#include "Em8CalorimeterSD.hh"

#include "Em8CalorHit.hh"
#include "Em8DetectorConstruction.hh"

#include "G4VPhysicalVolume.hh"
#include "G4Step.hh"
#include "G4VTouchable.hh"
#include "G4TouchableHistory.hh"
#include "G4SDManager.hh"
  
#include "G4ios.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

Em8CalorimeterSD::Em8CalorimeterSD(G4String name,
                                   Em8DetectorConstruction* det)
:G4VSensitiveDetector(name),Detector(det)
{
  collectionName.insert("CalCollection");
  HitID = new G4int[500];
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

Em8CalorimeterSD::~Em8CalorimeterSD()
{
  delete [] HitID;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

void Em8CalorimeterSD::Initialize(G4HCofThisEvent*)
{
  CalCollection = new Em8CalorHitsCollection
                      (SensitiveDetectorName,collectionName[0]); 

  for (G4int j=0;j<1; j++) 
  {
    HitID[j] = -1;
  }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

G4bool Em8CalorimeterSD::ProcessHits(G4Step* aStep,G4TouchableHistory*)
{
  G4double edep = aStep->GetTotalEnergyDeposit();
   
  G4double stepl = 0.;

  if (aStep->GetTrack()->GetDefinition()->GetPDGCharge() != 0.)
  {
    stepl = aStep->GetStepLength();
  }    
  if ( ( edep == 0. ) && ( stepl == 0. ) ) return false;      

  G4TouchableHistory* theTouchable
    = (G4TouchableHistory*)(aStep->GetPreStepPoint()->GetTouchable());
    
  G4VPhysicalVolume* physVol = theTouchable->GetVolume(); 
  
  G4int Em8Number = 0 ;

  if ( HitID[Em8Number] == -1 )
  { 
    Em8CalorHit* calHit = new Em8CalorHit();

    if ( physVol == Detector->GetAbsorber()) // &&
      //  aStep->GetTrack()->GetTrackID() == 1) 
    {
      calHit->AddAbs(edep,stepl);
    }
    HitID[Em8Number] = CalCollection->insert(calHit) - 1;

    if ( verboseLevel > 0 )
    {
      G4cout << " New Calorimeter Hit on Em8: " << Em8Number << G4endl;
    }
  }
  else
  { 
    if (physVol == Detector->GetAbsorber()) // &&
      //   aStep->GetTrack()->GetTrackID() == 1 )
    {
       (*CalCollection)[HitID[Em8Number]]->AddAbs(edep,stepl);
    }
    if ( verboseLevel > 0 )
    {
      G4cout << " Energy added to Em8: " << Em8Number << G4endl;
    } 
  }    
  return true;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

void Em8CalorimeterSD::EndOfEvent(G4HCofThisEvent* HCE)
{
  static G4int HCID = -1;
  if(HCID<0)
  { HCID = G4SDManager::GetSDMpointer()->GetCollectionID(collectionName[0]); }
  HCE->AddHitsCollection(HCID,CalCollection);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

void Em8CalorimeterSD::clear()
{} 

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....


void Em8CalorimeterSD::PrintAll()
{} 

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

