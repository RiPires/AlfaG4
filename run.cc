#include "run.hh"

MyRunAction::MyRunAction()
{
    G4AnalysisManager *man = G4AnalysisManager::Instance();
    
    man->CreateNtuple("Photons", "Photons");
    man->CreateNtupleIColumn("EventNr");
    man->CreateNtupleDColumn("EventX");
    man->CreateNtupleDColumn("EventY");
    man->CreateNtupleDColumn("EventZ");
    man->CreateNtupleDColumn("Photon_cdo");
    man->CreateNtupleDColumn("Photon_Energy");
    man->FinishNtuple(0);
    
    man->CreateNtuple("DetInfo", "DetInfo");
    man->CreateNtupleIColumn("EventNr");
    man->CreateNtupleDColumn("DetX");
    man->CreateNtupleDColumn("DetY");
    man->CreateNtupleDColumn("DetZ");
    man->FinishNtuple(1); 
    
    man->CreateNtuple("Scoring", "Scoring");
    man->CreateNtupleDColumn("Edep");
    man->FinishNtuple(2);
}

MyRunAction::~MyRunAction()
{}


void MyRunAction::BeginOfRunAction(const G4Run* run)
{
    G4AnalysisManager *man = G4AnalysisManager::Instance();
    
    G4int runID = run->GetRunID();
    std::stringstream strRunID;
    strRunID << runID;
    
    man->OpenFile("output"+strRunID.str()+".root");
}

void MyRunAction::EndOfRunAction(const G4Run*)
{
    G4AnalysisManager *man = G4AnalysisManager::Instance();
    man->Write();
    man->CloseFile();
}
