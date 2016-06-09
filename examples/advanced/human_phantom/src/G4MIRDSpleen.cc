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
// Authors: S. Guatelli and M. G. Pia, INFN Genova, Italy
// 
// Based on code developed by the undergraduate student G. Guerrieri 
// Note: this is a preliminary beta-version of the code; an improved 
// version will be distributed in the next Geant4 public release, compliant
// with the design in a forthcoming publication, and subject to a 
// design and code review.
//
#include "G4MIRDSpleen.hh"

#include "G4Processor/GDMLProcessor.h"
#include "globals.hh"
#include "G4SDManager.hh"
#include "G4VisAttributes.hh"
#include "G4ThreeVector.hh"
#include "G4VPhysicalVolume.hh"
#include "G4RotationMatrix.hh"
#include "G4Material.hh"
#include "G4LogicalVolume.hh"
#include "G4HumanPhantomMaterial.hh"
#include "G4VPhysicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4Ellipsoid.hh"
G4MIRDSpleen::G4MIRDSpleen()
{
}

G4MIRDSpleen::~G4MIRDSpleen()
{

}

G4VPhysicalVolume* G4MIRDSpleen::ConstructSpleen(G4VPhysicalVolume* mother, G4String sex, G4bool sensitivity)
{

 G4cout << "ConstructSpleen for " << sex << G4endl;
 G4HumanPhantomMaterial* material = new G4HumanPhantomMaterial();
 G4Material* soft = material -> GetMaterial("soft_tissue");
 delete material;

 G4double ax= 2.90 *cm;
 G4double by= 1.88 *cm;
 G4double cz= 5.19 * cm; 

 G4Ellipsoid* spleen = new G4Ellipsoid("spleen", ax, by, cz);


  G4LogicalVolume* logicSpleen = new G4LogicalVolume(spleen, soft,
						     "SpleenVolume",
						     0, 0, 0);
  
  // Define rotation and position here!
  G4VPhysicalVolume* physSpleen = new G4PVPlacement(0,
			       G4ThreeVector(9.49 *cm, 2.94 *cm, 1.8*cm),
      			       "physicalSpleen",
  			       logicSpleen,
			       mother,
			       false,
			       0, true);

  // Sensitive Body Part
  if (sensitivity==true)
  { 
    G4SDManager* SDman = G4SDManager::GetSDMpointer();
    logicSpleen->SetSensitiveDetector( SDman->FindSensitiveDetector("BodyPartSD") );
  }

  // Visualization Attributes
  G4VisAttributes* SpleenVisAtt = new G4VisAttributes(G4Colour(0.41,0.41,0.41));
  SpleenVisAtt->SetForceSolid(true);
  logicSpleen->SetVisAttributes(SpleenVisAtt);

  G4cout << "Spleen created !!!!!!" << G4endl;

  // Testing Spleen Volume
  G4double SpleenVol = logicSpleen->GetSolid()->GetCubicVolume();
  G4cout << "Volume of Spleen = " << SpleenVol/cm3 << " cm^3" << G4endl;
  
  // Testing Spleen Material
  G4String SpleenMat = logicSpleen->GetMaterial()->GetName();
  G4cout << "Material of Spleen = " << SpleenMat << G4endl;
  
  // Testing Density
  G4double SpleenDensity = logicSpleen->GetMaterial()->GetDensity();
  G4cout << "Density of Material = " << SpleenDensity*cm3/g << " g/cm^3" << G4endl;

  // Testing Mass
  G4double SpleenMass = (SpleenVol)*SpleenDensity;
  G4cout << "Mass of Spleen = " << SpleenMass/gram << " g" << G4endl;


  
  return physSpleen;
}
