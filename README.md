SARAH-Plugin-arduino
====================

Arduino server handling with S.A.R.A.H.

Ce plugin développé pour SARAH a pour but de permettre une intéraction entre une arduino et l'extérieur, un serveur NodeJS faisant le lien entre les deux mondes.

Il est composé d'une partie Arduino, avec un script à uploader sur la carte et des librairies à installer; ce servant à actionner des emmetteurs par exemple.
Et d'une parte NodeJS, avec laquelle les clients intéragissent, de sorte que l'arduino soir reliée indirectement au monde extérieur.


====================
README script arduino:

Les libraires MySwitch et IRremote sont destinées respectivement à gérer des prises commandées en 433Mhz et à interpréter le code de télécommandes infrarouges, ils ne sont donc pas nécessaires au code de base.

Pour ajouter les librairies, copier le dossier './libraries' ) la racine du dossier de l'IDE arduino (eg: 'C:\Program Files (x86)\Arduino').

Pour configurer les I/O et les autres paramètres, les lignes à modifier sont repérables à leur commentaire "// CONFIG".
