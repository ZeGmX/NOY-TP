# NOY - TP 3 : Gestion de mémoire virtuelle et de fichiers mappés


<div style="text-align: justify">

Par Léo Cosseron et Lendy Mulot

### Où trouver le code

Notre code est disponible sur https://github.com/ZeGmX/NOY-TP.

Les fichiers modifiés pour ce TP sont les suivants :

* `vm/pagefaultmanager.cc`
* `vm/physmem.cc`
* `kernel/addrspace.cc`

Les fichiers ajoutés sont les suivants :

* `test/test_data_overflow.c`
* `test/test_big_file.c`
* `test_mapped_files.c`
* `nachos_few_pages.cfg`

### Avancement du TP

Les parties 2.5.1 à 2.5.3 ont été complètement implémentées et validées pendant les TPs. Cette partie correspond à celle du commit `946ab3d58c3cf43fa2b56e3c39b713ab3a2f016d`.

La partie sur la pagination pour du multi-thread n'a pas été implémentée.

Pour la partie 2.5.5 (fichiers mappés), l'implémentation a été vérifiée en TP, mais pas le fichier de test. La partie éviction de page n'a pas été implémentée pour les fichiers mappés. Cette partie correspond à celle du commit `0f24b605cd60d56d961408544773e90b6c64e51b`.

### Tests

Nous avons implémenté trois fichiers de test, présents dans le dossier `test/` :

* `test_data_overflow.c` implémente un programme pour lequel les données dépassent la taille de la RAM et sont donc en partie paginées.
* `test_big_file.c` est un programme est long et ne tient pas en RAM.

Ces deux programmes doivent être lancés avec l'argument `-f nachos_few_pages.cfg` pour utiliser un fichier de configuration avec très peu de pages

* `test_mapped_files.c` crée un fichier mappé, écrit et lit dedans, afin de vérifier le fonctionnement des fichiers mappés.

Il ne resterait donc qu'à implémenter la partie sur l'éviction de pages de fichiers mappés ainsi que la gestion de pagination pour du multi-thread.
</div>
