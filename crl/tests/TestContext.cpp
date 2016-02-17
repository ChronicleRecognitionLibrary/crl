/** ***********************************************************************************
 * \file TestContext.cpp
 * \author Ariane Piel & Jean Bourrely / Onera DCPS
 * \date 2012-2013
 * \brief Test Context class
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

#include "Event.h"
#include "Context.h"
#include "TestUtils.h"

using namespace CRL;


// ----------------------------------------------------------------------------
// TESTS UNITAIRES
// ----------------------------------------------------------------------------

void testContext1()
{
  int count = 0; // awaited exceptions counter

  // Default constructor
  Context c1, c2(true);
  CRL::testInteger(c1.size(), 0);
  CRL::testInteger(c2.size(), 1);

  // Copy constructor
  Context c3(c1), c4(c2), c5(c2, false);
  CRL::testInteger(c3.size(), 0);
  CRL::testInteger(c4.size(), 1);
  CRL::testInteger(c5.size(), 0);

  // Methods: add and remove c1 = { }
  c1.add("a");                     
  c1.add("b");
  CRL::testInteger(c1.size(), 2);
  try { c1.add("a"); } catch(...) { count++; } 
  CRL::testInteger(c1.size(), 2);
  c1.remove("c");
  CRL::testInteger(c1.size(), 2);
  c1.remove("a");
  c1.remove("b");
  CRL::testInteger(c1.size(), 0);

  // Method: clear c2 = c4 = {ANONYMOUS}
  c2.clear();                     
  CRL::testInteger(c2.size(), 0);
  c4.clear(true);                 
  CRL::testInteger(c4.size(), 1);
  c4.clear(false);
  CRL::testInteger(c4.size(), 0);

  // Method: [] const
  c1.clear();
  try { c1[0];  } catch(...) { count++; } // out of bounds
  try { c1[-2]; } catch(...) { count++; } // out of bounds
  try { c1[10]; } catch(...) { count++; } // out of bounds
  Context c6(true);
  c6.add("a");
  c6.add("b");    // c6 = { ANONYMOUS, a, b }
  std::cout << c6[0] << " " << c6[1] << " " << c6[2] << std::endl;
  try { c6[-2]; } catch(...) { count++; } // out of bounds
  try { c6[10]; } catch(...) { count++; } // out of bounds
  CRL::testString(c6[1].c_str(), "a");
  CRL::testString(c6[2].c_str(), "b");

  // Method: [] non const
  try { c6[-2] = "aa"; } catch(...) { count++; } // out of bounds
  try { c6[10] = "bb"; } catch(...) { count++; } // out of bounds
  c6[1] = "aa";
  c6[2] = "bb";
  CRL::testString(c6[1].c_str(), "aa");
  CRL::testString(c6[2].c_str(), "bb");

  // Method: contains
  CRL::testBoolean(c6.contains("toto"), false);
  CRL::testBoolean(c6.contains("aa"), true);

  // Method: isSingletonAnonymous
  CRL::testBoolean(c6.isSingletonAnonymous(), false);
  Context c7, c8(true);
  CRL::testBoolean(c7.isSingletonAnonymous(), false);
  CRL::testBoolean(c8.isSingletonAnonymous(), true);

  // Counting of the awaited exceptions
  CRL::testInteger(count, 8);
}


void testContext2()
{
  Context c1;                     // c1 = { }
  Context c2(true);               // c2 = { ANONYMOUS }
  Context c3; c3.add("a");        // c3 = { a }
  Context c4(true); c4.add("a");  // c4 = { ANONYMOUS, a }

  // Method: areEquals
  CRL::testBoolean( Context::areEquals(c1, c1), true );
  CRL::testBoolean( Context::areEquals(c1, c1, true), true );
  CRL::testBoolean( Context::areEquals(c1, c2), false );
  CRL::testBoolean( Context::areEquals(c1, c2, true), true );
  CRL::testBoolean( Context::areEquals(c3, c4), false );
  CRL::testBoolean( Context::areEquals(c3, c4, true), true );

  // Method: areDisjoint
  Context c5; c5.add("b");                     // c5 = { b }
  Context c6(true); c6.add("b");               // c6 = { ANONYMOUS, b }
  Context c7; c7.add("a"); c7.add("b");        // c7 = { a, b }
  Context c8(true); c8.add("a"); c8.add("b");  // c8 = { ANONYMOUS, a, b }
  CRL::testBoolean( Context::areDisjoint(c1, c1), true );
  CRL::testBoolean( Context::areDisjoint(c1, c1, true), true );
  CRL::testBoolean( Context::areDisjoint(c1, c2), true );
  CRL::testBoolean( Context::areDisjoint(c1, c2, true), true );
  CRL::testBoolean( Context::areDisjoint(c3, c4), false );
  CRL::testBoolean( Context::areDisjoint(c3, c4, true), false );

  CRL::testBoolean( Context::areDisjoint(c3, c5, true),  true );
  CRL::testBoolean( Context::areDisjoint(c3, c5, false), true );
  CRL::testBoolean( Context::areDisjoint(c3, c6, true),  true );
  CRL::testBoolean( Context::areDisjoint(c3, c6, false), true );
  CRL::testBoolean( Context::areDisjoint(c3, c7, true),  false );
  CRL::testBoolean( Context::areDisjoint(c3, c7, false), false );
  CRL::testBoolean( Context::areDisjoint(c3, c8, true),  false );
  CRL::testBoolean( Context::areDisjoint(c3, c8, false), false );

  CRL::testBoolean( Context::areDisjoint(c4, c5, true),  true );
  CRL::testBoolean( Context::areDisjoint(c4, c5, false), true );
  CRL::testBoolean( Context::areDisjoint(c4, c6, true),  true );
  CRL::testBoolean( Context::areDisjoint(c4, c6, false), false );
  CRL::testBoolean( Context::areDisjoint(c4, c7, true),  false );
  CRL::testBoolean( Context::areDisjoint(c4, c7, false), false );
  CRL::testBoolean( Context::areDisjoint(c4, c8, true),  false );
  CRL::testBoolean( Context::areDisjoint(c4, c8, false), false );

  // Method: contextUnion
  Context result;
  Context::contextUnion(c1, c1, result);         // { }+{ } 
  CRL::testInteger(result.size(), 0);
  Context::contextUnion(c1, c1, result, true);   // { }+{ } 
  CRL::testInteger(result.size(), 0);
  Context::contextUnion(c2, c2, result);         // { ANONYMOUS }+{ ANONYMOUS }
  CRL::testInteger(result.size(), 1);
  Context::contextUnion(c2, c2, result, true);   // { ANONYMOUS }+{ ANONYMOUS }
  CRL::testInteger(result.size(), 0);
  Context::contextUnion(c1, c4, result);         // { }+{ ANONYMOUS, a }
  CRL::testInteger(result.size(), 2);
  Context::contextUnion(c4, c1, result);         // Same
  CRL::testInteger(result.size(), 2);
  Context::contextUnion(c1, c4, result, true);   // { }+{ ANONYMOUS, a }
  CRL::testInteger(result.size(), 1);
  Context::contextUnion(c4, c1, result, true);   // Same
  CRL::testInteger(result.size(), 1);
  Context::contextUnion(c4, c6, result);         // { ANONYMOUS, a }+{ ANONYMOUS, b }
  CRL::testInteger(result.size(), 3);
  Context::contextUnion(c6, c4, result);         // Same
  CRL::testInteger(result.size(), 3);
  Context::contextUnion(c4, c6, result, true);   // { ANONYMOUS, a }+{ ANONYMOUS, b }
  CRL::testInteger(result.size(), 2);
  Context::contextUnion(c6, c4, result, true);   // Same
  CRL::testInteger(result.size(), 2);

  // Method: contextUnion
  Context::contextIntersection(c1, c1, result);         // { }+{ } 
  CRL::testInteger(result.size(), 0);
  Context::contextIntersection(c1, c1, result, true);   // { }+{ } 
  CRL::testInteger(result.size(), 0);
  Context::contextIntersection(c2, c2, result);         // { ANONYMOUS }+{ ANONYMOUS }
  CRL::testInteger(result.size(), 1);
  Context::contextIntersection(c2, c2, result, true);   // { ANONYMOUS }+{ ANONYMOUS }
  CRL::testInteger(result.size(), 0);
  Context::contextIntersection(c1, c4, result);         // { }+{ ANONYMOUS, a }
  CRL::testInteger(result.size(), 0);
  Context::contextIntersection(c2, c4, result);         // { ANONYMOUS }+{ ANONYMOUS, a }
  CRL::testInteger(result.size(), 1);
  Context::contextIntersection(c2, c4, result, true);   // Same
  CRL::testInteger(result.size(), 0);
  Context::contextIntersection(c4, c2, result);         // { ANONYMOUS }+{ ANONYMOUS, a }
  CRL::testInteger(result.size(), 1);
  Context::contextIntersection(c4, c2, result, true);   // Same
  CRL::testInteger(result.size(), 0);

  std::cout << std::endl;
}


void testContext()
{
  CRL::CRL_ErrReport::START("CRL", "Context");
  std::cout << "##### ------- Tests of Context class" << std::endl << std::endl;

  testContext1();
  testContext2();
  Event::freeAllInstances();
}


#ifdef UNITARY_TEST
int main() 
{
  try
  {
    testContext();
    
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