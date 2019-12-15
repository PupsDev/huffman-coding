/**
 * @file huf.c
 * @brief Huffman Compressor/Decompressor
 * @author Jean-Charles ALLA - Mathieu LADEUIL
 *
 * @date 14 novembre 2019
 *
 * Compresseur Huffman
 *
 */

#include <stdio.h>
#include <stdlib.h>
/*
* Set de 2 macros pour afficher le code binaire
* d'un  code ascii durant les tests.
*/
#define MAX_TREE_HT 20
#define ERROR_10 "Impossible d'allouer la mémoire\n"
#define CAPACITY_MAX 256


  /*
**********************************************************************
*
* STRUCTURES
*
**********************************************************************/
  /**
 * @struct Dist
 * @brief Objet distribution des caractères
 *
 * Dist est une structure de données qui va stocker les caractères du fichier lu en entrée
 */

typedef struct{
    char *tab; /* @param tab : Tableau contenant l'ensemble des caractères présent dans le fichier*/
    int size; // Taille du tableau
    int *freq; // Tableau contenant la fréquence (ou le nombre d'occurences) de chaque caractères
}Dist;
/**
 * @struct Noeud
 * @brief Objet qui décrit un noeud du Tas
 * @param data : variable qui contient 0 ou 1
 * @param freq : variable qui contient la frequence d'un caractère
 * @param fg : Similaire à fd, pointeur vers les noeuds enfants gauche et droit
 *
 * Noeud est une structure de donnée qui contient les informations d'un noeud
 */


struct Noeud
{
    char data;
    unsigned int freq;
    struct Noeud *fg,*fd;
};
/**
 * @struct Tas
 * @brief Objet qui décrit le Tas
 * @param size : Taille courante du Tas
 * @param capacity : Taille maximal du Tas
 * @param tab : Tableau de pointeurs de Noeuds
 *
 * Tas est une structure de donnée qui contient les informations
 * du tas et le tableau de pointeurs de Noeuds.
 */
struct Tas
{
    unsigned int taille;
    unsigned int capacity;
    struct Noeud **tab;
};
/*
**********************************************************************
*
* FONCTIONS
*
**********************************************************************/

/**
 * \fn void freeHuffman(struct Tas *tas)
 * \brief fonction de libère de tas.
 *
 * \param tas : Adresse de l'objet struct Tas à détruire
 */
void freeHuffman(struct Tas *tas)
{
    free(tas->tab);
    free(tas);
}

/**
 * \fn void freeDist(Dist *dist)
 * \brief fonction de libération de dist.
 *
 * \param dist : Adresse de l'objet Dist à détruire
 */
void freeDist(Dist *dist)
{
    free(dist->tab);
    free(dist->freq);
    free(dist);
}

/**
 * \fn int inArray(char *tab,  int size_tab, char c)
 * \brief Vérifie si un char est dans le tableau de char, retourne la position
 *
 * \param tab : Tableau de caractère
 * \param size_tab : Taille du tableau
 * \param c : Caractère 
 * \return -1 si c est dans tab sinon une valeur positive
 */
int inArray(char *tab,  int size_tab, char c)
{
  for( int i=0; i<size_tab;i++)
  {
      if(tab[i]==c)
      {
          return i;
      }
  }
  return -1;
}

/**
 * \fn void handleAlloc(void *pt)
 * \brief Fonction pour gérer les échecs d'allocations d'un pointeur
 *
 * \param pt adresse de la variable utilisée pour l'allocation
 */
void handleAlloc(void *pt)
{
    fprintf(stderr,ERROR_10);
    free(pt);
}

/**
 * \fn Dist* loadFile(char *filename)
 * \brief Fonction qui ouvre le ficher, et renvoie la structure de distribution
 *
 * \param filename : nom du fichier
 * \return un tableau de Dist établi par rapport au fichier
 */
