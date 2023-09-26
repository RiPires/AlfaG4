#include "construction.hh"
#include "G4PhysicalConstants.hh"
#include "G4SystemOfUnits.hh"


MyDetectorConstruction::MyDetectorConstruction()
{
    fMessenger = new G4GenericMessenger(this, "/detector/", "DetectorConstruction");
    
    DefineMaterial();
}

MyDetectorConstruction::~MyDetectorConstruction()
{}

void MyDetectorConstruction::DefineMaterial()
{
        //  Materials  //
    G4NistManager *nist = G4NistManager::Instance();

    //   Elements   //
    Al = nist->FindOrBuildElement("Al");
    Au = nist->FindOrBuildElement("Au");
    C = nist->FindOrBuildElement("C");
    Ge = nist->FindOrBuildElement("Ge");
    Si = nist->FindOrBuildElement("Si");
    Sn = nist->FindOrBuildElement("Sn");

    
    
    //   Defines world material as VACUUM   //
        //  Vacuum standard definition  //
        //density = universe_mean_density;
    vacuum = new G4Material("Vacuum", 1., 1.01*g/mole, 1.e-25*g/cm3);
    worldMat = vacuum;
/*
    //   Defines world material as Air  - as it's own properties  //
    worldMat = nist->FindOrBuildMaterial("G4_AIR");
     //   WHICH ENERGY???   //
    G4double energy[2] = {1.239841939*eV/0.2, 1.239841939*eV/0.9};
    //   Refraction indexes   //
    G4double rindexWorld[2] = {1.0, 1.0};
    //   Defines world material refraction index   //
    G4MaterialPropertiesTable *mptWorld = new G4MaterialPropertiesTable();
    mptWorld->AddProperty("RINDEX", energy, rindexWorld, 2);
    worldMat->SetMaterialPropertiesTable(mptWorld);
*/

    //   Defines target Holder material as Al   //
    holderMat = new G4Material("Aluminium", 2.7*g/cm3, 1);
    holderMat->AddElement(Al, 1.);
    
    //   Defines target material as Sn   //
    targetMat = new G4Material("Tin", 7.31*g/cm3, 1);
    targetMat->AddElement(Sn, 1.);
    
    /*// Defines detector material as Ge  //
    detMat =  new G4Material("Germanium", 5.323*g/cm3, 1);
    detMat->AddElement(Ge, 1.);*/

    // Defines detector material as Si  //
    detMat =  new G4Material("Silicon", 2.329002*g/cm3, 1);
    detMat->AddElement(Si, 1.);

    // Defines WINDOW material as Au  //
    winMat =  new G4Material("Gold", 19.3*g/cm3, 1);
    winMat->AddElement(Au, 1.);
}
///OOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOO///



G4VPhysicalVolume *MyDetectorConstruction::Construct()
{
    G4double xWorld = 100*mm;//World half lenght
    G4double yWorld = 100*mm;//World half height
    G4double zWorld = 100*mm;//World half depth
    
           
    //   Defines WORLD volume   //
    solidWorld = new G4Box("solidWorld", xWorld, yWorld, zWorld); 
    logicWorld = new G4LogicalVolume(solidWorld, vacuum, "LogicWorld");
    physWorld = new G4PVPlacement(0, G4ThreeVector(0., 0., 0.), logicWorld, "PhysWorld", 0, false, 0, true);
    
    //   Defines cylinder for target frame hole   //
    solidHole = new G4Tubs("SolidHole", 0., 5.*mm, 1.*mm, 0., 2*pi);
    //   Defines entire solid target frame   //
    solidFrame = new G4Box("SolidFrame", 6.*mm, 15.*mm, 0.5*mm);
    //   Subtracts target holder solid hole from the entire target frame   //
    G4SubtractionSolid *subtraction = new G4SubtractionSolid("RealTargetFrame", solidFrame, solidHole);
    //   Defines subtraction logical volume   //
    logicSubtraction = new G4LogicalVolume(subtraction, holderMat, "LogicSubtraction");
    //   Defines subtraction physical volume   //
    physSubtraction = new G4PVPlacement(0, G4ThreeVector(0., 0., -50.*mm), logicSubtraction, "PhysSubtraction", logicWorld, false, 0, true);
    
    //   Defines TARGET volume   //
    solidTarget = new G4Tubs("SolidTarget", 0.*m, 5.5*mm, 100*nm, 0., 2*pi);
    logicTarget = new G4LogicalVolume(solidTarget, targetMat, "LogicTarget");
    physTarget = new G4PVPlacement(0, G4ThreeVector(0., 0., -49.5*mm), logicTarget, "PhysTarget", logicWorld, false, 0., true);
    
    //   Defines detector WINDOW volume   //
    solidWindow = new G4Tubs("SolidWindow", 0.*m, 60.*mm, 0.3*mm, 0., 2*pi);
    logicWindow = new G4LogicalVolume(solidWindow, winMat, "LogicWindow");
    physWindow = new G4PVPlacement(0, G4ThreeVector(0., 0., 0.005*m), logicWindow, "PhysWindow", logicWorld, false, 0., true);
    
    //   Defines Ge DETECTOR volume  //
    solidDetector = new G4Tubs("SolidDetector", 0.*m, 0.060*m, 0.013*m, 0., 2*pi);
    logicDetector = new G4LogicalVolume(solidDetector, detMat, "LogicDetector");
    physDetector = new G4PVPlacement(0, G4ThreeVector(0., 0., 0.023*m), logicDetector, "PhysDetector", logicWorld, false, 0., true);
    
    ScoringVolume = logicDetector;
        
  return physWorld;
}    

//   //
void MyDetectorConstruction::ConstructSDandField()
{
    MySensitiveDetector *sensDet = new MySensitiveDetector("SensitiveDetector");
    logicDetector->SetSensitiveDetector(sensDet); 
}
