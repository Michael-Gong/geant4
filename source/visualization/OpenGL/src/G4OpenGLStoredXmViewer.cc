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
// $Id: G4OpenGLStoredXmViewer.cc,v 1.6 2001/07/14 21:47:55 johna Exp $
// GEANT4 tag $Name: geant4-04-00 $
//
// 
// Andrew Walkden  10th February 1997
// Class G4OpenGLStoredXmViewer : a class derived from G4OpenGLXmViewer 
//                              and G4OpenGLStoredViewer.

#ifdef G4VIS_BUILD_OPENGLXM_DRIVER

#include "G4OpenGLStoredXmViewer.hh"

#include <GL/gl.h>
#include <GL/glx.h>
#include <GL/glu.h>

#include "G4ios.hh"
#include <assert.h>
#include <unistd.h>

G4OpenGLStoredXmViewer::G4OpenGLStoredXmViewer (G4OpenGLStoredSceneHandler& scene,
					    const G4String& name):
G4OpenGLViewer (scene),
G4OpenGLXmViewer (scene),
G4OpenGLStoredViewer (scene),
G4VViewer (scene, scene.IncrementViewCount (), name) {

  if (fViewId < 0) return;  // In case error in base class instantiation.

  if (!vi_stored) {
    fViewId = -1;  // This flags an error.
    G4cerr << "G4OpenGLStoredXmViewer::G4OpenGLStoredXmViewer -"
      " G4OpenGLXmViewer couldn't get a visual." << G4endl;
    return;
  }
}

G4OpenGLStoredXmViewer::~G4OpenGLStoredXmViewer () {}

void G4OpenGLStoredXmViewer::Initialise () {

  CreateGLXContext (vi_stored);

  CreateMainWindow ();

  InitializeGLView ();

// clear the buffers and window.
  ClearView ();
  FinishView ();
  
  glDepthFunc (GL_LEQUAL);
  glDepthMask (GL_TRUE);
  
  glEnable (GL_BLEND);
  glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  glShadeModel (GL_FLAT);
}

void G4OpenGLStoredXmViewer::DrawView () {

  if (white_background == true) {
    glClearColor (1., 1., 1., 1.);
  } else {
    glClearColor (0., 0., 0., 1.);
  }

  //Make sure current viewer is attached and clean...
  glXMakeCurrent (dpy, win, cx);
  glViewport (0, 0, WinSize_x, WinSize_y);
  ClearView ();

  G4ViewParameters::DrawingStyle style = GetViewParameters().GetDrawingStyle();

  //See if things have changed from last time and remake if necessary...
  KernelVisitDecision ();
  ProcessView ();

  if(style!=G4ViewParameters::hlr &&
     haloing_enabled) {

    HaloingFirstPass ();
    DrawDisplayLists ();
    glFlush ();

    HaloingSecondPass ();

  }

  DrawDisplayLists ();

  FinishView ();
  
}

#endif