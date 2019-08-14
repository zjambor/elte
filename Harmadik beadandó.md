Harmadik beadandó
changelog:
•	2019.04.29: typo fixes, formázás
•	2019.04.28: feladatkiírás, határidő
________________________________________
Keretsztori:
Miután beindult a világok közötti turizmus, ideje volt fellendíteni a gazdasági vonalakat is. Ehhez a városokat egymás után járták a kereskedők, ahol különböző nyersanyagokkal üzleteltek. A tapasztalat alapján úgy alakult, hogy kereskedelmi szempontból 5 különböző árucikkben volt üzleti potenciál. A pénz alapú társadalom helyett a cserekereskedelem lett a lelke a monetáris szemléletnek. A vámosok szempontjából azonban szükségessé vált, hogy valamilyen módon nyomon kövessék az árusokat, hogy a határon átlépve ellenőrizni lehessen őket.
Ennek a rendszernek a kidolgozása a Te feladatod lett.
Az országba beutazva minden kereskedőnek számot kellett adnia arról, hogy pontosan milyen árucikkekkel rendelkezik. A bolygón eltöltött hosszú idő után köztudott volt, hogy a városok kereskedelmi érdekeltsége csupán addig terjedt, amíg a hasznosnak ítélt nyersanyagok mindegyikéből legalább egy “egységgel” (jelentsen ez bármit is) nem rendelkezik. A városokban dolgozó emberek legfeljebb 2 tájegységgel arrébbról tudták begyűjteni az ott lévő kincseket, így a városok raktárát ez alapján lehetett feltölteni. A cserekereskedelemnek azonban megvoltak a maga üzleti szabályai az árusok között: mindenki saját kereskedelmi aránnyal rendelkezett. A városok, amíg lehetőségeik (és érdekeltségeik) engedték, megpróbáltak üzletelni egy-egy kereskedővel, de amint a készleteik kellően teli voltak, vagy az árussal már nem jutottak dűlőre, már nem voltak hajlandóak beszélni velük. Ez a hozzáállás az előnyődre vált, mivel így ki tudtál dolgozni egy olyan pipeline szimulációt, mely tökéletesen modellezi az itt történteket.
Mivel a hatóságok menet közben nem tudták nyomon követni az egyes árusok mozgását, ezért felügyelni sem tudták őket, de az útjuk végén mindegyiknek ellenőrizni tudták a készleteit. Így ha lopás, vagy egyéb bűncselekmény történt, egyértelműen kiderült az az általad tervezett rendszernek köszönhetően. 
________________________________________
A megoldandó probléma tehát a következő:
A bemeneti fájlban (world.map) elsőként egy hexagonális (hatszögletű) rácson alapuló világ térképe található. A térképnél esetén feltesszük, hogy annak “hasznos része” az, amit a fájlból tudunk beolvasni, az összes, fájlban nem szereplő mezőre a világot körülölelő tengerként tekintünk.
A térkép hasznos részéhez meg van adva hány sorból és hány oszlopból áll (N x M), ezt követik az egyes mezők azonosítói.
A korábbiak alapján az összes civilizáció már választott magának fővárost, így ezeket is a térképből tudjuk kiolvasni (11-es szám jelzi a városok helyét, összesen C db. ilyen van).
A térképhez tartozó adatok után olvashatóak a kereskedők információi. Elsőként egy egész érték, a kereskedők száma (K >= 0), utána összesen K sorban egy-egy kereskedőhöz tartozó adatokat lehet beolvasni.
A kereskedelmi szempontokból fontos nyersanyagok a fa, arany, vas, hal és gabona (azok a mezők, amikből ezeket lehet szerezni: FIELD::FOREST, FIELD::GOLD_MINE, FIELD::IRON, FIELD::LAKE, FIELD::WHEAT).
A városok elsődleges szempontja, hogy mind az 5 típusú nyersanyagból legalább 1 db legyen a birtokukban. A kezdőállapotukat a körülöttük 2 sugárban lévő mezők alapján lehet megállapítani.
A kereskedés módja: - Meg kell nézni, hogy van -e olyan áruja (készleten) a kereskedőnek, amiből a város hiányt szenved (nincs neki egy se). - Ha nem, akkor ez a város nem kereskedik. - Ha igen, akkor meg kell nézni, van -e olyan másik, kereskedés szempontjából fontos nyersanyag, amiből kellő többlete van a városnak (legalább x+1 db, ahol x jelzi a kereskedő mohósági indexét). - Ha nem, akkor ez a város nem kereskedik. - Ha igen, akkor x:1 arányban megtörténik a csere (pl. x egység fát ad a város, ami levonásra kerül a készletéből, és gyarapszik egy vassal, a kereskedőnél pedig a fa mennyisége nő x-szel, a vaskészlet pedig eggyel csökken). - Amíg a város tud kereskedni, addig azt megteszi. - Ha már nem kereskedik tovább a város, akkor a kereskedő a meglátogatja a következő várost, amelyik ugyan ilyen alapon fog működni. - Egy csere során egyszerre csak egy típusú nyersanyagot ad a város, tehát nincs vegyes trade.
A feladatunk meghatározni azt, hogy az egyes kereskedők, miután végigmentek az összes városon milyen nyersanyagokkal rendelkeznek! Az így kiszámított eredményt (az egyes kereskedők megmaradt árukészletét) írjuk ki az output.txt fájlba (a bemenet sorrendjében)!
Tekintve, hogy egy-egy kereskedés sok időt vehet igénybe, ezért a kereskedők nem egyesével, egymást bevárva fogják végigjárni a városokat, hanem egymás után, amint “felszabadult” egy város egyből mennek a következőbe ( O(C*K) helyett O(C+K) ). Programozási szempontból ez az adatcsatorna tételének fog megfelelni!
________________________________________
Az inputfájl felépítése az alábbi:
Az első sorban két nemnegatív egész szám olvasható (jelölje ezt N és M), ezt követően egy N x M méretű, azaz N sorból és M oszlopból felépített világ térképe.
Az ezt következő N sor a térkép egy-egy sorának reprezentációját tartalmazza, pozitív egész számokat az [0..11] intervallumból szóközzel tagolva.
Ezt egy nemnegatív egész szám követi (jelöljük K-val), ami azt mondja meg, összesen hány kereskedő fogja végiglátogatni a városokat.
Az ezt követő K sor mindegyikében 5+1, azaz 6 egész számot lehet olvasni, ebből az első 5 azt jelzi, hogy a kereskedőnél kezdetben a “fontosnak” ítélt nyersanyagokból mennyi található (kezdő árukészlet), a következő szám pedig azt jelzi, hogy az adott kereskedővel milyen arányban lehet kereskedni, azaz hány db/egység (azonos típusú) árut kell a városnak odaadnia ahhoz, hogy a kereskedő egy árut átadjon a saját készletéből (mohósági index (mi.)). A 1 0 4 3 5 1 sor jelentése így tehát pl.: 1 fa, 0 arany, 4 vas, 3 hal, 5 gabona, 1:1 arányú cserelehetőség.
Feltehetjük, hogy az input fájl a fent leírtaknak megfelelően van kitöltve, és nem található benne pl. negatív, valós vagy olyan szám, ami nem felel meg mezőknek. A kezdeti mennyiségek mindegyike nemnegatív szám, azaz >= 0. A kereskedők minden esetben legfeljebb 1:1 arányú cserét biztosítanak, vagy ennél kedvezőtlenebbet. Ingyen egy kereskedő sem fog adni árut, és negatív értéket sem lehet olvasni (mi. >= 1 minden esetben). 
________________________________________
A program olvassa be a térképet (a már implementált módon), illetve a kereskedőkhöz tartozó adatokat, majd hívja meg calculate_trades(..) metódust. A program kimenete az egyes kereskedők árukészlete azután, hogy az összes várost meglátogatták abban a sorrendben, ahogy elindultak!
Egy példa bemenet (world.map):
9 13
4 5 2 5 2 7 9 5 8 2 5 0 7 
0 2 2 10 2 3 10 2 7 4 11 3 9 
9 0 8 4 4 1 9 2 7 7 5 6 6 
5 5 3 4 5 0 8 2 9 6 2 8 1 
10 10 9 10 8 10 2 10 2 7 10 4 6 
3 10 2 10 5 11 5 10 2 6 7 6 11 
10 6 8 8 5 9 7 4 1 7 6 0 0 
7 9 3 6 5 3 3 4 5 9 6 8 1 
6 3 5 2 8 6 6 10 5 7 7 10 11 
3
4 1 1 4 1 3
4 2 5 2 3 3
4 0 1 0 2 2
Az ehhez tartozó elvárt kimeneti fájl (output.txt):
4 1 4 3 1 
4 2 5 2 3 
3 0 1 2 2 
________________________________________
Az implementációs feladat
A feladat megoldásához kiindulásnak innen tudtok letölteni egy ZIP állományt. Ebben négy fájlt találtok - egy pipe.hpp, egy types.hpp és egy impl.cpp nevű fájlt, valamint egy minta.cpp-t, ami egy minimális reprezentációja egy adatcsatornának (az összerakáshoz és a push/pop műveletekhez illetve a paraméterezéshez).
A pipe.hpp fájlnak tartalmán nem kell (nem is szabad) változtatni - ezt az osztály egy konkurens felhasználásra tervezett, szálbiztos implementációt tartalmaz egy olyan (generikus) adatszerkezetre, melynek van egy T pop() és egy push(T&) metódusa adatok továbbításához (ha üres adatszerkezetre hív pop()-ot a program, akkor nem elszáll segfaulttal / exceptionnel, hanem addig vár, amíg nem érkezik adat. Pont emiatt vigyázni kell, hogy ne okozzon deadlockot a hibásan megírt felhasználása).
A types.hpp fájlnak tartalmán nem kell változtatni - ezt csupán érdemes nagyjából átnézni, hogy mi minden található benne - a lentebbi segítségben található metódusok és osztályok prototípusát / definícióját tartalmazza.
A main.cpp fájlban a térkép, majd a traderek beolvasása, utána a térképet reprezentáló objektumon a calculate_trades(..) függvény meghívása a beolvasott kereskedőkkel a feladat, más dolga ennek most nincs.
A nagyobb programozási feladat szokás szerint az impl.cpp fájlban található, meg nem írt függvények rendes implementációja (működésüket lásd lejjebb). A kész függvények a korábbi beadandókhoz tartozó implementációkat biztosítják, ezekkel már nem kell foglalkozni, lehet folytatni ezt a fájlt.
________________________________________
Segítség a megoldáshoz:
Az egyes, (harmadik feladat esetén) implementálandó függvények rövid működése:
•	Az is_useful_field(..) függvény visszaadja azt a logikai értéket, hogy a megadott mező típusa az 5, kereskedés szempontjából érdekes mező egyike e.
•	A get_field_index(..) az 5 hasznos típusú mezőt sorra a [0..4] intervallum egész elemeire képzi le (fa-0, arany-1, vas-2, hal-3, gabona-4). Ez a későbbiekben az árukészlet könnyebb kezeléséhez lehet hasznos (a metódus használata opcionális, lehet használni std::map<FIELD, int> adatszerkezetet is a nyilvántartáshoz, vagy egyedi logikát).
•	A have_sellable(..) metódus visszaadja, hogy a megadott raktárkészlet tartalmaz e még árut (azaz van e a tömbben nem 0 elem, használata szintén nem kötelező).
•	A Map::calculate_trades(..) az adatcsatornát létrehozó és a szálakat kezelő függvény. A konkrét implementációja később olvasható.
•	A Map::city_trade(..) az adatcsatorna egyes feldolgozóegységeinek függvényét reprezentálja. Ennek működése szintén később lesz kifejtve.
Az utóbbi két függvény működése lejjebb olvasható.
FONTOS:
A második beadandóhoz képest változás, hogy a cities_ adatszerkezetet (ami a térképen található városokat tartalmazza) már nem halmazként (std::set<>), hanem vektorként (std::vector<>) kerül reprezentálásra. Ehhez módosítani kell(ett) a Map::find_cities() metódust, illetve a gettert, és az ezt használó műveleteket. Ennek oka az, hogy a városokat mindenhol ugyan olyan sorrendben akarjuk elérni, hogy a sorrend ne függjön attól, milyen komparátor objektum alapján kerülnek tárolásra az elemek - így a feladatra is egyértelmű megoldást tudunk adni.

