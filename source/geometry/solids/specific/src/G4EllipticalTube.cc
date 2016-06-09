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
// $Id: G4EllipticalTube.cc,v 1.27 2006/10/20 13:45:21 gcosmo Exp $
// GEANT4 tag $Name: geant4-08-02 $
//
// 
// --------------------------------------------------------------------
// GEANT 4 class source file
//
//
// G4EllipticalTube.cc
//
// Implementation of a CSG volume representing a tube with elliptical cross
// section (geant3 solid 'ELTU')
//
// --------------------------------------------------------------------

#include "G4EllipticalTube.hh"

#include "G4ClippablePolygon.hh"
#include "G4AffineTransform.hh"
#include "G4SolidExtentList.hh"
#include "G4VoxelLimits.hh"
#include "meshdefs.hh"

#include "Randomize.hh"

#include "G4VGraphicsScene.hh"
#include "G4Polyhedron.hh"
#include "G4VisExtent.hh"

using namespace CLHEP;

//
// Constructor
//
G4EllipticalTube::G4EllipticalTube( const G4String &name, 
                                          G4double theDx,
                                          G4double theDy,
                                          G4double theDz )
  : G4VSolid( name ), fCubicVolume(0.), fSurfaceArea(0.), fpPolyhedron(0)
{
  dx = theDx;
  dy = theDy;
  dz = theDz;
}


//
// Fake default constructor - sets only member data and allocates memory
//                            for usage restricted to object persistency.
//
G4EllipticalTube::G4EllipticalTube( __void__& a )
  : G4VSolid(a), fCubicVolume(0.), fSurfaceArea(0.), fpPolyhedron(0)
{
}


//
// Destructor
//
G4EllipticalTube::~G4EllipticalTube()
{
  delete fpPolyhedron;
}


//
// CalculateExtent
//
G4bool
G4EllipticalTube::CalculateExtent( const EAxis axis,
                                   const G4VoxelLimits &voxelLimit,
                                   const G4AffineTransform &transform,
                                         G4double &min, G4double &max ) const
{
  G4SolidExtentList  extentList( axis, voxelLimit );
  
  //
  // We are going to divide up our elliptical face into small
  // pieces
  //
  
  //
  // Choose phi size of our segment(s) based on constants as
  // defined in meshdefs.hh
  //
  G4int numPhi = kMaxMeshSections;
  G4double sigPhi = twopi/numPhi;
  
  //
  // We have to be careful to keep our segments completely outside
  // of the elliptical surface. To do so we imagine we have
  // a simple (unit radius) circular cross section (as in G4Tubs) 
  // and then "stretch" the dimensions as necessary to fit the ellipse.
  //
  G4double rFudge = 1.0/std::cos(0.5*sigPhi);
  G4double dxFudge = dx*rFudge,
           dyFudge = dy*rFudge;
  
  //
  // As we work around the elliptical surface, we build
  // a "phi" segment on the way, and keep track of two
  // additional polygons for the two ends.
  //
  G4ClippablePolygon endPoly1, endPoly2, phiPoly;
  
  G4double phi = 0, 
           cosPhi = std::cos(phi),
           sinPhi = std::sin(phi);
  G4ThreeVector v0( dxFudge*cosPhi, dyFudge*sinPhi, +dz ),
                v1( dxFudge*cosPhi, dyFudge*sinPhi, -dz ),
                w0, w1;
  transform.ApplyPointTransform( v0 );
  transform.ApplyPointTransform( v1 );
  do
  {
    phi += sigPhi;
    if (numPhi == 1) phi = 0;  // Try to avoid roundoff
    cosPhi = std::cos(phi), 
    sinPhi = std::sin(phi);
    
    w0 = G4ThreeVector( dxFudge*cosPhi, dyFudge*sinPhi, +dz );
    w1 = G4ThreeVector( dxFudge*cosPhi, dyFudge*sinPhi, -dz );
    transform.ApplyPointTransform( w0 );
    transform.ApplyPointTransform( w1 );
    
    //
    // Add a point to our z ends
    //
    endPoly1.AddVertexInOrder( v0 );
    endPoly2.AddVertexInOrder( v1 );
    
    //
    // Build phi polygon
    //
    phiPoly.ClearAllVertices();
    
    phiPoly.AddVertexInOrder( v0 );
    phiPoly.AddVertexInOrder( v1 );
    phiPoly.AddVertexInOrder( w1 );
    phiPoly.AddVertexInOrder( w0 );
    
    if (phiPoly.PartialClip( voxelLimit, axis ))
    {
      //
      // Get unit normal
      //
      phiPoly.SetNormal( (v1-v0).cross(w0-v0).unit() );
      
      extentList.AddSurface( phiPoly );
    }

    //
    // Next vertex
    //    
    v0 = w0;
    v1 = w1;
  } while( --numPhi > 0 );

  //
  // Process the end pieces
  //
  if (endPoly1.PartialClip( voxelLimit, axis ))
  {
    static const G4ThreeVector normal(0,0,+1);
    endPoly1.SetNormal( transform.TransformAxis(normal) );
    extentList.AddSurface( endPoly1 );
  }
  
  if (endPoly2.PartialClip( voxelLimit, axis ))
  {
    static const G4ThreeVector normal(0,0,-1);
    endPoly2.SetNormal( transform.TransformAxis(normal) );
    extentList.AddSurface( endPoly2 );
  }
  
  //
  // Return min/max value
  //
  return extentList.GetExtent( min, max );
}


