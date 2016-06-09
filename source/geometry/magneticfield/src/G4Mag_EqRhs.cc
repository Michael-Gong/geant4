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
// $Id: G4Mag_EqRhs.cc,v 1.11 2006/06/29 18:24:36 gunter Exp $
// GEANT4 tag $Name: geant4-08-02 $
//
//  This is the standard right-hand side for equation of motion  
//    in a pure Magnetic Field .
//
//   Other that might be required are:
//     i) is when using a moving reference frame ... or
//    ii) extending for other forces, eg an electric field
//
//            J. Apostolakis, January 13th, 1997
//
// --------------------------------------------------------------------

#include "G4MagneticField.hh"
#include "G4Mag_EqRhs.hh"
#include "globals.hh"

const G4double G4Mag_EqRhs::fUnitConstant = 0.299792458 * (GeV/(tesla*m)); 

// Constructor Implementation
//
G4Mag_EqRhs::G4Mag_EqRhs( G4MagneticField *magField ) 
   : G4EquationOfMotion(magField)
{ 
}

void  
G4Mag_EqRhs::SetChargeMomentumMass( G4double particleCharge, // e+ units
			            G4double,                // MomentumXc
                                    G4double )               // particleMass
{
   fCof_val = particleCharge*eplus*c_light ; //  B must be in Tesla
   //  fCof_val = fUnitConstant*particleCharge/MomentumXc; //  B must be in Tesla
   // fMass = particleMass;
}

G4Mag_EqRhs::~G4Mag_EqRhs() { }
