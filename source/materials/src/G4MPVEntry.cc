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
// $Id: G4MPVEntry.cc,v 1.7 2006/06/29 19:12:49 gunter Exp $
// GEANT4 tag $Name: geant4-08-02 $
//
// 
////////////////////////////////////////////////////////////////////////
// G4MPVEntry Class Implementation
////////////////////////////////////////////////////////////////////////
//
// File:        G4MPVEntry.cc
// Version:     1.0
// Created:     1996-02-08
// Author:      Juliet Armstrong
// Updated:     1997-03-25 by Peter Gumplinger
//              > cosmetics (only)
// mail:        gum@triumf.ca
//
////////////////////////////////////////////////////////////////////////

#include "G4MPVEntry.hh"

/////////////////////////
// Class Implementation
/////////////////////////

        //////////////
        // Operators
        //////////////

// Overload the == operator
// ------------------------
// Well defined == semantics required by G4RWTPtrSortedVector
//
G4bool G4MPVEntry::operator ==(const G4MPVEntry &right) const  
{
	if (thePhotonMomentum == right.thePhotonMomentum) 
		return true;
	else
		return false; 
}

// Overload the < operator
// -----------------------
// Well defined < semantics required by G4RWTPtrSortedVector
//
G4bool G4MPVEntry::operator <(const G4MPVEntry &right) const  
{
	if (thePhotonMomentum < right.thePhotonMomentum) 
		return true;
	else
		return false;
}

// Overload the = operator
// -----------------------
// Well defined = semantics required by G4RWTPtrSortedVector
//
G4MPVEntry& G4MPVEntry::operator =(const G4MPVEntry& right)
{
        if (this == &right) return *this;
	
        thePhotonMomentum = right.thePhotonMomentum;
        theProperty = right.theProperty;
        return *this;
}

        /////////////////
        // Constructors
        /////////////////

G4MPVEntry::G4MPVEntry(G4double aPhotonMomentum, G4double aProperty)
{
        thePhotonMomentum = aPhotonMomentum;
        theProperty = aProperty;
}

G4MPVEntry::G4MPVEntry(const G4MPVEntry &right)
{
        thePhotonMomentum = right.thePhotonMomentum;
        theProperty = right.theProperty;
}


        ////////////////
        // Destructors
        ////////////////

G4MPVEntry::~G4MPVEntry(){}

        ////////////
        // Methods
        ////////////

void G4MPVEntry::DumpEntry()
{
	G4cout << "(" 
	     << thePhotonMomentum 
	     << ", " 
	     << theProperty
 	     << ")" 
	     << G4endl;	
}