//
// Inside
//
// Note that for this solid, we've decided to define the tolerant
// surface as that which is bounded by ellipses with axes
// at +/- 0.5*kCarTolerance.
//
EInside G4EllipticalTube::Inside( const G4ThreeVector& p ) const
{
  static const G4double halfTol = 0.5*kCarTolerance;
  
  //
  // Check z extents: are we outside?
  //
  G4double absZ = std::fabs(p.z());
  if (absZ > dz+halfTol) return kOutside;
  
  //
  // Check x,y: are we outside?
  //
  // G4double x = p.x(), y = p.y();
  
  if (CheckXY(p.x(), p.y(), +halfTol) > 1.0) return kOutside;
  
  //
  // We are either inside or on the surface: recheck z extents
  //
  if (absZ > dz-halfTol) return kSurface;
  
  //
  // Recheck x,y
  //
  if (CheckXY(p.x(), p.y(), -halfTol) > 1.0) return kSurface;
  
  return kInside;
}


//
// SurfaceNormal
//
G4ThreeVector G4EllipticalTube::SurfaceNormal( const G4ThreeVector& p ) const
{
  //
  // Which of the three surfaces are we closest to (approximately)?
  //
  G4double distZ = std::fabs(p.z()) - dz;
  
  G4double rxy = CheckXY( p.x(), p.y() );
  G4double distR2 = (rxy < DBL_MIN) ? DBL_MAX : 1.0/rxy;

  //
  // Closer to z?
  //
  if (distZ*distZ < distR2) 
    return G4ThreeVector( 0.0, 0.0, p.z() < 0 ? -1.0 : 1.0 );

  //
  // Closer to x/y
  //
  return G4ThreeVector( p.x()*dy*dy, p.y()*dx*dx, 0.0 ).unit();
}


