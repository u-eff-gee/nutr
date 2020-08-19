//--------------------------------------------------------------------
// DetectorConstruction.cc
//
// Description: The detector definitions, materials etc.
// Changes: 7/15/05 None yet
//--------------------------------------------------------------------

#include <memory>

using std::make_unique;

#include "G4Box.hh"
#include "G4Colour.hh"
#include "G4Cons.hh"
#include "G4LogicalVolume.hh"
#include "G4NistManager.hh"
#include "G4PhysicalConstants.hh"
#include "G4PVPlacement.hh"
#include "G4SubtractionSolid.hh"
#include "G4SystemOfUnits.hh"
#include "G4Tubs.hh"
#include "G4VisAttributes.hh"

#include "DetectorConstruction.hh"
#include "NaI.hh"

G4VPhysicalVolume* DetectorConstruction::Construct()
{
	G4NistManager* nist_manager = G4NistManager::Instance();

	world_solid = make_unique<G4Box>("world_solid", 2.*m, 2.*m, 2.*m);
	world_logical = make_unique<G4LogicalVolume>(world_solid.get(), nist_manager->FindOrBuildMaterial("G4_AIR"), "world_logical");
	world_logical->SetVisAttributes(G4VisAttributes::GetInvisible());
	world_phys = make_unique<G4PVPlacement>(new G4RotationMatrix(), G4ThreeVector(), world_logical.get(), "world", nullptr, false, 0);

	/********* NaI(Tl) Detectors *********/

	NaI nai1(world_logical.get(), "nai1");
	nai1.Construct(G4ThreeVector(), 0., 0., 18.5*mm);
	RegisterSensitiveLogicalVolumes(nai1.get_sensitive_logical_volumes());

	NaI nai2(world_logical.get(), "nai2");
	nai2.Construct(G4ThreeVector(), pi, pi, 18.5*mm);
	RegisterSensitiveLogicalVolumes(nai2.get_sensitive_logical_volumes());

	/********* Divider Wall *********/

	G4double DividerWallLengthX = 19.05*0.5*cm; // Define Wall dimensions
	G4double DividerWallLengthY = (19.05-3)*0.5*cm; //Y is -1 because the wall is too tall
	G4double DividerWallLengthZ = 2.54*0.5*cm; //physical problem is unknown. 

	G4Box* DividerWall_solid
		= new G4Box("DividerWall",
					DividerWallLengthX, 
					DividerWallLengthY, 
					DividerWallLengthZ);
	
	G4double pRMin = 0; // Define Hole Size
	G4double pRMax = 3.8*0.5*cm;
	G4double pDz = 2.54*0.5*cm;
	G4double pSPhi = 0; 
	G4double pDPhi = 360; 

	G4Tubs* Hole_solid		// Make Cylindrical Hole
		= new G4Tubs("Hole",
					pRMin, 
					pRMax, 
					pDz, 
					pSPhi, 
					pDPhi);

	//  G4SubtractionSolid* DetectorDivider	//Make Wall with Cylindrical Hole
	//    = new G4SubtractionSolid("DetectorDivider", DividerWall_solid, Hole_solid);

	G4double pRmin1 = 0; 		// Make Conic Hole dimensions
	G4double pRmax1 = 0; 
	G4double pRmin2 = 0; 
	G4double pRmax2 = 53.34*0.5*mm; 
	G4double pDz1 = 53.34*0.5*0.5*mm; 
	G4double pSPhi1 = 0; 
	G4double pDPhi1 = 360; 

	G4Cons* Bevel1_solid	//Define Bevel 1 for Hole
		= new G4Cons ("Bevel1", 
				pRmin1,
				pRmax1, 
				pRmin2, 
				pRmax2, 
				pDz1, 
				pSPhi1, 
				pDPhi1); 

	G4Cons* Bevel2_solid	//Define Bevel 2 for Hole
	= new G4Cons ("Bevel2",
					pRmin1, 
					pRmax2, 
					pRmin2, 
					pRmax1, 
					pDz1, 
					pSPhi1, 
					pDPhi1); 

	G4SubtractionSolid* DetectorDivider1_solid 	//Make Wall with Cylinder
		= new G4SubtractionSolid("DetectorDivider1", DividerWall_solid, Hole_solid);

	G4SubtractionSolid* DetectorDivider2_solid	//Make Wall with Cylinder Hole and Bevel
		= new G4SubtractionSolid("DetectorDivider2", DetectorDivider1_solid, Bevel1_solid);
	G4SubtractionSolid* DetectorDivider		//Make wall with hole and bevels
		= new G4SubtractionSolid("DetectorDivider", DetectorDivider2_solid, Bevel2_solid);

	G4LogicalVolume* DetectorDivider_log		// Make wall a logical volume
		= new G4LogicalVolume(DetectorDivider, nist_manager->FindOrBuildMaterial("G4_Pb"),
							"DividerWall Solid", 0,0,0);
	DetectorDivider_log->SetVisAttributes(G4Colour::Grey());

	new G4PVPlacement(0, 
				// G4ThreeVector(0,5.3/2*cm+158.65/2*mm-(2*5.3/2*cm-2.55*cm)-1.1*cm,0),  // at (0,0,0)
				G4ThreeVector(),
						DetectorDivider_log, 
						"DetectorDivider",
				world_logical.get(),
						false, 
						0);

	/********* Sample holder *********/

	G4double pRMin1 = 0; 		//Approximate dimensions of aluminum sample 
	G4double pRMax1 = 1.85*cm; 	//holder. Subject to change. 
	pDz1 = 2.7*0.5*cm; 
	pSPhi1 = 0; 
	pDPhi1 = 360; 

	G4Tubs* AluminumCylinder_solid 	//Make the outside of the sample holder.
	= new G4Tubs("Aluminum Cylinder", 
				pRMin1, 
				pRMax1, 
				pDz1, 
				pSPhi1, 
				pDPhi1);  
	
	G4double pRMin2 = 0; 		//Dimensions of cavity inside sample holder.
	G4double pRMax2 = 1.75*cm; 	//A subtraction solid will be used to make
	G4double pDz2 = 2.5*0.5*cm; 	//the final piece of geometry. 
	G4double pSPhi2 = 0; 
	G4double pDPhi2 = 360; 

	G4Tubs* Cavity_solid		//create a cavity to subtract from the
	= new G4Tubs("Cavity", 	//SampleHolder_soild.
				pRMin2, 
				pRMax2, 
				pDz2, 
				pSPhi2, 
				pDPhi2); 

	G4SubtractionSolid* SampleHolder_solid
	= new G4SubtractionSolid("Sample Holder", AluminumCylinder_solid, Cavity_solid); 

	G4LogicalVolume* SampleHolder_log
	= new G4LogicalVolume(SampleHolder_solid, nist_manager->FindOrBuildMaterial("G4_Al"), "Sample Holder", 0,0,0);
	SampleHolder_log->SetVisAttributes(G4Colour::Cyan());

	new G4PVPlacement (0, 
		G4ThreeVector(), 
		SampleHolder_log, 
		"Sample Holder",
		world_logical.get(), 
		false,
		0);

	/********* Sample *********/

	// Sample material (Monazite Powder)
	G4double sample_density = 0.551*g/cm3; 
	G4Material* Monazite = new G4Material("Monazite",sample_density,10); 
	Monazite->AddElement(nist_manager->FindOrBuildElement("Ce"),0.268); 
	Monazite->AddElement(nist_manager->FindOrBuildElement("La"),0.119);
	Monazite->AddElement(nist_manager->FindOrBuildElement("Pr"),0.024); 
	Monazite->AddElement(nist_manager->FindOrBuildElement("Nd"),0.078); 
	Monazite->AddElement(nist_manager->FindOrBuildElement("Th"),0.104);
	Monazite->AddElement(nist_manager->FindOrBuildElement("Y"),0.005); 
	Monazite->AddElement(nist_manager->FindOrBuildElement("U"),0.004); 
	Monazite->AddElement(nist_manager->FindOrBuildElement("Si"),0.002);
	Monazite->AddElement(nist_manager->FindOrBuildElement("P"),0.128);
	Monazite->AddElement(nist_manager->FindOrBuildElement("O"),0.268); 

	//Build the Sample itself. This section defines the dimensions of the sample
	//and then allows for the material the sample is made of to be easily changed
	//by commenting out the correct material. 

	pRMin = 0; 
	pRMax = 1.75*cm; 
	pDz = 2.5*0.5*cm; 
	pSPhi = 0; 
	pDPhi = 360; 

	G4Tubs* Sample_solid
	= new G4Tubs("Sample", 
				pRMin, 
				pRMax, 
				pDz, 
				pSPhi, 
				pDPhi);         

	G4LogicalVolume* Sample_log
	= new G4LogicalVolume(Sample_solid, Monazite, "Sample Solid", 0,0,0);
	Sample_log->SetVisAttributes(G4Colour::Red());

	new G4PVPlacement(0, 
			G4ThreeVector(), 
			Sample_log, 
			"Sample", 
			world_logical.get(), 
			false, 
			0); 

	return world_phys.get();
}