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
/// \file B1/src/RunAction.cc
/// \brief Implementation of the B1::RunAction class

#include "RunAction.hh"
#include "PrimaryGeneratorAction.hh"
#include "DetectorConstruction.hh"
// #include "Run.hh"
#include "G4AnalysisManager.hh"
#include "G4RunManager.hh"
#include "G4Run.hh"
#include "G4AccumulableManager.hh"
#include "G4LogicalVolumeStore.hh"
#include "G4LogicalVolume.hh"
#include "G4UnitsTable.hh"
#include "G4SystemOfUnits.hh"

namespace B1
{

  //....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

  RunAction::RunAction()
  {
    // add new units for dose
    //
    // const G4double milligray = 1.e-3*gray;
    // const G4double microgray = 1.e-6*gray;
    // const G4double nanogray  = 1.e-9*gray;
    // const G4double picogray  = 1.e-12*gray;

    // new G4UnitDefinition("milligray", "milliGy" , "Dose", milligray);
    // new G4UnitDefinition("microgray", "microGy" , "Dose", microgray);
    // new G4UnitDefinition("nanogray" , "nanoGy"  , "Dose", nanogray);
    // new G4UnitDefinition("picogray" , "picoGy"  , "Dose", picogray);

    // // Register accumulable to the accumulable manager
    // G4AccumulableManager* accumulableManager = G4AccumulableManager::Instance();
    // accumulableManager->RegisterAccumulable(fEdep);
    // accumulableManager->RegisterAccumulable(fEdep2);
  }

  //....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

  void RunAction::BeginOfRunAction(const G4Run *)
  {
    // inform the runManager to save random number seed
    G4RunManager::GetRunManager()->SetRandomNumberStore(false);

    // reset accumulables to their initial values
    G4AccumulableManager *accumulableManager = G4AccumulableManager::Instance();
    accumulableManager->Reset();
    // Get analysis manager
    auto analysisManager = G4AnalysisManager::Instance();

    // Open an output file
    //
    G4String fileName = "test/B4.root";
    // Other supported output types:
    // G4String fileName = "B4.csv";
    // G4String fileName = "B4.hdf5";
    // G4String fileName = "B4.xml";
    analysisManager->OpenFile(fileName);
    G4cout << "Using " << analysisManager->GetType() << G4endl;

    analysisManager->SetVerboseLevel(1);
  analysisManager->SetNtupleMerging(true);
    // Note: merging ntuples is available only with Root output

  // Book histograms, ntuple
  //

  // Creating histograms
  // analysisManager->CreateH1("Eabs","Edep in absorber", 100, 0., 800*MeV);
  // analysisManager->CreateH1("Egap","Edep in gap", 100, 0., 100*MeV);
  // analysisManager->CreateH1("Labs","trackL in absorber", 100, 0., 1*m);
  // analysisManager->CreateH1("Lgap","trackL in gap", 100, 0., 50*cm);

  // Creating ntuple
  //
  analysisManager->CreateH2("Reconstruction plane info","zxplane distribution",1000,-6*m,6*m,1000,-6*m,6*m);
  // analysisManager->CreateH2("Reconstruction plane info","xyplane distribution",1000,-3*m,3*m,1000,-3*m,3*m);
  // analysisManager->CreateH2("Reconstruction plane info","yzplane distribution",1000,-3*m,3*m,1000,-3*m,3*m);
  analysisManager->CreateNtuple("B1", "reconstruction info");
  analysisManager->CreateNtupleDColumn("DirecX");
  analysisManager->CreateNtupleDColumn("DirecY");
  analysisManager->CreateNtupleDColumn("DirecZ");
  analysisManager->FinishNtuple();

  // analysisManager->CreateNtuple("B1", "hit direction and position");
  // analysisManager->CreateNtupleDColumn("DirecX");
  // analysisManager->CreateNtupleDColumn("DirecY");
  // analysisManager->CreateNtupleDColumn("DirecZ");
  // analysisManager->CreateNtupleDColumn("PosX");
  // analysisManager->CreateNtupleDColumn("PosY");
  // analysisManager->CreateNtupleDColumn("PosZ");

  // analysisManager->FinishNtuple();

  }

  //....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

  void RunAction::EndOfRunAction(const G4Run *run)
  {
    auto analysisManager = G4AnalysisManager::Instance();
    analysisManager->Write();
    analysisManager->CloseFile();
  }

  //....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......


  //....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

}
