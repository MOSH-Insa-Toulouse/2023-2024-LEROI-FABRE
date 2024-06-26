# Projet MOSH
## Par Quentin et Laetitia 🥇
### ~~Le meilleur binôme~~ (il faut pas le dire) 

![IMG_8024](https://github.com/MOSH-Insa-Toulouse/2023-2024-LEROI-FABRE/assets/159795025/75c8a517-8a90-4ea4-baef-abd0838ea5a8)

Cohésion d'équipe ☑️
## Diagramme du projet
```mermaid
graph TD;
    A[Capteur Graphite]-->B((Arduino Uno));
    F[Capteur Flex]-->B((Arduino Uno));
    G[Potentiomètre]-->B((Arduino Uno));
    B((Arduino Uno))-->D[Ecran LCD];
    B((Arduino Uno))-->C[Module Bluetooth];
    C[Module Bluetooth]-.->E[App Android];

```

<details>
<summary>  Etapes du projet MOSH :tada:  </summary>
  
  - [x] Attribution des inputs de l'arduino aux différents capteurs
  - [X] Electronique Analogique
  - [x] Schématique KiCad  
  - [x] Routage KiCad
  - [x] Impression du cricuit
  - [x] Percage
  - [X] Montage des capteurs sur le circuit imprimé (par Cathy, MERCIII)
  - [X] Code Arduino
  - [X] App Android
  - [X] Création du banc de test
  - [X] Création de la datasheet
        
</details>


# Quel est ce projet ?

Ce projet consiste en...

<details>
<summary> la création d'un capteur Graphite. </summary>
Un capteur graphite, c'est tout simplement un morceau de papier où l'on colorie une zone à l'aide d'un crayon à papier. Il permet de mesurer des variations de résistance lorqu'on le déforme. Voici la forme de capteur que nous utilisons : 

![IMG_8024](https://github.com/MOSH-Insa-Toulouse/2023-2024-LEROI-FABRE/blob/main/Images/Jauge_Contrainte_A_Imprimer.png)
</details>

<details>
<summary> la création d'un shield Arduino. </summary>
Cela permet d'effectuer les acquisitions des données envoyées par notre capteur (des variations de tensions que nous pouvons traduire en résistance !). 
Nous y implémenterons également un capteur Flex dont voici la datasheet : (insérer la datasheet), un écran LCD pour afficher les données du capteur graphite ou du capteur flex, un potentiomètre qui permettra à l'utilisateur de naviguer dans un menu qui permet d'afficher les données du capteur graphite ou du capteur flex, un potentiomètre numérique qui permettra d'ajuster une résistance pour la maniulation des données du capteur graphite,... 

Pour cela, il nous faut créer le shield sous KiCad, le faire imprimer, percer le trous, et y monter nos capteurs.
</details>

<details>
<summary> la création d'un code Arduino. </summary>
Le code permettra de commander le microcontrôleur Arduino Uno : gestion des données, commnication Bluetooth (voir App android), choix de la résistance du potentiomètre numérique, affichage sur l'écran LCD...
</details>

<details>
<summary> la création d'une App Android. </summary>
Notre projet s'accompagne également d'une App Android : grâce à la communication Bluetooth, nous pouvons afficher sur un smartphone des graphiques des données enregistrées par nos deux capteurs. Nous créons l'app sur  MIT app Inventor (https://community.appinventor.mit.edu/).
</details>

<details>
<summary> tout ça pour l'élaboration d'une Datasheet ! </summary>
L'objectif final, le graal du projet, c'est de rédiger une datasheet du capteur Graphite. Le banc de test et notre code arduino nous ont permis de réaliser toutes les mesures. 
</details>

# Je veux aller voir...
 __(cliquez)__ 

## L'Electronique Analogique  [Clique](https://github.com/MOSH-Insa-Toulouse/2023-2024-LEROI-FABRE/tree/main/LTSpice%20-%20Electronique%20Analogique)
<details>
<summary> Explications. </summary>
    
Vous trouverez dans ce dossier le circuit analogique du capteur graphite. Il y a un amplificateur transimpédance et un filtre qui permettent d'obtenir un signal utilisable. En effet, le signal délivré par le capteur sans ce circuit est trop faible : il a un courant moyen de 100nA, donc il faudrait un microcontrôleur très précis pour pouvoir détecter des variations sur ce signal. C'est pourquoi nous l'amplifions. 

Les filtres sont nécessaires afin d'élminer les fréquences 50Hz dans lesquelles nos environnements sont baignés (à cause du réseau électrique), et d'autres fréquences parasites.

Voici un schéma, qui est une capture d'écran de nos simulations LTSpice : 
![IMG_8024](https://github.com/MOSH-Insa-Toulouse/2023-2024-LEROI-FABRE/blob/main/Images/LTSpice)


Les parties qui servent à filtrer sont dans les rectangles :
- le rectangle vert, avec une capacité de 100n, sert à filtrer le signal d'entrée. C'est un filtre passe-bas. Sa fréquence de transition est d'envirion 159Hz (formule f=1/(2*pi*R*C)). Il permet donc d'éliminer le bruit qui pourrait naître à l'entrée du circuit.
- le rectangle violet a une fonction similaire : c'est un filtre passe-bas, qui permet de filtrer le bruit en sortie, de fréquence de transition de 1591Hz.
- le rectangle rouge entoure le filtre passe-bas qui permet de filtrer le bruit environnant à 50Hz.

Le reste du circuit permet de réaliser l'amplifictation du signal. 

</details>

## Le KiCad ! [C'est ici](https://github.com/MOSH-Insa-Toulouse/2023-2024-LEROI-FABRE/tree/main/KiCad)

<details>
<summary> Explications. </summary>
    
Nous avons créé des empreintes KiCad pour notre capteur graphite, et pour les éléments qui ne sont pas déjà disponibles dans a librairie intégrée KiCad. Vous pouvez les retrouver dans le dossier KiCad. 

Nous avons ces contraintes pour la fabrication de nos shields : 
- largeur de routage de 0.5mm minimum.
- largeur de 0.5mm minimum entre différents routages afin d'assurer l'isolation électrique.
- nous avons élargi les pads du module bluetooth, de l'encodeur rotatoire, de l'écran OLED et des connecteurs du header à 2*2.54 (oval).
- vias de 0.8mm, cercle de 2.54mm.

Cela assure le fonctionnement du shield, et nous permet de percer les vias facilement.

| :exclamation:  ATTENTION CHERS COLLEGUES   |
|----------------------------------------------|

Nous avons commis des erreurs sur le fichier KiCad ! Chers ingénieurs/techniciens/instrumentateurs qui réalisez ce projet en suivant nos pas, n'oubliez pas de relier le MCP au reste du circuit !! Et n'inversez pas les pattes du LTC !!


</details>


## Non, je veux voir le code Arduino [Juste là](https://github.com/MOSH-Insa-Toulouse/2023-2024-LEROI-FABRE/tree/main/Arduino%20V2/projet_capteur) 

<details>
<summary> Explications. </summary>

Ce code a toutes ces fonctionalités :
- gère l'affichage sur l'écran OLED via une fonction.
- l'interruption DoEncodeur est réalisée à chaque fois que quelqu'un tourne l'encodeur rotatoire : l'utilisateur tourne l'encodeur, et au prochain tick de clock, linterruption sera réalisée. Elle permet d'ajouter +1 à la valeur que compte l'encodeur. Celui-ci sert à gérer le menu de l'écran OLED. Le principe est que lorsque nous tournons l'encodeur, c'est pour changer l'affichage de l'écran. Ainsi, si la valeur comptée par l'encodeur est paire nous aurons un afichage (capteur Flex), et s'il est impair, nous aurons un autre affichage (capteur graphite).
- la communication Bluetooth : nosu envoyons les données du capteur graphite ou du capteur flex à une application MIT via bluetooth. Si le module bluetooth recoit 1 de la part de l'application, c'est que l'utilisateur a appuyé sur le bouton "capteur flex". S'il recoit 2, c'est que l'utilisateur a appuyé sur "capteur graphite" dans l'app.
- la modification de la résistance du potentiomètre numérique : cela permet de calibrer le signal renvoyé par le capteur graphite.

  </details>

## Je préfère voir l'app Android [Okay...](https://github.com/MOSH-Insa-Toulouse/2023-2024-LEROI-FABRE/tree/main/App%20Android)
<details>
<summary> Explications. </summary>

Le projet s'accompagne d'une App qui fonctionne sur les téléphones android. Elle permet d'abotenir en temps réel les mesures du capteur graphite ou du capteur flex. Cela fonctionne grâce à une connexion Bluetooth. L'application est simple d'utilisation pour l'utilisateur.

Le code de l'application se fait sous [MIT App Inventor.](https://community.appinventor.mit.edu/) C'est un site gratuit simple dutilisation. Le code se fait via des blocks que l'utilisateur ajoute petit à petite, c'est très intuitif.
Voici une photo de l'application en fonctionnement :

![IMG_8024](https://github.com/MOSH-Insa-Toulouse/2023-2024-LEROI-FABRE/blob/main/Images/app.png)

</details>

## La Datasheet [Elle est au chaud ici](https://github.com/MOSH-Insa-Toulouse/2023-2024-LEROI-FABRE/tree/main/Datasheet)

## Des photos du projet, plutôt ! 📷 
<details>
<summary> Attention, époustouflant. </summary>

| :boom: BANG               |
|:----------------------------|
| Le capteur fonctionne ! On capte bien des variations quand on bouge le capteur Graphite |

![IMG_8024](https://github.com/MOSH-Insa-Toulouse/2023-2024-LEROI-FABRE/blob/main/Images/ca-marche!.png)

| :boom: BANG               |
|:----------------------------|
| Le projet, c'est aussi beaucoup de fils lors de la phase de tests ! On a nettoyé tous ces fils ensuite, et c'était tout de suite beaucoup plus clair..! |


![IMG_8024](https://github.com/MOSH-Insa-Toulouse/2023-2024-LEROI-FABRE/blob/main/Images/bcp-de-fils.png)

| :boom: BANG               |
|:----------------------------|
| Une des erreurs du fichier KiCad... |


![IMG_8024](https://github.com/MOSH-Insa-Toulouse/2023-2024-LEROI-FABRE/blob/main/Images/erreurs.png)

| :boom: BANG               |
|:----------------------------|
| N'oubliez pas de faire un poisson d'avril à votre binôme |

![IMG_8024](https://github.com/MOSH-Insa-Toulouse/2023-2024-LEROI-FABRE/blob/main/Images/poisson.png)

| :boom: BANG               |
|:----------------------------|
| Quelle belle application Android |

![IMG_8024](https://github.com/MOSH-Insa-Toulouse/2023-2024-LEROI-FABRE/blob/main/Images/app.png)

| :boom: BANG               |
|:----------------------------|
| Petit selfie de fin de projet |

![IMG_8024](https://github.com/MOSH-Insa-Toulouse/2023-2024-LEROI-FABRE/blob/main/Images/selfie.png)


| :boom: BANG               |
|:----------------------------|
| C'était un projet à en tomber par terre |

![IMG_8024](https://github.com/MOSH-Insa-Toulouse/2023-2024-LEROI-FABRE/blob/main/Images/a-en-tomber-par-terre.png)


