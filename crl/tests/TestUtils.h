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
   *  Cette classe permet de comptabiliser les nombres d'erreurs relevées
   *  durant l'exécution de tests logiciels. Les statistiques sont 
   *  collectées à trois niveaux :
   *  - globalement : pour une exécution d'un programme ('main')
   *  - au niveau de chaque 'module' 
   *  - au niveau 'd'éléments' composant ces modules
   *
   *  Les instances de CRL_ErrReport sont associées à un module de 
   *  bibliothèque (= ensemble d'éléments). Les données statiques de la 
   *  classe sont associées aux statistiques globales d'une exécution.
   */ 
  class CRL_ErrReport
  {
  private:
    
    /** \class Element 
     * \brief Un element est associé à une fonction de vérification,
     *  à un test particulier, voire à une classe.
     */
    class Element
    {
    public:
      
      //! Nom de l'élément de test, identifie l'élément.
      std::string name;
      
      //! Nombre d'erreurs comptabilisées pour l'élément.
      int nbErrors;
      
      //! Constructeur qui prend en entrée le nom de l'élément.
      Element(const std::string& s)
        : name(s), nbErrors(0) {}
    };
    
    //! Nom du module.
    std::string name;
    
    //! Nombre d'erreurs comptabilisées pour le module.
    int nbErrors;
    
    //! La liste des éléments du module.
    std::list<Element*> elements;
    
    //! Constructeur qui prend en entrée le nom du module.
    CRL_ErrReport(const std::string& s);
    
    //! Destructeur, détruit la liste des éléments.
    ~CRL_ErrReport();
    
    //! Retourne un élément à partir de son nom.
    Element* getElement(const std::string& s) const;
    
    //! Ajoute un élément dans la liste d'un module.
    Element* addElement(const std::string& s);
    
    //! Le module courant qui est en train d'être testé.
    static CRL_ErrReport*  currentModule;
    
    //! L'élément courant qui est en train d'être testé.
    static Element* currentElement;
    
    //! Le nombre global d'erreurs relevées pendant l'exécution.
    static int totalNbErrors;
    
    //! La liste des modules testés pendant l'exécution du programme.
    static std::list<CRL_ErrReport*> modules;
    
    //! Retourne un module à partir de son nom.
    static CRL_ErrReport* getModule(const std::string& s);
    
    //! Ajoute un module dans la liste des modules.
    static CRL_ErrReport* addModule(const std::string& s);
    
  public:
      
    //! Méthode qui change le module et l'élément courant.
    static void START(const char* mod, const char* ele);
     
    //! Réinitialise tout, efface toutes les statistiques.
    static void RESET();

    //! Incrémente les compteurs d'erreurs.
    static void INCREMENT_ERROR();
      
    //! Affiche les informations statistiques globales.
    static void PRINT();
     
    //! Affiche les informations statistiques détaillées.
    static void PRINT_ALL();

    //! Indique s'il y a des erreurs.
    static bool HAS_ERRORS();
  
    //! sortie finale avec un code d'erreur
    static void EXIT_IF_FAILURE();

  public:
     /** Classe interne pour appel automatique de RESET et éviter 
     *  les fuites mémoire. La classe est publique, mais l'utilisateur
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
  
  //! Retourne \c true si deux \c double sont égaux, à une précision près
  bool isEqual(double x, double y, double precision = DBL_MIN);
  
  //! Retourne \c true si deux \c double sont égaux, à une précision près
  bool isEqualRel(double x, double y, unsigned  int digits);
  

  // --------------------------------------------------------------------
  // Tests unitaires
  // --------------------------------------------------------------------
  
  // Les fonctions suivantes servent pour les tests unitaires. Si les deux
  // grandeurs a comparer sont egales et si 'verbose' est vrai, on affiche
  // un message du type : 'valeur1 (attendu : valeur2)'.
  // Si les grandeurs ne sont pas egales, quel que soit verbose, on affiche
  // un message d'erreur.
  
  //! Test d'égalité entre deux booléens \c bool 
  void testBoolean(bool b1, bool b2, bool verbose = true);
  
  //! Test d'égalité entre deux entiers \c long 
  void testInteger(long i1, long i2, bool verbose = true);
  
  //! Test d'égalité entre deux réels \c double, à une précision près
  void testDouble(double d1, double d2, 
                  double precision = DBL_MIN, bool verbose = true);
  
  //! Test d'égalité entre deux réels \c double, à une précision près
  void testDoubleRel(double d1, double d2, 
                     unsigned int digits, bool verbose = true);

  //! Test d'égalité entre deux chaînes de caractères
  void testString(const char* s1, const char* s2, 
                  bool verbose = true, bool strictCase = true);
  
} // namespace CRL

#endif // CRL_TEST_UTILS_H
