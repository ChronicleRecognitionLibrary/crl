/*****************************************************************************//**
 * \file     TestUtils.h
 * \version  4.0
 * \author   Jean Bourrely / DPRS
 * \date     &copy; ONERA
 * \brief    Des utilitaires pour les tests unitaires
 *********************************************************************************/

#ifndef  CRL_TEST_UTILS_H
#define  CRL_TEST_UTILS_H

// --------------------------------------------------------------------
// FICHIERS INCLUS
// --------------------------------------------------------------------

#include <list>
#include <cfloat> 
#include <string>
#include <iostream>


// --------------------------------------------------------------------
// CLASSE CRL_ErrReport POUR LA GESTION DES TESTS UNITAIRES/VALIDATION
// --------------------------------------------------------------------

namespace CRL
{
  
  /** \class CRL_ErrReport
   *  \brief Statistiques d'erreurs dans les tests unitaires et validation.
   *
   *  Cette classe permet de comptabiliser les nombres d'erreurs relev�es
   *  durant l'ex�cution de tests logiciels. Les statistiques sont 
   *  collect�es � trois niveaux :
   *  - globalement : pour une ex�cution d'un programme ('main')
   *  - au niveau de chaque 'module' 
   *  - au niveau 'd'�l�ments' composant ces modules
   *
   *  Les instances de CRL_ErrReport sont associ�es � un module de 
   *  biblioth�que (= ensemble d'�l�ments). Les donn�es statiques de la 
   *  classe sont associ�es aux statistiques globales d'une ex�cution.
   */ 
  class CRL_ErrReport
  {
  private:
    
    /** \class Element 
     * \brief Un element est associ� � une fonction de v�rification,
     *  � un test particulier, voire � une classe.
     */
    class Element
    {
    public:
      
      //! Nom de l'�l�ment de test, identifie l'�l�ment.
      std::string name;
      
      //! Nombre d'erreurs comptabilis�es pour l'�l�ment.
      int nbErrors;
      
      //! Constructeur qui prend en entr�e le nom de l'�l�ment.
      Element(const std::string& s)
        : name(s), nbErrors(0) {}
    };
    
    //! Nom du module.
    std::string name;
    
    //! Nombre d'erreurs comptabilis�es pour le module.
    int nbErrors;
    
    //! La liste des �l�ments du module.
    std::list<Element*> elements;
    
    //! Constructeur qui prend en entr�e le nom du module.
    CRL_ErrReport(const std::string& s);
    
    //! Destructeur, d�truit la liste des �l�ments.
    ~CRL_ErrReport();
    
    //! Retourne un �l�ment � partir de son nom.
    Element* getElement(const std::string& s) const;
    
    //! Ajoute un �l�ment dans la liste d'un module.
    Element* addElement(const std::string& s);
    
    //! Le module courant qui est en train d'�tre test�.
    static CRL_ErrReport*  currentModule;
    
    //! L'�l�ment courant qui est en train d'�tre test�.
    static Element* currentElement;
    
    //! Le nombre global d'erreurs relev�es pendant l'ex�cution.
    static int totalNbErrors;
    
    //! La liste des modules test�s pendant l'ex�cution du programme.
    static std::list<CRL_ErrReport*> modules;
    
    //! Retourne un module � partir de son nom.
    static CRL_ErrReport* getModule(const std::string& s);
    
    //! Ajoute un module dans la liste des modules.
    static CRL_ErrReport* addModule(const std::string& s);
    
  public:
      
    //! M�thode qui change le module et l'�l�ment courant.
    static void START(const char* mod, const char* ele);
     
    //! R�initialise tout, efface toutes les statistiques.
    static void RESET();

    //! Incr�mente les compteurs d'erreurs.
    static void INCREMENT_ERROR();
      
    //! Affiche les informations statistiques globales.
    static void PRINT();
     
    //! Affiche les informations statistiques d�taill�es.
    static void PRINT_ALL();

    //! Indique s'il y a des erreurs.
    static bool HAS_ERRORS();
  
    //! sortie finale avec un code d'erreur
    static void EXIT_IF_FAILURE();

  public:
     /** Classe interne pour appel automatique de RESET et �viter 
     *  les fuites m�moire. La classe est publique, mais l'utilisateur
     *  n'a pas besoin de s'en servir.
     */
    class GlobalRESET {
    public:
      GlobalRESET() {}
      ~GlobalRESET() {CRL_ErrReport::RESET();}
    };
  
  }; // Classe CRL_ErrReport
  
  
  // --------------------------------------------------------------------
  // Test d'egalite entre deux reels
  // --------------------------------------------------------------------
  
  //! Retourne \c true si deux \c double sont �gaux, � une pr�cision pr�s
  bool isEqual(double x, double y, double precision = DBL_MIN);
  
  //! Retourne \c true si deux \c double sont �gaux, � une pr�cision pr�s
  bool isEqualRel(double x, double y, unsigned  int digits);
  

  // --------------------------------------------------------------------
  // Tests unitaires
  // --------------------------------------------------------------------
  
  // Les fonctions suivantes servent pour les tests unitaires. Si les deux
  // grandeurs a comparer sont egales et si 'verbose' est vrai, on affiche
  // un message du type : 'valeur1 (attendu : valeur2)'.
  // Si les grandeurs ne sont pas egales, quel que soit verbose, on affiche
  // un message d'erreur.
  
  //! Test d'�galit� entre deux bool�ens \c bool 
  void testBoolean(bool b1, bool b2, bool verbose = true);
  
  //! Test d'�galit� entre deux entiers \c long 
  void testInteger(long i1, long i2, bool verbose = true);
  
  //! Test d'�galit� entre deux r�els \c double, � une pr�cision pr�s
  void testDouble(double d1, double d2, 
                  double precision = DBL_MIN, bool verbose = true);
  
  //! Test d'�galit� entre deux r�els \c double, � une pr�cision pr�s
  void testDoubleRel(double d1, double d2, 
                     unsigned int digits, bool verbose = true);

  //! Test d'�galit� entre deux cha�nes de caract�res
  void testString(const char* s1, const char* s2, 
                  bool verbose = true, bool strictCase = true);
  
} // namespace CRL

#endif // CRL_TEST_UTILS_H
