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
// $Id: DetectorMessenger.cc,v 1.2 2006/06/29 16:40:12 gunter Exp $
// GEANT4 tag $Name: geant4-08-02 $
//
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#include "DetectorMessenger.hh"

#include "DetectorConstruction.hh"
#include "G4UIdirectory.hh"
#include "G4UIcmdWithAString.hh"
#include "G4UIcmdWithAnInteger.hh"
#include "G4UIcmdWithADoubleAndUnit.hh"
#include "G4UIcmdWithoutParameter.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

DetectorMessenger::DetectorMessenger(DetectorConstruction * Det)
:Detector(Det)
{ 
  testemDir = new G4UIdirectory("/testem/");
  testemDir->SetGuidance(" detector control.");
  
  detDir = new G4UIdirectory("/testem/det/");
  detDir->SetGuidance("detector construction commands");
      
  MaterCmd = new G4UIcmdWithAString("/testem/det/setMat",this);
  MaterCmd->SetGuidance("Select material of the box.");
  MaterCmd->SetParameterName("choice",false);
  MaterCmd->AvailableForStates(G4State_PreInit,G4State_Idle);
  
  SizeXCmd = new G4UIcmdWithADoubleAndUnit("/testem/det/setSizeX",this);
  SizeXCmd->SetGuidance("Set sizeX of the absorber");
  SizeXCmd->SetParameterName("SizeX",false);
  SizeXCmd->SetRange("SizeX>0.");
  SizeXCmd->SetUnitCategory("Length");
  SizeXCmd->AvailableForStates(G4State_PreInit,G4State_Idle);
  
  SizeYZCmd = new G4UIcmdWithADoubleAndUnit("/testem/det/setSizeYZ",this);
  SizeYZCmd->SetGuidance("Set sizeYZ of the absorber");
  SizeYZCmd->SetParameterName("SizeYZ",false);
  SizeYZCmd->SetRange("SizeYZ>0.");
  SizeYZCmd->SetUnitCategory("Length");
  SizeYZCmd->AvailableForStates(G4State_PreInit,G4State_Idle);
  
  NbLayersCmd = new G4UIcmdWithAnInteger("/testem/det/setNbOfLayers",this);
  NbLayersCmd->SetGuidance("Set number of layers");
  NbLayersCmd->SetParameterName("NbLayers",false);
  NbLayersCmd->SetRange("NbLayers>0");
  NbLayersCmd->AvailableForStates(G4State_PreInit,G4State_Idle);
          
  MagFieldCmd = new G4UIcmdWithADoubleAndUnit("/testem/det/setField",this);  
  MagFieldCmd->SetGuidance("Define magnetic field.");
  MagFieldCmd->SetGuidance("Magnetic field will be in Z direction.");
  MagFieldCmd->SetParameterName("Bz",false);
  MagFieldCmd->SetUnitCategory("Magnetic flux density");
  MagFieldCmd->AvailableForStates(G4State_PreInit,G4State_Idle);
      
  UpdateCmd = new G4UIcmdWithoutParameter("/testem/det/update",this);
  UpdateCmd->SetGuidance("Update calorimeter geometry.");
  UpdateCmd->SetGuidance("This command MUST be applied before \"beamOn\" ");
  UpdateCmd->SetGuidance("if you changed geometrical value(s).");
  UpdateCmd->AvailableForStates(G4State_Idle);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

DetectorMessenger::~DetectorMessenger()
{
  delete MaterCmd;
  delete SizeXCmd;
  delete SizeYZCmd;
  delete NbLayersCmd;   
  delete MagFieldCmd;
  delete UpdateCmd;
  delete detDir;  
  delete testemDir;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void DetectorMessenger::SetNewValue(G4UIcommand* command,G4String newValue)
{ 
  if( command == MaterCmd )
   { Detector->SetMaterial(newValue);}
   
  if( command == SizeXCmd )
   { Detector->SetSizeX(SizeXCmd->GetNewDoubleValue(newValue));}
   
  if( command == SizeYZCmd )
   { Detector->SetSizeYZ(SizeYZCmd->GetNewDoubleValue(newValue));}
   
  if( command == NbLayersCmd )
   { Detector->SetNbOfLayers(NbLayersCmd->GetNewIntValue(newValue));}
         
  if( command == MagFieldCmd )
   { Detector->SetMagField(MagFieldCmd->GetNewDoubleValue(newValue));}
              
  if( command == UpdateCmd )
   { Detector->UpdateGeometry();}
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
