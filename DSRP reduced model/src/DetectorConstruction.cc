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
#include "G4Cons.hh"
#include "G4LogicalVolume.hh"
#include "G4SubtractionSolid.hh"
#include "G4NistManager.hh"
#include "G4PVPlacement.hh"
#include "G4SystemOfUnits.hh"
#include "G4Trd.hh"

namespace B1
{

    //....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

    G4VPhysicalVolume* DetectorConstruction::Construct()
    {
        // Get nist material manager
        G4NistManager* nist = G4NistManager::Instance();

        // Envelope parameters
        //
        G4double env_sizeXY = 30 * cm, env_sizeZ = 30 * cm;
        G4Material* env_mat = nist->FindOrBuildMaterial("G4_WATER");

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
        auto solidEnv = new G4Box("Envelope",  // its name
            0.5 * env_sizeXY, 0.5 * env_sizeXY, 0.5 * env_sizeZ);  // its size

        auto logicEnv = new G4LogicalVolume(solidEnv,  // its solid
            world_mat,  // its material
            "Envelope");  // its name

        new G4PVPlacement(nullptr,  // no rotation
            G4ThreeVector(),  // at (0,0,0)
            logicEnv,  // its logical volume
            "Envelope",  // its name
            logicWorld,  // its mother  volume
            false,  // no boolean operation
            0,  // copy number
            checkOverlaps);  // overlaps checking
        ////Al shielding
        auto Al_shielding = nist->FindOrBuildMaterial("G4_Al");

        G4double sh_x = 100/2 * mm;
        G4double sh_y = 100/2 * mm;
        G4double sh_z = 2.5/2 * mm;

        auto solidShielding = new G4Box("Chassis",
            sh_x, sh_y, sh_z);
        auto logicshielding = new G4LogicalVolume(solidShielding,  // its solid
            Al_shielding,  // its material
            "Chassis");  // its name
        new G4PVPlacement(nullptr,
            G4ThreeVector(0,0,-10),
            logicshielding,
            "Chassis",
            logicEnv,
            false,
            0,
            checkOverlaps);
        new G4PVPlacement(nullptr,
            G4ThreeVector(0, 0, 15),
            logicshielding,
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
        new G4PVPlacement(nullptr, 
            G4ThreeVector(0,0,2),
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
        new G4PVPlacement(nullptr,
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

    //....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

}  // namespace B1

//namespace B1
//{
//
//    //....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//
//    G4VPhysicalVolume* DetectorConstruction::Construct()
//    {
//        // Get nist material manager
//        G4NistManager* nist = G4NistManager::Instance();
//
//        // Envelope parameters
//        //
//        G4double env_sizeXY = 5 * mm, env_sizeZ = 6 * mm;
//        G4Material* env_mat = nist->FindOrBuildMaterial("G4_WATER");
//
//        // Option to switch on/off checking of volumes overlaps
//        //
//        G4bool checkOverlaps = true;
//
//        //
//        // World
//        //
//        G4double world_sizeXY = 1.2 * env_sizeXY;
//        G4double world_sizeZ = 1.2 * env_sizeZ;
//        G4Material* world_mat = nist->FindOrBuildMaterial("G4_AIR");
//
//        auto solidWorld =
//            new G4Box("World",  // its name
//                0.5 * world_sizeXY, 0.5 * world_sizeXY, 0.5 * world_sizeZ);  // its size
//
//        auto logicWorld = new G4LogicalVolume(solidWorld,  // its solid
//            world_mat,  // its material
//            "World");  // its name
//
//        auto physWorld = new G4PVPlacement(nullptr,  // no rotation
//            G4ThreeVector(),  // at (0,0,0)
//            logicWorld,  // its logical volume
//            "World",  // its name
//            nullptr,  // its mother  volume
//            false,  // no boolean operation
//            0,  // copy number
//            checkOverlaps);  // overlaps checking
//
//        //
//        // Envelope
//        //
//        auto solidEnv = new G4Box("Envelope",  // its name
//            0.5 * env_sizeXY, 0.5 * env_sizeXY, 0.5 * env_sizeZ);  // its size
//        //auto Al_air = nist->FindOrBuildMaterial("G4_AIR");
//        auto logicEnv = new G4LogicalVolume(solidEnv,  // its solid
//            env_mat,  // its material
//            "Envelope");  // its name
//
//        new G4PVPlacement(nullptr,  // no rotation
//            G4ThreeVector(),  // at (0,0,0)
//            logicEnv,  // its logical volume
//            "Envelope",  // its name
//            logicWorld,  // its mother  volume
//            false,  // no boolean operation
//            0,  // copy number
//            checkOverlaps);  // overlaps checking
//
//        
//        //// Shape 1
//        ////
//        //G4Material* shape1_mat = nist->FindOrBuildMaterial("G4_A-150_TISSUE");
//        //G4ThreeVector pos1 = G4ThreeVector(0, 2 * cm, -7 * cm);
//
//        //// Conical section shape
//        //G4double shape1_rmina = 0. * cm, shape1_rmaxa = 2. * cm;
//        //G4double shape1_rminb = 0. * cm, shape1_rmaxb = 4. * cm;
//        //G4double shape1_hz = 3. * cm;
//        //G4double shape1_phimin = 0. * deg, shape1_phimax = 360. * deg;
//        //auto solidShape1 = new G4Cons("Shape1", shape1_rmina, shape1_rmaxa, shape1_rminb, shape1_rmaxb,
//        //    shape1_hz, shape1_phimin, shape1_phimax);
//
//        //auto logicShape1 = new G4LogicalVolume(solidShape1,  // its solid
//        //    shape1_mat,  // its material
//        //    "Shape1");  // its name
//
//        //new G4PVPlacement(nullptr,  // no rotation
//        //    pos1,  // at position
//        //    logicShape1,  // its logical volume
//        //    "Shape1",  // its name
//        //    logicEnv,  // its mother  volume
//        //    false,  // no boolean operation
//        //    0,  // copy number
//        //    checkOverlaps);  // overlaps checking
//         // ----------------------------------------------------
//     ////2. Aluminum shielding box (2.8 mm thick)
//     //----------------------------------------------------
//     //   G4double box_inner = 1.6 * mm;       // you may modify
//     //   G4double shell_thick = 0.3 * mm;
//     //   G4double box_outer = box_inner + 2 * shell_thick;
//
//     //   //auto Al_mat = nist->FindOrBuildMaterial("G4_Al");
//     //   auto plastic_mat = nist->FindOrBuildMaterial("G4_POLYETHYLENE");
//     //   // outer box
//     //   auto solidBoxOuter = new G4Box("AlBoxOuter",
//     //       box_outer / 2, box_outer / 2, box_outer / 2);
//
//     //   // inner hollow region (cavity)
//     //   auto solidBoxInner = new G4Box("AlBoxInner",
//     //       box_inner / 2, box_inner / 2, box_inner / 2);
//
//     //   // subtraction to create shell
//     //   auto solidAlShell = new G4SubtractionSolid(
//     //       "AlShell", solidBoxOuter, solidBoxInner);
//
//     //   auto logicAlShell
//     //       = new G4LogicalVolume(solidAlShell, plastic_mat, "AlShell");
//
//     //   new G4PVPlacement(nullptr, G4ThreeVector(),
//     //       logicAlShell, "AlShell",
//     //       logicWorld, false, 0, checkOverlaps);
//        ////// PCB
//        //auto plastic_mat = nist->FindOrBuildMaterial("G4_POLYETHYLENE");
//
//        //G4double sot_x = 1.9 * mm;
//        //G4double sot_y = 1.6 * mm;
//        //G4double sot_z = 0.3 * mm;
//
//        //auto solidSOT = new G4Box("SOT23",
//        //    sot_x, sot_y, sot_z);
//
//        //auto logicSOT = new G4LogicalVolume(solidSOT, plastic_mat, "SOT23");
//
//        //// place SOT23 at cavity center
//        //new G4PVPlacement(nullptr, G4ThreeVector(),
//        //    logicSOT, "SOT23",
//        //    logicEnv, false, 0, checkOverlaps);
//
//        //// test shape
//        //auto plastic_mat = nist->FindOrBuildMaterial("G4_POLYETHYLENE");
//
//        //G4double sot_x = 10 * cm;
//        //G4double sot_y = 10 * cm;
//        //G4double sot_z = 1 * cm;
//
//        //auto solidSOT = new G4Box("SOT23",
//        //    sot_x, sot_y, sot_z);
//
//        //auto logicSOT = new G4LogicalVolume(solidSOT, plastic_mat, "SOT23");
//
//        //// place SOT23 at cavity center
//        //new G4PVPlacement(nullptr, G4ThreeVector(),
//        //    logicSOT, "SOT23",
//        //    logicEnv, false, 0, checkOverlaps);
//
//        //
//        //// Shape 2
//        ////
//        //G4Material* shape2_mat = nist->FindOrBuildMaterial("G4_BONE_COMPACT_ICRU");
//        //G4ThreeVector pos2 = G4ThreeVector(0, -1 * cm, 7 * cm);
//
//        //// Trapezoid shape
//        //G4double shape2_dxa = 12 * cm, shape2_dxb = 12 * cm;
//        //G4double shape2_dya = 10 * cm, shape2_dyb = 16 * cm;
//        //G4double shape2_dz = 6 * cm;
//        //auto solidShape2 =
//        //    new G4Trd("Shape2",  // its name
//        //        0.5 * shape2_dxa, 0.5 * shape2_dxb, 0.5 * shape2_dya, 0.5 * shape2_dyb,
//        //        0.5 * shape2_dz);  // its size
//
//        //auto logicShape2 = new G4LogicalVolume(solidShape2,  // its solid
//        //    shape2_mat,  // its material
//        //    "Shape2");  // its name
//
//        //new G4PVPlacement(nullptr,  // no rotation
//        //    pos2,  // at position
//        //    logicShape2,  // its logical volume
//        //    "Shape2",  // its name
//        //    logicEnv,  // its mother  volume
//        //    false,  // no boolean operation
//        //    0,  // copy number
//        //    checkOverlaps);  // overlaps checking
//
//        //// Set Shape2 as scoring volume
//        ////
//        //fScoringVolume = logicShape2;
//
//        
//        //always return the physical World
//        
//        return physWorld;
//    }
//
//    //....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//
//}  // namespace B1
