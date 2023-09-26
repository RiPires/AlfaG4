#include "stepping.hh"

MySteppingAction::MySteppingAction(MyEventAction *eventAction)
{
    EventAction = eventAction;
}

MySteppingAction::~MySteppingAction()
{}

void MySteppingAction::UserSteppingAction(const G4Step *step)
{
    G4LogicalVolume *volume = // Creates variable 'volume' which corresponds to the volume where the   //
    step->GetPreStepPoint()->GetTouchableHandle()->GetVolume()->GetLogicalVolume();// step is occuring //
    
    const MyDetectorConstruction *detectorConstruction = 
    static_cast<const MyDetectorConstruction*> (G4RunManager::GetRunManager()->GetUserDetectorConstruction());
    
    G4LogicalVolume *ScoringVolume = // Creates variable 'ScoringVolume' from the SensitiveDetector volume //
    detectorConstruction->GetScoringVolume();// deffined in construction.cc                                //
    
    if(volume != ScoringVolume) // If the 'volume' is not the SensitiveDetector  //  
        return;                 //   do nothing                                  //
    
    G4double edep = step->GetTotalEnergyDeposit(); // Else, we record the energy deposition 'edep' on the step //
    EventAction->AddEdep(edep);                    // and add it to the total energy deposited in the event    //  
}
