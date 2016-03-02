/** ***********************************************************************************
 * \file TestChronicle.cpp
 * \author Ariane Piel & Jean Bourrely / Onera DCPS
 * \date 2012
 * \brief Test Chronicle
 **************************************************************************************/

/*  Copyright (C) 2012, 2013, 2014  ONERA � http://www.onera.fr
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

#include "Event.h"
#include "Chronicle.h"
#include "Operators.h"
#include "ChronicleConjunction.h"
#include "RecoTreeSingle.h"
#include "TestUtils.h"

using namespace CRL;


// ----------------------------------------------------------------------------
// UNIT TESTS
// ----------------------------------------------------------------------------

class TestChronicle : public Chronicle
{
public:
  std::string toString() const { return std::string("Test"); }
  bool process(const DateType& d, CRL::Event* e = NULL) { return true; }
  DateType lookAhead(const DateType& tcurr) const { return 0.0; }
  void eval(const std::string&, const RecoTree&, PropertyManager&) {  }
  void setMyEngine(CRL::RecognitionEngine*) {  }
  Chronicle* getChild1() { return NULL; }
  Chronicle* getChild2() { return NULL; }
};



void testChronicle()
{
  CRL::CRL_ErrReport::START("CRL", "Chronicle");
  std::cout << "##### ------- Tests of Chronicle class" << std::endl << std::endl;

  TestChronicle tc1;
  std::cout << tc1.prettyPrint();

  // Accessor
  CRL::testInteger((long)tc1.getNewRecognitions().size(), 0);
  CRL::testInteger((long)tc1.getRecognitionSet().size(), 0);

  RecoTreeSingle *r1 = new RecoTreeSingle;
  RecoTreeSingle *r2 = new RecoTreeSingle;
  tc1.getNewRecognitions().insert(r1);
  tc1.getRecognitionSet().insert(r1);
  tc1.getRecognitionSet().insert(r2);
  CRL::testInteger((long)tc1.getNewRecognitions().size(), 1);
  CRL::testInteger((long)tc1.getRecognitionSet().size(), 2);

  tc1.purgeNewRecognitions();
  CRL::testInteger((long)tc1.getNewRecognitions().size(), 0);

  CRL::testBoolean(tc1.isIn(*r1, tc1.getNewRecognitions()), false);
  CRL::testBoolean(tc1.isIn(*r1, tc1.getRecognitionSet()), true);
  CRL::testBoolean(tc1.isIn(*r2, tc1.getRecognitionSet()), true);

  // Tree accessors
  Chronicle& cT     = T(1.0);
  Chronicle& cA     = $(A);
  Chronicle& cB     = $(B);
  Chronicle& cAT    = AT(cA);
  Chronicle& cAnd   = cA && cB;
  Chronicle& cNamed = $$(cA,aa);
  Chronicle& cDelay = cA + 1.0;
  
  CRL::testBoolean(cT.getChild() == NULL, true);
  CRL::testBoolean(cT.getChildLeft() == NULL, true);
  CRL::testBoolean(cT.getChildRight() == NULL, true);

  CRL::testBoolean(cA.getChild() == NULL, true);
  CRL::testBoolean(cA.getChildLeft() == NULL, true);
  CRL::testBoolean(cA.getChildRight() == NULL, true);

  CRL::testBoolean(cAT.getChild() == &cA, true);
  CRL::testBoolean(cAT.getChildLeft() == &cA, true);
  CRL::testBoolean(cAT.getChildRight() == NULL, true);

  CRL::testBoolean(cAnd.getChild() == &cA, true);
  CRL::testBoolean(cAnd.getChildLeft() == &cA, true);
  CRL::testBoolean(cAnd.getChildRight() == &cB, true);

  CRL::testBoolean(cDelay.getChild() == &cA, true);
  CRL::testBoolean(cDelay.getChildLeft() == &cA, true);
  CRL::testBoolean(cDelay.getChildRight() == NULL, true);

  CRL::testBoolean(cNamed.getChild() == &cA, true);
  CRL::testBoolean(cNamed.getChildLeft() == &cA, true);
  CRL::testBoolean(cNamed.getChildRight() == NULL, true);

  Event::freeAllInstances();
  std::cout << std::endl;
}


#ifdef UNITARY_TEST
int main() 
{
  try
  {
    testChronicle();
    
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