A Map::calculate_trades(..) függvény működése:
A függvény feladata az adatcsatorna összeállítása, az azokhoz tartozó folyamatok felparaméterezése és elindítása, az adatcsatorna első egységének az adatok továbbítása, majd a végén a folyamatok bevárása. Paraméteréül azt a vektort kapja, melyben az összes kereskedő megtalálható (traders). Ennek menete:
•	Keressük meg a térképen szereplő városokat (mivel korábban ezt még nem tettük meg a mostani implementációban - ha a saját kódot folytatjátok, akkor erre lehet már nincs szükség).
•	A városok számát jelöljük C-vel, ez a cities_ vektor méretével egyenlő.
•	Hozzunk létre két vektort. Az egyik fogja a csatornaváltozókat reprezentálni (pipes), ennek összesen C+1 eleme van (adjuk át a konstruktorban az elemszámot, hogy véletlenül se ütközzünk Pipe(const Pipe<T>&) vagy operator=() tiltásokba a fordító által). A másik (working_cities) tárolja azokat az std::thread objektumokat, amik a párhuzamosan futó folymatokat reprezentálják.
•	Indítsunk el C folyamatot, melyek rendre a city_trade(..) függvényt fogják meghívni 4 paraméterrel. Az i-ik iterációban indított threadnek a i-ik várost, azt az információt, hogy összesen hány kereskedővel lesz dolguk (traders alapján kiszámítható), valamint a be- és kimeneti adatokat reprezentáló csöveket, azaz az i-ik és i+1-ik elemét a pipes vektornak kell átadni (lásd: lambda lejjebb).
•	Az első csatornára küldjük el az összes elemét a traders vektornak.
•	Várjuk be az összes indított folyamatot az std::thread::join() műveletével.
•	Az adatok fájlbaírására két lehetőség van. Az egyik, hogy az indított szálak bevárása előtt a calculate_trades(..) függvény hozza létre az output.txt fájlt, és az utolsó csatornának az összes elemét kiírja ide. Ez a lassabb megoldás. A másik lehetőség plusz egy szálat elindítani, ami csak két paramétert kap - a kereskedők számát és az utolsó csatornaváltozót, és ez végzi a kiírást (hatékonyabb megoldás, mivel amint elér egy adat a csatorna végére, azt le is lehet emelni onnan, nem kell megvárni, hogy előbb mindegyiket az első csatornára feltegye a “fő” folyamat).
Segítség az indításhoz:
auto f = [&](int j){ city_trade(cities_[j], traders.size(), pipes[j], pipes[j+1]); };
A folyamatokat indító ciklusban ezután az egyes, working_cities vektorba elhelyezett std::thread objektumoknak az f lambdakifejezést és a ciklusváltozót kell átadni két paraméterül.