Dist* loadFile(char *filename) 
{
    FILE *file;
    Dist *dist = malloc(sizeof(Dist));
    dist->size=0;

    char c;
    int i=0;
    int position;

    if(!dist)
    {
        handleAlloc(dist);
    }
    dist->tab = malloc(CAPACITY_MAX*sizeof(char));
    if(!dist->tab)
    {
        handleAlloc(dist->tab);
    }
    dist->freq = malloc(CAPACITY_MAX*sizeof(int));
    if(!dist->freq)
    {
        handleAlloc(dist->freq);
    }


    if( (file = fopen(filename, "rb")) == NULL)
    {
        fprintf(stderr,"Impossible d'ouvrir le fichier en lecture\n");
        exit(1);
    }
    else{
        while((c=fgetc(file))!=EOF)
        {

            position = inArray(dist->tab,dist->size+1,c);

            if(position== -1) // Si le caractère n'a pas encore été lu
            {
                dist->tab[i]=c;
                dist->size = dist->size+1;
                dist->freq[i]=1;
                i=i+1;

            }
            else{

                dist->freq[position]=dist->freq[position]+1; //sinon on incrémente la position
            }
        }


    }
    char eof = -96;   // On ajoute le caractère ■, pour signaler la fin du fichier
    dist->tab[i]=eof;
    dist->size++;
    dist->freq[i]=1;

    fclose(file);
    return dist;

}


void afficheTab(int tab[], int n) //fonction qui affiche un tableau d'entier
{
    int i;
    for (i = 0; i < n; ++i)
        printf("%d", tab[i]);

    printf("\n");
}

/**
 * \fn struct Noeud* nouveauNoeud(char data, unsigned freq)
 * \brief Fonction de création d'un nouveau noeud avec le caractère donné
 * et sa fréquence.
 *
 * \param tmp : Le noeud temporaire qu'on va renvoyer
 * \return tmp Un pointeur vers une structure Noeud
 *
*/
struct Noeud* nouveauNoeud(char data, unsigned freq)
{
    struct Noeud* tmp = (struct Noeud*)malloc(sizeof(struct Noeud));
    if(!tmp)
    {
        handleAlloc(tmp);
    }

    tmp->fg = tmp->fd = NULL;

    tmp->freq = freq;
    tmp->data = data;
    return tmp;
}

/**
 * @file
 * @fn struct Tas* nouveauTas(unsigned capacity)
 * @brief Fonction de création et d'allocation d'un nouveau tas avec une
 * capacité donnée.
 *
 *
 * @return un pointeur sur le nouveau tas
 *
*/
struct Tas* nouveauTas(unsigned capacity)
{

    struct Tas* tas
        = (struct Tas*)malloc(sizeof(struct Tas));
    if(!tas)
    {
        handleAlloc(tas);
    }

    // taille courante est de 0
    tas->taille = 0;

    tas->capacity = capacity; //taille maximal du tas

    tas->tab
        = (struct Noeud**)malloc(tas->
        capacity * sizeof(struct Noeud*));
    if(!tas->tab)
    {
        handleAlloc(tas->tab);
    }
    return tas;
}

/**
 * @file
 * @fn void echangeNoeud(struct Noeud** a, struct Noeud** b)
 * @brief Fonction d'échange de deux Noeuds, utilisant un pointeur
 * sur un pointeur et pas des copies.
 *
 *
*/
void echangeNoeud(struct Noeud** a, struct Noeud** b)
{

    struct Noeud* tmp = *a;
    *a = *b;
    *b = tmp;
}
/**
 * @file
 * @fn void Tasser(struct Tas* tas, int ind)
 * @brief Fonction qui va réarranger le tas afin qu'il respecte
 * à nouveau ses propriétés.
 *
 *
*/
void Tasser(struct Tas* tas, int ind)
{
    int gauche = 2 * ind + 1;  // Propriétés du tas
    int droit = 2 * ind + 2;
    int plusPetit = ind;

    if (gauche < tas->taille
        && tas->tab[gauche]-> freq < tas->tab[plusPetit]->freq)
    {
        plusPetit = gauche;
    }
    if (droit < tas->taille &&
        tas->tab[droit]-> freq < tas->tab[plusPetit]->freq)
    {
        plusPetit = droit;
    }
    if (plusPetit != ind)
    {
        echangeNoeud(&tas->tab[plusPetit], &tas->tab[ind]); // on échange les noeuds pour mettre le plus petit
        Tasser(tas, plusPetit);                             // à la bonne place
    }
}

