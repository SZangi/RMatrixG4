#ifndef runAction_hh
#define runAction_hh 1

#include "G4UserRunAction.hh"
#include "G4Run.hh"

#include <string>
using namespace std;

class G4Run;

class runAction : public G4UserRunAction
{

public:
  runAction();
  ~runAction();

  void BeginOfRunAction(const G4Run*) override;
  void EndOfRunAction(const G4Run*) override;

};

#endif