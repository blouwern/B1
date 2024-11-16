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

#include "G4RunManager.hh"
#include "G4NistManager.hh"
#include "G4Box.hh"
#include "G4Cons.hh"
#include "G4Orb.hh"
#include "G4Sphere.hh"
#include "G4Trd.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4SystemOfUnits.hh"
#include "TrackerSD.hh"
#include "G4SDManager.hh"

namespace B1
{

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4VPhysicalVolume* DetectorConstruction::Construct()
{
  // Get nist material manager
  G4NistManager* nist = G4NistManager::Instance();

  // Tracker parameters
  //
  G4Material* env_mat = nist->FindOrBuildMaterial("G4_WATER");
  G4Material* world_mat = nist->FindOrBuildMaterial("G4_AIR");

  // Option to switch on/off checking of volumes overlaps
  //
  G4bool checkOverlaps = true;

  //
  // World
  //
  const auto world_sizeXYZ{2*m};
  auto solidWorld = new G4Box("World",                           // its name
    0.5 * world_sizeXYZ, 0.5 * world_sizeXYZ, 0.5 * world_sizeXYZ);  // its size

  auto logicWorld = new G4LogicalVolume(solidWorld,  // its solid
    world_mat,                                       // its material
    "World");                                        // its name

  auto physWorld = new G4PVPlacement(nullptr,  // no rotation
    G4ThreeVector(),                           // at (0,0,0)
    logicWorld,                                // its logical volume
    "World",                                   // its name
    nullptr,                                   // its mother  volume
    false,                                     // no boolean operation
    0,                                         // copy number
    checkOverlaps);                            // overlaps checking

  //
  // Tracker
  //
  const auto tracker_sizeXZ{world_sizeXYZ};
  const auto tracker_sizeY{1*m};
  auto solidTracker = new G4Box("Tracker",                    // its name
    0.5 * tracker_sizeXZ, 0.5 * tracker_sizeY, 0.5 * tracker_sizeXZ);  // its size

  fLogicTracker = new G4LogicalVolume(solidTracker,  // its solid
    env_mat,                                     // its material
    "Tracker");                                 // its name

  new G4PVPlacement(nullptr,  // no rotation
    G4ThreeVector(0,-world_sizeXYZ*0.5+tracker_sizeY*0.5,0),          // at (0,0,0)
    fLogicTracker,                 // its logical volume
    "Tracker",               // its name
    logicWorld,               // its mother  volume
    false,                    // no boolean operation
    0,                        // copy number
    checkOverlaps);           // overlaps checking
  const auto blockerlength{30*cm};
  const auto pb{G4NistManager::Instance()->FindOrBuildMaterial("G4_Pb")};
  const auto blockerSize{G4ThreeVector{blockerlength,blockerlength,blockerlength}};
  const auto solidBlocker{new G4Box("temp",blockerSize.getX()/2,blockerSize.getY()/2,blockerSize.getZ()/2)};
  const auto blockerTransform{G4Transform3D{G4RotationMatrix::IDENTITY,G4ThreeVector{0.6*m,-0.6*m,0}}};
  const auto logicBlocker{new G4LogicalVolume(solidBlocker,pb,"Blocker")};
  const auto phyBlocker{new G4PVPlacement(blockerTransform,logicBlocker,"Blocker",logicWorld,false,0,checkOverlaps)};


  //
  //always return the physical World
  //
  return physWorld;
}


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void DetectorConstruction::ConstructSDandField()
{
  // Sensitive detectors

  G4String trackerChamberSDname = "B2/TrackerSD";
  auto aTrackerSD = new TrackerSD(trackerChamberSDname, "TrackerHitsCollection");
  G4SDManager::GetSDMpointer()->AddNewDetector(aTrackerSD);
  SetSensitiveDetector( fLogicTracker,  aTrackerSD );

  // Create global magnetic field messenger.
  // Uniform magnetic field is then created automatically if
  // the field value is not zero.
}



}
