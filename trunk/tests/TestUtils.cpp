/** ***********************************************************************************
 * \file TestUtils.cpp
 * \author Ariane Piel & Jean Bourrely / Onera DCPS
 * \date 2012
 * \brief Utilities for CRL unit tests
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

#include <iostream>
#include <cstring>
#include <cctype>
#include <cmath>
#include <cstdlib>

#include "TestUtils.h"


// --------------------------------------------------------------------
// CLASSE CRL_ErrReport FOR UNIT TESTS/VALIDATION
// --------------------------------------------------------------------

namespace CRL
{

  //-----------------------------------------------------------
  // Initialisation of class variables
  //-----------------------------------------------------------

  CRL_ErrReport*  CRL_ErrReport::currentModule = NULL;

  CRL_ErrReport::Element* CRL_ErrReport::currentElement = NULL;

  int CRL_ErrReport::totalNbErrors = 0;

  std::list<CRL_ErrReport*> CRL_ErrReport::modules;

  // Initialiation of a global object which will be destroyed at the end of
  // teh execution of the program and will then call RESET, thus avoiding
  // memory leaks if the user has forgotten.
  CRL_ErrReport::GlobalRESET theInstanceOfGlobalRESET;

  //-----------------------------------------------------------
  // Méthods of CRL_ErrReport class
  //-----------------------------------------------------------

  /** Creates the module with its name, initialises the error counter to zero.
   *  \param[in] s name of the module to be created
   */
  CRL_ErrReport::CRL_ErrReport(const std::string& s)
    : name(s), nbErrors(0) {}

  
  CRL_ErrReport::~CRL_ErrReport() 
  {
    std::list<Element*>::iterator it;
    for (it = elements.begin(); it != elements.end(); it++) {
      delete (*it);
    }
    elements.clear();
  }


  /** Returns the element if it exists, if not, returns \a NULL.
   *  \param[in] s name of the looked-for element
   *  \return the element
   */
  CRL_ErrReport::Element* 
  CRL_ErrReport::getElement(const std::string& s) const
  {
    std::list<Element*>::const_iterator cit;
    for (cit = elements.begin(); cit != elements.end(); cit++)
      if ((*cit)->name == s) return (*cit);
      return NULL;
  }


  /** \param[in] s name of the element to be created
   *  \return the element
   */
  CRL_ErrReport::Element* 
  CRL_ErrReport::addElement(const std::string& s)
  {
    Element* ele = new Element(s);
    elements.push_back(ele);
    return ele;
  }
  

  /** returns the module if it exists, if not, returns \a NULL.
   *  \param[in] s name of the looked-for module
   *  \return the module
   */
  CRL_ErrReport* 
  CRL_ErrReport::getModule(const std::string& s)
  {
    std::list<CRL_ErrReport*>::const_iterator cit;
    for (cit = modules.begin(); cit != modules.end(); cit++)
      if ((*cit)->name == s) return (*cit);
      return NULL;
  }
  

  /** \param[in] s name of the module to be created
   *  \return the module
   */
  CRL_ErrReport* 
  CRL_ErrReport::addModule(const std::string& s)
  {
    CRL_ErrReport* mod = new CRL_ErrReport(s);
    modules.push_back(mod);
    return mod;
  }

  
  /** Changes the current module and test element. If they don't
   *  exist, the function creates them and adds them to their respective 
   *  lists.
   *  \param[in] mod name of the module
   *  \param[in] ele name of the element
   */
  void 
  CRL_ErrReport::START(const char* mod, const char* ele)
  {
    std::string moduleName = std::string(mod); 
    currentModule = getModule(moduleName);
    if (currentModule == NULL)               
      currentModule = addModule(moduleName);
    
    std::string fctName = std::string(ele);   
    currentElement = currentModule->getElement(fctName);
    if (currentElement == NULL)  
      currentElement = currentModule->addElement(fctName);
  }
  

  /** Reinitialises everything, deletes all statistics
   */
  void 
  CRL_ErrReport::RESET()
  {
    std::list<CRL_ErrReport*>::iterator it;
    for (it = modules.begin(); it != modules.end(); it++) {
      delete (*it);
    }
    modules.clear();

    currentModule = NULL;
    currentElement = NULL;
    totalNbErrors = 0;
  }


  /** This method increments three error counters: the one of the current test element,
   *  the one of the current module, and the global counter.
   */
  void 
  CRL_ErrReport::INCREMENT_ERROR()
  {
    if (currentElement) 
      currentElement->nbErrors++;   
    if (currentModule)                
      currentModule->nbErrors++;    
    totalNbErrors++;
  }
  

  /** Only displays on the logger the number of errors collected by the 
   *  global counter, regarding the entire execution of the program.
   */
  void 
  CRL_ErrReport::PRINT()
  {
    std::cout
      << "\n---------------------------------------------\n"
      << "- GLOBAL ERROR REPORT = "
      << totalNbErrors << " errors"
      << "\n---------------------------------------------\n"
      << std::endl;
  }
  

  /** Displays on the logger all the error information which has been collected, 
   *  for each test element and each module.
   */
  void 
  CRL_ErrReport::PRINT_ALL()
  {
    PRINT();
    std::list<CRL_ErrReport*>::const_iterator cit1;
    std::list<Element*>::const_iterator cit2;
    for (cit1 = modules.begin(); cit1 != modules.end(); cit1++)
    {
      if ((*cit1)->nbErrors == 0) {
        std::cout
          << " 0\t error in module "
          << (*cit1)->name << std::endl;
      } else {
        std::cout 
          << " " << (*cit1)->nbErrors << "\t errors in module "
          << (*cit1)->name << std::endl;
      }
      for (cit2 = (*cit1)->elements.begin(); 
      cit2 != (*cit1)->elements.end(); cit2++)
      {
        if ((*cit2)->nbErrors == 0) {
          std::cout
            << "   0\t error in element "
            << (*cit2)->name << std::endl;
        } else {
          std::cout
            << "   " << (*cit2)->nbErrors << "\t errors in element "
            << (*cit2)->name << std::endl;
        }
      }
    }
  }

  
  /** Retruns a boolean indicating whether there are some errors (true) or not (false)
   */
  bool
  CRL_ErrReport::HAS_ERRORS() {
    return (totalNbErrors > 0);
  }
  
  
  /** If there are some errors, reinitialises the error counter and exits with an error code (EXIT_FAILURE)
   */
  void 
  CRL_ErrReport::EXIT_IF_FAILURE()
  {
  if (HAS_ERRORS()) 
    {
      RESET();
      exit (EXIT_FAILURE);
    }
  }

} // namespace CRL