/**
 * @file
 * @fn struct Noeud* extraireMin(struct Tas* tas)
 * @brief Fonction qui va extraire le minimun. On réduit la taille du tableau
 * et le plus grand élément est placé au début du tableau, donc on doit tasser à nouveau.
 * @return Le noeud le plus petit du tas.
 *
*/
struct Noeud* extraireMin(struct Tas* tas)
{

    struct Noeud* tmp = tas->tab[0];
    tas->tab[0] = tas->tab[tas->taille - 1];
    tas->taille=tas->taille-1;
    Tasser(tas, 0);

    return tmp;
}

/**
 * @file
 * @fn void insereNoeud(struct Tas* tas, struct Noeud* noeud)
 * @brief Fonction qui va placer le noeud dans le tas à la bonne place,en partant
 * de (taille T-1)/2 jusqu'à 0;
 *
 *
*/
void insereNoeud(struct Tas* tas, struct Noeud* noeud)
{
    tas->taille = tas->taille+1 ;
    int i = tas->taille - 1;

    while (i>0 && noeud->freq < tas->tab[(i - 1) / 2]->freq) {

        tas->tab[i] = tas->tab[(i - 1) / 2];
        i = (i - 1) / 2;
    }

    tas->tab[i] = noeud;
}

/**
 * @file
 * @fn void creerTas(struct Tas* tas)
 * @brief Fonction qui va transformer un "simple tableau" en un vrai tas.
 *
 *
*/
void creerTas(struct Tas* tas)
{
    int n = tas->taille - 1;
    int i;
    for (i = (n - 1) / 2; i >= 0; i--)
    {
        Tasser(tas, i);
    }

}

/**
 * @file
 * @fn struct Tas* ConstruitTas(char data[], int freq[], int taille)
 * @brief Fonction qui va remplir le tableau de noeud
 * suivant les 2 tableaux donnés en paramètre.
 *
 *
*/
struct Tas* ConstruitTas(char data[], int freq[], int taille)
{
    struct Tas* tas = nouveauTas(taille);

    for (int i = 0; i < taille; ++i)
        tas->tab[i] = nouveauNoeud(data[i], freq[i]);

    tas->taille = taille;
    creerTas(tas);

    return tas;
}

/**
 * @fn void afficheCode(struct Noeud* root, int tab[],int top)
 * @brief Affiche le code de chaque caractère
*/
void afficheCode(struct Noeud* root, int tab[],int top)
{
    if (root->fg) {  // arbitrairement le fils gauche sera codé 0
        tab[top] = 0;
        afficheCode(root->fg, tab, top + 1);
    }

    if (root->fd) { // arbitrairement le fils droit sera codé 1
        tab[top] = 1;
        afficheCode(root->fd, tab, top + 1);
    }


    if (!(root->fg) && !(root->fd)) { // si on arrive à une feuille on affiche le code
        printf("codeChar(%c)=", root->data);
        afficheTab(tab, top);
    }
}

/**
 * @fn void calculLongeur(struct Noeud* root, int tab[],int top, int *moyencodage)
 * @brief Calcule la longueur moyenne du codage
 * @param root : Pointeur sur un noeud
 * @param tab : Tableau d'entier
 * @param top : Taille de tab
 * @param moyencodage : Adresse de la variable du codage moyen
*/
void calculLongeur(struct Noeud* root, int tab[],int top, int *moyencodage)
{
    if (root->fg) {
        tab[top] = 0;
        calculLongeur(root->fg, tab, top + 1, moyencodage);
    }
    if (root->fd) {
        tab[top] = 1;
        calculLongeur(root->fd, tab, top + 1,moyencodage);
    }
    if (!(root->fg) && !(root->fd)) {
		if(root->data != -96) // si la feuille est différente du EOF
			(*moyencodage) += top * root->freq; // on ajoute la longeur moyenne à diviser par le total freq

    }
}

