/** ***********************************************************************************
 * \file Utils.h
 * \author Ariane Piel & Jean Bourrely / Onera DCPS
 * \date 2012
 * \brief Utilities for CRL
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

#ifndef CRL_UTILS_H
#define CRL_UTILS_H

// ----------------------------------------------------------------------------
// INCLUDE FILES
// ----------------------------------------------------------------------------

#include <set>


// ----------------------------------------------------------------------------
// CLASSES AND FUNCTIONS
// ----------------------------------------------------------------------------

namespace CRL {

  class Set 
  {
  public:

    template <typename T>
    static void set_union(const std::set<T>& s1, const std::set<T>& s2, 
                          std::set<T>& result)
    {
      result.clear();
      typename std::set<T>::const_iterator it;

      for (it = s1.begin(); it != s1.end(); it++)
        result.insert(*it);

      for (it = s2.begin(); it != s2.end(); it++)
        result.insert(*it);
    }


    template <typename T>
    static void set_union(const std::set<T>& s1, const std::set<T>& s2, 
                          std::set<T>& result, const T& exceptionValue)
    {
      result.clear();
      typename std::set<T>::const_iterator it;

      for (it = s1.begin(); it != s1.end(); it++)
        if (*it != exceptionValue)
          result.insert(*it);

      for (it = s2.begin(); it != s2.end(); it++)
        if (*it != exceptionValue)
          result.insert(*it);
    }


    template <class T>
    static void set_intersection(const std::set<T>& s1, const std::set<T>& s2, 
                                 std::set<T>& result)
    {
      result.clear();
      typename std::set<T>::const_iterator it1 = s1.begin();
      typename std::set<T>::const_iterator it2 = s2.begin();

      while ( (it1 != s1.end()) && (it2 != s2.end()) )
      {
        if ( (*it1) < (*it2) ) it1++;
        else if ( (*it2) < (*it1) ) it2++;
        else { result.insert(*it1); it1++; it2++; }
      }
    }


    template <class T>
    static void set_intersection(const std::set<T>& s1, const std::set<T>& s2, 
                                 std::set<T>& result, const T& exceptionValue)
    {
      result.clear();
      typename std::set<T>::const_iterator it1 = s1.begin();
      typename std::set<T>::const_iterator it2 = s2.begin();

      while ( (it1 != s1.end()) && (it2 != s2.end()) )
      {
        if ( (*it1) < (*it2) ) it1++;
        else if ( (*it2) < (*it1) ) it2++;
        else { 
          if (*it1 != exceptionValue)
            result.insert(*it1); 
          it1++; 
          it2++; 
        }
      }
    }


    template <class T>
    static void set_difference(const std::set<T>& s1, const std::set<T>& s2, 
                               std::set<T>& result)
    {
      result.clear();
      typename std::set<T>::const_iterator it1 = s1.begin();
      typename std::set<T>::const_iterator it2 = s2.begin();

      while ( (it1 != s1.end()) && (it2 != s2.end()) )
      {
        if ( (*it1) < (*it2) ) { result.insert(*it1); it1++; }
        else if ( (*it2) < (*it1) ) it2++;
        else { it1++; it2++; }
      }
      while (it1 != s1.end()) {
        result.insert(*it1); it1++; }
    }


    template <class T>
    static bool areDisjoint(const std::set<T>& s1, const std::set<T>& s2)
    {
      typename std::set<T>::const_iterator it1 = s1.begin();
      typename std::set<T>::const_iterator it2 = s2.begin();

      while ( (it1 != s1.end()) && (it2 != s2.end()) )
      {
        if ( (*it1) < (*it2) ) it1++;
        else if ( (*it2) < (*it1) ) it2++;
        else return false;
      }
      return true;
    }


    template <class T>
    static bool areDisjoint(const std::set<T>& s1, const std::set<T>& s2,
                            const T& exceptionValue)
    {
      typename std::set<T>::const_iterator it1 = s1.begin();
      typename std::set<T>::const_iterator it2 = s2.begin();

      while ( (it1 != s1.end()) && (it2 != s2.end()) )
      {
        if (*it1 == exceptionValue) {
          it1++;
          continue;
        }
        if (*it2 == exceptionValue) {
          it2++;
          continue;
        }
        if ( (*it1) < (*it2) ) it1++;
        else if ( (*it2) < (*it1) ) it2++;
        else return false;
      }
      return true;
    }


    template <class T>
    static bool equal(const std::set<T>& s1, const std::set<T>& s2)
    {
      typename std::set<T>::const_iterator it1 = s1.begin();
      typename std::set<T>::const_iterator it2 = s2.begin();

      while ( (it1 != s1.end()) && (it2 != s2.end()) )
      {
        if (!(*it1 == *it2)) return false;
        it1++;
        it2++;
      }
      return ( (it1 == s1.end()) && (it2 == s2.end()) );
    }


    template <class T>
    static bool equal(const std::set<T>& s1, const std::set<T>& s2,
                      const T& exceptionValue)
    {
      typename std::set<T>::const_iterator it1 = s1.begin();
      typename std::set<T>::const_iterator it2 = s2.begin();

      if ( (it1 != s1.end()) && (*it1 == exceptionValue) ) it1++;
      if ( (it2 != s2.end()) && (*it2 == exceptionValue) ) it2++;

      while ( (it1 != s1.end()) && (it2 != s2.end()) )
      {
        if ( (it1 != s1.end()) && (*it1 == exceptionValue) ) {
          it1++;
          continue;
        }
        if ( (it2 != s2.end()) && (*it2 == exceptionValue) ) {
          it2++;
          continue;
        }
        if (!(*it1 == *it2)) return false;
        it1++;
        it2++;
      }
      return ( (it1 == s1.end()) && (it2 == s2.end()) );
    }

  }; // class Set

} /* namespace CRL */

