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
// $Id: PerspectiveVisActionMessenger.cc,v 1.3 2006/11/06 20:07:36 allison Exp $
// GEANT4 tag $Name: geant4-08-02 $

#include "PerspectiveVisActionMessenger.hh"

#include "PerspectiveVisAction.hh"

#include "G4UImanager.hh"
#include "G4UIcommand.hh"
#include "G4UIdirectory.hh"
#include "G4UIcmdWithAString.hh"

PerspectiveVisActionMessenger::PerspectiveVisActionMessenger
(PerspectiveVisAction* PVA):
  fPVA(PVA)
{
  G4bool omitable;

  fpDirectory = new G4UIdirectory ("/perspectiveDemo/");
  fpDirectory -> SetGuidance ("Perspective demonstration commands.");

  fpCommandOS = new G4UIcmdWithAString ("/perspectiveDemo/optionString", this);
  fpCommandOS -> SetGuidance
    ("Option string - any combination of \"x\", \"y\", \"z\", \"a[ll]\".");
  fpCommandOS -> SetParameterName ("option-string", omitable = true);
  fpCommandOS -> SetDefaultValue("all");

  fpCommandScene = new G4UIcmdWithAString ("/perspectiveDemo/scene", this);
  fpCommandScene -> SetGuidance
    ("Scene name.");
  fpCommandScene -> SetParameterName ("scene-name", omitable = true);
  fpCommandScene -> SetDefaultValue("room-and-chair");
  fpCommandScene -> SetCandidates("room-and-chair");
}

PerspectiveVisActionMessenger::~PerspectiveVisActionMessenger ()
{
  delete fpCommandScene;
  delete fpCommandOS;
  delete fpDirectory;
}

void PerspectiveVisActionMessenger::SetNewValue
(G4UIcommand* command, G4String newValue)
{
  if (command == fpCommandOS)
    {
      fPVA->SetOptionString(newValue);
    }

  else if (command == fpCommandScene)
    {
      fPVA->SetScene(newValue);
    }

  G4UImanager::GetUIpointer()->ApplyCommand("/vis/viewer/rebuild");
}
