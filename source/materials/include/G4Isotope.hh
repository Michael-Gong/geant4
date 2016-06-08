//
// ********************************************************************
// * DISCLAIMER                                                       *
// *                                                                  *
// * The following disclaimer summarizes all the specific disclaimers *
// * of contributors to this software. The specific disclaimers,which *
// * govern, are listed with their locations in:                      *
// *   http://cern.ch/geant4/license                                  *
// *                                                                  *
// * Neither the authors of this software system, nor their employing *
// * institutes,nor the agencies providing financial support for this *
// * work  make  any representation or  warranty, express or implied, *
// * regarding  this  software system or assume any liability for its *
// * use.                                                             *
// *                                                                  *
// * This  code  implementation is the  intellectual property  of the *
// * GEANT4 collaboration.                                            *
// * By copying,  distributing  or modifying the Program (or any work *
// * based  on  the Program)  you indicate  your  acceptance of  this *
// * statement, and all its terms.                                    *
// ********************************************************************
//
//
// $Id: G4Isotope.hh,v 1.14 2002/02/26 17:34:34 maire Exp $
// GEANT4 tag $Name: geant4-04-00-patch-02 $
// 
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

// class description
//
// An isotope is a chemical isotope defined by its name,
//                                                 Z (atomic number),
//                                                 N (number of nucleons), 
//                                                 A (mass of a mole).
//
// The class contains as a private static member the table of defined
// isotopes (an ordered vector of isotopes).
//
// Isotopes can be assembled into elements via the G4Element class.
//

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

// 26.02.02: fIndexInTable renewed 
// 14.09.01: fCountUse: nb of elements which use this isotope 
// 13.09.01: stl migration. Suppression of the data member fIndexInTable
// 30.03.01: suppression of the warning message in GetIsotope
// 04.08.98: new method GetIsotope(isotopeName) (mma)
// 17.01.97: aesthetic rearrangement (mma)

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#ifndef G4ISOTOPE_HH
#define G4ISOTOPE_HH

#include "globals.hh"
#include "G4ios.hh"
#include "g4std/vector"

class G4Isotope;
typedef G4std::vector<G4Isotope*> G4IsotopeTable;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo.... ....oooOO0OOooo......

class G4Isotope
{
 public:  // with description

    // Make an isotope
    G4Isotope(const G4String& name,		//its name
                    G4int     z,		//atomic number
                    G4int     n,		//number of nucleons
                    G4double  a);		//mass of mole
                    
    virtual ~G4Isotope();

    // Retrieval methods
    G4String GetName()  const {return fName;};    
    G4int    GetZ()     const {return fZ;};
    G4int    GetN()     const {return fN;};
    G4double GetA()     const {return fA;};
    
    G4int GetCountUse() const {return fCountUse;};
    void  increaseCountUse()  {fCountUse++;};
    void  decreaseCountUse()  {fCountUse--;};
    
    static  
    G4Isotope* GetIsotope(G4String name);
    
    static const
    G4IsotopeTable* GetIsotopeTable();
    
    static 
    size_t GetNumberOfIsotopes();
    
    size_t GetIndex() const {return fIndexInTable;};    
    
    friend
    G4std::ostream& operator<<(G4std::ostream&, G4Isotope*);
    
    friend
    G4std::ostream& operator<<(G4std::ostream&, G4Isotope&);
    
    friend
    G4std::ostream& operator<<(G4std::ostream&, G4IsotopeTable);
     
 public:  // without description
 
    G4int operator==(const G4Isotope&) const;
    G4int operator!=(const G4Isotope&) const;
    
 private:
     
    G4Isotope(G4Isotope&);
    G4Isotope& operator=(const G4Isotope&);
     
 private:

    G4String fName;              // name of the Isotope
    G4int    fZ;                 // atomic number
    G4int    fN;                 // number of nucleons
    G4double fA;                 // mass of a mole
    
    G4int    fCountUse;          // nb of elements which use this isotope

    static 
    G4IsotopeTable theIsotopeTable;
    
    size_t fIndexInTable;        // index in the Isotope table
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......


#endif