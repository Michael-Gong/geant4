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
// $Id: G4ParallelStepper.hh,v 1.7 2006/06/29 18:16:25 gunter Exp $
// GEANT4 tag $Name: geant4-08-02 $
//
// ----------------------------------------------------------------------
// Class G4ParallelStepper
//
// Class description:
//
// Used internally by importance sampling and scoring in a "parallel"
// geometry. (See G4VParallelStepper.hh)

// Author: Michael Dressel (Michael.Dressel@cern.ch)
// ----------------------------------------------------------------------
#ifndef G4ParallelStepper_hh
#define G4ParallelStepper_hh G4ParallelStepper_hh

#include "G4VParallelStepper.hh"
#include "G4GeometryCellStep.hh"

class G4ParallelStepper : public G4VParallelStepper
{

public:  // with description

  G4ParallelStepper();
    // imitilisation

  G4ParallelStepper(const G4ParallelStepper &);
    // create new G4GeometryCellStep

  virtual ~G4ParallelStepper();
    // delete G4Pstep if created

  virtual G4GeometryCellStep GetPStep() const;
    // get the current G4GeometryCellStep
  
  virtual void Init(const G4GeometryCell &agCell);
    // initialise the parallel stepper and the G4GeometryCellStep
    // pre and post G4GeometryCell of the step are set equal

  virtual void Update(const G4GeometryCell &agCell);
    // to be called when crossing a boundary of the 
    // "parallel" geometry to update the G4GeometryCellStep

  virtual void UnSetCrossBoundary();
    // to be called to unset the fCrossBoundary member of the G4GeometryCellStep


  G4ParallelStepper &operator=(const G4ParallelStepper &);
    // create new G4GeometryCellStep

private:

  void Error(const G4String &m);

private:

  G4GeometryCellStep *fPStep;
};



#endif
