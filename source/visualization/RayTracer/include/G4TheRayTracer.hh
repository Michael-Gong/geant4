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
// $Id: G4TheRayTracer.hh,v 1.3 2006/06/29 21:23:49 gunter Exp $
// GEANT4 tag $Name: geant4-08-02 $
//
//


#ifndef G4TheRayTracer_H
#define G4TheRayTracer_H 1

// class description:
//
// G4TheRayTracer
//   This is a graphics driver of Geant4 which generates a figure file by
// ray tracing technique. The format of output figure file can be selected
// by assigning a pointer of G4VFigureFileMaker concrete class object.
//   The main entry of ray tracing is Trace() method, which is available
// only at Idle state. G4TheRayTracer shoots rays and controls its own event
// loop. It generates G4Event objects used for its own purpose. When ray
// tracing is working, all sensitive detectors are inactivated and all
// user action classes are swapped out. Still, verbosities set to Geant4
// manager classes are concerned. Thus, it is recommended to set verbosities
// to minimum (usually zero).
//   G4TheRayTracer can visualise absolutely all kinds of geometrical shapes
// which G4Navigator can deal with. Instead, it can NOT visualise hits
// nor trajectories generated by usual simulation.

#include "globals.hh"
#include "G4ThreeVector.hh"
#include "G4Colour.hh"

class G4Event;
class G4EventManager;
class G4UserEventAction;
class G4UserStackingAction;
class G4UserTrackingAction;
class G4UserSteppingAction;
class G4RTTrackingAction;
class G4RTSteppingAction;
class G4RTMessenger;
class G4RayShooter;
class G4VFigureFileMaker;
class G4RayTrajectoryPoint;
class G4VisAttributes;
class G4VRTScanner;


class G4TheRayTracer
{
  public: // with description
    G4TheRayTracer(G4VFigureFileMaker* figMaker = 0,
		   G4VRTScanner* scanner = 0);
    // Constructor. The argument is the pointer to G4VFigureFileMaker
    // concrete class object. If it is not set and
    // SetFigureFileMaker() method is not invoked before Trace()
    // command is invoked, then G4RTJpegMaker will be used and JPEG
    // file will be generated.  The second argument is a scanner that
    // produces a sequence of window coordinates.  If it is not set
    // here or if SetScanner is not invoked before Trace(), a default
    // G4RTSimpleScanner will be used.

  public:
    ~G4TheRayTracer();

  public: // with description
    void Trace(G4String fileName);
    // The main entry point which triggers ray tracing. "fileName" is output
    // file name, and it must contain extention (e.g. myFigure.jpg). This
    // method is available only if Geant4 is at Idle state.

  protected:
    G4bool CreateBitMap();
    // Event loop
    void CreateFigureFile(G4String fileName);
    // Create figure file after an event loop
    G4bool GenerateColour(G4Event* anEvent);
    // Calcurate RGB for one trajectory
    void StoreUserActions();
    void RestoreUserActions();
    // Store and restore user action classes if defined

    G4Colour GetSurfaceColour(G4RayTrajectoryPoint* point);
    G4Colour GetMixedColour(G4Colour surfCol,G4Colour transCol,G4double weight=0.5);
    G4Colour Attenuate(G4RayTrajectoryPoint* point, G4Colour sourceCol);
    G4bool ValidColour(const G4VisAttributes* visAtt);

  public: // with description
    inline void SetFigureFileMaker(G4VFigureFileMaker* figMaker)
    // Set a concrete class of G4VFigureFileMaker for assigning the format of
    // output figure file.
    { theFigMaker = figMaker; }
    inline G4VFigureFileMaker* GetFigureFileMaker() {return theFigMaker;}
    inline void SetScanner(G4VRTScanner* scanner)
    // Set a concrete class of G4VRTScanner for producing a sequence
    // of window coordinates.
    { theScanner = scanner; }
    inline G4VRTScanner* GetScanner() {return theScanner;}

  protected:
    G4RayShooter * theRayShooter;
    G4VFigureFileMaker * theFigMaker;
    G4RTMessenger * theMessenger;
    G4VRTScanner * theScanner;

    G4EventManager * theEventManager;

    G4UserEventAction * theUserEventAction;
    G4UserStackingAction * theUserStackingAction;
    G4UserTrackingAction * theUserTrackingAction;
    G4UserSteppingAction * theUserSteppingAction;

    G4UserEventAction * theRayTracerEventAction;
    G4UserStackingAction * theRayTracerStackingAction;
    G4RTTrackingAction * theRayTracerTrackingAction;
    G4RTSteppingAction * theRayTracerSteppingAction;

    unsigned char* colorR;
    unsigned char* colorG;
    unsigned char* colorB;

    G4int nColumn;
    G4int nRow;

    G4ThreeVector eyePosition;
    G4ThreeVector targetPosition;
    G4ThreeVector eyeDirection;
    G4ThreeVector lightDirection;
    G4double headAngle;
    G4double viewSpan; // Angle per 100 pixels
    G4double attenuationLength;

    G4bool distortionOn;
    G4bool antialiasingOn;

    G4Colour rayColour;
    G4Colour backgroundColour;

  public:
    inline void SetNColumn(G4int val) { nColumn = val; }
    inline G4int GetNColumn() const { return nColumn; }
    inline void SetNRow(G4int val) { nRow = val; }
    inline G4int GetNRow() const { return nRow; }
    inline void SetEyePosition(const G4ThreeVector& val) { eyePosition = val; }
    inline G4ThreeVector GetEyePosition() const { return eyePosition; }
    inline void SetTargetPosition(const G4ThreeVector& val) { targetPosition = val; }
    inline G4ThreeVector GetTargetPosition() const { return targetPosition; }
    inline void SetLightDirection(const G4ThreeVector& val) { lightDirection = val.unit(); }
    inline G4ThreeVector GetLightDirection() const { return lightDirection; }
    inline void SetHeadAngle(G4double val) { headAngle = val; }
    inline G4double GetHeadAngle() const { return headAngle; }
    inline void SetViewSpan(G4double val) { viewSpan = val; }
    inline G4double GetViewSpan() const { return viewSpan; }
    inline void SetAttenuationLength(G4double val) { attenuationLength = val; }
    inline G4double GetAttenuationLength() const { return attenuationLength; }
    inline void SetDistortion(G4bool val) { distortionOn = val; }
    inline G4bool GetDistortion() const { return distortionOn; }
    inline void SetBackgroundColour(G4Colour val) { backgroundColour = val; }
    inline G4Colour GetBackgroundColour() const { return backgroundColour; }
};

#endif
