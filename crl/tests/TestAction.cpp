/** ***********************************************************************************
 * \file TestDisjunction.cpp
 * \author Ariane Piel & Jean Bourrely / Onera DCPS
 * \date 2012
 * \brief Test Disjunction
 **************************************************************************************/

/*  Copyright (C) 2012, 2013, 2014  ONERA – http://www.onera.fr
    This file is part of CRL : Chronicle Recognition Library.

    CRL is free software: you can redistribute it and/or modify it under
    the terms of the Lesser GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    CRL is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    Lesser GNU General Public License for more details.

    You should have received a copy of the Lesser GNU General Public License
    along with CRL.  If not, see <http://www.gnu.org/licenses/>.
*/

// ----------------------------------------------------------------------------
// INCLUDE FILES
// ----------------------------------------------------------------------------

#include "TestUtils.h"
#include "Operators.h"
#include "RecognitionEngine.h"
#include "ChronicleDelayOp.h"

using namespace CRL;


// ----------------------------------------------------------------------------
// UNIT TESTS
// ----------------------------------------------------------------------------

bool testChronicleLoginLogout_ID1(const PropertyManager& pm)
{
  return (std::string(pm["loginx"]["username"]) == std::string(pm["cdx"]["username"]));
}


bool testChronicleLoginLogout_ID2(const PropertyManager& pm)
{
  return (std::string(pm["loginx"]["username"]) == std::string(pm["logoutx"]["username"]));
}


class ChronicleDetectionPiracy : public ChronicleAbsence
{
private:
  // "CD" per user counter
  std::map<std::string, int> counter;

public:

  // Chronicle linked to the detection chronicle which sets the user counter to 0
  // when the user logs in
  class ChronicleLogin : public ChronicleNamed
  {
  private:
    ChronicleDetectionPiracy* root;
  public:
    ChronicleLogin(ChronicleDetectionPiracy* r)
      : ChronicleNamed( &$(Login), "loginx"), root(r)  { }

    void actionMethod(const RecoTree& rc) {
      root->setCounter(std::string(rc["loginx"]["username"]), 0);
    }
  };


  ChronicleDetectionPiracy() 
    : ChronicleAbsence( & ((*(new ChronicleLogin(this))) + $$($(CD),cdx)), & ($$($(Logout),logoutx)) )
  {
    this->getOpLeft()->setPredicateFunction(testChronicleLoginLogout_ID1);
    this->setPredicateFunction(testChronicleLoginLogout_ID2);
  }

  int getCounter(const std::string& user) {

    return counter[user];
  }

  void setCounter(const std::string& user, int n) {
    counter[user] = n;
  }

  void addCounter(const std::string& user, int n) {
    counter[user] += n;
  }

  void actionMethod(const RecoTree& rc);
};

void ChronicleDetectionPiracy::actionMethod(const RecoTree& rc)
{
  counter[ std::string(rc["loginx"]["username"]) ] += int(rc["cdx"]["ncommand"]);
  if ( counter[ std::string(rc["loginx"]["username"]) ] > 256)
    std::cout << "GENERAL ALERT CALL THE GUARD" << std::endl;
}



