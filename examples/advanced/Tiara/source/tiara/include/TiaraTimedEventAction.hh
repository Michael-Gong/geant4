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
// $Id: TiaraTimedEventAction.hh,v 1.1.1.2 2006/06/29 15:44:21 gunter Exp $
// GEANT4 tag $Name: geant4-08-02 $
//
// ----------------------------------------------------------------------
// Class TiaraTimedEventAction
//
// Class description:
//
// Event action used for visualization.

// Author: Michael Dressel (Michael.Dressel@cern.ch)
// ----------------------------------------------------------------------


#ifndef TiaraTimedEventAction_h
#define TiaraTimedEventAction_h 1

#include "globals.hh"
#include "G4UserEventAction.hh"

class TiaraCellScorerStore;

class TiaraTimedEventAction : public G4UserEventAction
{
public:
  TiaraTimedEventAction(G4int time);
  virtual ~TiaraTimedEventAction();

  virtual void   BeginOfEventAction(const G4Event*);
  virtual void   EndOfEventAction(const G4Event*);
  void SetScorerStore(TiaraCellScorerStore *scorerStore);
  G4int GetTotalProcessedTime() const;
  void SetRnadomNumFilename(const G4String &fname);

private:
  TiaraTimedEventAction(const TiaraTimedEventAction &);
  TiaraTimedEventAction &operator=(const TiaraTimedEventAction &);
  TiaraCellScorerStore *fScorerStore;
  G4int fEvStartTime;
  G4int fCurrentRunTime;
  G4int fMaxRunTime;
  G4int fTimeFromPreviousRuns;
  G4String fFilename;
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......



#endif

    
