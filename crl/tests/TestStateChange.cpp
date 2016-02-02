/** ***********************************************************************************
 * \file TestStateChange.cpp
 * \author Ariane Piel & Jean Bourrely / Onera DCPS
 * \date 2013
 * \brief Test State Change
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

using namespace CRL;


// ----------------------------------------------------------------------------
// UNIT TESTS
// ----------------------------------------------------------------------------

void testStateChange0()
{
  std::cout << "------- Tests with chronicles (A !! A) and (A !! B)" << std::endl << std::endl;

  RecognitionEngine engine(&std::cout, RecognitionEngine::VERBOSE);
  Chronicle& AdcutA = ($(A) != $(A));
  Chronicle& AdcutB = ($(A) != $(B));
  engine.addChronicle(AdcutA);
  engine.addChronicle(AdcutB);

  engine << 0.0 << "C" << "B" << flush;
  CRL::testInteger((long)AdcutB.getRecognitionSet().size(), 0, false);
  CRL::testInteger((long)AdcutA.getRecognitionSet().size(), 0, false);

  engine << "A" << flush;
  CRL::testInteger((long)AdcutB.getRecognitionSet().size(), 0, false);
  CRL::testInteger((long)AdcutA.getRecognitionSet().size(), 0, false);

  engine << "F" << "A" << flush;
  CRL::testInteger((long)AdcutB.getRecognitionSet().size(), 0, false);
  CRL::testInteger((long)AdcutA.getRecognitionSet().size(), 1, false);

  engine << "B" << flush;
  CRL::testInteger((long)AdcutB.getRecognitionSet().size(), 1, false);
  CRL::testInteger((long)AdcutA.getRecognitionSet().size(), 1, false);

  engine << "B" << flush;
  CRL::testInteger((long)AdcutB.getRecognitionSet().size(), 1, false);
  CRL::testInteger((long)AdcutA.getRecognitionSet().size(), 1, false);

  engine << "A" << flush;
  CRL::testInteger((long)AdcutB.getRecognitionSet().size(), 1, false);
  CRL::testInteger((long)AdcutA.getRecognitionSet().size(), 2, false);

  std::cout << std::endl;

  AdcutA.deepDelete();
  AdcutB.deepDelete();
  delete &AdcutA;
  delete &AdcutB;
}

void testStateChangeFirstToFinish()
{
  std::cout << "------- Tests with chronicle ((E F) !! (B D))" << std::endl << std::endl;

  RecognitionEngine engine(&std::cout, RecognitionEngine::VERBOSE);
  Chronicle& EFdcutBD = (($(E)+$(F)) != ($(B)+$(D)));
  engine.addChronicle(EFdcutBD);

  engine << 0.0 << "C" << "B" << "E" << "F" << "E" << "F"  << flush;
  CRL::testInteger((long)EFdcutBD.getRecognitionSet().size(), 0, false);

  engine << "B" << "D" << flush;
  CRL::testInteger((long)EFdcutBD.getRecognitionSet().size(), 2, false);

  engine << "D" << flush;
  CRL::testInteger((long)EFdcutBD.getRecognitionSet().size(), 2, false);

  std::cout << std::endl;

  EFdcutBD.deepDelete();
  delete &EFdcutBD;
}

void testStateChangeSeveral()
{
  std::cout << "------- Tests with chronicle ((F G) !! (B D E))             (1)" << std::endl << std::endl;

  RecognitionEngine engine(&std::cout, RecognitionEngine::VERBOSE);
  Chronicle& FGdcutBDE = (($(F)+$(G)) != ($(B)+$(D)+$(E)));
  engine.addChronicle(FGdcutBDE);

  engine << 0.0 << "C" << "B" << "F" << "G" << 1.0 << "F" << "G"  << flush;
  CRL::testInteger((long)FGdcutBDE.getRecognitionSet().size(), 0, false);

  engine << "B" << "D" << 2.0 << "D" << flush;
  CRL::testInteger((long)FGdcutBDE.getRecognitionSet().size(), 0, false);

  engine << "E" << flush;
  CRL::testInteger((long)FGdcutBDE.getRecognitionSet().size(), 4, false);

  engine << 3.0 << "E" << flush;
  CRL::testInteger((long)FGdcutBDE.getRecognitionSet().size(), 4, false);

  std::cout << std::endl;

  FGdcutBDE.deepDelete();
  delete &FGdcutBDE;
}


void testStateChangeSeveral2()
{
  std::cout << "------- Tests with chronicle ((F G) !! (B D E))             (2)" << std::endl << std::endl;

  RecognitionEngine engine(&std::cout, RecognitionEngine::VERBOSE);
  Chronicle& FGdcutBDE = (($(F)+$(G)) != ($(B)+$(D)+$(E)));
  engine.addChronicle(FGdcutBDE);

  engine << 0.0 << "C" << "B" << "F" << "G" << 1.0 << "F" << "G"  << flush;
  CRL::testInteger((long)FGdcutBDE.getRecognitionSet().size(), 0, false);

  engine << "B" << "D" << 2.0 << flush;
  CRL::testInteger((long)FGdcutBDE.getRecognitionSet().size(), 0, false);

  engine << "E" << flush;
  CRL::testInteger((long)FGdcutBDE.getRecognitionSet().size(), 2, false);

  engine << 3.0 << "E" << flush;
  CRL::testInteger((long)FGdcutBDE.getRecognitionSet().size(), 2, false);

  std::cout << std::endl;

  FGdcutBDE.deepDelete();
  delete &FGdcutBDE;
}


bool pred(const PropertyManager& p)
{
  return ( (int)p["id1"]["x"] < ((int)p["id2"]["x"]) );
}


void testStateChangePredicate()
{
  std::cout << "------- Tests with chronicle ((A->id1 B) !! D->id2)< id1.x<id2.x >              (1)" << std::endl << std::endl;

  RecognitionEngine engine(&std::cout, RecognitionEngine::VERBOSE);

  CRL::ChronicleStateChange &ABdcutD = (  ( $$($(A),id1) + $(B) ) != $$($(D),id2)  );
  ABdcutD.setPredicateFunction(pred);

  engine.addChronicle(ABdcutD);

  Event aa("A");
  Event ab("A");
  aa["x"] = 1;
  ab["x"] = 2;

  Event da("D");
  Event db("D");
  Event dc("D");
  da["x"] = 1;
  db["x"] = 3;
  dc["x"] = 2;

  engine << 0.0 << aa << ab << "B" << da << flush;
  CRL::testInteger((long)ABdcutD.getRecognitionSet().size(), 0, false);

  engine << db << flush;
  CRL::testInteger((long)ABdcutD.getRecognitionSet().size(), 2, false);

  engine << dc << flush;
  CRL::testInteger((long)ABdcutD.getRecognitionSet().size(), 2, false);

  std::cout << std::endl;

  ABdcutD.deepDelete();
  delete &ABdcutD;
}

void testStateChangePredicate2()
{
  std::cout << "------- Tests with chronicle ((A->id1 B) !! D->id2)< id1.x<id2.x >              (2)" << std::endl << std::endl;

  RecognitionEngine engine(&std::cout, RecognitionEngine::VERBOSE);

  CRL::ChronicleStateChange &ABdcutD = (  ( $$($(A),id1) + $(B) ) != $$($(D),id2)  );
  ABdcutD.setPredicateFunction(pred);

  engine.addChronicle(ABdcutD);

  Event aa("A");
  Event ab("A");
  aa["x"] = 1;
  ab["x"] = 2;

  Event da("D");
  Event dc("D");
  da["x"] = 1;
  dc["x"] = 2;

  engine << 0.0 << aa << ab << "B" << da << flush;
  CRL::testInteger((long)ABdcutD.getRecognitionSet().size(), 0, false);

  engine << dc << flush;
  CRL::testInteger((long)ABdcutD.getRecognitionSet().size(), 1, false);

  std::cout << std::endl;

  ABdcutD.deepDelete();
  delete &ABdcutD;
}



void testChronicleStateChange()
{
  CRL::CRL_ErrReport::START("CRL","ChronicleStateChange");
  std::cout << "##### ------- Tests of ChronicleStateChange class" 
              << std::endl << std::endl;
  testStateChange0();
  testStateChangeFirstToFinish();
  testStateChangeSeveral();
  testStateChangeSeveral2();
  testStateChangePredicate();
  testStateChangePredicate2();
  std::cout << std::endl;
}


#ifdef UNITARY_TEST
int main() 
{
  try
  {
    testChronicleStateChange();
    
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