void testLoginLogout()
{
  std::cout << "------- Unix intrusion test with Login-CD" 
              << std::endl << std::endl;

  RecognitionEngine engine(&std::cout, RecognitionEngine::VERBOSE);

  ChronicleDetectionPiracy& c1 = *(new ChronicleDetectionPiracy());
  engine.addChronicle(c1);

  Event log1("Login");
  log1["username"] = "john";
  Event log2("Login");
  log2["username"] = "ariane";

  engine << 0.0 << log1 << log2 << flush;
  CRL::testInteger((long)c1.getRecognitionSet().size(), 0, false);

  Event cd1("CD");  cd1["username"] = "john"; cd1["ncommand"]=10;
  Event cd2("CD");  cd2["username"] = "ariane"; cd2["ncommand"]=30;
  Event cd3("CD");  cd3["username"] = "ariane"; cd3["ncommand"]=50;
  Event cd4("CD");  cd4["username"] = "ariane"; cd4["ncommand"]=60;

  engine << 10.0 << cd1 << 11.0 << cd2 << 12.0 << cd3 << 13.0 << cd4 << flush;
  CRL::testInteger((long)c1.getRecognitionSet().size(), 4, false);

  Event cd5("CD");  cd5["username"] = "ariane"; cd5["ncommand"]=130;
  engine << 14.0 << cd5 << flush;
  CRL::testInteger((long)c1.getRecognitionSet().size(), 5, false);

  Event logout1("Logout");
  logout1["username"] = "john";
  engine <<logout1 << flush;

  Event log3("Login");
  log3["username"] = "john";
  engine << log3 << flush;
  Event cd6("CD");  cd6["username"] = "john"; cd6["ncommand"]=255;
  engine << 15.0 << cd6 << flush;
  CRL::testInteger((long)c1.getRecognitionSet().size(), 6, false);

  c1.deepDestroy();
  std::cout << std::endl;
}


int testAction1_flag = 0;

void testAction1(RecoTree& r)
{
  std::cout << "AB RECOGNISED" << std::endl; 
  testAction1_flag++;
}


void testAction1()
{
  std::cout << "------- Test testAction1 : (A B)<displays AB RECOGNISED>" 
              << std::endl << std::endl;

  RecognitionEngine engine(&std::cout, RecognitionEngine::VERBOSE);
  ChronicleSequence&    c1 = $(A) + $(B);         //A B
  c1.setActionFunction(testAction1);
  engine.addChronicle(&c1);

  engine << 0.0 << "B" << "A" << flush;
  CRL::testInteger((long)c1.getRecognitionSet().size(), 0, false);
  CRL::testInteger(testAction1_flag, 0, false);

  engine << "B" << flush;
  CRL::testInteger((long)c1.getRecognitionSet().size(), 1, false);
  CRL::testInteger(testAction1_flag, 1, false);

  engine << "B" << flush;
  CRL::testInteger((long)c1.getRecognitionSet().size(), 2, false);
  CRL::testInteger(testAction1_flag, 2, false);

  std::cout << std::endl;

  c1.deepDestroy();
}



void testActionPostEventNow_act(RecoTree& r)
{
  std::cout << "AB RECOGNISED => EVENT C ADDED TO EVENT FLOW" << std::endl; 
  r.getMyEngine()->addEvent( new Event("C"), true );
}


void testActionPostEventNow()
{
  std::cout << "------- Test ActionPostEventNow : (A B)<post evt C>" 
              << std::endl << std::endl;

  RecognitionEngine engine(&std::cout, RecognitionEngine::VERBOSE);
  ChronicleSequence&    c1 = $(A) + $(B);         //  A B
  ChronicleSequence&    c2 =  c1  + $(C);         // (A B) C
  c1.setActionFunction(testActionPostEventNow_act);
  engine.addChronicle(&c1);
  engine.addChronicle(&c2);

  engine << 0.0 << "B" << "A" << flush;
  CRL::testInteger((long)c1.getRecognitionSet().size(), 0, true);
  CRL::testInteger((long)c2.getRecognitionSet().size(), 0, true);

  engine << "B" << flush; // Equivalent to : engine << "B" << "C" << flush;
  CRL::testInteger((long)c1.getRecognitionSet().size(), 1, true);
  CRL::testInteger((long)c2.getRecognitionSet().size(), 1, true);

  engine << "D" << flush;
  CRL::testInteger((long)c1.getRecognitionSet().size(), 1, true);
  CRL::testInteger((long)c2.getRecognitionSet().size(), 1, true);

  std::cout << std::endl;

  c2.deepDestroy();
}