/**
 * @fn void trouveCarac(struct Noeud* root, unsigned char tab[],int top, char c, unsigned char tab2[], int *s)
 * @brief Recherche un caractère à travers des noeuds
 * @param root : Pointeur sur un noeud
 * @param tab : Tableau de charactères
 * @param top : Taille de tab
 * @param c : Caractère recherché
 * @param tab2 : L'adresse du code correspondant au caractère recherché
 * @param s : Pointeur sur la longueur du code associé au caractère recherché
*/
void trouveCarac(struct Noeud* root, unsigned char tab[],int top, char c, unsigned char tab2[], int *s)
{
    if (root->fg) {
        tab[top] = '0';
        trouveCarac(root->fg, tab, top + 1, c, tab2, s);
    }

    if (root->fd) {
        tab[top] = '1';
       trouveCarac(root->fd, tab, top + 1, c, tab2, s);
    }


    if (!(root->fg) && !(root->fd)) { //Si c'est une feuille

        if(c == root->data) //et que la feuille est le caractère qu'on veut placer
        {
            for(int i=0; i<top;i++)
            {
                tab2[i]=tab[i]; //on modifie tab2 qui est la sortie du code

            }
            *s=top; //taille du tab2
        }

    }
}

int write_bit(unsigned char * byte, int *pos, FILE* fp, unsigned char e)
{
    int written;
    if(*pos==8) //si les 8bits sont remplis
    {
        written = fputc(*byte, fp); //on écrit l'octet dans le fichier
        if(written==EOF)
            return -1;
        *byte=0; //on initialise ensuite la pos et le buffer
        *pos=0;
      }

    if( e =='1') // si c'est un 1
    {
        *byte |= 0x1 <<(7-(*pos)); //on réalise un ou binaire qui va assigner le décalage de 7 - position bits
                                    // pour le buffer
    }
    if(e=='0'); //si c'est 0 on ne change rien, on part de 0000 0000 pour travailler donc les 0 ne changent pas
    (*pos)++;
    if(*pos >8) // cas où on dépasserait, cela n'arrive jamais en théorie
    {
        printf("\n DEPASSEMENT \n");
        *pos=0;
    }
}

void codageArbre(struct Noeud* root, FILE *fp, unsigned char *byte, int *pos)
{
    if((root->fg) || (root->fd)) // Si le noeud n'est pas une feuille on code 0 pour l'entête
    {
        write_bit(byte,pos, fp, '0');

        codageArbre(root->fg, fp,byte,pos);
        codageArbre(root->fd, fp,byte,pos);
    }
    else // Si on arrive sur une feuille
    {
        int bit;
        int base = 0x80;
        char c;
        bit = root->data; //  char to int conversion
        /*
        *  Compare chaque bit avec
        *  0x80,0x40,...,0x01
        *  et écrit le code binaire dans le
        *  fichier
        */
        write_bit(byte,pos, fp, '1'); // On va coder un 1 puis le caractère en binaire
        for(int i =0 ; i<8;i++)
        {
             c = bit & base ? '1' : '0';
             write_bit(byte,pos, fp, c);
            base=base>>1;

        }
    }

}