//
// DistanceToIn(p,v)
//
// Unlike DistanceToOut(p,v), it is possible for the trajectory
// to miss. The geometric calculations here are quite simple.
// More difficult is the logic required to prevent particles
// from sneaking (or leaking) between the elliptical and end
// surfaces.
//
// Keep in mind that the true distance is allowed to be
// negative if the point is currently on the surface. For oblique
// angles, it can be very negative. 
//
G4double G4EllipticalTube::DistanceToIn( const G4ThreeVector& p,
                                         const G4ThreeVector& v ) const
{
  static const G4double halfTol = 0.5*kCarTolerance;
    
  //
  // Check z = -dz planer surface
  //
  G4double sigz = p.z()+dz;

  if (sigz < halfTol)
  {
    //
    // We are "behind" the shape in z, and so can
    // potentially hit the rear face. Correct direction?
    //
    if (v.z() <= 0)
    {
      //
      // As long as we are far enough away, we know we
      // can't intersect
      //
      if (sigz < 0) return kInfinity;
      
      //
      // Otherwise, we don't intersect unless we are
      // on the surface of the ellipse
      //
      if (CheckXY(p.x(),p.y(),-halfTol) <= 1.0) return kInfinity;
    }
    else
    {
      //
      // How far?
      //
      G4double s = -sigz/v.z();
      
      //
      // Where does that place us?
      //
      G4double xi = p.x() + s*v.x(),
               yi = p.y() + s*v.y();
      
      //
      // Is this on the surface (within ellipse)?
      //
      if (CheckXY(xi,yi) <= 1.0)
      {
        //
        // Yup. Return s, unless we are on the surface
        //
        return (sigz < -halfTol) ? s : 0;
      }
      else if (xi*dy*dy*v.x() + yi*dx*dx*v.y() >= 0)
      {
        //
        // Else, if we are traveling outwards, we know
        // we must miss
        //
        return kInfinity;
      }
    }
  }

  //
  // Check z = +dz planer surface
  //
  sigz = p.z() - dz;
  
  if (sigz > -halfTol)
  {
    if (v.z() >= 0)
    {
      if (sigz > 0) return kInfinity;
      if (CheckXY(p.x(),p.y(),-halfTol) <= 1.0) return kInfinity;
    }
    else {
      G4double s = -sigz/v.z();

      G4double xi = p.x() + s*v.x(),
               yi = p.y() + s*v.y();
      
      if (CheckXY(xi,yi) <= 1.0)
      {
        return (sigz > -halfTol) ? s : 0;
      }
      else if (xi*dy*dy*v.x() + yi*dx*dx*v.y() >= 0)
      {
        return kInfinity;
      }
    }
  }
  
  //
  // Check intersection with the elliptical tube
  //
  G4double s[2];
  G4int n = IntersectXY( p, v, s );
  
  if (n==0) return kInfinity;
  
  //
  // Is the original point on the surface?
  //
  if (std::fabs(p.z()) < dz+halfTol) {
    if (CheckXY( p.x(), p.y(), halfTol ) < 1.0)
    {
      //
      // Well, yes, but are we traveling inwards at this point?
      //
      if (p.x()*dy*dy*v.x() + p.y()*dx*dx*v.y() < 0) return 0;
    }
  }
  
  //
  // We are now certain that point p is not on the surface of 
  // the solid (and thus std::fabs(s[0]) > halfTol). 
  // Return kInfinity if the intersection is "behind" the point.
  //
  if (s[0] < 0) return kInfinity;
  
  //
  // Check to see if we intersect the tube within
  // dz, but only when we know it might miss
  //
  G4double zi = p.z() + s[0]*v.z();

  if (v.z() < 0)
  {
    if (zi < -dz) return kInfinity;
  }
  else if (v.z() > 0)
  {
    if (zi > +dz) return kInfinity;
  }

  return s[0];
}


//
// DistanceToIn(p)
//
// The distance from a point to an ellipse (in 2 dimensions) is a
// surprisingly complicated quadric expression (this is easy to
// appreciate once one understands that there may be up to
// four lines normal to the ellipse intersecting any point). To 
// solve it exactly would be rather time consuming. This method, 
// however, is supposed to be a quick check, and is allowed to be an
// underestimate.
//
// So, I will use the following underestimate of the distance
// from an outside point to an ellipse. First: find the intersection "A"
// of the line from the origin to the point with the ellipse.
// Find the line passing through "A" and tangent to the ellipse 
// at A. The distance of the point p from the ellipse will be approximated
// as the distance to this line.
//
G4double G4EllipticalTube::DistanceToIn( const G4ThreeVector& p ) const
{
  static const G4double halfTol = 0.5*kCarTolerance;
  
  if (CheckXY( p.x(), p.y(), +halfTol ) < 1.0)
  {
    //
    // We are inside or on the surface of the
    // elliptical cross section in x/y. Check z
    //
    if (p.z() < -dz-halfTol) 
      return -p.z()-dz;
    else if (p.z() > dz+halfTol)
      return p.z()-dz;
    else
      return 0;    // On any surface here (or inside)
  }
  
  //
  // Find point on ellipse
  //
  G4double qnorm = CheckXY( p.x(), p.y() );
  if (qnorm < DBL_MIN) return 0;  // This should never happen
  
  G4double q = 1.0/std::sqrt(qnorm);
  
  G4double xe = q*p.x(), ye = q*p.y();
     
  //
  // Get tangent to ellipse
  //
  G4double tx = -ye*dx*dx, ty = +xe*dy*dy;
  G4double tnorm = std::sqrt( tx*tx + ty*ty );
  
  //
  // Calculate distance
  //
  G4double distR = ( (p.x()-xe)*ty - (p.y()-ye)*tx )/tnorm;
  
  //
  // Add the result in quadrature if we are, in addition,
  // outside the z bounds of the shape
  //
  // We could save some time by returning the maximum rather
  // than the quadrature sum
  //
  if (p.z() < -dz) 
    return std::sqrt( (p.z()+dz)*(p.z()+dz) + distR*distR );
  else if (p.z() > dz)
    return std::sqrt( (p.z()-dz)*(p.z()-dz) + distR*distR );

  return distR;
}


