# NAR
A distributed file cloud
## Project Structure
- design: UMLs that describes the designs
  - nar-node: Software design of nar-nodes.
  - nar-server: Software design of nar-server.
- nar: Code of the project
  - lib: Libraries that are external and internal
  - narnode: Code of narnodes.
  - narserver: Code of narserver.
- tests: Test scripts.




constantlar full büyük harf olsun. farklı kelimeler alttan tireyle ayrılsın
işte tab yerine 4 space atalım
preferencesta şeyyapabilirsin
tab atınca 4 space olsun diye
topluca yazıyom hepsini dur
- Class isimleri büyük harfle başlar. Sonraki her kelime camel case yazılır
- Kanatlı parantezler ait olduğu yapının sağından başlar
    Örnek:
        class Naber {
        };

        void calc() {
        }

        for(int i=0; i<100; i++) {
        }

- Noktalı virgül, virgül vs. gibi noktalama işaretlerinden sonra 1 boşluk bırakılır.
- Indentation için 4 space kullanılır.
- Fonksiyon isimleri küçük harfle yazılır. Farklı kelimeler alttan tire ile ayrılır.
- Classlarda private variableların sonunda _ bulunur.
- Constantlar komple büyük harf yazılır. Farklı kelimeler _ ile ayrılır.
- Döngü değişkenleri döngüden hemen önce initialize edilir.
- if-else yapıları örnekteki gibi yapılır:

    if(cond) {
    } else if(cond2) {
    } else {
    }



 For Example
/**
* Class that is responsible for
* handling it all
*
* @author: baskin
* @privar: var1, int, holds the fucking thing
* @privar: var2, char, hold the fuck fuck
* @pubvar: var3, int, naber asdasdfaf
* @tested: no
* @todo: hede hude
**/
class RCP {
private:
int var1;
char var2;
public:
int var3;
/**
* A function that does it all
*
* @author: baskin
* @param: naber, int, biseyler
* @param: iyidir, int, comment
* @todo: implement etmek lazım
**/
void f(int naber, int iyidir);
};