void compresseurHuffman(struct Noeud* root, FILE *input, FILE *fp)
{
    char data;
    unsigned char tab[MAX_TREE_HT], tab2[MAX_TREE_HT], x=0;
    int start = 0,taille2=0, pos=0, reste_eof=0;
    long int tinput=0, toutput=0;
    float compression=1.0;

    codageArbre(root, fp, &x, &pos); // On code l'entête donc codage de l'arbre et du dictionnaire
                                     // sous la forme 00..01[8bits] 01[8bits] 1 [8bits]

    while( (data = getc(input)) !=EOF ) // On lit le ficher en entrée
    {

       trouveCarac(root,tab,start, data , tab2, &taille2); // On cherche chaque caractère du fichier dans l'arbre
                                                            // et on renvoie le tableau qui contient le code


        for(int i=0; i< taille2; i++)
        {
            write_bit(&x,&pos, fp, tab2[i]); // on écrit le code dans le buffer
        }

    }

	if( input)  //On regarde la taille du ficher en entrée
	{
	fseek(input, 0, SEEK_END);
    tinput= ftell(input);
	}

    fclose(input); // On peut le fermer on n'en a plus besoin

    char eof = -96;
    trouveCarac(root,tab,start, eof , tab2, &taille2); // CODAGE DU EOF

    for(int i=0; i< taille2; i++)
    {
            write_bit(&x,&pos, fp, tab2[i]);


    }

    reste_eof=pos; //Si le EOF n'est pas codé entièrement sur 8bits, si il est plus grand ou s'il faut rajouter
                    // des 0 pour finir l'octet à coder


    if(reste_eof<8) // On ajoute  (8- la position) +1 '0' car position est automatiquement incrémenté par write_bit
    {
        for(int i=0; i<(8-reste_eof)+1;i++)
            write_bit(&x,&pos, fp, '0');
    }
	else // Si on est à 8bits ou si EOF >8bits on ajoute ce qui reste
	{
		for(int i=0; i< (taille2-reste_eof); i++)
        {
            write_bit(&x,&pos, fp, tab2[i]);
        }
	}
	if(fp)
	{
		toutput= ftell (fp); //on récupère la taille du fichier sortie

	}


    compression =  ((float)toutput/(float)tinput);

    printf("Taille originelle : %ld octet ; taille compressée %ld octet ;compression  %.3f %%  \n",tinput, toutput, 100*(1-compression));
    fclose(fp);

}

void HuffmanCodes(char data[], int freq[], int taille, FILE*finput, FILE *foutput)
  {
    int tab[MAX_TREE_HT], start = 0,  moyen=0, sommefreq=0;
    struct Noeud *fg, *fd, *top;

	for(int i=0 ; i<taille ;i++)
	{
			sommefreq+= freq[i]; // on récupère la somme des frèquences

	}


    struct Tas* tas = ConstruitTas(data, freq, taille);
    while (tas->taille != 1) { //tant qu'on arrive pas à la racine


        fg = extraireMin(tas); //on extrait les 2 noeuds min
        fd = extraireMin(tas);
        top = nouveauNoeud('X', fg->freq + fd->freq); //on crée un noeud parent qui n'a pas de data utile
        top->fg = fg;
        top->fd = fd;
        insereNoeud(tas, top); //on insère le noeud dans le tas
    }

    struct Noeud* root = extraireMin(tas); //il reste un seul noeud on l'extrait c'est la racine
    freeHuffman(tas); //on peut libèrer le tas on continue à travailler sur l'arbre simplement

	calculLongeur(root, tab, start, &moyen); // calcul de la longueur moyenne dans moyen

    afficheCode(root, tab, start); // affichage de chaque code
	printf("Longueur moyenne : %.2f\n", (float)moyen/(float)sommefreq);  // affichage de la longueur moyenne
   compresseurHuffman(root,finput, foutput); // on compresse
   free(root); // on peut libèrer l'arbre

}

/**
 * \fn int main (void)
 * \brief Entrée du programme.
 *
 * \return EXIT_SUCCESS - Arrêt normal du programme.
 */
int main(int argc, char *argv[] )
{

    if(argc<3){
		fprintf(stderr, " necessite : - un fichier à compresser - un fichier sortie compressé \n");
		exit(1);
	}
	FILE* finput = fopen(argv[1], "rb");
	if(!finput){
		fprintf(stderr, "Impossible d'ouvrir le fichier à compresser : %s\n", argv[1]);
		exit(1);
	}

	FILE* foutput = fopen(argv[2], "wb");
	if(!foutput){
		fprintf(stderr, "Impossible d'ouvrir le fichier compresse : %s\n", argv[2]);
		exit(1);
	}


    Dist *dist;
    dist = loadFile(argv[1]);

    for(size_t i=0; i<dist->size;i++)
    {
        printf(" Char : %c Freq : %d \n", dist->tab[i], dist->freq[i]);

    }
    printf("Total Occurences : %d + EOF\n", dist->size-1);

    HuffmanCodes(dist->tab,dist->freq,dist->size, finput, foutput);
    freeDist(dist);
	fclose(finput);
	fclose(foutput);

    return EXIT_SUCCESS;
}
