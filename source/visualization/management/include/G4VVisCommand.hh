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
// $Id: G4VVisCommand.hh,v 1.18 2006/06/29 21:28:18 gunter Exp $
// GEANT4 tag $Name: geant4-08-02 $

// Base class for visualization commands - John Allison  9th August 1998
// It is really a messenger - we have one command per messenger.

#ifndef G4VVISCOMMAND_HH
#define G4VVISCOMMAND_HH

#include "G4VisManager.hh"
#include "G4UImessenger.hh"
#include "G4ThreeVector.hh"
#include <vector>

class G4UIcommand;
class G4UIcmdWithAString;

class G4VVisCommand: public G4UImessenger {
public:
  // Uses compiler defaults for copy constructor and assignment.
  G4VVisCommand ();
  virtual ~G4VVisCommand ();
  static void SetVisManager (G4VisManager*);

protected:

  // Conversion routines augmenting those in G4UIcommand.
  static G4String ConvertToString(G4double x, G4double y,
				  const char * unitName);
  static void  ConvertToDoublePair(const G4String& paramString,
				   G4double& xval,
				   G4double& yval);

  // Other utilities.
  void UpdateVisManagerScene (const G4String& sceneName = "");

  // Data members.
  static G4VisManager* fpVisManager;
};

#include "G4VVisCommand.icc"

#endif
