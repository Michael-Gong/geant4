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
// $Id: G4BSplineCurveWithKnotsCreator.cc,v 1.7 2001/07/11 10:00:08 gunter Exp $
// GEANT4 tag $Name: geant4-04-00 $
//
// ----------------------------------------------------------------------
// Class G4BSplineCurveWithKnotsCreator
//
// Authors: J.Sulkimo, P.Urban.
// Revisions by: L.Broglia, G.Cosmo.
//
// History:
//   18-Nov-1999: First step of re-engineering - G.Cosmo
// ----------------------------------------------------------------------

#include <instmgr.h>
#include "g4std/vector"
#include "G4BSplineCurveWithKnotsCreator.hh"
#include "G4GeometryTable.hh"
#include "G4BSplineCurve.hh"

G4BSplineCurveWithKnotsCreator G4BSplineCurveWithKnotsCreator::csc;


G4BSplineCurveWithKnotsCreator::G4BSplineCurveWithKnotsCreator()
{
  G4GeometryTable::RegisterObject(this);
}


G4BSplineCurveWithKnotsCreator::~G4BSplineCurveWithKnotsCreator() {}


void G4BSplineCurveWithKnotsCreator::CreateG4Geometry(STEPentity& Ent)
{
  // Created by L. Broglia

  G4int             degree;
  G4Point3DVector   controlPointsList;
  G4std::vector<G4double>    knots;
  G4std::vector<G4double>    weightsData;
  G4BSplineCurve*   bSpline = new G4BSplineCurve();

  // degree
  G4String attrName("degree");
  STEPattribute *Attr = GetNamedAttribute(attrName, Ent);
  degree = *Attr->ptr.i;
  

  // controlPointsList
  attrName = "control_points_list";
  Attr = GetNamedAttribute(attrName, Ent);
   
  // Loop to find the entities
  // Fill points
  // Temporary solution until the STEP toolkit has been updated:
  char c = ' ';
  STEPaggregate *Aggr = Attr->ptr.a;
  char tmp[16];
  SCLstring s;
  const char *Str = Aggr->asStr(s);
  G4int Count=0;
  G4int nbpoint = 0;
  STEPentity *Entity;
  G4int stringlength = strlen(Str);  

  while(c != ')')
  {  
    while(c != '#')
    {
      c = Str[Count];
      Count++;
      
      if(Count>stringlength)
      {
	G4cout << "\nString index overflow in G4ControlPoints:116";
	exit(0);
      }
    }
    
    c = Str[Count];
    int Index=0;
    
    while(c != ',' && c != ')')
    {
      tmp[Index]=c;
      Index++;
      Count++;
      c = Str[Count];
    }

    tmp[Index]='\0'; 
    //c = ' ';
    Index = atoi(tmp);
  
    MgrNode* MgrTmp = instanceManager.FindFileId(Index);
    Index = instanceManager.GetIndex(MgrTmp);
//  Entity = instanceManager.GetSTEPentity(Index);
    Entity = instanceManager.GetApplication_instance(Index);

    void *tmp =G4GeometryTable::CreateObject(*Entity);
    if (!tmp)
    {
      G4cerr << "ERROR - G4BSplineCurveWithKnotsCreator::CreateG4Geometry" << G4endl
             << "\tUnexpected NULL control point!" << G4endl;
      G4Exception("G4BSplineCurveWithKnotsCreator: NULL G4Point3D* ?");
    }
    G4Point3D* pt = (G4Point3D*) tmp;
    G4Point3D Pt(pt->x(), pt->y(), pt->z());

    if(G4int(controlPointsList.size()) <= nbpoint+1)
      controlPointsList.resize(nbpoint+1);

    controlPointsList[nbpoint] = Pt;
    
    nbpoint++;
  }
  
  
  // weightsData
  attrName = "knot_multiplicities";
  Attr = GetNamedAttribute(attrName, Ent);
  c = ' ';
  Aggr = Attr->ptr.a;
  Str = Aggr->asStr(s);
  nbpoint = 0;
  Count = 0;

  while(c != ')')
  {      
    Count++;
    while(c == '(')
    {
      c = Str[Count];
      Count++;
    }

    int Index=0;
    c = Str[Count];

    while( c != ',' && c != ')' )
    {
      tmp[Index]=c;
      Index++;
      Count++;
      c = Str[Count];
    }

    tmp[Index]='\0';

    // L. Broglia : I am not sure of the function "atoi"
    G4double weight = atoi(tmp);
    if (weight <= 0)
      weight = 1;

    if(G4int(weightsData.size()) <= nbpoint+1)
      weightsData.resize(nbpoint+1);
    
    weightsData[nbpoint] = weight;
    nbpoint++;
  }


  // knots
  attrName = "knots";
  Attr = GetNamedAttribute(attrName, Ent); 
  c = ' ';
  Aggr = Attr->ptr.a;
  Str = Aggr->asStr(s);
  nbpoint = 0;
  Count = 0;

  while(c != ')')
  {  
    Count++;
    
    while(c == '(')
    {
      c = Str[Count];
      Count++;
    }

    int Index=0;
    c = Str[Count];

    while(c != ',' && c != ')')
    {
      tmp[Index]=c;
      Index++;
      Count++;
      c = Str[Count];
    }

    tmp[Index]='\0';
   
    // L. Broglia : I am not sure of the function "atoi"
    G4double knot = atoi(tmp);
    if (knot <= 0)
      knot = 1;

    if(G4int(knots.size()) <= nbpoint+1)
      knots.resize(nbpoint+1);
   
    knots[nbpoint] = knot;
    nbpoint++;
  }

  bSpline->Init(degree, &controlPointsList, &knots, &weightsData);
  
  createdObject = bSpline;
}

void G4BSplineCurveWithKnotsCreator::CreateSTEPGeometry(void* G4obj)
{
}