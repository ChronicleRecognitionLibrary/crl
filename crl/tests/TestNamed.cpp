/** ***********************************************************************************
 * \file TestNamed.cpp
 * \author Ariane Piel & Jean Bourrely / Onera DCPS
 * \date 2013
 * \brief Test des chroniques nommees
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


bool testNamed1_pred(const PropertyManager& p)
{
  return ( (int)p[Context::ANONYMOUS()]["x"] > 5 );
}


void testNamed1_func(const PropertyManager& inp, PropertyManager& outp)
{
  try
  {
    outp["y"] = ((int)inp[Context::ANONYMOUS()]["x"])  +10;
  }
  catch(...) {}
}

void testNamed1()
{
  std::cout << "------- Tests with chronicles (A), (A->x)P[X], and  (((A)f[X])->x)P[X]" 
              << std::endl << std::endl;

  RecognitionEngine engine(&std::cout, RecognitionEngine::VERBOSE);

  ChronicleNamed& A1 =  $$($(A),aa);
  engine.addChronicle(A1);

  ChronicleNamed& A2 =  $$($(A),aa);
  A2.setPredicateFunction(testNamed1_pred);
  engine.addChronicle(A2);

  ChronicleNamed& A3 =  $$($(A),aa);
  A3.setOutputPropertiesFunction(testNamed1_func);
  engine.addChronicle(A3);

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
  CRL::Chronicle::RecoSet::iterator it =  A3.getRecognitionSet().begin();
  CRL::testInteger((int)((**it)[Context::ANONYMOUS()]["y"]), 11, true);

  engine << b << flush;
  CRL::testInteger((long)A1.getRecognitionSet().size(), 1, true);
  CRL::testInteger((long)A2.getRecognitionSet().size(), 0, true);
  CRL::testInteger((long)A3.getRecognitionSet().size(), 1, true);

  engine << a6 << flush;
  CRL::testInteger((long)A1.getRecognitionSet().size(), 2, true);
  CRL::testInteger((long)A2.getRecognitionSet().size(), 1, true);
  CRL::testInteger((long)A3.getRecognitionSet().size(), 2, true);
  it =  A3.getRecognitionSet().begin();
  if ( (int)((**it)[Context::ANONYMOUS()]["y"]) == 11)
    it++;
  CRL::testInteger((int)((**it)[Context::ANONYMOUS()]["y"]), 16, true);

  engine << b3 << flush;
  CRL::testInteger((long)A1.getRecognitionSet().size(), 2, false);
  CRL::testInteger((long)A2.getRecognitionSet().size(), 1, false);
  CRL::testInteger((long)A3.getRecognitionSet().size(), 2, false);

  engine << "A" << flush;
  CRL::testInteger((long)A1.getRecognitionSet().size(), 3, false);
  CRL::testInteger((long)A2.getRecognitionSet().size(), 1, false);
  CRL::testInteger((long)A3.getRecognitionSet().size(), 3, false);

  std::cout << std::endl;

  A1.deepDelete();
  A2.deepDelete();
  A3.deepDelete();
  delete &A1;
  delete &A2;
  delete &A3;
}


void testNamed()
{
  CRL::CRL_ErrReport::START("CRL","ChronicleNamed");
  std::cout << "##### ------- Tests of ChronicleNamed class"
              << std::endl << std::endl;
  testNamed1();
  std::cout << std::endl;
}



#ifdef UNITARY_TEST
int main() 
{
  try
  {
    testNamed();
    
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