//
// DistanceToOut(p,v)
//
// This method can be somewhat complicated for a general shape.
// For a convex one, like this, there are several simplifications,
// the most important of which is that one can treat the surfaces
// as infinite in extent when deciding if the p is on the surface.
//
G4double G4EllipticalTube::DistanceToOut( const G4ThreeVector& p,
                                          const G4ThreeVector& v,
                                          const G4bool calcNorm,
                                                G4bool *validNorm,
                                                G4ThreeVector *norm ) const
{
  static const G4double halfTol = 0.5*kCarTolerance;
  
  //
  // Our normal is always valid
  //
  if (calcNorm) *validNorm = true;
  
  G4double sBest = kInfinity;
  const G4ThreeVector *nBest=0;
  
  //
  // Might we intersect the -dz surface?
  //
  if (v.z() < 0)
  {
    static const G4ThreeVector normHere(0.0,0.0,-1.0);
    //
    // Yup. What distance?
    //
    sBest = -(p.z()+dz)/v.z();
    
    //
    // Are we on the surface? If so, return zero
    //
    if (p.z() < -dz+halfTol) {
      if (calcNorm) *norm = normHere;
      return 0;
    }
    else
      nBest = &normHere;
  }
  
  //
  // How about the +dz surface?
  //
  if (v.z() > 0)
  {
    static const G4ThreeVector normHere(0.0,0.0,+1.0);
    //
    // Yup. What distance?
    //
    G4double s = (dz-p.z())/v.z();
    
    //
    // Are we on the surface? If so, return zero
    //
    if (p.z() > +dz-halfTol) {
      if (calcNorm) *norm = normHere;
      return 0;
    }
    
    //
    // Best so far?
    //
    if (s < sBest) { sBest = s; nBest = &normHere; }
  }
  
  //
  // Check furthest intersection with ellipse 
  //
  G4double s[2];
  G4int n = IntersectXY( p, v, s );

  if (n == 0)
  {
    if (sBest == kInfinity)
    {
      G4cout.precision(16) ;
      G4cout << G4endl ;
      DumpInfo();
      G4cout << "Position:"  << G4endl << G4endl ;
      G4cout << "p.x() = "   << p.x()/mm << " mm" << G4endl ;
      G4cout << "p.y() = "   << p.y()/mm << " mm" << G4endl ;
      G4cout << "p.z() = "   << p.z()/mm << " mm" << G4endl << G4endl ;
      G4cout << "Direction:" << G4endl << G4endl;
      G4cout << "v.x() = "   << v.x() << G4endl;
      G4cout << "v.y() = "   << v.y() << G4endl;
      G4cout << "v.z() = "   << v.z() << G4endl << G4endl;
      G4cout << "Proposed distance :" << G4endl << G4endl;
      G4cout << "snxt = "    << sBest/mm << " mm" << G4endl << G4endl;
      G4Exception( "G4EllipticalTube::DistanceToOut(p,v,...)",
                   "Notification", JustWarning, "Point p is outside !?" );
    }
    if (calcNorm) *norm = *nBest;
    return sBest;
  }
  else if (s[n-1] > sBest)
  {
    if (calcNorm) *norm = *nBest;
    return sBest;
  }  
  sBest = s[n-1];
      
  //
  // Intersection with ellipse. Get normal at intersection point.
  //
  if (calcNorm)
  {
    G4ThreeVector ip = p + sBest*v;
    *norm = G4ThreeVector( ip.x()*dy*dy, ip.y()*dx*dx, 0.0 ).unit();
  }
  
  //
  // Do we start on the surface?
  //
  if (CheckXY( p.x(), p.y(), -halfTol ) > 1.0)
  {
    //
    // Well, yes, but are we traveling outwards at this point?
    //
    if (p.x()*dy*dy*v.x() + p.y()*dx*dx*v.y() > 0) return 0;
  }
  
  return sBest;
}