A city_trade(..) függvény működése:
Ez a függvény jelöli egy város kereskedési folyamatát valamelyik kereskedővel. A 4 paraméter amit megkap a város maga (ez egy koordináta a térképen), annak száma, hogy összesen hány kereskedő fogja meglátogatni (K), illetve két csatornaváltozó, az egyik a bemeneti (from), a másik a kimeneti adatok helyét jelzi (to).
•	Szedjük össze a város körül 2 sugárban lévő mezőket (első beadandó - get_tiles_in_radius(..))!
•	A mezők alapján állítsuk össze, hogy a város raktárkészlete kezdetben milyen - hasznos - nyersanyagokból hány darabot tartalmaz (ehhez lehet segítségünkre az is_useful_field(..) valamint a get_field_index(..) metódus). Alternatívaként használható std::map<FIELD, int> is (csak később egyeztetni kell ezt akkor a trader dolgaival).
•	Mivel összesen K db kereskedőt kell kiszolgálni, ezért összesen K-szor kell végrehajtani a következőket:
•	Kérjük le a következő kereskedőhöz tartozó adatokat a csatornából (from.pop()).
•	A kereskedő mohósági indexe (mi.) alapján állítsuk össze azoknak az áruknak a listáját, amiből van kellő fölöslegünk (legalább mi.+1 db.), ezekkel tudunk az adott körben kereskedni.
•	Az öt nyersanyag típus mindegyikén végigmenve nézzük meg, hogy van e lehetőség kereskedésre (have_sellable(..) metódus a városnál). Ha igen, és van olyan árufajta, amelyikből még nincs a városnak, akkor történjen meg a csere! (pl. 3:1 arányú cserénél egy fa/arany esetén a város elveszít 3 fát, a kereskedő kap 3-at, a város 1 arannyal gazdagodik, míg a kereskedőtől az elkerül).
•	Ha mind az 5 nyersanyag típust végignéztük (vagy már hamarabb elfogyott a város készlete), akkor a kereskedőt az útjára küldjük a következő város felé (to.push(..);), és folytatjuk az egészet a következő kereskedő kiszolgálásával (azaz a ciklus következő iterációja következik).
•	A ciklus befejezése után más dolga már nincs a városnak (a függvénynek).

