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
// $Id: G4PhysicsLinearVector.cc,v 1.12 2006/06/29 19:04:13 gunter Exp $
// GEANT4 tag $Name: geant4-08-02 $
//
// 
//--------------------------------------------------------------------
//      GEANT 4 class implementation file
//
//  G4PhysicsLinearVector.cc
//
//  History:
//    02 Dec. 1995, G.Cosmo : Structure created based on object model
//    15 Feb. 1996, K.Amako : Implemented the 1st version
//    01 Jul. 1996, K.Amako : Cache mechanism and hidden bin from the 
//                            user introduced.
//    26 Sep. 1996, K.Amako : Constructor with only 'bin size' added.
//    11 Nov. 2000, H.Kurashige : use STL vector for dataVector and binVector
//    9  Mar. 2001, H.Kurashige : add PhysicsVector type and Retrieve
//
//--------------------------------------------------------------------

#include "G4PhysicsLinearVector.hh"

G4PhysicsLinearVector::G4PhysicsLinearVector()
  : dBin(0.), baseBin(0.)
{
  edgeMin = 0.0;
  edgeMax = 0.0;
  numberOfBin = 0;
  type = T_G4PhysicsLinearVector;
}

G4PhysicsLinearVector::G4PhysicsLinearVector(size_t theNbin)
  : dBin(0.), baseBin(0.)
{
  type = T_G4PhysicsLinearVector;

  // Add extra one bin (hidden to user) to handle correctly when 
  // Energy=theEmax in getValue. 
  dataVector.reserve(theNbin+1);
  binVector.reserve(theNbin+1);      

  numberOfBin = theNbin;

  edgeMin = 0.;
  edgeMax = 0.;

  lastBin = INT_MAX;
  lastEnergy = -DBL_MAX;
  lastValue = DBL_MAX;

  for (size_t i=0; i<=numberOfBin; i++)
  {
     binVector.push_back(0.0);
     dataVector.push_back(0.0);
  }
}  

G4PhysicsLinearVector::G4PhysicsLinearVector(G4double theEmin, 
                                             G4double theEmax, size_t theNbin)
  : dBin((theEmax-theEmin)/theNbin),
    baseBin(theEmin/dBin)
{
  type = T_G4PhysicsLinearVector;

  // Add extra one bin (hidden to user) to handle correctly when 
  // Energy=theEmax in getValue. 
  dataVector.reserve(theNbin+1);
  binVector.reserve(theNbin+1);      

  numberOfBin = theNbin;

  for (size_t i=0; i<numberOfBin+1; i++)
  {
    binVector.push_back( theEmin + i*dBin );
    dataVector.push_back(0.0);
  }

  edgeMin = binVector[0];
  edgeMax = binVector[numberOfBin-1];

  lastBin = INT_MAX;
  lastEnergy = -DBL_MAX;
  lastValue = DBL_MAX;
}  

G4PhysicsLinearVector::~G4PhysicsLinearVector(){}

G4bool G4PhysicsLinearVector::Retrieve(std::ifstream& fIn, G4bool ascii)
{
  G4bool success = G4PhysicsVector::Retrieve(fIn, ascii);
  if (success)
  {
    G4double theEmin = binVector[0];
    dBin = binVector[1]-theEmin;
    baseBin = theEmin/dBin;
  }
  return success;
}
