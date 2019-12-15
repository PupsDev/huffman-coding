#include <stdio.h>
#include <stdlib.h>
#define ERROR_10 "Impossible d'allouer la mémoire\n"
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
void handleAlloc(void *pt)
{
    fprintf(stderr,ERROR_10);
    free(pt);
}
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
int read_byte(FILE *fp, char buffer[])
{
    int byte;
    int bit =0x80;
    byte = fgetc(fp);

    for(int i=0; i<8;i++)
    {
        buffer[i]= byte & bit ? '1' : '0';
        bit=bit>>1;
    }

    return byte;
}
char read( FILE *fp, int *pos, char buffer[])
{
    char bit;
   if((*pos)==8)
   {
       //printf("depasse");
       (*pos)=0;
       read_byte(fp, buffer);
   }

   bit = buffer[*pos];


   (*pos)++;
   return bit;

}
char bitToChar(char buffer[])
{
    char c=0;
    int bit = 0x80;
    for(int i=0 ; i<8 ; i++)
    {
        if( buffer[i] =='1')
        {
            c |= bit >>i;
        }
    }
    return c;
}
void decode_tree(struct Noeud *root, int *pos, char buffer[], FILE *fp)
{
    char bit;
    char carac;

    char bitBuffer[8];


    bit =read(fp,pos,buffer);

        if(bit =='1')
        {
            root->fd = NULL;
            root->fg = NULL;

            for(int i=0;i<8;i++)
            {
                bit=read(fp,pos,buffer);
                bitBuffer[i]=bit;

            }

            carac = bitToChar(bitBuffer);
            root->data = carac;

           // printf("carac : %d\n", carac);

        }

    else{
        struct Noeud *gauche =nouveauNoeud('X',0);;
		if(!gauche)
		{
			handleAlloc(gauche);
		}
        root->fg=gauche;
        decode_tree(root->fg,pos,buffer,fp);

        struct Noeud *droit =nouveauNoeud('X',0);
		if(!droit)
		{
			handleAlloc(droit);
		}
        root->fd=droit;
        decode_tree(root->fd,pos,buffer,fp);
    }



}

void decode_file(FILE *fp, int * pos, char buffer[], struct Noeud *root)
{
    char carac='0';
    int count=0;
    struct Noeud * n = root;
    char bit;
	


    while(carac != -96 )
    {
        if(n->fg==NULL && n->fd ==NULL)
        {

                carac = n->data;
				
                if(carac ==-96 )
                    break;
               printf("%c",carac);
				//fputc(carac, ftest);
				
                n = root;

        }
        else{
            bit =read(fp,pos,buffer);

            if(bit=='0')
            {
                n=n->fg;

            }
            else if(bit=='1')
            {
                n=n->fd;
            }
        }
    }
	
	bit =read(fp,pos,buffer);
	 carac=0;
	 n=root;
	 /*
	 
	 printf("\npos %d",*pos);
	 printf("\nbuffer : %c\n",bitToChar(buffer));
	 printf(" bit = %c\n",bit);
	 */
	
	 
	 
	if( bit!='0' || ( bitToChar(buffer)!='0' && bitToChar(buffer)>0)  )
	{

		while(carac != -96)
    {
		
        if(n->fg==NULL && n->fd ==NULL)
        {

                carac = n->data;
                if(carac ==-96)
                    break;
                printf("%c",carac);
				//fputc(carac, ftest);
                n = root;

        }
        else{
            bit =read(fp,pos,buffer);

            if(bit=='0')
            {
                n=n->fg;

            }
            else if(bit=='1')
            {
                n=n->fd;
            }
        }
    }
	
	}
	


}

void afficheTab(int tab[], int n)
{
    int i;
    for (i = 0; i < n; ++i)
        printf("%d", tab[i]);

    printf("\n");
}
void afficheCode(struct Noeud* root, int tab[],int top)
{

    if (root->fg) {

        tab[top] = 0;
        afficheCode(root->fg, tab, top + 1);
    }

    if (root->fd) {

        tab[top] = 1;
        afficheCode(root->fd, tab, top + 1);
    }


    if (!(root->fg) && !(root->fd)) {

        printf("codeChar(%c)=", root->data);
        afficheTab(tab, top);
    }
}
int main(int argc, char *argv[])
{
    char buffer[8];
    int pos=0;
	
	 if(argc<2){
		fprintf(stderr, " necessite : - un fichier à décompresser \n");
		exit(1);
	}
	FILE* finput = fopen(argv[1], "rb");
	if(!finput){
		fprintf(stderr, "Impossible d'ouvrir le fichier à décompresser : %s\n", argv[1]);
		exit(1);
	}


    struct Noeud *root = nouveauNoeud('X',0);
	if(!root)
		{
			handleAlloc(root);
		}
    read_byte(finput, buffer);
    decode_tree(root,&pos,buffer,finput);
    int tab[256];

	afficheCode(root, tab,0);
	
		
    decode_file(finput, &pos, buffer, root);
	free(root);
	
	printf("\n Lecture de %ld octets.\n", ftell(finput));
	printf("\n");
	fclose(finput);

    return 0;
}
