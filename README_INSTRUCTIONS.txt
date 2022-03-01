Pentru desenarea jucatorului folosesc formulele cu sin si cosinus dupa care adaug indicii.
""drawings.h/.cpp""

Efectul de miscarea al jucatorului se realizeaza miscand absolut toate celelalte obiecte mai putin jucatorul.
""game.cpp onInputUpdate()""

Orientarea jucatorului se realizeaza folosindu-ma de o orientare default (se uita la dreapta).
Astfel acum iau coordonatele mouse-ului si realizez un triunghi dreptunghic:
              Y
           -  |
         -    |
      -       |
    -         |
 - alfa       |
X-------------Z
X - este pozitia jucatorului
Y - este pozitia mouse-ului
Z - este pozitia de orientare default (asa am asezat initial jucatorul sa se uite)

Asadar jucatorul se uita XZ dar noi trebuie sa il orientam XY deci rotit cu un unghi ALFA.
Cunoastem toate pozitiile XYZ. Nu cunoastem alfa, dar il vom afla folosind produs scalar (dot product).

Pentru a pune in practica si mai multe detalii despre produs scalar atasez urmatorul link:
https://www.omnicalculator.com/math/angle-between-two-vectors

Pentru cazul cand mouse-ul este sub jucator inmultim cu -1.
Coordonatele originii mouse-ului (0,0) se afla in stanga sus.
""game.cpp onMouseMove()""


Inamicii folosesc aceeasi logica de orientare ca mai sus doar ca in loc de pozitia mouse-ului va fi pozitia jucatorului.
""level.cpp RotateEnemiesTowards()""

Orientarea proiectilelor se foloseste cu aceleasi formule.
""game.cpp OnMouseBtnPress()""


Harta este tinuta intr-un std::vector (patrate ce au o anumita pozitie si o anumita scala)
Acestea vor fi miscate atunci cand jucatorul apasa W/A/S/D pentru a simula miscarea jucatorului pe harta.
""level.cpp walls field""
""level.cpp CreateMap()""

Camera este fixa. Nu a fost folosita la nimic pentru ca misc harta pentru a simula viewport.

Inamicii sunt desenati din patrate:
""level.cpp enemies field si game.cpp renderEnemies()""

Inamicii vor fi miscati cu o viteza in directia jucatorului, dar si atunci cand jucatorul apasa W/A/S/D precum miscarea peretilor hartii.
""game.cpp OnInputUpdate() mai exact level->MoveObjects()""

Inamicii vor aparea ALEATOR in EXTERIORUL hartii prin folosirea unei limite astfel ca nu vor aparea nici foarte departe. Am ales aceasta abordare pentru a nu exista posibilitatea de instantiere a unui inamic fix langa jucator rezultand ca inamicul sa omoare jucatorul instant.
Astfel nu exista niciun motiv de realizare a coliziunii dintre inamic si perete harta, intrucat inamicii sunt instantiati in exteriorul hartii.
""enemy.cpp Spawn()""

Pentru rata de tragere al proiectilelor folosesc o variabila in care numar secundele si ma asigur ca daca nu a trecut 0.5 secunde nu ii dau voie sa traga.
Proiectilele vor disparea dupa o anumita distanta (default: 600 pixeli)
""bullet.h""

Coliziunile dintre jucator si pereti sunt facute inainte de a simula miscarea jucatorului, astfel daca dupa apasarea tastei W urmeaza sa se afle intr-o coliziune cu un perete atunci miscarea nu va fi realizata. Astfel coliziunile cu peretii au o trecere "smooth' (nu exista efectul de "jittering"). Acest efect ar fi fost intalnit atunci cand miscam jucatorul si dupa incercam sa remediez pozitia lui in caz de coliziune.
Coliziunile sunt luate dupa site-ul urmator:
https://learnopengl.com/In-Practice/2D-Game/Collisions/Collision-detection
""game.cpp OnInputUpdate()""

Restul coliziunilor sunt doar pe concept de tragaci ("trigger") ce se afla intr-o functie. Prin asta ma refer la faptul ca observam coliziunea si facem ceva cu obiectele (in cazul acestui joc, dispar de exemplu proiectilele la lovirea unui perete/inamic)
""game.cpp DetectTriggerCollisions()""

Scorul este pur si simplu un int la care adaug puncte cand un inamic este omorat de proiectil. Atunci cand scorul creste este afisat in consola.
""game.cpp int *score""   //  "game.cpp RenderLifes()"

Viata este realizata printr-un healthbar ce contine 3 patrate ce semnifica cate vieti mai avem. Numarul de vieti este de asemenea afisat in consola.
""player.h lifes field""

Jocul dupa pornire te anunta in consola ca pentru a incepe trebuie sa apesi orice tasta.
""game.cpp Init()""
De asemenea, iti spune ca ai scorul 0.

Dupa moarte, se reseteaza harta, inamicii si afiseaza din nou in consola ca pentru a incepe va trebui sa apesi orice tasta. In plus, iti spune si scorul final.
""game.cpp DetectTriggerCollisions()""
