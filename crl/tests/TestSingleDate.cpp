/** ***********************************************************************************
 * \file TestSingleEvent.cpp
 * \author Ariane Piel & Jean Bourrely / Onera DCPS
 * \date 2013
 * \brief Tests on single date chronicles
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

#include "Context.h"
#include "TestUtils.h"
#include "Operators.h"
#include "PropertyManager.h"
#include "RecognitionEngine.h"


using namespace CRL;


// ----------------------------------------------------------------------------
// UNIT TESTS
// ----------------------------------------------------------------------------

void testSingleDate0PromptedByTimeEvent()
{
  std::cout << "------- Tests with chronicle T(0) (prompted by time-event)" 
              << std::endl << std::endl;

  RecognitionEngine engine(&std::cout, RecognitionEngine::VERBOSE);
  engine.setCurrentTime(-1.0);

  ChronicleSingleDate& T0 = T(0.0);
  engine.addChronicle(T0);

  engine.process(0.0);
  CRL::testInteger((long)T0.getRecognitionSet().size(), 1, false);
  engine.process(1.0);
  CRL::testInteger((long)T0.getRecognitionSet().size(), 1, false);

  std::cout << std::endl;

  T0.deepDelete();
  delete &T0;
}

void testSingleDate0PromptedByEvent()
{
  std::cout << "------- Tests with chronicle T(0) (prompted by event)" 
              << std::endl << std::endl;

  RecognitionEngine engine(&std::cout, RecognitionEngine::VERBOSE);
  engine.setCurrentTime(-1.0);

  ChronicleSingleDate& T0 = T(0.0);
  engine.addChronicle(T0);

  Event a("A");
  a.setDate(0.0);

  engine.process(a);
  CRL::testInteger((long)T0.getRecognitionSet().size(), 1, false);
  engine.process(1.0);
  CRL::testInteger((long)T0.getRecognitionSet().size(), 1, false);

  std::cout << std::endl;

  T0.deepDelete();
  delete &T0;
}

void testSingleDate0PromptedByLookAhead()
{
  std::cout << "------- Tests with chronicle T(0) (prompted by LookAhead)" 
              << std::endl << std::endl;

  RecognitionEngine engine(&std::cout, RecognitionEngine::VERBOSE);
  engine.setCurrentTime(-1.0);

  ChronicleSingleDate& T0 = T(0.0);
  engine.addChronicle(T0);

  engine.process(1.0);
  CRL::testInteger((long)T0.getRecognitionSet().size(), 1, false);

  std::cout << std::endl;

  T0.deepDelete();
  delete &T0;
}

void testSingleDatePromptedByLookAhead()
{
  std::cout << "------- Tests with chronicle T(5) (prompted by LookAhead)" 
              << std::endl << std::endl;

  RecognitionEngine engine(&std::cout, RecognitionEngine::VERBOSE);

  ChronicleSingleDate& T5 = T(5.0);
  engine.addChronicle(T5);

  engine << 0.0 << 1.0 << "A" << flush;
  CRL::testInteger((long)T5.getRecognitionSet().size(), 0, false);

  engine << 6.0 << "A" << flush;
  CRL::testInteger((long)T5.getRecognitionSet().size(), 1, false);

  std::cout << std::endl;

  T5.deepDelete();
  delete &T5;
}

void testSingleDatePromptedByEvent()
{
  std::cout << "------- Tests with chronicle T(5) (prompted by event)" 
              << std::endl << std::endl;

  RecognitionEngine engine(&std::cout, RecognitionEngine::VERBOSE);

  ChronicleSingleDate& T5 =  T(5.0);
  engine.addChronicle(T5);

  Event a("A");
  a.setDate(5.0);

  engine << 0.0 << 1.0 << "A" << flush;
  CRL::testInteger((long)T5.getRecognitionSet().size(), 0, false);

  engine << a << flush;
  CRL::testInteger((long)T5.getRecognitionSet().size(), 1, false);

  engine << 8.0 << flush;
  CRL::testInteger((long)T5.getRecognitionSet().size(), 1, false);

  std::cout << std::endl;

  T5.deepDelete();
  delete &T5;
}

void testSingleDatePromptedByTimeEvent()
{
  std::cout << "------- Tests with chronicle T(5) (prompted by time-event)" 
              << std::endl << std::endl;

  RecognitionEngine engine(&std::cout, RecognitionEngine::VERBOSE);

  ChronicleSingleDate& T5 =  T(5.0);
  engine.addChronicle(T5);

  engine << 0.0 << 1.0 << "A" << flush;
  CRL::testInteger((long)T5.getRecognitionSet().size(), 0, false);

  engine << 5.0 << flush;
  CRL::testInteger((long)T5.getRecognitionSet().size(), 1, false);

  engine << 8.0 << flush;
  CRL::testInteger((long)T5.getRecognitionSet().size(), 1, false);

  std::cout << std::endl;

  T5.deepDelete();
  delete &T5;
}


void testSingleDate()
{
  CRL::CRL_ErrReport::START("CRL","ChronicleSingleDate");
  std::cout << "##### ------- Tests of ChronicleSingleDate class"
              << std::endl << std::endl;
  testSingleDate0PromptedByTimeEvent();
  testSingleDate0PromptedByEvent();
  testSingleDate0PromptedByLookAhead();
  testSingleDatePromptedByLookAhead();
  testSingleDatePromptedByEvent();
  testSingleDatePromptedByTimeEvent();
  std::cout << std::endl;
}


#ifdef UNITARY_TEST
int main() 
{
  try
  {
    testSingleDate();
    
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