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
/// \file B1/src/DetectorConstruction.cc
/// \brief Implementation of the B1::DetectorConstruction class

#include "DetectorConstruction.hh"
#include "G4Box.hh"
#include "G4Sphere.hh"
#include "G4Cons.hh"
#include "G4LogicalVolume.hh"
#include "G4SubtractionSolid.hh"
#include "G4NistManager.hh"
#include "G4GenericMessenger.hh"
#include "G4RotationMatrix.hh"
#include "G4PVPlacement.hh"
#include "G4SystemOfUnits.hh"
#include "G4Trd.hh"
#include "G4RunManager.hh"
#include "G4AssemblyVolume.hh"


namespace B1
{
    DetectorConstruction::DetectorConstruction()
    {
        fRotation = new G4RotationMatrix();
        fRotation->rotateY(fAngle);
        //fRotation_xy = new G4RotationMatrix();
        //fRotation_xy->rotateY(fAngle);
        // define commands for this class
        DefineCommands();
    }

    //....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

    DetectorConstruction::~DetectorConstruction()
    {
        delete fRotation;
        //delete fRotation_xy;
        delete fMessenger;
    }

    //....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

    G4VPhysicalVolume* DetectorConstruction::Construct()
    {
        // Get nist material manager
        G4NistManager* nist = G4NistManager::Instance();

        // Envelope parameters
        //
        G4double env_sizeXY = 30 * cm, env_sizeZ = 30 * cm;
        G4Material* env_mat = nist->FindOrBuildMaterial("G4_Galactic");

        // Option to switch on/off checking of volumes overlaps
        //
        G4bool checkOverlaps = true;

        //
        // World
        //
        G4double world_sizeXY = 1.2 * env_sizeXY;
        G4double world_sizeZ = 1.2 * env_sizeZ;
        G4Material* world_mat = nist->FindOrBuildMaterial("G4_Galactic");

        auto solidWorld =
            new G4Box("World",  // its name
                0.5 * world_sizeXY, 0.5 * world_sizeXY, 0.5 * world_sizeZ);  // its size

        auto logicWorld = new G4LogicalVolume(solidWorld,  // its solid
            world_mat,  // its material
            "World");  // its name

        auto physWorld = new G4PVPlacement(nullptr,  // no rotation
            G4ThreeVector(),  // at (0,0,0)
            logicWorld,  // its logical volume
            "World",  // its name
            nullptr,  // its mother  volume
            false,  // no boolean operation
            0,  // copy number
            checkOverlaps);  // overlaps checking

        //
        // Envelope
        //
        //// Envelope (改成球形)
        //G4double env_radius = 15 * cm;  // 原本30 cm box → 半徑抓15 cm

        //auto solidEnv = new G4Sphere("Envelope",
        //    0.0,              // inner radius
        //    env_radius,       // outer radius
        //    0.0,              // phi start
        //    360.0 * deg,      // phi span
        //    0.0,              // theta start
        //    180.0 * deg);     // theta span

        //auto logicEnv = new G4LogicalVolume(solidEnv,
        //    env_mat,
        //    "Envelope");

        //new G4PVPlacement(nullptr,
        //    G4ThreeVector(),
        //    logicEnv,
        //    "Envelope",
        //    logicWorld,
        //    false,
        //    0,
        //    checkOverlaps);
        auto solidEnv = new G4Box("Envelope",  // its name
            0.5 * env_sizeXY, 0.5 * env_sizeXY, 0.5 * env_sizeZ);  // its size

        auto logicEnv = new G4LogicalVolume(solidEnv,  // its solid
            env_mat,  // its material
            "Envelope");  // its name

        new G4PVPlacement(nullptr,  // no rotation
            G4ThreeVector(),  // at (0,0,0)
            logicEnv,  // its logical volume
            "Envelope",  // its name
            logicWorld,  // its mother  volume
            false,  // no boolean operation
            0,  // copy number
            checkOverlaps);  // overlaps checking
        //Al shielding +z -z
        auto Al_shielding = nist->FindOrBuildMaterial("G4_Al");

        G4double sh_x = 100/2 * mm;
        G4double sh_y = 100/2 * mm;
        G4double sh_z = 2.5/2 * mm;

        auto solidShielding_z = new G4Box("Chassis",
            sh_x, sh_y, sh_z);
        auto logicshielding_z = new G4LogicalVolume(solidShielding_z,  // its solid
            Al_shielding,  // its material
            "Chassis");  // its name
        auto x_z = 25/2. * mm * std::sin(fAngle); //0
        auto z_z = 25/2. * mm * std::cos(fAngle); //25/2
        fDetectorPhys_z1 =new G4PVPlacement(fRotation,
            G4ThreeVector(x_z,0, -z_z),
            logicshielding_z,
            "Chassis",
            logicEnv,
            false,
            0,
            checkOverlaps);
        fDetectorPhys_z2 =new G4PVPlacement(fRotation,
            G4ThreeVector(-x_z, 0, z_z),
            logicshielding_z,
            "Chassis",
            logicEnv,
            false,
            0,
            checkOverlaps);
        //Al shielding +x -x
        G4double x_sh_x = 2.5 / 2 * mm;
        G4double x_sh_y = 100/2 * mm;
        G4double x_sh_z = 25 / 2 * mm;

        auto solidShielding_x = new G4Box("Chassis",
            x_sh_x, x_sh_y, x_sh_z);
        auto logicshielding_x = new G4LogicalVolume(solidShielding_x,  // its solid
            Al_shielding,  // its material
            "Chassis");  // its name
        //auto rot_x = new G4RotationMatrix();
        //rot_x->rotateY(90 * deg);
        //rot_x->rotateX(90 * deg);
        auto x_x = 100 / 2 * mm * std::cos(fAngle);
        auto z_x = 100 / 2 * mm * std::sin(fAngle);
        //+x 
        fDetectorPhys_x1 =new G4PVPlacement(fRotation,
            G4ThreeVector(x_x, 0, z_x),
            logicshielding_x,
            "Chassis",
            logicEnv,
            false,
            0,
            checkOverlaps);
        //-x
        fDetectorPhys_x2 =new G4PVPlacement(fRotation,
            G4ThreeVector(-x_x, 0, -z_x),
            logicshielding_x,
            "Chassis",
            logicEnv,
            false,
            0,
            checkOverlaps);

        //Al shielding +y -y

        G4double y_sh_x = 100/2 * mm;
        G4double y_sh_y = 2.5 / 2 * mm;
        G4double y_sh_z = 25 / 2 * mm;

        auto solidShielding_y = new G4Box("Chassis",
            y_sh_x, y_sh_y, y_sh_z);
        //auto rot_y = new G4RotationMatrix();
        //rot_y->rotateX(90 * deg);
        auto logicshielding_y = new G4LogicalVolume(solidShielding_y,  // its solid
            Al_shielding,  // its material
            "Chassis");  // its name
        //+y
        fDetectorPhys_y1 =new G4PVPlacement(fRotation,
            G4ThreeVector(0, 50, 0),
            logicshielding_y,
            "Chassis",
            logicEnv,
            false,
            0,
            checkOverlaps);
        //-y
        fDetectorPhys_y2 =new G4PVPlacement(fRotation,
            G4ThreeVector(0, -50, 0),
            logicshielding_y,
            "Chassis",
            logicEnv,
            false,
            0,
            checkOverlaps);
        //PCB
        G4Element* H  = new G4Element("Hydrogen", "H", 1., 1.01*g/mole);
        G4Element* C = new G4Element("Carbon", "C", 6., 12.01 * g / mole);
        G4Element* O = new G4Element("Oxygen", "O", 8., 16.00 * g / mole);
        G4Element* Si = new G4Element("Silicon", "Si", 14., 28.09 * g / mole);
        G4Element* Cu = new G4Element("Copper", "Cu", 29., 63.55 * g / mole);
        G4Material* FR4 = new G4Material("FR4", 1.85 * g / cm3, 4); // 密度可依文獻調整
        FR4->AddElement(C, 0.50);
        FR4->AddElement(H, 0.05);
        FR4->AddElement(O, 0.35);
        FR4->AddElement(Si, 0.10);
        auto solidPCB = new G4Box("PCB",
            80/2, 80/2, 1.6/2);

        auto logicPCB = new G4LogicalVolume(solidPCB,  // its solid
            FR4,  // its material
            "PCB");  // its name
        auto x_PCB = 2 * mm * std::sin(fAngle);
        auto z_PCB = 2 * mm * std::cos(fAngle);
        fDetectorPhys_PCB =new G4PVPlacement(fRotation,
            G4ThreeVector(-x_PCB,0, z_PCB),
            logicPCB,
            "PCB",
            logicEnv, 
            false, 
            0, 
            checkOverlaps);

        //RADFET
        auto sio2_mat = nist->FindOrBuildMaterial("G4_SILICON_DIOXIDE");

        G4double sio_x = 50/2 * mm; //300mum=0.3mm
        G4double sio_y = 50/2 * mm; //50mum=0.05mm
        G4double sio_z = 0.0004/2 * mm; //400nm=0.0004mm

        auto solidsio = new G4Box("RADFET",
            sio_x, sio_y, sio_z);
        auto logicsio = new G4LogicalVolume(solidsio,  // its solid
            sio2_mat,  // its material
            "RADFET");  // its name
        // place SOT23 at cavity center
        fDetectorPhys_RADFET =new G4PVPlacement(fRotation,
            G4ThreeVector(0, 0, 0),
            logicsio,
            "RADFET",
            logicEnv,
            false,
            0,
            checkOverlaps);
        ////
        fScoringVolume = logicsio;

        //
        // always return the physical World
        //
        return physWorld;
    }
    void DetectorConstruction::SetAngle(G4double val)
    {
        if (!fDetectorPhys_z1) {
            G4cerr << "Detector has not yet been constructed." << G4endl;
            return;
        }
        if (!fDetectorPhys_z2) {
            G4cerr << "Detector has not yet been constructed." << G4endl;
            return;
        }
        if (!fDetectorPhys_x1) {
            G4cerr << "Detector has not yet been constructed." << G4endl;
            return;
        }
        if (!fDetectorPhys_x2) {
            G4cerr << "Detector has not yet been constructed." << G4endl;
            return;
        }
        if (!fDetectorPhys_y1) {
            G4cerr << "Detector has not yet been constructed." << G4endl;
            return;
        }
        if (!fDetectorPhys_y2) {
            G4cerr << "Detector has not yet been constructed." << G4endl;
            return;
        }
        if (!fDetectorPhys_PCB) {
            G4cerr << "Detector has not yet been constructed." << G4endl;
            return;
        }

        if (!fDetectorPhys_RADFET) {
            G4cerr << "Detector has not yet been constructed." << G4endl;
            return;
        }

        fAngle = val;
        *fRotation = G4RotationMatrix();  // make it unit vector
        fRotation->rotateY(fAngle);
        auto x_z = 25 / 2*mm * std::sin(fAngle);
        auto z_z = 25 / 2 * mm * std::cos(fAngle);
        fDetectorPhys_z1->SetTranslation(G4ThreeVector(x_z, 0., -z_z));
        fDetectorPhys_z2->SetTranslation(G4ThreeVector(-x_z, 0., z_z));
        auto x_x = 100 / 2 * mm * std::cos(fAngle);
        auto z_x = 100 / 2 * mm * std::sin(fAngle);
        fDetectorPhys_x1->SetTranslation(G4ThreeVector(x_x, 0., z_x));
        fDetectorPhys_x2->SetTranslation(G4ThreeVector(-x_x, 0., -z_x));
        //auto x_y = 100 / 2 * mm * std::cos(fAngle);
        //auto z_y = 100 / 2 * mm * std::sin(fAngle);
        fDetectorPhys_y1->SetTranslation(G4ThreeVector(0, 50., 0));
        fDetectorPhys_y2->SetTranslation(G4ThreeVector(0, -50., 0));
        auto x_PCB = 2  * mm * std::sin(fAngle);
        auto z_PCB = 2  * mm * std::cos(fAngle);
        fDetectorPhys_PCB->SetTranslation(G4ThreeVector(-x_PCB, 0., z_PCB));
        fDetectorPhys_RADFET->SetTranslation(G4ThreeVector(0, 0., 0));
        // tell G4RunManager that we change the geometry
        G4RunManager::GetRunManager()->GeometryHasBeenModified();
    
        //if (!fDetectorPhys_xy) {
        //    G4cerr << "Detector has not yet been constructed." << G4endl;
        //    return;
        //}

        //fAngle = val;
        //*fRotation_xy = G4RotationMatrix();  // make it unit vector
        //fRotation_xy->rotateY(fAngle);
        //auto x_xy = -50*mm*std::cos(fAngle);
        //auto z_xy = 50*mm*std::sin(fAngle);
        //fDetectorPhys_xy->SetTranslation(G4ThreeVector(x_xy, 0., z_xy));
        //// tell G4RunManager that we change the geometry
        //G4RunManager::GetRunManager()->GeometryHasBeenModified();
    }
    //....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

    void DetectorConstruction::DefineCommands()
    {
        // Define /B5/detector command directory using generic messenger class
        fMessenger = new G4GenericMessenger(this, "/B5/detector/", "Detector control");

        // armAngle command
        auto& AngleCmd = fMessenger->DeclareMethodWithUnit(
            "SetAngle", "deg", &DetectorConstruction::SetAngle, "Set rotation angle of the second arm.");
        AngleCmd.SetParameterName("angle", true);
        AngleCmd.SetRange("angle>=0. && angle<180.");
        AngleCmd.SetDefaultValue("30.");
    }
    //....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

}  // namespace B1

