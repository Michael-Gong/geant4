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
// $Id: G4LossTableManager.cc,v 1.75 2006/12/13 15:44:42 gunter Exp $
// GEANT4 tag $Name: geant4-08-02 $
//
// -------------------------------------------------------------------
//
// GEANT4 Class file
//
//
// File name:     G4LossTableManager
//
// Author:        Vladimir Ivanchenko
//
// Creation date: 03.01.2002
//
// Modifications:
//
// 20-01-03 Migrade to cut per region (V.Ivanchenko)
// 15-02-03 Lambda table can be scaled (V.Ivanchenko)
// 17-02-03 Fix problem of store/restore tables (V.Ivanchenko)
// 10-03-03 Add Ion registration (V.Ivanchenko)
// 25-03-03 Add deregistration (V.Ivanchenko)
// 02-04-03 Change messenger (V.Ivanchenko)
// 26-04-03 Fix retrieve tables (V.Ivanchenko)
// 13-05-03 Add calculation of precise range (V.Ivanchenko)
// 23-07-03 Add exchange with G4EnergyLossTables (V.Ivanchenko)
// 05-10-03 Add G4VEmProcesses registration and Verbose command (V.Ivanchenko)
// 17-10-03 Add SetParameters method (V.Ivanchenko)
// 23-10-03 Add control on inactive processes (V.Ivanchenko)
// 04-11-03 Add checks in RetrievePhysicsTable (V.Ivanchenko)
// 12-11-03 G4EnergyLossSTD -> G4EnergyLossProcess (V.Ivanchenko)
// 14-01-04 Activate precise range calculation (V.Ivanchenko)
// 10-03-04 Fix a problem of Precise Range table (V.Ivanchenko)
// 08-11-04 Migration to new interface of Store/Retrieve tables (V.Ivanchenko)
// 13-01-04 Fix problem which takes place for inactivate eIoni (V.Ivanchenko)
// 25-01-04 Fix initialisation problem for ions (V.Ivanchenko)
// 11-03-05 Shift verbose level by 1 (V.Ivantchenko)
// 10-01-06 PreciseRange -> CSDARange (V.Ivantchenko)
// 20-01-06 Introduce G4EmTableType to remove repeating code (VI)
// 23-03-06 Set flag isIonisation (VI)
// 10-05-06 Add methods  SetMscStepLimitation, FacRange and MscFlag (VI)
// 22-05-06 Add methods  Set/Get bremsTh (VI)
// 05-06-06 Do not clear loss_table map between runs (VI)
//
// Class Description:
//
// -------------------------------------------------------------------
//
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

#include "G4LossTableManager.hh"
#include "G4EnergyLossMessenger.hh"
#include "G4PhysicsTable.hh"
#include "G4ParticleDefinition.hh"
#include "G4MaterialCutsCouple.hh"
#include "G4ProcessManager.hh"
#include "G4Electron.hh"
#include "G4Proton.hh"
#include "G4VMultipleScattering.hh"
#include "G4VEmProcess.hh"
#include "G4ProductionCutsTable.hh"
#include "G4PhysicsTableHelper.hh"
#include "G4EmCorrections.hh"
#include "G4EmTableType.hh"
#include "G4LossTableBuilder.hh"

G4LossTableManager* G4LossTableManager::theInstance = 0;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo.....

