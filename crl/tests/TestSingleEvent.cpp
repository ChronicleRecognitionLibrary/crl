/** ***********************************************************************************
 * \file TestSingleEvent.cpp
 * \author Ariane Piel & Jean Bourrely / Onera DCPS
 * \date 2013
 * \brief Test des chroniques sur evts simples
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


bool testSingleEvent1_pred(const PropertyManager& p)
{
  return ( (int)p[Context::ANONYMOUS()]["x"] > 5 );
}


void testSingleEvent1_func(const PropertyManager& inp, PropertyManager& outp)
{
  try
  {
    outp["y"] = ((int)inp[Context::ANONYMOUS()]["x"])  +10;
  }
  catch(...) {}
}

void testSingleEvent1()
{
  std::cout << "------- Tests with chronicles (A), (A)P[X], and (A)f[X]" 
              << std::endl << std::endl;

  RecognitionEngine engine(&std::cout, RecognitionEngine::VERBOSE);

  ChronicleSingleEvent& A1 =  $(A);
  engine.addChronicle(A1);

  ChronicleSingleEvent& A2 =  $(A);
  A2.setPredicateFunction(testSingleEvent1_pred);
  engine.addChronicle(A2);

  ChronicleSingleEvent& A3 =  $(A);
  A3.setOutputPropertiesFunction(testSingleEvent1_func);
  engine.addChronicle(A3);

  ChronicleSingleEvent& A4 =  $(A);
  A4.setPredicateFunction(testSingleEvent1_pred);
  A4.setOutputPropertiesFunction(testSingleEvent1_func);
  engine.addChronicle(A4);

  Event a1("A");
  Event b("B");
  Event a6("A");
  Event b3("B");
  a1["x"] = 1;
  a6["x"] = 6;
  b3["x"] = 3;

  engine << 0.0 << a1 << flush;
  CRL::testInteger((long)A1.getRecognitionSet().size(), 1, false);
  CRL::testInteger((long)A2.getRecognitionSet().size(), 0, false);
  CRL::testInteger((long)A3.getRecognitionSet().size(), 1, false);
  CRL::testInteger((int)((**A3.getRecognitionSet().begin())[Context::ANONYMOUS()]["y"]), 11, true);
  CRL::testInteger((long)A4.getRecognitionSet().size(), 0, false);

  engine << b << flush;
  CRL::testInteger((long)A1.getRecognitionSet().size(), 1, false);
  CRL::testInteger((long)A2.getRecognitionSet().size(), 0, false);
  CRL::testInteger((long)A3.getRecognitionSet().size(), 1, false);
  CRL::testInteger((long)A4.getRecognitionSet().size(), 0, false);

  engine << a6 << flush;
  CRL::testInteger((long)A1.getRecognitionSet().size(), 2, false);
  CRL::testInteger((long)A2.getRecognitionSet().size(), 1, false);
  CRL::testInteger((long)A3.getRecognitionSet().size(), 2, false);
  //CRL::testInteger((int)((**(++(A3.getRecognitionSet().begin())))[Context::ANONYMOUS()]["y"]), 16, true);
  CRL::testInteger((long)A4.getRecognitionSet().size(), 1, false);
  CRL::testInteger((int)((**A4.getRecognitionSet().begin())[Context::ANONYMOUS()]["y"]), 16, true);

  engine << b3 << flush;
  CRL::testInteger((long)A1.getRecognitionSet().size(), 2, false);
  CRL::testInteger((long)A2.getRecognitionSet().size(), 1, false);
  CRL::testInteger((long)A3.getRecognitionSet().size(), 2, false);
  CRL::testInteger((long)A4.getRecognitionSet().size(), 1, false);

  engine << "A" << flush;
  CRL::testInteger((long)A1.getRecognitionSet().size(), 3, false);
  CRL::testInteger((long)A2.getRecognitionSet().size(), 1, false);
  CRL::testInteger((long)A3.getRecognitionSet().size(), 3, false);
  CRL::testInteger((long)A4.getRecognitionSet().size(), 1, false);

  std::cout << std::endl;

  A1.deepDestroy();
  A2.deepDestroy();
  A3.deepDestroy();
  A4.deepDestroy();
}


void testSingleEvent()
{
  CRL::CRL_ErrReport::START("CRL","ChronicleSingleEvent");
  std::cout << "##### ------- Tests of ChronicleSingleEvent class"
              << std::endl << std::endl;
  testSingleEvent1();
  Event::freeAllInstances();
  std::cout << std::endl;
}

#ifdef UNITARY_TEST
int main() 
{
  try
  {
    testSingleEvent();
    
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
