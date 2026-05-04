#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h> // Permet d’utiliser des variables de type booleen ("false" ou "true)

#define BASE 2

typedef struct {
    int *Tdigits; //Tableau de bits : Tdigits[0] = bit de poids fort, Tdigits[Taille - 1] = bit de poids faible
    int Taille; //Nombre de bits significatifs
    int Signe; //+1 pour positif, -1 pour negatif, 0 pour nul
} BigBinary;

//Initialisation manuelle d'un BigBinary vide (tout est a zero)
BigBinary initBigBinary(int taille, int signe) {
    BigBinary nb;
    nb.Taille = taille;
    nb.Signe = signe;
    nb.Tdigits = malloc(sizeof(int) * taille);
    for (int i = 0; i < taille ; ++i) {
        nb.Tdigits[i] = 0;
    }
    return nb;
}

//Affichage du nombre binaire
void afficheBigBinary(BigBinary nb) {
    if (nb.Signe == -1) printf("-");
    if (nb.Signe == 0 || nb.Taille == 0) {
        printf("0\n");
        return;
    }
    for (int i = 0; i < nb.Taille; i++) {
        printf("%d", nb.Tdigits[i]);
    }
    printf("\n");
}

//Liberation de la memoire
void libereBigBinary(BigBinary *nb) {
    free(nb->Tdigits);
    nb->Tdigits = NULL;
    nb->Taille = 0;
    nb->Signe = 0;
}

//Algorithme d'addition
BigBinary addition(BigBinary A, BigBinary B) {
    int TailleMax = 0; //Plus grand tableau entre A et B
    int retenue = 0; //Retenue pour l'addition
    int *bitsCalcul = NULL; //Tableau pour le resultat
    int bitA = 0; //variable locale pour les calculs bit par bit du tableau A
    int bitB = 0; //variable locale pour les calculs bit par bit du tableau B
    int somme = 0; //variable pour le calcul
    BigBinary Resultat; //Creation d'une structure pour le resultat a retourner
    
    //Recherche du tableau le plus grand entre A et B
    if (A.Taille > B.Taille) {
        TailleMax = A.Taille + 1;
    } else {
        TailleMax = B.Taille + 1;
    }

    Resultat.Taille = TailleMax;

    bitsCalcul = malloc(sizeof(int) * TailleMax); //Allocation  de memoire de size int x la taille du resultat


    for (int i = 0; i < TailleMax; i++) {
        int i_A = A.Taille - 1 - i; //remonte BigBinary A de droite a gauche en fonction de i
        int i_B = B.Taille - 1 - i; //remonte BigBinary B de droite a gauche en fonction de i

        //Calcul
        if (i_A >= 0) {
            bitA = A.Tdigits[i_A];
        } else {
            bitA = 0;
        }

        if (i_B >= 0) {
            bitB = B.Tdigits[i_B];
        } else {
            bitB = 0;
        }

        somme = bitA + bitB + retenue; //Calcul des bits de la colonne A et B

        bitsCalcul[TailleMax - 1 - i] = somme % 2; //bit plus faible enregistrer courant

        retenue = somme / 2; // nouvelle retenue
    }
    Resultat.Tdigits = bitsCalcul;

    return Resultat;
}

