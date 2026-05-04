# BigBinary - Arbitrary Precision Binary Arithmetic in C

## Description

Ce projet implémente des opérations arithmétiques sur des entiers binaires de taille arbitraire en langage C.

L’objectif est de simuler des entiers de grande taille (BigInteger) en utilisant une structure personnalisée, sans utiliser de bibliothèques externes.

---

## Fonctionnalités

Le projet permet de manipuler des nombres binaires avec les opérations suivantes :

- Addition binaire
- Soustraction binaire
- Comparaison :
  - égalité
  - infériorité
- Calcul du PGCD (algorithme binaire d’Euclide / Stein)
- Calcul du modulo (division par soustractions successives et décalages)
- Multiplication / division par 2 (décalage de bits)

---

## Structure du projet

Le projet repose sur une structure principale :

```c
typedef struct {
    int *Tdigits;   // tableau de bits (MSB -> LSB)
    int Taille;     // taille du nombre binaire
    int Signe;      // +1 positif, -1 négatif, 0 nul
} BigBinary;
