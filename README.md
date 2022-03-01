# Survival_Shooter
In cadrul acestui proiect se va implementa un survival shooter simplu cu camera top-down in care jucatorul trebuie sa distruga cat mai multi inamici, inainte sa moara.

**Jucatorul**

Jucatorul este reprezentat de cateva primitive geometrice (minimum 3), pozitionate sugestiv, astfel incat directia in care priveste jucatorul sa fie clara. Un exemplu este cel din figura de mai jos, in partea stanga, in care jucatorul priveste in jos. Directia in care priveste utilizatorul este data de cursorul mouse-ului. Acest lucru se poate observa de asemenea in figura de mai jos, unde sunt prezentate doua orientari diferite ale personajului, in functie de pozitia cursorului mouse-ului. Acesta se deplaseaza pe axele X și Y (sus, stanga, jos, dreapta), cu tastele W, A, S, D.

**Harta**

Spatiul de joc este un dreptunghi. Figura de mai jos prezinta moduri in care se poate reprezenta harta:
* dreptunghi solid (un obiect desenat in spatele tuturor elementelor din joc) - stanga
* dreptunghi wireframe - centru
* prin desenarea peretilor spatiului - dreptunghiuri solide - dreapta

Jucatorul nu poate sa iasa din harta. Harta trebuie sa fie mai mare decat fereastra de afisare. Pe harta vor fi si alte obstacole, reprezentate prin dreptunghiuri scalate diferit, prin care jucatorul nu poate sa treaca.

**Proiectilele**

Jucatorul poate sa traga cu proiectile reprezentate prin dreptunghiuri negre (la click stanga cu mouse-ul). Proiectilele vor fi lanstate din pozitia jucatorului, in directia in care jucatorul priveste (data de cursorul mouse-ului). Traiectoria acestora va liniara, iar viteza de deplasare, constanta. Ele vor fi de asemenea rotite in functie de directia de tragere. In momentul coliziunii cu un inamic, proiectilul va fi distrus (va disparea). De asemenea, inamicul va disparea, iar jucatorul va castiga un numar de puncte. Proiectilele nu pot trece prin obstacole. Astfel, la intalnirea unui obstacol sau al unui perete al hartii, proiectilele dispar. Arma jucatorului are o distanta maxima de tragere, astfel ca dupa parcurgerea unei anumite distante, un proiectil dispare, chiar daca nu a intalnit un inamic. Arma jucatorului are si un “fire rate” maxim. Astfel, jucatorul va putea lansa proiectile in mod succesiv dupa o regula de timp (de exemplu, nu va putea trage un proiectul decat o data la minim 0.5 secunde). Animatia de mai jos prezinta un exemplu in care jucatorul trage cu proiectile.

**Camera**

Camera va urmari in permanenta jucatorul (jucatorul va fi desenat tot timpul in centrul ferestrei de afisare). In video-ul de mai jos puteti vedea un exemplu de harta, in care spatiul logic se deplaseaza impreuna cu jucatorul. Animatia aceasta nu prezinta ce se vede pe ecran (in fereastra de afisare). In fereastra de afisare se va vedea numai ce este inclus in viewport.

**Inamicii**

Inamicii se reprezinta tot prin forme geometrice simple, dar cu alte culori/primitive, astfel incat sa se diferentieze fata de jucator. De exemplu, daca jucatorul este format din cercuri crem, asemenea figurii, inamicii pot fi reprezentati prin aranjamente de patrate rosii. Inamicii se vor crea incontinuu de-a lungul jocului, fie cate unul pe rand (de exemplu, sa apara un inamic nou la 3 secunde), fie in valuri (de exemplu, sa apara 5 inamici noi la 10 secunde). Fiecare inamic are o viteza diferita de deplasare. Pozitiile unde apar inamicii si vitezele acestora se pot stabili aleator sau se pot calcula dupa diferite reguli, dar nu trebuie sa fie hardcodate. Inamicii dispar cand sunt loviti de un proiectil al jucatorului. Inamicii trebuie sa urmareasca incontinuu jucatorul. Inamicii pot trece prin obstacole.

**Gameplay**

Scopul jocului este ca jucatorul sa supravietuiasca inamicilor cat mai mult timp. Jucatorul porneste de la un nivel maxim de viata (health). La fiecare coliziune cu un inamic, acest nivel scade. Daca nivelul scade la 0, atunci jucatorul moare si jocul se termina. Healthbar-ul se va prezenta pe ecran (orizontal, in partea de sus, sau vertical, in partea dreapta) prin doua dreptunghiuri:
* un dreptunghi wireframe, care reprezinta nivelul maxim de viata
* un dreptunghi solid, in interiorul celui wireframe, care reprezinta nivelul curent

Scorul va creste la eliminarea inamicilor. Scorul se va afisa periodic in consola sau se va ilustra pe ecran prin doua dreptunghiuri, similar cu healthbar-ul (scorebar).