void testActionPostEventInFuture_act(RecoTree& r)
{
  std::cout << "AB RECOGNISED => EVENT C ADDED TO EVENT FLOW IN 4.0 sec" << std::endl; 
  DateType d = r.getMyEngine()->getCurrentTime() + 4.0;
  r.getMyEngine()->addEvent( new Event("C", d), true );
}

void testActionPostEventInFuture()
{
  std::cout << "------- Test ActionPostEventInFuture : (A B)<post evt C>" 
              << std::endl << std::endl;

  RecognitionEngine engine(&std::cout, RecognitionEngine::VERBOSE);
  ChronicleSequence&    c1 = $(A) + $(B);         //  A B
  ChronicleSequence&    c2 =  c1  + $(C);         // (A B) C
  c1.setActionFunction(testActionPostEventInFuture_act);
  engine.addChronicle(&c1);
  engine.addChronicle(&c2);

  engine << 0.0 << "B" << "A" << flush;
  CRL::testInteger((long)c1.getRecognitionSet().size(), 0, true);
  CRL::testInteger((long)c2.getRecognitionSet().size(), 0, true);

  engine << "B";
  engine.process(3.0);
  CRL::testInteger((long)c1.getRecognitionSet().size(), 1, true);
  CRL::testInteger((long)c2.getRecognitionSet().size(), 0, true);

  engine << 5.0 << "D" << flush;
  CRL::testInteger((long)c1.getRecognitionSet().size(), 1, true);
  CRL::testInteger((long)c2.getRecognitionSet().size(), 1, true);

  std::cout << std::endl;

  c2.deepDestroy();
}


int testChangeDelay_flag = 0;

void testChangeDelay(RecoTree& r)
{
  std::cout << "(A then 4) RECOGNISED => DELAY +=1 " << std::endl; 

  ChronicleDelayOp* tmpChr = static_cast<ChronicleDelayOp*>(r.getMyChronicle());

  DurationType d = tmpChr->getDelay() + 1.0;
  tmpChr->setDelay(d);
}


void testChangeDelay()
{
  std::cout << "------- Test testChangeDelay : (A then 4)<delay += 1>" 
              << std::endl << std::endl;

  RecognitionEngine engine(&std::cout, RecognitionEngine::VERBOSE);
  Chronicle&    c1 = $(A) + 4.0;
  c1.setActionFunction(testChangeDelay);
  engine.addChronicle(&c1);

  engine << 0.0 << "A" << flush;
  CRL::testInteger((long)c1.getRecognitionSet().size(), 0, false);

  engine << 4.0 << "A" << flush;
  CRL::testInteger((long)c1.getRecognitionSet().size(), 1, false);

  engine << 8.0 << flush;
  CRL::testInteger((long)c1.getRecognitionSet().size(), 1, false);

  engine << 9.0 << flush;
  CRL::testInteger((long)c1.getRecognitionSet().size(), 2, false);

  std::cout << std::endl;

  c1.deepDestroy();
}





void testAction()
{
  CRL::CRL_ErrReport::START("CRL","ChronicleAction");
  std::cout << "##### ------- Tests of chronicles with actions" 
              << std::endl << std::endl;
  testLoginLogout();
  testAction1();
  testActionPostEventNow();
  testActionPostEventInFuture();
  testChangeDelay();
  Event::freeAllInstances();
  std::cout << std::endl;
}

#ifdef UNITARY_TEST
int main() 
{
  try
  {
    testAction();
    
    CRL::CRL_ErrReport::PRINT_ALL();

    return 0;
  }

  catch(std::string& msg) {                        
    std::cout << "main : "     
    << msg << std::endl;
    return 1;                                      
  }                                                
  catch(const char* msg) {                         
  std::cout << "main : "       
  << msg << std::endl;
  return 1;                                        
  }                                                                                           
  catch(...) {                                     
  std::cout << "main : Unknown Exception"
  << std::endl;
  return 1;                                        
  }

}
#endif