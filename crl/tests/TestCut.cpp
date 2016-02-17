/** ***********************************************************************************
 * \file TestCut.cpp
 * \author Ariane Piel & Jean Bourrely / Onera DCPS
 * \date 2013
 * \brief Test Cut
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

void testCut0()
{
  std::cout << "------- Tests with chronicle (A ! A) et (A ! B)" << std::endl << std::endl;

  RecognitionEngine engine(&std::cout, RecognitionEngine::VERBOSE);
  ChronicleCut& AcutA = ($(A) += $(A));
  ChronicleCut& AcutB = ($(A) += $(B));
  engine.addChronicle(AcutA);
  engine.addChronicle(AcutB);

  engine << 0.0 << "C" << "B" << flush;
  CRL::testInteger((long)AcutB.getRecognitionSet().size(), 0, false);
  CRL::testInteger((long)AcutA.getRecognitionSet().size(), 0, false);

  engine << "A" << flush;
  CRL::testInteger((long)AcutB.getRecognitionSet().size(), 0, false);
  CRL::testInteger((long)AcutA.getRecognitionSet().size(), 0, false);

  engine << "F" << "A" << flush;
  CRL::testInteger((long)AcutB.getRecognitionSet().size(), 0, false);
  CRL::testInteger((long)AcutA.getRecognitionSet().size(), 1, false);

  engine << "B" << flush;
  CRL::testInteger((long)AcutB.getRecognitionSet().size(), 2, false);
  CRL::testInteger((long)AcutA.getRecognitionSet().size(), 1, false);

  engine << "A" << flush;
  CRL::testInteger((long)AcutB.getRecognitionSet().size(), 2, false);
  CRL::testInteger((long)AcutA.getRecognitionSet().size(), 2, false);

  std::cout << std::endl;

  AcutA.deepDestroy();
  AcutB.deepDestroy();
}


void testCutFirstToFinish()
{
  std::cout << "------- Tests with chronicle (A ! (B D))" << std::endl << std::endl;

  RecognitionEngine engine(&std::cout, RecognitionEngine::VERBOSE);
  ChronicleCut& AcutBD = ($(A) += ($(B)+$(D)));
  engine.addChronicle(AcutBD);

  engine << 0.0 << "C" << "B" << flush;
  CRL::testInteger((long)AcutBD.getRecognitionSet().size(), 0, false);

  engine << "A" << flush;
  CRL::testInteger((long)AcutBD.getRecognitionSet().size(), 0, false);

  engine << "B" << "D" << flush;
  CRL::testInteger((long)AcutBD.getRecognitionSet().size(), 1, false);

  engine << "D" << flush;
  CRL::testInteger((long)AcutBD.getRecognitionSet().size(), 1, false);

  std::cout << std::endl;

  AcutBD.deepDestroy();
}

void testCutSeveral()
{
  std::cout << "------- Tests with chronicle (A ! (B D E))" << std::endl << std::endl;

  RecognitionEngine engine(&std::cout, RecognitionEngine::VERBOSE);
  ChronicleCut& AcutBDE = ($(A) += ($(B)+$(D)+$(E)));
  engine.addChronicle(AcutBDE);

  engine << 0.0 << "C" << "B" << flush;
  CRL::testInteger((long)AcutBDE.getRecognitionSet().size(), 0, false);

  engine << "A" << flush;
  CRL::testInteger((long)AcutBDE.getRecognitionSet().size(), 0, false);

  engine << "B" << "D" << "D" << flush;
  CRL::testInteger((long)AcutBDE.getRecognitionSet().size(), 0, false);

  engine << "E" << flush;
  CRL::testInteger((long)AcutBDE.getRecognitionSet().size(), 2, false);

  engine << "E" << flush;
  CRL::testInteger((long)AcutBDE.getRecognitionSet().size(), 2, false);

  std::cout << std::endl;

  AcutBDE.deepDestroy();
}

void testCut1()
{
  std::cout << "------- Tests with chronicle ((A ! B) C)" << std::endl << std::endl;

  RecognitionEngine engine(&std::cout, RecognitionEngine::VERBOSE);
  ChronicleSequence& AcutBC = (($(A) += $(B))+$(C));
  ChronicleSequence& ABC = ($(A) + $(B) + $(C));
  engine.addChronicle(AcutBC);
  engine.addChronicle(ABC);

  engine << 0.0 << "A" << "B" << flush;
  CRL::testInteger((long)AcutBC.getRecognitionSet().size(), 0, false);
  CRL::testInteger((long)ABC.getRecognitionSet().size(), 0, false);

  engine << "B" << "C" << flush;
  CRL::testInteger((long)AcutBC.getRecognitionSet().size(), 1, false);
  CRL::testInteger((long)ABC.getRecognitionSet().size(), 2, false);

  engine  << "C" << flush;
  CRL::testInteger((long)AcutBC.getRecognitionSet().size(), 2, false);
  CRL::testInteger((long)ABC.getRecognitionSet().size(), 4, false);

  std::cout << std::endl;

  AcutBC.deepDestroy();
  ABC.deepDestroy();
}

void testCut2()
{
  std::cout << "------- Tests with chronicle ((0 ! A) B)" << std::endl << std::endl;

  RecognitionEngine engine(&std::cout, RecognitionEngine::VERBOSE);
  ChronicleSequence& T0cutAB = ((T(0.0) += $(A))+$(B));
  engine.addChronicle(T0cutAB);

  engine << 0.0 << "A" << "B" << flush;
  CRL::testInteger((long)T0cutAB.getRecognitionSet().size(), 1, false);

  engine  << "B" << flush;
  CRL::testInteger((long)T0cutAB.getRecognitionSet().size(), 2, false);

  engine << "A" << flush;
  CRL::testInteger((long)T0cutAB.getRecognitionSet().size(), 2, false);

  engine  << "B" << flush;
  CRL::testInteger((long)T0cutAB.getRecognitionSet().size(), 3, false);

  std::cout << std::endl;

  T0cutAB.deepDestroy();
}

void testCut2bis()
{
  std::cout << "------- Tests with chronicle ((0 ! A) B)" << std::endl << std::endl;

  RecognitionEngine engine(&std::cout, RecognitionEngine::VERBOSE);
  ChronicleSequence& T0cutAB = ((T(0.0) += $(A))+$(B));
  engine.addChronicle(T0cutAB);

  Event a("A");
  a.setDate(1.0);

  engine << 0.0 << a << "B" << flush;
  CRL::testInteger((long)T0cutAB.getRecognitionSet().size(), 1, false);

  engine  << "B" << flush;
  CRL::testInteger((long)T0cutAB.getRecognitionSet().size(), 2, false);

  engine << "A" << flush;
  CRL::testInteger((long)T0cutAB.getRecognitionSet().size(), 2, false);

  engine  << "B" << flush;
  CRL::testInteger((long)T0cutAB.getRecognitionSet().size(), 3, false);

  std::cout << std::endl;

  T0cutAB.deepDestroy();
}


void testChronicleCut()
{
  CRL::CRL_ErrReport::START("CRL","ChronicleCut");
  std::cout << "##### ------- Tests of ChronicleCut class" 
              << std::endl << std::endl;
  testCut0();
  testCutFirstToFinish();
  testCutSeveral();
  testCut1();
  testCut2();
  testCut2bis();
  Event::freeAllInstances();
  std::cout << std::endl;
}


#ifdef UNITARY_TEST
int main() 
{
  try
  {
    testChronicleCut();
    
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
