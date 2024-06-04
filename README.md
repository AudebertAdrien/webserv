# README.md

## Webserv

### Description
Webserv est un serveur web écrit en C++, inspiré de la norme HTTP/1.1. Ce projet vise à implémenter un serveur capable de gérer des requêtes HTTP, de servir des fichiers statiques, et de traiter des scripts CGI.

### Fonctionnalités
- Gestion des requêtes HTTP/1.1
- Support des méthodes GET, POST, et DELETE
- Gestion des fichiers statiques
- Support des scripts CGI
- Configuration flexible via des fichiers de configuration

### Installation
1. Cloner le dépôt :
    ```sh
    git clone https://github.com/AudebertAdrien/webserv.git
    ```
2. Naviguer dans le répertoire du projet :
    ```sh
    cd webserv
    ```
3. Compiler le projet :
    ```sh
    make
    ```

### Utilisation
Pour lancer le serveur, exécutez :
```sh
./webserv <fichier_de_configuration>
```

### Configuration
Le fichier de configuration permet de définir les paramètres du serveur, tels que le port, les routes, et les options des scripts CGI. Un exemple de fichier de configuration est fourni dans le répertoire `config_files`.

### Structure du Projet
- `srcs/` : Contient le code source du serveur.
- `includes/` : Contient les fichiers d'en-tête.
- `config_files/` : Contient les fichiers de configuration d'exemple.
- `exemple.com/` : Contient des exemples de sites web à servir.

### Contribuer
Les contributions sont les bienvenues ! Veuillez ouvrir une issue ou soumettre une pull request pour toute amélioration ou correction.

### Auteurs
- Adrien Audebert
- Théo Lorne

### Licence
Ce projet est sous licence MIT. Consultez le fichier `LICENSE` pour plus d'informations.