//Algorithme de soustraction avec A >= B)
BigBinary soustraction(BigBinary A, BigBinary B) {
    int TailleMax = 0; //Plus grand tableau entre A et B
    int emprunt = 0; //Emprunt de la soustraction dans le cas ou le bit de A est plus petit que celui de B
    int *bitsCalcul = NULL; //Tableau pour le resultat
    int bitA = 0; //variable locale pour les calculs bit par bit du tableau A
    int bitB = 0; //variable locale pour les calculs bit par bit du tableau B
    int diff = 0; //variable pour le calcul
    BigBinary Resultat; //Creation d'une structure pour le resultat a retourner

    //Recherche du tableau le plus grand entre A et B
    if (A.Taille > B.Taille) {
        TailleMax = A.Taille + 1;
    } else {
        TailleMax = B.Taille + 1;
    }

    Resultat.Taille = TailleMax;
    bitsCalcul = malloc(sizeof(int) * TailleMax); //Allocation dynamique de memoire de size int x la taille du resultat

    for (int i = 0; i < TailleMax; i++) {
        int i_A = A.Taille - 1 - i;
        int i_B = B.Taille - 1 - i;

        //Calcul
        if (i_A >= 0) {
            bitA = A.Tdigits[i_A];
        } else {
            bitA = 0;
        }

        if (i_B >= 0) {
            bitB = B.Tdigits[i_B];
        } else {
            bitB = 0;
        }

        diff = bitA - bitB - emprunt; //Calcul de la différence des deux bits en prenant en compte l'emprunt effectué

        if (diff >= 0) {
            bitsCalcul[TailleMax - 1 - i] = diff; //rien ne change
            emprunt = 0;
        } else {
            bitsCalcul[TailleMax - 1 - i] = diff + 2; //emprunt de 2 unités binaires
            emprunt = 1;
        }
    }
    Resultat.Tdigits = bitsCalcul;

    return Resultat;
}

//Algorithme de comparaison
//Egal :
bool Egal(BigBinary A, BigBinary B) {
    if (A.Taille != B.Taille || A.Signe != B.Signe) {
        return false;
    }

    for (int i = 0; i < A.Taille; i++) {
        if (A.Tdigits[i] != B.Tdigits[i]) {
            return false;
        }
    }

    return true;
}

//Inferieur
bool Inferieur(BigBinary A, BigBinary B) {
    // On compare le signe de A et B afin de savoir lequel si c'est inferieur
    if (A.Signe < B.Signe) {
        return true;
    }

    if (A.Signe > B.Signe) {
        return false;
    }


    // Si les deux sont positifs on compare leur taille afin de voir lequel est le plus grand
    if (A.Signe == 1 && B.Signe == 1) {
        if (A.Taille < B.Taille) {
            return true;
        }
        if (A.Taille > B.Taille) {
            return false;
        }

        // Si les tailles sont egales on compare bit par bit et on verifie lesquels sont plus petit pour en deduire l'inferiorite de A
        for (int i = 0; i < A.Taille; i++) {
            if (A.Tdigits[i] < B.Tdigits[i]) {
                return true;
            }
            if (A.Tdigits[i] > B.Tdigits[i]) {
                return false;
            }
        }

        return false; //Cas ou tous les tests ont echous, A n'est pas inferieur
    }

    // Si les deux sont negatifs
    if (A.Signe == -1 && B.Signe == -1) {
        //On compare d'abord les tailles, en inversant les retour car le plus grand des negatifs est le plus petit
        if (A.Taille < B.Taille) {
            return false;
        }
        if (A.Taille > B.Taille) {
            return true;
        }

        //On compare bit par bit en inversant les retours car le plus grand est le plus petit dans les negatifs
        for (int i = 0; i < A.Taille; i++) {
            if (A.Tdigits[i] < B.Tdigits[i]) {
                return false;
            }
            if (A.Tdigits[i] > B.Tdigits[i]) {
                return true;
            }
        }

        //Cas ou tous les tests ont echous, A n'est pas inferieur
        return false;
    }

    //Cas ou tous les tests ont echous, A n'est pas inferieur
    return false;
}

// Creation depuis une chaine binaire
BigBinary creerBigBinaryDepuisChaine(const char *chaine) {
    BigBinary nb;
    int n = strlen(chaine);
    nb.Taille = 0;

    // Comptons uniquement les caracteres valides ('0' ou '1')
    for (int i = 0; i < n; ++i) {
        if (chaine[i] == '0' || chaine[i] == '1') {
            nb.Taille++;
        }
    }

    nb.Tdigits = malloc(sizeof(int) * nb.Taille);
    nb.Signe = +1;
    int index = 0;
    int tousZeros = 1;

    for (int i = 0; i < n; ++i) {
        if (chaine[i] == '0' || chaine[i] == '1') {
            nb.Tdigits[index] = chaine[i] - '0';
            if (nb.Tdigits[index] == 1) {
                tousZeros = 0;
            }
            index++;
        }
    }

    if (tousZeros) {
        nb.Signe = 0;
    }
    return nb;
}

