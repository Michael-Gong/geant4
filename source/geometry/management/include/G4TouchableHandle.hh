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
// $Id: G4TouchableHandle.hh,v 1.6 2006/06/29 18:32:25 gunter Exp $
// GEANT4 tag $Name: geant4-08-02 $
// 
// Class G4TouchableHandle
//
// Class description:
//
// A type providing reference counting mechanism for any kind of touchable
// objects.
// The basic rule for the use of this type is that this handle must always
// be exchanged by reference never dinamically allocated (i.e. never
// instantiated using 'new').
// For more details see G4ReferenceCountedHandle.

// Author:      Radovan Chytracek, CERN  (Radovan.Chytracek@cern.ch)
// Version:     1.0
// Date:        February 2001
// ----------------------------------------------------------------------
#ifndef _G4TOUCHABLEHANDLE_H_
#define _G4TOUCHABLEHANDLE_H_ 1

#include "G4VTouchable.hh"
#include "G4ReferenceCountedHandle.hh"

typedef G4ReferenceCountedHandle<G4VTouchable> G4TouchableHandle;

#endif // _G4TOUCHABLEHANDLE_H_