G4LossTableManager* G4LossTableManager::Instance()
{
  if(0 == theInstance) {
    static G4LossTableManager manager;
    theInstance = &manager;
  }
  return theInstance;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo.....

G4LossTableManager::~G4LossTableManager()
{
  for (G4int i=0; i<n_loss; i++) {
    if( loss_vector[i] ) delete loss_vector[i];
  }
  size_t msc = msc_vector.size();
  for (size_t j=0; j<msc; j++) {
    if(msc_vector[j] ) delete msc_vector[j];
  }
  size_t emp = emp_vector.size();
  for (size_t k=0; k<emp; k++) {
    if(emp_vector[k] ) delete emp_vector[k];
  }
  Clear();
  delete theMessenger;
  delete tableBuilder;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo.....

G4LossTableManager::G4LossTableManager()
{
  n_loss = 0;
  first_entry = true;
  all_tables_are_built = false;
  all_tables_are_stored = false;
  currentLoss = 0;
  currentParticle = 0;
  lossFluctuationFlag = true;
  subCutoffFlag = false;
  rndmStepFlag = false;
  minSubRange = 0.0;
  maxRangeVariation = 1.0;
  maxFinalStep = 0.0;
  minKinEnergy = 0.1*keV;
  maxKinEnergy = 100.0*TeV;
  maxKinEnergyForMuons = 100.*TeV;
  theMessenger = new G4EnergyLossMessenger();
  theElectron  = G4Electron::Electron();
  tableBuilder = new G4LossTableBuilder();
  emCorrections= new G4EmCorrections();
  integral = true;
  integralActive = false;
  buildCSDARange = false;
  minEnergyActive = false;
  maxEnergyActive = false;
  maxEnergyForMuonsActive = false;
  stepFunctionActive = false;
  flagLPM = true;
  flagMSC = true;
  facRange = 0.02;
  bremsTh = DBL_MAX;
  mscActive = false;
  verbose = 1;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo.....

void G4LossTableManager::Clear()
{
  all_tables_are_built = false;
  currentLoss = 0;
  currentParticle = 0;
  if(n_loss)
    {
      dedx_vector.clear();
      range_vector.clear();
      inv_range_vector.clear();
      loss_map.clear();
      loss_vector.clear();
      part_vector.clear();
      base_part_vector.clear();
      tables_are_built.clear();
      isActive.clear();
      n_loss = 0;
    }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo.....

void G4LossTableManager::Register(G4VEnergyLossProcess* p)
{
  n_loss++;
  loss_vector.push_back(p);
  part_vector.push_back(0);
  base_part_vector.push_back(0);
  dedx_vector.push_back(0);
  range_vector.push_back(0);
  inv_range_vector.push_back(0);
  tables_are_built.push_back(false);
  isActive.push_back(true);
  all_tables_are_built = false;
  if(!lossFluctuationFlag) p->SetLossFluctuations(false);
  if(subCutoffFlag)        p->ActivateSubCutoff(true);
  if(rndmStepFlag)         p->SetRandomStep(true);
  if(stepFunctionActive)   p->SetStepFunction(maxRangeVariation, maxFinalStep);
  if(integralActive)       p->SetIntegral(integral);
  if(minEnergyActive)      p->SetMinKinEnergy(minKinEnergy);
  if(maxEnergyActive)      p->SetMaxKinEnergy(maxKinEnergy);
  if(verbose > 1) 
    G4cout << "G4LossTableManager::Register G4VEnergyLossProcess : " 
	   << p->GetProcessName() << G4endl;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo.....

void G4LossTableManager::DeRegister(G4VEnergyLossProcess* p)
{
  for (G4int i=0; i<n_loss; i++) {
    if(loss_vector[i] == p) loss_vector[i] = 0;
  }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo.....

void G4LossTableManager::Register(G4VMultipleScattering* p)
{
  msc_vector.push_back(p);
  if(mscActive) p->MscStepLimitation(flagMSC, facRange);
  if(verbose > 1) 
    G4cout << "G4LossTableManager::Register G4VMultipleScattering : " 
	   << p->GetProcessName() << G4endl;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo.....

void G4LossTableManager::DeRegister(G4VMultipleScattering* p)
{
  size_t msc = msc_vector.size();
  for (size_t i=0; i<msc; i++) {
    if(msc_vector[i] == p) msc_vector[i] = 0;
  }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo.....

void G4LossTableManager::Register(G4VEmProcess* p)
{
  emp_vector.push_back(p);
  if(verbose > 1) 
    G4cout << "G4LossTableManager::Register G4VEmProcess : " 
	   << p->GetProcessName() << G4endl;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo.....

void G4LossTableManager::DeRegister(G4VEmProcess* p)
{
  size_t emp = emp_vector.size();
  for (size_t i=0; i<emp; i++) {
    if(emp_vector[i] == p) emp_vector[i] = 0;
  }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo.....

void G4LossTableManager::RegisterIon(const G4ParticleDefinition* ion, 
				     G4VEnergyLossProcess* p)
{
  loss_map[ion] = p;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo.....

void G4LossTableManager::RegisterExtraParticle(
     const G4ParticleDefinition* part,
     G4VEnergyLossProcess* p)
{
  n_loss++;
  loss_vector.push_back(p);
  part_vector.push_back(part);
  base_part_vector.push_back(p->BaseParticle());
  dedx_vector.push_back(0);
  range_vector.push_back(0);
  inv_range_vector.push_back(0);
  tables_are_built.push_back(false);
  all_tables_are_built = false;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo.....

void G4LossTableManager::EnergyLossProcessIsInitialised(
                   const G4ParticleDefinition* particle, 
		   G4VEnergyLossProcess* p)
{
  if (first_entry || (particle == firstParticle && all_tables_are_built) ) {
    all_tables_are_built = true;

    if(1 < verbose) 
      G4cout << "### G4LossTableManager start initilisation of tables" 
	     << G4endl;
    for (G4int i=0; i<n_loss; i++) {
      G4VEnergyLossProcess* el = loss_vector[i];

      if(el) {
	const G4ProcessManager* pm = el->GetProcessManager();
        isActive[i] = pm->GetProcessActivation(el);
        tables_are_built[i] = false;
	all_tables_are_built = false;
	if(1 < verbose) { 
	  G4cout << i <<".   "<< el->GetProcessName() 
		 << "  for "  << pm->GetParticleType()->GetParticleName()
		 << "  active= " << pm->GetProcessActivation(el)
                 << "  table= " << tables_are_built[i]
		 << "  isIonisation= " << el->IsIonisationProcess()
		 << G4endl;
	}
      } else {
        tables_are_built[i] = true;
        part_vector[i] = 0;
      }
    }
    if (first_entry) {
      first_entry = false;
      firstParticle = particle;
    }
  }

  //  if(!all_tables_are_built) loss_map.clear();
  currentParticle = 0;

  SetParameters(p);
  for (G4int j=0; j<n_loss; j++) {
    if (p == loss_vector[j]) {

      if (!part_vector[j]) {
        part_vector[j] = particle;
      	base_part_vector[j] = p->BaseParticle();
      }
      if(maxEnergyForMuonsActive) {
        G4double dm = std::abs(particle->GetPDGMass() - 105.7*MeV);
	if(dm < 5.*MeV) p->SetMaxKinEnergy(maxKinEnergyForMuons);
      }

      if(1 < verbose) {
        G4cout << "For " << p->GetProcessName()
               << " for " << part_vector[j]->GetParticleName()
               << " tables_are_built= " << tables_are_built[j]
               << " procFlag= " << loss_vector[j]->TablesAreBuilt()
               << " all_tables_are_built= "     << all_tables_are_built
               << G4endl;
      }
    }
  }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo.....

G4EnergyLossMessenger* G4LossTableManager::GetMessenger()
{
  return theMessenger;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo.....

void G4LossTableManager::ParticleHaveNoLoss(
     const G4ParticleDefinition* aParticle)
{
  G4String s = "G4LossTableManager:: dE/dx table not found for "
             + aParticle->GetParticleName() + "!";
  G4Exception(s);
  exit(1);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo.....

G4bool G4LossTableManager::BuildCSDARange() const
{
  return buildCSDARange;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo.....

void G4LossTableManager::BuildPhysicsTable(
     const G4ParticleDefinition* aParticle,
     G4VEnergyLossProcess* p)
{
  if(1 < verbose) {
    G4cout << "### G4LossTableManager::BuildDEDXTable() is requested for "
           << aParticle->GetParticleName()
	   << " and process " << p->GetProcessName()
           << G4endl;
  }
  if (all_tables_are_built) return;
  all_tables_are_built = true;

  for(G4int i=0; i<n_loss; i++) {
    if(!tables_are_built[i] && !base_part_vector[i]) {
      const G4ParticleDefinition* curr_part = part_vector[i];
      G4VEnergyLossProcess* curr_proc = BuildTables(curr_part);
      if(curr_proc) CopyTables(curr_part, curr_proc);
    }
  }

  for (G4int ii=0; ii<n_loss; ii++) {
    if ( !tables_are_built[ii] ) {
      all_tables_are_built = false;
      break;
    }
  }

  if(1 < verbose) {
    G4cout << "### G4LossTableManager::BuildDEDXTable end: "
           << "all_tables_are_built= " << all_tables_are_built
           << G4endl;

    if(all_tables_are_built)
      G4cout << "### All dEdx and Range tables are built #####" << G4endl;
  }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo.....

void G4LossTableManager::CopyTables(const G4ParticleDefinition* part,
                                          G4VEnergyLossProcess* base_proc)
{
  for (G4int j=0; j<n_loss; j++) {

    G4VEnergyLossProcess* proc = loss_vector[j];
    if(proc == base_proc || proc->Particle() == part) 
      tables_are_built[j] = true;

    if (!tables_are_built[j] && part == base_part_vector[j]) {
      tables_are_built[j] = true;
      proc->SetDEDXTable(base_proc->DEDXTable());
      proc->SetDEDXTableForSubsec(base_proc->DEDXTableForSubsec());
      proc->SetDEDXunRestrictedTable(base_proc->DEDXunRestrictedTable());
      proc->SetCSDARangeTable(base_proc->CSDARangeTable());
      proc->SetRangeTableForLoss(base_proc->RangeTableForLoss());
      proc->SetInverseRangeTable(base_proc->InverseRangeTable());
      proc->SetLambdaTable(base_proc->LambdaTable());
      proc->SetSubLambdaTable(base_proc->SubLambdaTable());
      proc->SetIonisation(base_proc->IsIonisationProcess());
      loss_map[part_vector[j]] = proc;
      if (1 < verbose) {
         G4cout << "For " << proc->GetProcessName()
                << " for " << part_vector[j]->GetParticleName()
                << " base_part= " << part->GetParticleName()
                << " tables are assigned "
                << G4endl;
      }
    }

    if (theElectron == part && theElectron == proc->SecondaryParticle() )
      proc->SetSecondaryRangeTable(base_proc->RangeTableForLoss());
  }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo.....

G4VEnergyLossProcess* G4LossTableManager::BuildTables(
                      const G4ParticleDefinition* aParticle)
{
  if(1 < verbose) {
    G4cout << "G4LossTableManager::BuildTables() for "
           << aParticle->GetParticleName() << G4endl;
  }

  std::vector<G4PhysicsTable*> list;  
  std::vector<G4VEnergyLossProcess*> loss_list;
  loss_list.clear();
  G4VEnergyLossProcess* em = 0;
  G4VEnergyLossProcess* p = 0;
  G4int iem = 0;
  G4PhysicsTable* dedx = 0;
  G4int i;

  for (i=0; i<n_loss; i++) {
    p = loss_vector[i];
    if (p && aParticle == part_vector[i] && !tables_are_built[i]) {
      if (p->IsIonisationProcess() && isActive[i] || !em || em && !isActive[iem] ) {
        em = p;
        iem= i;
      }
      dedx = p->BuildDEDXTable(fRestricted);
      p->SetDEDXTable(dedx); 
      list.push_back(dedx);
      loss_list.push_back(p);
      tables_are_built[i] = true;
    }
  }

  G4int n_dedx = list.size();
  if (!n_dedx) {
    G4cout << "G4LossTableManager WARNING: no DEDX processes for " 
	   << aParticle->GetParticleName() << G4endl;
    return 0;
  }
  G4int nSubRegions = em->NumberOfSubCutoffRegions();

  if (1 < verbose) {
    G4cout << "G4LossTableManager::BuildTables() start to build range tables"
           << " and the sum of " << n_dedx << " processes"
           << " iem= " << iem << " em= " << em->GetProcessName()
           << " buildCSDARange= " << buildCSDARange
	   << " nSubRegions= " << nSubRegions
	   << G4endl;
  }

  dedx = em->DEDXTable();
  if (1 < n_dedx) tableBuilder->BuildDEDXTable(dedx, list);
  dedx_vector[iem] = dedx;

  G4PhysicsTable* range = em->RangeTableForLoss();
  if(!range) range  = G4PhysicsTableHelper::PreparePhysicsTable(range);
  range_vector[iem] = range;

  G4PhysicsTable* invrange = em->InverseRangeTable();
  if(!invrange) invrange = G4PhysicsTableHelper::PreparePhysicsTable(invrange);
  inv_range_vector[iem]  = invrange;

  G4bool flag = em->IsIonisationProcess();
  tableBuilder->BuildRangeTable(dedx, range, flag);
  tableBuilder->BuildInverseRangeTable(range, invrange, flag);

  //  if(1<verbose) G4cout << *dedx << G4endl;

  em->SetRangeTableForLoss(range);
  em->SetInverseRangeTable(invrange);

  if(1<verbose) G4cout << *range << G4endl;

  std::vector<G4PhysicsTable*> listSub;
  std::vector<G4PhysicsTable*> listCSDA;

  for (i=0; i<n_dedx; i++) {
    p = loss_list[i];
    p->SetIonisation(false);
    p->SetLambdaTable(p->BuildLambdaTable(fRestricted));
    if (0 < p->NumberOfSubCutoffRegions()) {
      dedx = p->BuildDEDXTable(fSubRestricted);
      p->SetDEDXTableForSubsec(dedx);
      listSub.push_back(dedx);
      p->SetSubLambdaTable(p->BuildLambdaTable(fSubRestricted));
    }
    if(buildCSDARange) { 
      dedx = p->BuildDEDXTable(fTotal);
      p->SetDEDXunRestrictedTable(dedx);
      listCSDA.push_back(dedx); 
    }     
  }

  if (0 < nSubRegions) {
    G4PhysicsTable* dedxSub = em->DEDXTableForSubsec();
    if (1 < listSub.size()) tableBuilder->BuildDEDXTable(dedxSub, listSub);
    em->SetDEDXTableForSubsec(dedxSub);
  }
  if(buildCSDARange) {
    G4PhysicsTable* dedxCSDA = em->DEDXunRestrictedTable();
    if (1 < n_dedx) tableBuilder->BuildDEDXTable(dedxCSDA, listCSDA);
    em->SetDEDXunRestrictedTable(dedxCSDA);
    G4PhysicsTable* rCSDA = em->CSDARangeTable();
    if(!rCSDA) range  = G4PhysicsTableHelper::PreparePhysicsTable(rCSDA);
    tableBuilder->BuildRangeTable(dedxCSDA, rCSDA, flag);
    em->SetCSDARangeTable(rCSDA);
  }

  em->SetIonisation(true);
  loss_map[aParticle] = em;
  if (1 < verbose) {
    G4cout << "G4LossTableManager::BuildTables: Tables are built for "
           << aParticle->GetParticleName()
	   << "; ionisation process: " << em->GetProcessName()
           << G4endl;
  }
  return em;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo.....

void G4LossTableManager::SetLossFluctuations(G4bool val)
{
  lossFluctuationFlag = val;
  for(G4int i=0; i<n_loss; i++) {
    if(loss_vector[i]) loss_vector[i]->SetLossFluctuations(val);
  }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo.....

void G4LossTableManager::SetSubCutoff(G4bool val)
{
  subCutoffFlag = val;
  for(G4int i=0; i<n_loss; i++) {
    if(loss_vector[i]) loss_vector[i]->ActivateSubCutoff(val);
  }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo.....

void G4LossTableManager::SetIntegral(G4bool val)
{
  integral = val;
  integralActive = true;
  for(G4int i=0; i<n_loss; i++) {
    if(loss_vector[i]) loss_vector[i]->SetIntegral(val);
  }
  size_t emp = emp_vector.size();
  for (size_t k=0; k<emp; k++) {
    if(emp_vector[k]) emp_vector[k]->SetIntegral(val);
  }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo.....

void G4LossTableManager::SetMinSubRange(G4double val)
{
  minSubRange = val;
  for(G4int i=0; i<n_loss; i++) {
    if(loss_vector[i]) loss_vector[i]->SetMinSubRange(val);
  }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo.....

void G4LossTableManager::SetRandomStep(G4bool val)
{
  rndmStepFlag = val;
  for(G4int i=0; i<n_loss; i++) {
    if(loss_vector[i]) loss_vector[i]->SetRandomStep(val);
  }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo.....

void G4LossTableManager::SetMinEnergy(G4double val)
{
  minEnergyActive = true;
  minKinEnergy = val;
  for(G4int i=0; i<n_loss; i++) {
    if(loss_vector[i]) loss_vector[i]->SetMinKinEnergy(val);
  }
  size_t msc = msc_vector.size();
  for (size_t j=0; j<msc; j++) {
    if(msc_vector[j]) msc_vector[j]->SetMinKinEnergy(val);
  }
  size_t emp = emp_vector.size();
  for (size_t k=0; k<emp; k++) {
    if(emp_vector[k]) emp_vector[k]->SetMinKinEnergy(val);
  }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo.....

void G4LossTableManager::SetMaxEnergy(G4double val)
{
  maxEnergyActive = true;
  maxKinEnergy = val;
  for(G4int i=0; i<n_loss; i++) {
    if(loss_vector[i]) loss_vector[i]->SetMaxKinEnergy(val);
  }
  size_t msc = msc_vector.size();
  for (size_t j=0; j<msc; j++) {
    if(msc_vector[j]) msc_vector[j]->SetMaxKinEnergy(val);
  }
  size_t emp = emp_vector.size();
  for (size_t k=0; k<emp; k++) {
    if(emp_vector[k]) emp_vector[k]->SetMaxKinEnergy(val);
  }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo.....

void G4LossTableManager::SetMaxEnergyForCSDARange(G4double val)
{
  for(G4int i=0; i<n_loss; i++) {
    if(loss_vector[i]) loss_vector[i]->SetMaxKinEnergyForCSDARange(val);
  }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo.....

void G4LossTableManager::SetMaxEnergyForMuons(G4double val)
{
  maxEnergyForMuonsActive = true;
  maxKinEnergyForMuons = val;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo.....

void G4LossTableManager::SetDEDXBinning(G4int val)
{
  for(G4int i=0; i<n_loss; i++) {
    if(loss_vector[i]) loss_vector[i]->SetDEDXBinning(val);
  }
}


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo.....

void G4LossTableManager::SetDEDXBinningForCSDARange(G4int val)
{
  for(G4int i=0; i<n_loss; i++) {
    if(loss_vector[i]) loss_vector[i]->SetDEDXBinningForCSDARange(val);
  }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo.....

void G4LossTableManager::SetLambdaBinning(G4int val)
{
  for(G4int i=0; i<n_loss; i++) {
    if(loss_vector[i]) loss_vector[i]->SetLambdaBinning(val);
  }
  size_t msc = msc_vector.size();
  for (size_t j=0; j<msc; j++) {
    if(msc_vector[j]) msc_vector[j]->SetBinning(val);
  }
  size_t emp = emp_vector.size();
  for (size_t k=0; k<emp; k++) {
    if(emp_vector[k]) emp_vector[k]->SetLambdaBinning(val);
  }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo.....

void G4LossTableManager::SetVerbose(G4int val)
{
  verbose = val;
  for(G4int i=0; i<n_loss; i++) {
    if(loss_vector[i]) loss_vector[i]->SetVerboseLevel(val);
  }
  size_t msc = msc_vector.size();
  for (size_t j=0; j<msc; j++) {
    if(msc_vector[j]) msc_vector[j]->SetVerboseLevel(val);
  }
  size_t emp = emp_vector.size();
  for (size_t k=0; k<emp; k++) {
    if(emp_vector[k]) emp_vector[k]->SetVerboseLevel(val);
  }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo.....

void G4LossTableManager::SetStepFunction(G4double v1, G4double v2)
{
  stepFunctionActive = true;
  maxRangeVariation = v1;
  maxFinalStep = v2;
  for(G4int i=0; i<n_loss; i++) {
    if(loss_vector[i]) loss_vector[i]->SetStepFunction(v1, v2);
  }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo.....

void G4LossTableManager::SetBuildCSDARange(G4bool val)
{
  buildCSDARange = val;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo.....

void G4LossTableManager::SetParameters(G4VEnergyLossProcess* p)
{
  if(stepFunctionActive) p->SetStepFunction(maxRangeVariation, maxFinalStep);
  if(integralActive)     p->SetIntegral(integral);
  if(minEnergyActive)    p->SetMinKinEnergy(minKinEnergy);
  if(maxEnergyActive)    p->SetMaxKinEnergy(maxKinEnergy);
  p->SetVerboseLevel(verbose);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo.....

const std::vector<G4VEnergyLossProcess*>& 
      G4LossTableManager::GetEnergyLossProcessVector()
{
  return loss_vector;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo.....

const std::vector<G4VEmProcess*>& G4LossTableManager::GetEmProcessVector()
{
  return emp_vector;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo.....

const std::vector<G4VMultipleScattering*>& 
      G4LossTableManager::GetMultipleScatteringVector()
{
  return msc_vector;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo.....

void G4LossTableManager::SetLPMFlag(G4bool val)
{
  flagLPM = val;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo.....

G4bool G4LossTableManager::LPMFlag() const
{
  return flagLPM;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

void G4LossTableManager::SetMscStepLimitation(G4bool val, G4double factor)
{
  mscActive = true;
  flagMSC = val;
  facRange = factor;
  size_t msc = msc_vector.size();
  for (size_t j=0; j<msc; j++) {
    if(msc_vector[j]) msc_vector[j]->MscStepLimitation(val, factor);
  }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

G4bool G4LossTableManager::MscFlag() const
{
  return flagMSC;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

G4double G4LossTableManager::FacRange() const
{
  return facRange;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

void G4LossTableManager::SetBremsstrahlungTh(G4double val) 
{
  bremsTh = val;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

G4double G4LossTableManager::BremsstrahlungTh() const
{
  return bremsTh;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....