//
// DistanceToOut(p)
//
// See DistanceToIn(p) for notes on the distance from a point
// to an ellipse in two dimensions.
//
// The approximation used here for a point inside the ellipse
// is to find the intersection with the ellipse of the lines 
// through the point and parallel to the x and y axes. The
// distance of the point from the line connecting the two 
// intersecting points is then used.
//
G4double G4EllipticalTube::DistanceToOut( const G4ThreeVector& p ) const
{
  static const G4double halfTol = 0.5*kCarTolerance;
  
  //
  // We need to calculate the distances to all surfaces,
  // and then return the smallest
  //
  // Check -dz and +dz surface
  //
  G4double sBest = dz - std::fabs(p.z());
  if (sBest < halfTol) return 0;
  
  //
  // Check elliptical surface: find intersection of
  // line through p and parallel to x axis
  //
  G4double radical = 1.0 - p.y()*p.y()/dy/dy;
  if (radical < +DBL_MIN) return 0;
  
  G4double xi = dx*std::sqrt( radical );
  if (p.x() < 0) xi = -xi;
  
  //
  // Do the same with y axis
  //
  radical = 1.0 - p.x()*p.x()/dx/dx;
  if (radical < +DBL_MIN) return 0;
  
  G4double yi = dy*std::sqrt( radical );
  if (p.y() < 0) yi = -yi;
  
  //
  // Get distance from p to the line connecting
  // these two points
  //
  G4double xdi = p.x() - xi,
     ydi = yi - p.y();

  G4double normi = std::sqrt( xdi*xdi + ydi*ydi );
  if (normi < halfTol) return 0;
  xdi /= normi;
  ydi /= normi;
  
  G4double s = 0.5*(xdi*(p.y()-yi) - ydi*(p.x()-xi));
  if (xi*yi < 0) s = -s;
  
  if (s < sBest) sBest = s;
  
  //
  // Return best answer
  //
  return sBest < halfTol ? 0 : sBest;
}


//
// IntersectXY
//
// Decide if and where the x/y trajectory hits the elliptical cross
// section.
//
// Arguments:
//     p     - (in) Point on trajectory
//     v     - (in) Vector along trajectory
//     s     - (out) Up to two points of intersection, where the
//                   intersection point is p + s*v, and if there are
//                   two intersections, s[0] < s[1]. May be negative.
// Returns:
//     The number of intersections. If 0, the trajectory misses. If 1, the 
//     trajectory just grazes the surface.
//
// Solution:
//     One needs to solve: ( (p.x + s*v.x)/dx )**2  + ( (p.y + s*v.y)/dy )**2 = 1
//
//     The solution is quadratic: a*s**2 + b*s + c = 0
//
//           a = (v.x/dx)**2 + (v.y/dy)**2
//           b = 2*p.x*v.x/dx**2 + 2*p.y*v.y/dy**2
//           c = (p.x/dx)**2 + (p.y/dy)**2 - 1
//
G4int G4EllipticalTube::IntersectXY( const G4ThreeVector &p,
                                     const G4ThreeVector &v,
                                           G4double s[2] ) const
{
  G4double px = p.x(), py = p.y();
  G4double vx = v.x(), vy = v.y();
  
  G4double a = (vx/dx)*(vx/dx) + (vy/dy)*(vy/dy);
  G4double b = 2.0*( px*vx/dx/dx + py*vy/dy/dy );
  G4double c = (px/dx)*(px/dx) + (py/dy)*(py/dy) - 1.0;
  
  if (a < DBL_MIN) return 0;      // Trajectory parallel to z axis
  
  G4double radical = b*b - 4*a*c;
  
  if (radical < -DBL_MIN) return 0;    // No solution
  
  if (radical < DBL_MIN)
  {
    //
    // Grazes surface
    //
    s[0] = -b/a/2.0;
    return 1;
  }
  
  radical = std::sqrt(radical);
  
  G4double q = -0.5*( b + (b < 0 ? -radical : +radical) );
  G4double sa = q/a;
  G4double sb = c/q;    
  if (sa < sb) { s[0] = sa; s[1] = sb; } else { s[0] = sb; s[1] = sa; }
  return 2;
}


//
// GetEntityType
//
G4GeometryType G4EllipticalTube::GetEntityType() const
{
  return G4String("G4EllipticalTube");
}


