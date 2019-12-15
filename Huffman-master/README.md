
# Compresseur Huffman

Un compresseur et decompresseur Huffman en C. 


### Prérequis


Compiler les fichier huf.c et dehuf.c en utilisant la commande make.
L'archiveur python permet de parcourir les fichiers dans le dossier entrée par l'utilisateur, de les concaténer en un seul fichier et de l'archiver.


## Lancer les tests

Compresseur :

```
./huf.c source.txt output.txt
```
Decompresseur :

```
./dehuf.c output.txt
```
Archiveur:

```
python3 archiveur.py
```
## Bugs résiduels
Le désarchiveur n'est pas au point. Il ne s'arrête pas après le EOF.

## Conclusion
Nous aurions pu calculer l'entropie de Shannon pour chaque fichier afin de déterminer la limite maximal de la taille maximale du code. Dans un second temps, travailler sur des blocs de n caractères au lieu de de un par un, permettrait de compresser encore plus.

## Auteurs

* **Jean-Charles ALLA** (https://github.com/charlister)
* **Mathieu LADEUIL** (https://github.com/PupsDev)


## License



