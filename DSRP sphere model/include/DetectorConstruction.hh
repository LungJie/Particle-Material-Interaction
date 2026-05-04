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
/// \file B1/include/DetectorConstruction.hh
/// \brief Definition of the B1::DetectorConstruction class

#ifndef B1DetectorConstruction_h
#define B1DetectorConstruction_h 1

#include "G4VUserDetectorConstruction.hh"
#include "G4RotationMatrix.hh"
#include "G4GenericMessenger.hh"

class G4VPhysicalVolume;
class G4LogicalVolume;
class G4GenericMessenger;


namespace B1
{

/// Detector construction class to define materials and geometry.

class DetectorConstruction : public G4VUserDetectorConstruction
{
  public:
    DetectorConstruction();
    ~DetectorConstruction() override;

    G4VPhysicalVolume* Construct() override;

    G4LogicalVolume* GetScoringVolume() const { return fScoringVolume; }
    void SetAngle(G4double val);
  protected:
    G4LogicalVolume* fScoringVolume = nullptr;
  private:
    // ⭐ 角度
    G4double fAngle = 0.;

    // ⭐ rotation matrix
    G4RotationMatrix* fRotation = nullptr;
    //G4RotationMatrix* fRotation_xy = nullptr;

    // ⭐ physical volume（讓你可以動它）
    G4VPhysicalVolume* fDetectorPhys_z1 = nullptr;
    G4VPhysicalVolume* fDetectorPhys_z2 = nullptr;
    G4VPhysicalVolume* fDetectorPhys_x1 = nullptr;
    G4VPhysicalVolume* fDetectorPhys_x2 = nullptr;
    G4VPhysicalVolume* fDetectorPhys_y1 = nullptr;
    G4VPhysicalVolume* fDetectorPhys_y2 = nullptr;
    G4VPhysicalVolume* fDetectorPhys_PCB = nullptr;
    G4VPhysicalVolume* fDetectorPhys_RADFET =nullptr;

    //G4VPhysicalVolume* fDetectorPhys_xy = nullptr;

    // ⭐ messenger（讓 macro 控制）
    G4GenericMessenger* fMessenger = nullptr;

    void DefineCommands();
};

}  // namespace B1

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif
