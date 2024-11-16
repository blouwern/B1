#include "TrackerSD.hh"
#include "G4HCofThisEvent.hh"
#include "G4Step.hh"
#include "G4ThreeVector.hh"
#include "G4SDManager.hh"
#include "G4ios.hh"
#include "G4AnalysisManager.hh"
#include "G4SystemOfUnits.hh"

namespace{
  auto ComputeIntersection(G4ThreeVector vertexPlane, G4ThreeVector normPlane, G4ThreeVector vertexLine, G4ThreeVector direcLine) -> G4ThreeVector {
    double lambda{
        normPlane.dot(vertexPlane - vertexLine) / normPlane.dot(direcLine)};
    return vertexLine + direcLine * lambda;
}}
namespace B1{
TrackerSD::TrackerSD(const G4String& name,
                     const G4String& hitsCollectionName)
 : G4VSensitiveDetector(name)
{
  collectionName.insert(hitsCollectionName);
}

void TrackerSD::Initialize(G4HCofThisEvent* hce)
{
  // Create hits collection

  fHitsCollection
    = new TrackerHitsCollection(SensitiveDetectorName, collectionName[0]);

  // Add this collection in hce

  G4int hcID
    = G4SDManager::GetSDMpointer()->GetCollectionID(collectionName[0]);
  hce->AddHitsCollection( hcID, fHitsCollection );
}

G4bool TrackerSD::ProcessHits(G4Step* aStep,
                                     G4TouchableHistory*)
{
  // energy deposit=
  auto direction{aStep->GetTrack()->GetMomentumDirection()};

  if (direction==G4ThreeVector{0,0,0}) return false;

  auto newHit = new TrackerHit();

  newHit->SetTrackID  (aStep->GetTrack()->GetTrackID());
  newHit->SetDirection(direction);
  newHit->SetPos (aStep->GetPostStepPoint()->GetPosition());

  fHitsCollection->insert( newHit );

  //newHit->Print();

  return true;
}

void TrackerSD::EndOfEvent(G4HCofThisEvent*)
{
  for (int particleCount{};particleCount<fHitsCollection->entries();++particleCount){
  // auto direcX{(*fHitsCollection)[particleCount]->GetDirection().getX()};
  // auto direcY{(*fHitsCollection)[particleCount]->GetDirection().getY()};
  // auto direcZ{(*fHitsCollection)[particleCount]->GetDirection().getZ()};
  // auto posX{(*fHitsCollection)[particleCount]->GetPos().getX()};
  // auto posY{(*fHitsCollection)[particleCount]->GetPos().getY()};
  // auto posZ{(*fHitsCollection)[particleCount]->GetPos().getZ()};
  auto reconstruction{ComputeIntersection(G4ThreeVector{0,0.3*m,0},
                                          G4ThreeVector{0,1,0},
                                          (*fHitsCollection)[particleCount]->GetPos(),
                                          (*fHitsCollection)[particleCount]->GetDirection())};
  auto analysisManager = G4AnalysisManager::Instance();
  analysisManager->FillH2(0,reconstruction.getZ(),reconstruction.getX());
  // analysisManager->FillH2(1,reconstruction.getX(),reconstruction.getY());
  // analysisManager->FillH2(2,reconstruction.getY(),reconstruction.getZ());
  analysisManager->FillNtupleDColumn(0,reconstruction.getX());
  analysisManager->FillNtupleDColumn(1,reconstruction.getY());
  analysisManager->FillNtupleDColumn(2,reconstruction.getZ());
  analysisManager->AddNtupleRow();
  //debug test
  // analysisManager->FillNtupleDColumn(0,direcX);
  // analysisManager->FillNtupleDColumn(1,direcY);
  // analysisManager->FillNtupleDColumn(2,direcZ);
  // analysisManager->FillNtupleDColumn(3,posX);
  // analysisManager->FillNtupleDColumn(4,posY);
  // analysisManager->FillNtupleDColumn(5,posZ);
  // analysisManager->AddNtupleRow();


  }
}


}