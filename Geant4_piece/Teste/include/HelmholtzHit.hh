

#ifndef HelmholtzHit_h
#define HelmholtzHit_h 1

#include "G4VHit.hh"
#include "G4THitsCollection.hh"
#include "G4Allocator.hh"
#include "G4ThreeVector.hh"
#include "G4LogicalVolume.hh"
#include "G4Transform3D.hh"
#include "G4RotationMatrix.hh"
#include "TesteTrackInformation.hh"

#include "G4Types.hh"

class G4AttDef;
class G4AttValue;

/// Helmholtz hit
///
/// It records:
/// - the strip ID
/// - the particle time
/// - the strip logical volume, its position and rotation

class HelmholtzHit : public G4VHit 
//hit is a snapshot of the physical interaction of a track in the sensitive region of a detector
//serve para armazenar informações como: position and time of the step, momentum and energy of the track, energy
// deposition of the step, geometrical information
{
public:
    HelmholtzHit(G4LogicalVolume* logVol,G4int z,G4int phi);
    HelmholtzHit(const HelmholtzHit &right);
    virtual ~HelmholtzHit();

    const HelmholtzHit& operator=(const HelmholtzHit &right);
    int operator==(const HelmholtzHit &right) const;
    
    //definição de new() e delete() que os allocators usarão
    inline void *operator new(size_t);
    inline void operator delete(void*aHit);
    
    void Draw();
    virtual const std::map<G4String,G4AttDef>* GetAttDefs() const;  //especifico do exemplo
    virtual std::vector<G4AttValue>* CreateAttValues() const;	    //especifico do exemplo
    void Print();
    
    //some set/get methods
    inline G4int GetZ() { return fZCellID; }
    inline G4int GetPhi() { return fPhiCellID; }
    
    //energy deposition
    inline void SetEdep(G4double de) { fEdep = de; fEdepByATrack = de; }
    inline void AddEdep(G4double de) { fEdep += de; fEdepByATrack += de; }
    inline G4double GetEdep() { return fEdep; }
    inline G4double GetEdepByATrack() { return fEdepByATrack; }
    inline void ClearEdepByATrack() { fEdepByATrack = 0.; fTrackInfo = TesteTrackInformation(); }
    
    inline void SetPos(G4ThreeVector xyz)
    { fPos = xyz; }
    inline void SetRot(G4RotationMatrix rmat)
    { fRot = rmat; }
    inline void SetTrackInformation(const G4Track* aTrack)
    {
      TesteTrackInformation* anInfo = (TesteTrackInformation*)(aTrack->GetUserInformation());
      fTrackInfo = *anInfo;
    }
  inline TesteTrackInformation* GetTrackInformation()
  { return &fTrackInfo; }
    //some data members
private:
    G4int fZCellID;
    G4int fPhiCellID;
    G4double fEdep;
    G4ThreeVector fPos;
    G4RotationMatrix fRot;
    const G4LogicalVolume* fPLogV;
    G4double fEdepByATrack;
    TesteTrackInformation fTrackInfo;
    
    
    
    
    
    
};


//G4Allocator
//every Hit object is stored in the HitsCollection accessible through G4Event (analysis) or G4SDManager (filtering)
//for each concrete hit class you must also prepare a concrete class derived from G4VHitsCollection
typedef G4THitsCollection<HelmholtzHit> HelmholtzHitsCollection;

extern G4ThreadLocal G4Allocator<HelmholtzHit>* HelmholtzHitAllocator; 	//G4Allocator is a class for fast allocation of
									//objects to the heap through the paging 
									//mechanism
inline void* HelmholtzHit::operator new(size_t)
{
    if (!HelmholtzHitAllocator)
        HelmholtzHitAllocator = new G4Allocator<HelmholtzHit>;
    return (void*)HelmholtzHitAllocator->MallocSingle();
}

inline void HelmholtzHit::operator delete(void*aHit)
{
    HelmholtzHitAllocator->FreeSingle((HelmholtzHit*) aHit);
}

#endif