#endif /* CRL_UTILS_H */


/*
void test_set_union()
{
  {
    int tab1[]= { 10, 20, 30, 40, 50 };
    std::set<int> set1 (tab1, tab1+5); 
    std::set<int> set2;
    std::set<int> set3;
    set_union(set1, set2, set3);
    for (std::set<int>::iterator it=set3.begin(); it!=set3.end(); ++it)
      std::cout << ' ' << *it;
    std::cout << '\n';
  }
  {
    int tab1[]= { 10, 20, 30, 40, 50 };
    std::set<int> set1 (tab1, tab1+5); 
    std::set<int> set2;
    std::set<int> set3;
    set_union(set2, set1, set3);
    for (std::set<int>::iterator it=set3.begin(); it!=set3.end(); ++it)
      std::cout << ' ' << *it;
    std::cout << '\n';
  }
  {
    int tab1[]= { 10, 20, 30, 40, 50 };
    int tab2[]= { 10, 20, 30, 40, 50 };
    std::set<int> set1 (tab1, tab1+5); 
    std::set<int> set2 (tab2, tab2+5);
    std::set<int> set3;
    set_union(set1, set2, set3);
    for (std::set<int>::iterator it=set3.begin(); it!=set3.end(); ++it)
      std::cout << ' ' << *it;
    std::cout << '\n';
  }
  {
    int tab1[]= { 10, 20, 30 };
    int tab2[]= { 40, 50 };
    std::set<int> set1 (tab1, tab1+3); 
    std::set<int> set2 (tab2, tab2+2);
    std::set<int> set3;
    set_union(set1, set2, set3);
    for (std::set<int>::iterator it=set3.begin(); it!=set3.end(); ++it)
      std::cout << ' ' << *it;
    std::cout << '\n';
  }
  {
    int tab1[]= { 50, 10, 20, 30 };
    int tab2[]= { 40, 30, 50 };
    std::set<int> set1 (tab1, tab1+4); 
    std::set<int> set2 (tab2, tab2+3);
    std::set<int> set3;
    set_union(set1, set2, set3);
    for (std::set<int>::iterator it=set3.begin(); it!=set3.end(); ++it)
      std::cout << ' ' << *it;
    std::cout << '\n';
  }
}

void test_set_intersection()
{
  {
    int tab1[]= { 10, 20, 30, 40, 50 };
    std::set<int> set1 (tab1, tab1+5); 
    std::set<int> set2;
    std::set<int> set3;
    set_intersection(set1, set2, set3);
    if (areDisjoint(set1, set2)) std::cout << "Disjoints";
    else
      std::cout << "Non disjoints : ";
    for (std::set<int>::iterator it=set3.begin(); it!=set3.end(); ++it)
      std::cout << ' ' << *it;
    std::cout << '\n';
  }
  {
    int tab1[]= { 10, 20, 30, 40, 50 };
    std::set<int> set1 (tab1, tab1+5); 
    std::set<int> set2;
    std::set<int> set3;
    set_intersection(set2, set1, set3);
    if (areDisjoint(set1, set2)) std::cout << "Disjoints";
    else
      std::cout << "Non disjoints : ";
    for (std::set<int>::iterator it=set3.begin(); it!=set3.end(); ++it)
      std::cout << ' ' << *it;
    std::cout << '\n';
  }
  {
    int tab1[]= { 10, 20, 30, 40, 50 };
    int tab2[]= { 10, 20, 30, 40, 50 };
    std::set<int> set1 (tab1, tab1+5); 
    std::set<int> set2 (tab2, tab2+5);
    std::set<int> set3;
    set_intersection(set1, set2, set3);
    if (areDisjoint(set1, set2)) std::cout << "Disjoints";
    else
      std::cout << "Non disjoints : ";
    for (std::set<int>::iterator it=set3.begin(); it!=set3.end(); ++it)
      std::cout << ' ' << *it;
    std::cout << '\n';
  }
  {
    int tab1[]= { 10, 20, 30 };
    int tab2[]= { 40, 50 };
    std::set<int> set1 (tab1, tab1+3); 
    std::set<int> set2 (tab2, tab2+2);
    std::set<int> set3;
    set_intersection(set1, set2, set3);
    if (areDisjoint(set1, set2)) std::cout << "Disjoints";
    else
      std::cout << "Non disjoints : ";
    for (std::set<int>::iterator it=set3.begin(); it!=set3.end(); ++it)
      std::cout << ' ' << *it;
    std::cout << '\n';
  }
  {
    int tab1[]= { 50, 10, 20, 30 };
    int tab2[]= { 40, 30, 50 };
    std::set<int> set1 (tab1, tab1+4); 
    std::set<int> set2 (tab2, tab2+3);
    std::set<int> set3;
    set_intersection(set1, set2, set3);
    if (areDisjoint(set1, set2)) std::cout << "Disjoints";
    else
      std::cout << "Non disjoints : ";
    for (std::set<int>::iterator it=set3.begin(); it!=set3.end(); ++it)
      std::cout << ' ' << *it;
    std::cout << '\n';
  }
}

int main()
{
  test_set_union();
  test_set_intersection();

  return 0;
}

*/