//Calcul du PGCD de deux "nombres binaires" par l’Algorithme Binaire d’Euclide
void BigBinary_divisePar2(BigBinary *nb) {
    for (int i = 0; i < nb->Taille - 1; i++) {
        nb->Tdigits[i] = nb->Tdigits[i + 1];
    }

    nb->Tdigits[nb->Taille - 1] = 0;

    while (nb->Taille > 1 && nb->Tdigits[nb->Taille - 1] == 0) {
        nb->Taille--;
    }

}

void BigBinary_multiplicationPar2(BigBinary *nb) {
    int *tab = malloc(sizeof(int) * (nb->Taille + 1));

    for (int i = 0; i < nb->Taille; i++) {
        tab[i] = nb->Tdigits[i];
    }

    tab[nb->Taille] = 0;

    free(nb->Tdigits);

    nb->Tdigits = tab;
    nb->Taille += 1;
}

BigBinary BigBinary_PGCD(BigBinary A, BigBinary B) {
    BigBinary zero = initBigBinary(1, 0);
    zero.Tdigits[0] = 0;
    
    BigBinary tempA = initBigBinary(A.Taille, A.Signe);
    for (int i = 0; i < A.Taille; i++) {
        tempA.Tdigits[i] = A.Tdigits[i];
    }
    BigBinary tempB = initBigBinary(B.Taille, B.Signe);
    for (int i = 0; i < B.Taille; i++) {
        tempB.Tdigits[i] = B.Tdigits[i];
    }

    int A_pair = 0;
    if (A.Tdigits[A.Taille - 1] == 0) {
        A_pair = 1;
    }

    int B_pair = 0;
    if (B.Tdigits[B.Taille - 1] == 0) {
        B_pair = 1;
    }

    if(Egal(B, zero)) {
        return A;
    }

    if(A_pair && B_pair) {
        BigBinary_divisePar2(&tempA);
        BigBinary_divisePar2(&tempB);
        BigBinary PGCD = BigBinary_PGCD(tempA, tempB);
        BigBinary_multiplicationPar2(&PGCD);
        return PGCD;
    } else if(A_pair && !B_pair) {
        BigBinary_divisePar2(&tempA);
    } else if(!A_pair && B_pair) {
        BigBinary_divisePar2(&tempB);
    } else if(!A_pair && !B_pair) {
        BigBinary tmpSub = soustraction(tempA, tempB);
        libereBigBinary(&tempA);
        tempA = tmpSub;
    }

    return BigBinary_PGCD(tempA, tempB);
}

//Calcul du modulo de A par N
BigBinary BigBinary_mod(BigBinary A, BigBinary B) {
    BigBinary tempA = initBigBinary(A.Taille, A.Signe);
    BigBinary tempB = initBigBinary(B.Taille, B.Signe);

    for (int i = 0; i < A.Taille; i++) {
        tempA.Tdigits[i] = A.Tdigits[i];
    }

    for (int i = 0; i < B.Taille; i++) {
        tempB.Tdigits[i] = B.Tdigits[i];
    }

    while(!Inferieur(tempA, B)) {
        for (int i = 0; i < B.Taille; i++) {
            tempB.Tdigits[i] = B.Tdigits[i];
        }
        int k = 0;

        while (!Inferieur(tempA, tempB)) {
            BigBinary_multiplicationPar2(&tempB);
            k++;
        }

        for (int i = 0; i < k - 1; i++) {
            BigBinary_divisePar2(&tempB);
        }

        tempA = soustraction(tempA, tempB);
    }
    libereBigBinary(&tempB);
    return tempA;
}


int main() {
    printf("Debut du programme : ");
    char ligne[2048];

    printf("Entrez une chaine binaire : ");
    scanf("%s", ligne);
    BigBinary A1 = creerBigBinaryDepuisChaine(ligne);

    printf("Entrez une chaine binaire : ");
    scanf("%s", ligne);
    BigBinary B1 = creerBigBinaryDepuisChaine(ligne);
    
    BigBinary S = addition(A1, B1);
    printf("Somme : ");
    afficheBigBinary(S);

    libereBigBinary(&A1);
    libereBigBinary(&B1);
    libereBigBinary(&S);
}
