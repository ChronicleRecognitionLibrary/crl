/** ***********************************************************************************
 * \file TestProperty.cpp
 * \author Ariane Piel & Jean Bourrely / Onera DCPS
 * \date 2012
 * \brief Test Property class
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
#include "TestUtils.h"
#include "Property.h"
#include "PropertyManager.h"

using namespace CRL;


// ----------------------------------------------------------------------------
// TESTS UNITAIRES
// ----------------------------------------------------------------------------

void testProperty()
{
  CRL::CRL_ErrReport::START("CRL", "Property");
  std::cout << "##### ------- Tests of Property/PropertyManager classes"
              << std::endl << std::endl;

  CRL::PropertyManager m1;

  // 1) Level 1 properties
  m1["b1"]    = true;
  m1["c1"]    = 'a';
  m1["wc1"]   = L'b';
  m1["i1"]    = 30;
  m1["ui1"]   = 40U;
  m1["l1"]    = 50L;
  m1["ul1"]   = 60UL;
  m1["f1"]    = 3.14f;
  m1["d1"]    = 3.14159;
  m1["str1"]  = "Hello";
  m1["wstr1"] = L"World";
  m1["str2"]  = std::string("Hello2");
  m1["wstr2"] = std::wstring(L"World2");

  CRL::testBoolean((bool)m1["b1"] == true, true);
  CRL::testBoolean((char)m1["c1"] == 'a', true);
  CRL::testBoolean((wchar_t)m1["wc1"] == L'b', true);
  CRL::testBoolean((int)m1["i1"] == 30, true);
  CRL::testBoolean((unsigned int)m1["ui1"] == 40U, true);
  CRL::testBoolean((long)m1["l1"] == 50L, true);
  CRL::testBoolean((unsigned long)m1["ul1"] == 60UL, true);
  CRL::testBoolean((float)m1["f1"] == 3.14f, true);
  CRL::testBoolean((double)m1["d1"] == 3.14159, true);
  CRL::testBoolean((std::string)m1["str1"] == "Hello", true);
  CRL::testBoolean((std::wstring)m1["wstr1"] == L"World", true);
  CRL::testBoolean((std::string)m1["str2"] == "Hello2", true);
  CRL::testBoolean((std::wstring)m1["wstr2"] == L"World2", true);

  // 2) Exception tests
  int count = 0;
  try{ std::cout << (int)m1["unknown"] << std::endl; } catch(...) { count++; }
  try{ std::cout << (int)m1["b1"]      << std::endl; } catch(...) { count++; }
  try{ std::cout << (bool)m1["c1"]     << std::endl; } catch(...) { count++; }
  try{ std::cout << (bool)m1["wc1"]    << std::endl; } catch(...) { count++; }
  try{ std::cout << (bool)m1["i1"]     << std::endl; } catch(...) { count++; }
  try{ std::cout << (bool)m1["ui1"]    << std::endl; } catch(...) { count++; }
  try{ std::cout << (bool)m1["l1"]     << std::endl; } catch(...) { count++; }
  try{ std::cout << (bool)m1["ul1"]    << std::endl; } catch(...) { count++; }
  try{ std::cout << (bool)m1["f1"]     << std::endl; } catch(...) { count++; }
  try{ std::cout << (bool)m1["d1"]     << std::endl; } catch(...) { count++; }
  try{ std::cout << (bool)m1["str1"]   << std::endl; } catch(...) { count++; }
  try{ std::cout << (bool)m1["wstr1"]  << std::endl; } catch(...) { count++; }
  CRL::testInteger(count, 12);

  // 3) Properties of level 2 and more
  m1["a"]           = 10;
  m1["a"]["b"]      = 20;
  m1["a"]["b"]["c"] = 30;

  CRL::testBoolean((int)m1["a"]           == 10, true);
  CRL::testBoolean((int)m1["a"]["b"]      == 20, true);
  CRL::testBoolean((int)m1["a"]["b"]["c"] == 30, true);

  m1["a"]["d"]      = 3.14;
  CRL::testBoolean((double)m1["a"]["d"]   == 3.14, true);

  count = 0;
  try{ std::cout << (int)m1["a"]["??"] << std::endl; } catch(...) { count++; }
  try{ std::cout << (bool)m1["a"]["b"] << std::endl; } catch(...) { count++; }
  CRL::testInteger(count, 2);
  
  Event::freeAllInstances();
  std::cout << std::endl;
}


#ifdef UNITARY_TEST
int main() 
{
  try
  {
    testProperty();
    
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