//
// GetCubicVolume
//
G4double G4EllipticalTube::GetCubicVolume()
{
  if(fCubicVolume != 0.) {;}
    else { fCubicVolume = G4VSolid::GetCubicVolume(); }
  return fCubicVolume;
}

//
// GetSurfaceArea
//
G4double G4EllipticalTube::GetSurfaceArea()
{
  if(fSurfaceArea != 0.) {;}
  else   { fSurfaceArea = G4VSolid::GetSurfaceArea(); }
  return fSurfaceArea;
}

//
// Stream object contents to an output stream
//
std::ostream& G4EllipticalTube::StreamInfo(std::ostream& os) const
{
  os << "-----------------------------------------------------------\n"
     << "    *** Dump for solid - " << GetName() << " ***\n"
     << "    ===================================================\n"
     << " Solid type: G4EllipticalTube\n"
     << " Parameters: \n"
     << "    length Z: " << dz/mm << " mm \n"
     << "    surface equation in X and Y: \n"
     << "       (X / " << dx << ")^2 + (Y / " << dy << ")^2 = 1 \n"
     << "-----------------------------------------------------------\n";

  return os;
}


//
// GetPointOnSurface
//
// Randomly generates a point on the surface, 
// with ~ uniform distribution across surface.
//
G4ThreeVector G4EllipticalTube::GetPointOnSurface() const
{
  G4double xRand, yRand, zRand, phi, cosphi, sinphi, zArea, cArea,p, chose;

  phi    = RandFlat::shoot(0., 2.*pi);
  cosphi = std::cos(phi);
  sinphi = std::sin(phi);
  
  // the ellipse perimeter from: "http://mathworld.wolfram.com/Ellipse.html"
  //   m = (dx - dy)/(dx + dy);
  //   k = 1.+1./4.*m*m+1./64.*sqr(m)*sqr(m)+1./256.*sqr(m)*sqr(m)*sqr(m);
  //   p = pi*(a+b)*k;

  // perimeter below from "http://www.efunda.com/math/areas/EllipseGen.cfm"

  p = 2.*pi*std::sqrt(0.5*(dx*dx+dy*dy));

  cArea = 2.*dz*p;
  zArea = pi*dx*dy;

  xRand = dx*cosphi;
  yRand = dy*sinphi;
  zRand = RandFlat::shoot(dz, -1.*dz);
    
  chose = RandFlat::shoot(0.,2.*zArea+cArea);
  
  if( (chose>=0) && (chose < cArea) )
  {
    return G4ThreeVector (xRand,yRand,zRand);
  }
  else if( (chose >= cArea) && (chose < cArea + zArea) )
  {
    xRand = RandFlat::shoot(-1.*dx,dx);
    yRand = std::sqrt(1.-sqr(xRand/dx));
    yRand = RandFlat::shoot(-1.*yRand, yRand);
    return G4ThreeVector (xRand,yRand,dz); 
  }
  else
  { 
    xRand = RandFlat::shoot(-1.*dx,dx);
    yRand = std::sqrt(1.-sqr(xRand/dx));
    yRand = RandFlat::shoot(-1.*yRand, yRand);
    return G4ThreeVector (xRand,yRand,-1.*dz);
  }
}


//
// CreatePolyhedron
//
G4Polyhedron* G4EllipticalTube::CreatePolyhedron() const
{
  // create cylinder with radius=1...
  //
  G4Polyhedron* eTube = new G4PolyhedronTube(0.,1.,dz);

  // apply non-uniform scaling...
  //
  eTube->Transform(G4Scale3D(dx,dy,1.));
  return  eTube;
}


//
// GetPolyhedron
//
G4Polyhedron* G4EllipticalTube::GetPolyhedron () const
{
  if (!fpPolyhedron ||
      fpPolyhedron->GetNumberOfRotationStepsAtTimeOfCreation() !=
      fpPolyhedron->GetNumberOfRotationSteps())
    {
      delete fpPolyhedron;
      fpPolyhedron = CreatePolyhedron();
    }
  return fpPolyhedron;
}


//
// DescribeYourselfTo
//
void G4EllipticalTube::DescribeYourselfTo( G4VGraphicsScene& scene ) const
{
  scene.AddSolid (*this);
}


//
// GetExtent
//
G4VisExtent G4EllipticalTube::GetExtent() const
{
  return G4VisExtent( -dx, dx, -dy, dy, -dz, dz );
}
