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
// $Id: BrachyDetectorConstructionIr.hh,v 1.3 2006/06/29 17:31:13 gunter Exp $
// GEANT4 tag $Name: geant4-08-02 $
//
//    ****************************************
//    *                                      *
//    *    BrachyDetectorConstructionIr.hh   *
//    *                                      *
//    ****************************************
//
// Author: Susanna Guatelli
//

#ifndef BrachyDetectorConstructionIr_H
#define BrachyDetectorConstructionIr_H 1

#include "globals.hh"
#include "G4VUserDetectorConstruction.hh"
#include "G4LogicalVolume.hh"


class G4LogicalVolume;
class G4Tubs;
class G4Box;
class G4Sphere;
class G4Tubs;
class G4Colour;
class G4VPhysicalVolume;
class G4VPhysicalVolume;
class BrachyMaterial;

class BrachyDetectorConstructionIr 
{
public:
  BrachyDetectorConstructionIr();
  ~BrachyDetectorConstructionIr();
  void  ConstructIridium(G4VPhysicalVolume*);
  void  CleanIridium();

private:   
  G4Tubs* capsule ;
  G4LogicalVolume*  capsuleLog;    
  G4VPhysicalVolume* capsulePhys;
  G4Sphere* capsuleTip;
  G4LogicalVolume* capsuleTipLog;
  G4VPhysicalVolume* capsuleTipPhys;

  G4Tubs* iridiumCore;
  G4LogicalVolume* iridiumCoreLog;
  G4VPhysicalVolume* iridiumCorePhys;
    
  BrachyMaterial* pMat;    

  G4VisAttributes* simpleCapsuleVisAtt;
  G4VisAttributes*  simpleCapsuleTipVisAtt;
  G4VisAttributes*  simpleIridiumVisAtt;
};
#endif








