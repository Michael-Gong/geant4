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
// $Id: G4F21GEMProbability.cc,v 1.4 2006/06/29 20:22:05 gunter Exp $
// GEANT4 tag $Name: geant4-08-02 $
//
// Hadronic Process: Nuclear De-excitations
// by V. Lara (Nov 1999)
//


#include "G4F21GEMProbability.hh"

G4F21GEMProbability::G4F21GEMProbability() :
  G4GEMProbability(21,9,5.0/2.0) // A,Z,Spin
{

  ExcitEnergies.push_back(279.92*keV);
  ExcitSpins.push_back(1.0/2.0);
  ExcitLifetimes.push_back(6.1*nanosecond);

  ExcitEnergies.push_back(1100.9*keV);
  ExcitSpins.push_back(3.0/2.0);
  ExcitLifetimes.push_back(305.0e-3*picosecond);

  ExcitEnergies.push_back(3449.0*keV);
  ExcitSpins.push_back(5.0/2.0);
  ExcitLifetimes.push_back(0.7*picosecond);

  ExcitEnergies.push_back(3508.0*keV);
  ExcitSpins.push_back(5.0/2.0);
  ExcitLifetimes.push_back(0.7*picosecond);

}


G4F21GEMProbability::G4F21GEMProbability(const G4F21GEMProbability &) : G4GEMProbability()
{
  throw G4HadronicException(__FILE__, __LINE__, "G4F21GEMProbability::copy_constructor meant to not be accessable");
}




const G4F21GEMProbability & G4F21GEMProbability::
operator=(const G4F21GEMProbability &)
{
  throw G4HadronicException(__FILE__, __LINE__, "G4F21GEMProbability::operator= meant to not be accessable");
  return *this;
}


G4bool G4F21GEMProbability::operator==(const G4F21GEMProbability &) const
{
  return false;
}

G4bool G4F21GEMProbability::operator!=(const G4F21GEMProbability &) const
{
  return true;
}



