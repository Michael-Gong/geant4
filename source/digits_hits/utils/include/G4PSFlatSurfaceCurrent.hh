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
// $Id: G4PSFlatSurfaceCurrent.hh,v 1.3 2006/06/29 18:07:01 gunter Exp $
// GEANT4 tag $Name: geant4-08-02 $
//

#ifndef G4PSFlatSurfaceCurrent_h
#define G4PSFlatSurfaceCurrent_h 1

#include "G4VPrimitiveScorer.hh"
#include "G4THitsMap.hh"

#include "G4Box.hh"
#include "G4PSDirectionFlag.hh"
////////////////////////////////////////////////////////////////////////////////
// (Description)
//   This is a primitive scorer class for scoring Surface Current.
//  Current version assumes only for G4Box shape, and the surface
//  is defined at the -Z plane of the box.
//   The current is given in the unit of area. 
//    e.g.  (Number of tracks)/mm2.
//
// Surface is defined at the -Z surface.
// Direction                  -Z   +Z
//   0  IN || OUT            ->|<-  |      fCurrent_InOut
//   1  IN                   ->|    |      fCurrent_In
//   2  OUT                    |<-  |      fCurrent_Out
//
//
// Created: 2005-11-14  Tsukasa ASO, Akinori Kimura.
// 17-Nov-2005 T.Aso, Bug fix for area definition.
// 
///////////////////////////////////////////////////////////////////////////////

class G4PSFlatSurfaceCurrent : public G4VPrimitiveScorer
{
 
  public: // with description
      G4PSFlatSurfaceCurrent(G4String name ,G4int direction, G4int depth=0);
      virtual ~G4PSFlatSurfaceCurrent();

  protected: // with description
      virtual G4bool ProcessHits(G4Step*,G4TouchableHistory*);
      G4int IsSelectedSurface(G4Step*,G4Box*);

  public: 
      virtual void Initialize(G4HCofThisEvent*);
      virtual void EndOfEvent(G4HCofThisEvent*);
      virtual void clear();
      virtual void DrawAll();
      virtual void PrintAll();

  private:
      G4int  HCID;
      G4int  fDirection;
      G4THitsMap<G4double>* EvtMap;

};

#endif