A trade pontos menetéhez még egyszer a szabályok:
•	Meg kell nézni, hogy van -e olyan áruja (készleten) a kereskedőnek, amiből a város hiányt szenved (nincs a városnak egy se).
•	Ha nem, akkor ez a város nem kereskedik többet ezzel a kereskedővel (mert nem szorul rá).
•	Ha igen, akkor meg kell nézni, van -e olyan másik, kereskedés szempontjából fontos nyersanyag, amiből kellő többlete van a városnak (legalább mi.+1 db).
•	Ha nem, akkor ez a város nem kereskedik (mert nem tud).
•	Ha igen, akkor mi.:1 arányban megtörténik a csere (pl. mi. egység fát ad a város, ami levonásra kerül a készletéből, és gyarapszik egy arannyal, a kereskedőnél pedig a fa mennyisége nő mi.-vel, az aranykészlet pedig eggyel csökken).
•	Egy csere során egyszerre csak egy típusú nyersanyagot ad a város, tehát nincs vegyes trade (nem adhat oda egyszerre két fát és egy gabonát egy vasércért, ez nem Catan).
________________________________________
Általános elvárások:
Fokozottan kérünk mindenkit, hogy a beadandó határidejét (május 24, 19:59) vegye figyelembe! Kérjük osszátok be az időtöket, hogy ne csússzatok le a határidőről semmi esetben sem! Ez után nem áll módunkban elfogadni megoldásokat, kifogástól és indoklástól függetlenül! (Szült a halam, nem tudtam beadni, de elküldöm emailben; pont feltöltöttem volna, de lezárt a BEAD, jó lesz facebookon?; Késett a vonat de majd behozom pendriveon úgy jó? Nem, nem lesz jó egyik esetben sem.)
Kérünk mindenkit, hogy olvassa el a BEAD-on a hozzászólásokat, amiket írunk neki, mielőtt újra és újra feltölti a megoldást, így elkerülhető, hogy feleslegesen terheljétek a szervert, és egy körben lehet a legtöbb hibát javítani.
A megoldásaitokat egyetlen ZIP fájlba tömörítve töltsétek fel! Kérünk benneteket, hogy csak a szükséges forrásfájl(oka)t rakjátok bele az állomány gyökerébe, teljes projektet, solutiont (.cbp, .sln, illetve az ezekhez tartozó többi fájl), object fájlokat, vagy közvetlenül futtatható állományt (a.exe, a.out, etc..) semmiképp sem!
A beadandót százalékosan értékeljük!! Az értékelési útmutató a honlapunkon olvasható (az oldal alján található a link a PDF-hez).
Javítani bármennyiszer lehet, de a határidő szoros közeledtével egyre kisebb az esély rá, hogy lesz időnk megnézni azt.
Szeretnénk mindenkit megkérni, hogy az alábbiak vegye figyelembe a feltöltés során: 
•	A main.cpp, impl.cpp, pipe.hpp és types.hpp fájlokat mellékeljétek a ZIPbe, de a pipe.hpp, types.hpp fájlokon kérünk ne módosítsatok (ha nem muszáj)! 
•	A fájlokat a ZIP gyökerébe helyezzétek el, tehát ne mappát tömörítsetek, csupán a fájlokat! 
Értékelési megjegyzés: Az automata tesztelő elutasítja a megoldást, ha az nem fordult le (ennek okát is jelezni fogja). Amennyiben fordítható, lefuttatja több különböző inputra, és összeveti a megoldásodat az elvárt kimenetekkel. Ha mindegyik egyezik (a whitespacek között nem tesz különbséget, így mindegy, hogy a sorok végén van -e plusz szóköz vagy nincs, illetve sorvégi enter az utolsó bejegyzés után), akkor a BEADon átment a megoldásod. Ez után még valamelyik oktató is ellenőrzi a megoldást - a kódot átnézve, megjegyzéseket fűzve hozzá. Ha ez is rendben van, akkor elfogadottnak minősítjük a beadandót. Ha valami problémába ütközünk (vagy a BEADon nem megy át), akkor elutasításra kerül a megoldásod. Ennek okát részletezni fogjuk.
Egyéb segítség: https://people.inf.elte.hu/mykeesg/index.php?page=orsi&year=18_19_2
Az értékelési / kódolási guide szintén az előbbi linken érhető el (FAQ és követelmények).
Az automatikus tesztelés is hamarosan működni fog!
Ha hibát találtok a types.hpp fájlban, jelezzétek, hogy javíthassuk!
________________________________________