// --------------------------------------------------------------------
// Functions for user interactions
// --------------------------------------------------------------------

namespace CRL {
 
  
  // --------------------------------------------------------------------
  // Equality tests between two real numbers
  // --------------------------------------------------------------------
  
  /** The function returns \c true if the absolute precision is reached 
   *  \b OR the relative precision : \n
   *  - if <tt>    |x-y| <= precision     </tt>
   *  - or if <tt> |x-y| <= precision*|x| </tt> 
   *
   *  \param[in] x the value to be tested
   *  \param[in] y the awaited value
   *  \param[in] the relative and absolute precision
   *  \return Returns the result of the comparison
   */
  bool isEqual(double x, double y, double precision)
  {
    double diff = fabs(x-y);
    return ( (diff <= precision*fabs(x)) || (diff <= precision) );
  }


  /** Returns \c true if the relative precision is reached.
   *
   *  \param[in] x the value to be tested
   *  \param[in] y the awaited value
   *  \param[in] digits the relative precision (2 => 1%)
   *  \return the comparison result
   */
  bool isEqualRel(double x, double y, unsigned int digits)
  {
    return (fabs(x-y) <= pow(0.1, (double)digits)*fabs(y));
  }
  
  
  // --------------------------------------------------------------------
  // Unit tests
  // --------------------------------------------------------------------
    
  /** If the two values to be compared are equal and if \a verbose is
   *  true, the following kind of message is displayed : 'value1 (awaited : value2)'.
   *  If the values are not equal, disregarding \a verbose, an
   *  error message is displayed and the error test counters are incremented : CRL::CRL_ErrReport::INCREMENT_ERROR().
   *  \param[in] b1 the boolean to be tested
   *  \param[in] b2 the awaited boolean
   *  \param[in] verbose true if message has to be displayed no matter what the result is
   */
  void testBoolean(bool b1, bool b2, bool verbose) 
  {
    if (b1 == b2)
    {
      if (b1 == true)
      {
        if (verbose) 
          std::cout
          << "TRUE  (awaited : TRUE)" << std::endl;
      }
      else 
      {
        if (verbose) 
        {
          std::cout
            << "FALSE (awaited : FALSE)" << std::endl;
        }
      }
    }
    else
    {
      CRL::CRL_ErrReport::INCREMENT_ERROR();
      if (b1 == true)
        std::cout
          << "TRUE  (awaited : FALSE) ***** ERROR *****" << std::endl;
      else
        std::cout
          << "FALSE (awaited : TRUE) ***** ERROR *****" << std::endl;
    }
  }
  

  /** If the two values to be compared are equal and if \a verbose is
   *  true, the following kind of message is displayed : 'value1 (awaited : value2)'.
   *  If the values aren't equal, disregarding the value of \a verbose, an
   *  error message is displayed and the test error counter is incremented : CRL::CRL_ErrReport::INCREMENT_ERROR().
   *  \param[in] i1 the integer to be tested
   *  \param[in] i2 the awaited integer
   *  \param[in] verbose true if message has to be displayed no matter what the result is
   */
  void testInteger(long i1, long i2, bool verbose)
  {
    if (i1 == i2)
    {
      if (verbose) 
        std::cout
          << i1 << " (awaited : " << i2 << ")" << std::endl;
    }
    else 
    {
      CRL::CRL_ErrReport::INCREMENT_ERROR();
      std::cout << i1 << " (awaited : " << i2 
        << ") ***** ERROR *****" << std::endl;
    }
  }
  

