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
// $Id: G4OpenGLXmFourArrowButtons.hh,v 1.6 2006/06/29 21:18:26 gunter Exp $
// GEANT4 tag $Name: geant4-08-02 $
//
// 
//Four arrow buttons class. Inherits from G4OpenGLXmVWidgetComponent

#ifdef G4VIS_BUILD_OPENGLXM_DRIVER

#ifndef G4OPENGLXMFOURARROWBUTTONS_HH
#define G4OPENGLXMFOURARROWBUTTONS_HH

#include "G4OpenGLXmVWidgetComponent.hh"

class G4OpenGLXmFourArrowButtons : public G4OpenGLXmVWidgetComponent
{

public:
  G4OpenGLXmFourArrowButtons (XtCallbackRec** = NULL); // array of 4 callbacks
                                                       //constructor
  virtual ~G4OpenGLXmFourArrowButtons ();              //destructor

  void SetName (char*);
 
  char* GetName ();

  void AddYourselfTo (G4OpenGLXmVWidgetContainer*);

  Widget* GetPointerToParent ();
  Widget* GetPointerToWidget ();

private:
  XtCallbackRec** callback;
  Widget arrow_form;
  Widget arrow;
  Widget* parent;

private:
  G4OpenGLXmFourArrowButtons (const G4OpenGLXmFourArrowButtons&);
  G4OpenGLXmFourArrowButtons& operator = (const G4OpenGLXmFourArrowButtons&);
};

#endif

#endif