  /** This method uses
   *  bool CRL::isEqual(double x, double y, double precision) \n
   *  If the two values to be compared are equal and if \a verbose is
   *  true, the following kind of message is displayed : 'value1 (awaited : value2)'.
   *  If the values aren't equal, disregarding the value of \a verbose, an
   *  error message is displayed and the test error counter is incremented : CRL::CRL_ErrReport::INCREMENT_ERROR().
   *  \param[in] d1 the real number to be tested
   *  \param[in] d2 the awaited real number
   *  \param[in] precision the absolute and relative precision
   *  \param[in] verbose true if message has to be displayed no matter what the result is
   */
  void 
  testDouble(double d1, double d2, double precision, bool verbose)
  {
    if (isEqual(d1, d2, precision))
    {
      if (verbose) 
        std::cout
        << d1 << " (awaited : " << d2 << ")" << std::endl;
    }
    else 
    {
      CRL::CRL_ErrReport::INCREMENT_ERROR();
      std::cout << d1 << " (awaited : " << d2 
        << ") ***** ERROR *****" << std::endl;
    }
  }
  

  /** This method uses 
   *  bool CRL::isEqualRel(double x, double y, unsigned int digits) \n
   *  If the two values to be compared are equal and if \a verbose is
   *  true, the following kind of message is displayed : 'value1 (awaited : value2)'.
   *  If the values aren't equal, disregarding the value of \a verbose, an
   *  error message is displayed and the test error counter is incremented : CRL::CRL_ErrReport::INCREMENT_ERROR().
   *  \param[in] d1 the real number to be tested
   *  \param[in] d2 the awaited real number
   *  \param[in] digits the absolute and relative precision
   *  \param[in] verbose true if message has to be displayed no matter what the result is
   */
  void 
  testDoubleRel(double d1, double d2, unsigned int digits, bool verbose)
  {
    if (isEqualRel(d1, d2, digits))
    {
      if (verbose) 
        std::cout
        << d1 << " (awaited : " << d2 << ")" << std::endl;
    }
    else 
    {
      CRL::CRL_ErrReport::INCREMENT_ERROR();
      std::cout << d1 << " (awaited : " << d2 
        << ") ***** ERROR *****" << std::endl;
    }
  }


  /** If the two values to be compared are equal and if \a verbose is
   *  true, the following kind of message is displayed : 'value1 (awaited : value2)'.
   *  If the values aren't equal, disregarding the value of \a verbose, an
   *  error message is displayed and the test error counter is incremented : CRL::CRL_ErrReport::INCREMENT_ERROR().
   *  \param[in] s1 the string to be tested
   *  \param[in] s2 the awaited string
   *  \param[in] verbose true if message has to be displayed no matter what the result is
   *  \param[in] strictCase true if case has to be taken into account
   */
  void testString(const char* s1, const char* s2, 
                  bool verbose, bool strictCase) 
  {
    if ((s1 == NULL)||(s2 == NULL))
    {
      if (s1 == s2)
      {
        if (verbose)
        {
           std::cout << "NULL" << " (awaited : " 
             << "NULL" << ")" << std::endl;
        }
      }
      else
      {
        if (s1 == NULL)
        {
          CRL::CRL_ErrReport::INCREMENT_ERROR();
          std::cout << "NULL" << " (awaited : " << s2 
            <<  ") ***** ERROR *****"  << std::endl;
        }
        else
        {
          CRL::CRL_ErrReport::INCREMENT_ERROR();
          std::cout << s1 << " (awaited : " << "NULL" 
            <<  ") ***** ERROR *****" << std::endl;
        }
      }
    }
    else 
    {
      char *s1_test = new char[strlen(s1)+1];
      char *s2_test = new char[strlen(s2)+1];
      strcpy(s1_test, s1);
      strcpy(s2_test, s2);
      
      if (strictCase == false)
      {
       // toUpper(s1_test);
       // toUpper(s2_test);
      }
      
      if (strcmp(s1_test, s2_test) == 0)
      {
        if (verbose) 
          std::cout
            << s1 << " (awaited : " << s2 << ")" << std::endl;
      }
      else
      {
        CRL::CRL_ErrReport::INCREMENT_ERROR();
        std::cout << s1 << " (awaited : " << s2 
          << ") ***** ERROR *****" << std::endl;
      }
      
      delete[](s1_test);
      delete[](s2_test);
    }
  }
  
} // namespace CRL

