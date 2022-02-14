/****************************************************************************************
										  FIIT STU
Predmet: Datove struktury a algoritmy
Semester: LS 2020/2021
Autor: Roland Vdovjak

										 Zadanie 01

Funkcia programu: Spravca pamati
Casti: memory_alloc, memory_free, memory_check, memory_init
Deadline: 08.03.2021
*****************************************************************************************/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

void* memstart=NULL;			// Globalny smernik, ktory bude ukazovat na zaciatok regionu

// Funkcia "bestFit" vracia smernik na miesto kde sa nachadza volny block najblizsie velkosti 'size'

void* bestFit(unsigned int size) {		
	int* ptr = (int*)memstart + 1;
	long int i = *ptr;		// Hodnota smernika
	int* best = NULL;		// zatial najvhodnejsi blok 
	char* volny = NULL;		// Bajt rozhodujuci o volnosti bloku

		// Pokial nenarazi na koniec regionu skace po smernikoch a porovnava s hladanaou velkostou

	while ((ptr - memstart) * sizeof(int) < *(int*)memstart) {
		volny = (char*)((int*)ptr + 1);					// Piaty bajt hovori o tom, ci je pamat volna alebo alokovana
		if (*volny == 1 && i > size) {					// Ak je pamat volna, vojde do podmienky
			if (best == NULL) {
				best = ptr;
			}
			else if (*ptr <= *best) {	// Porovnavanie velkosti, ktoru chceme alokovat so zatial najlepsou moznostou
				best = ptr;								// Ak je velkost aktualneho smernika vhodnejsia, najlepsia moznost sa zmeni
			}
			if (*best == size) break;
		}
		if ((ptr - memstart) * sizeof(int) < *(int*)memstart) {	// Ak nevyjde z regionu, zmení hodnotu a posunie smernik
			ptr = ((char*)ptr + i + sizeof(int) + 1);
			i = *ptr;  
			
		}
	}
	return (void*)best;
}

// Funkcia "findPrev" najde predosly smernik zadaneho smerniku a vrati ho

void* findPrev(void* ptr) {
	int* prev_ptr = memstart;	// Predchadzajuci smernik
	prev_ptr += 1;

	while (prev_ptr - memstart + *(int*)prev_ptr < (int*)ptr - memstart) {
		(char*)prev_ptr += *((int*)prev_ptr) + sizeof(int) + 1;
	}

	return (void*)prev_ptr;
}
//________________A_L_L_O_C____________________//

void* memory_alloc(unsigned int size) {
		
			// Pociatocne podmienky, ktore su nutne pred alokovanim
	if (memstart == NULL) return NULL;
	if (size == 0) return NULL;
	char* ptr = bestFit(size);
	if (ptr == NULL) return NULL;

	long int poc = *(int*)ptr;			// Hodnota smernika
	*(char*)(ptr + sizeof(int)) = 2;		// Bajt rozhodujuci o obsadenosti

			// Ak je mozne delit blok (vojde sa hlavicka[5B] a minimalna velkost bloku[8B]), rozdeli blok
	if (poc > size + sizeof(int) + 1 + 8){
		*(int*)ptr = size;
		*(int*)(ptr + sizeof(int) + size + 1) = poc - size - sizeof(int) - 1;
		*(char*)(ptr + sizeof(int)*2 + size + 1) = 1;
	}
		
	ptr += sizeof(int) + 1;		//nastavenie smernika na prvy bajt volnej pamati
	return ptr;
}

//_________________F_R_E_E____________________//

int memory_free(void* valid_ptr) {

	if(memstart==NULL)return 1;
		
		// Nastavenei premennych pre lepsiu pracu vo funkcii
	valid_ptr = (char*)((int*)valid_ptr - 1) - 1;					
	int* a = (int*)valid_ptr;											// Smernik na povodny blok
	int* b = (int*)((char*)valid_ptr + *a + sizeof(int) + 1);			// Smernik na nasledujuci blok
	char* c = (char*)(((int*)b) + 1);									// Smernik na bajt rozhodujuci o obsadenosti nasledujuceho bloku

		// Ak je nasledujuci blok volny, spoji sa s povodnym
	if ( (*c == 1) && ((b - memstart) * sizeof(int) < *(int*)memstart) ) {
		*(int*)a = *b + *(int*)a + 2 + sizeof(int)*2;
		*(int*)b = 0;
		*(char*)(((int*)b) + 1) = 0;
	}
	
	*(char*)(((int*)a)+1) = 1;		// Rozhodujucemu bajtu sa priradi 1, t.j. volny
	
		// Ak blok ktory uvolnujeme nie je prvy platny v regione, podmienka najde predchadzajuci, zisti volnost, popripade spoji
	int f = ((char*)a - (char*)memstart);
	if (f > 4) {
		char* prev;		// Predchadzajuci
		prev = findPrev(a);
		char* d = (char*)prev + sizeof(int);	// Rozhodujuci bajt o volnosti predchadzajuceho bloku
		if (*d == 1) {	
			*(int*)prev += *a + sizeof(int) + 1;
			*a = 0;
			*((char*)a + sizeof(int)) = 0;
		}
	}
	return 0;
}

//________________C_H_E_C_K____________________//

int memory_check(void* ptr) {
		
	if (memstart == NULL)return 0;	// Pociatocna podmienka
	if ((ptr > memstart) && ((char*)ptr - *(int*)memstart < memstart)) {
		char* ptrtype = (char*)ptr - 1;			// Nastavenie smernika na bajt, ktory rozhoduje o type obsadenosti bloku
		if (*(char*)ptrtype == 2) return 1;		// Podla hodnoty smernika vrati pozadovanu hodnotu 1/0
		else return 0;
	}
	return 0;
}

//_________________I_N_I_T____________________//

void memory_init(void *ptr, unsigned int size) {
    
	if (size < 2*sizeof(int) + 1 + 8){		// Kontola minimalnej pozadovanej velkosti pola
		memstart=NULL;
		return;
	}

	memstart = ptr;								// Nastavenie globalneho smernika
	unsigned int freeMem = size - sizeof(int);		// Vypocitanie velkosti volnej pamate

	*(int*)memstart = freeMem;					// Nastavenie obsahu globalneho smernika na pociatocnu velkost volnej pamati
	*((int*)memstart + 1) = freeMem - sizeof(int);	// Prva fragmentacia, po hlavicke regionu nasleduje prvy volny blok, t.j. hlavicka bloku 
	*(char*)(((int*)memstart + 1) + 1) = 1;		// Piaty bajt obsahuje informaciu o volnosti bloku

}

void memory_test(char* region, char **pointer, int rovnake, int block_min, int block_max, int memory_min, int memory_max) {
	
	memset(region, 0, 100000);	// Pre lepsiu pracu s regionom nastavime cely region na '0'
		
		// Premenna sluzi na uchovanie nahodenj pamati v zadanom rozsahu
	int random_memory = (rand() % (memory_max - memory_min)) + memory_min, random_blk = 0; 

		// Inicializacia pamati
	memory_init(region + 500, random_memory);
		
		// Alokovanie, testovanie smernika a uvolnenie pamati
	int i = 0;
	unsigned int allocated = 0, mallocated = 0, allo_count = 0, mallo_count = 0;
	
		// Kym nie je velkost alokovanej pamati vacsia ako celkova - minimalny blok
	while (allocated <= random_memory - block_min) {	
			
			// Priradime nahodne velky blok
		random_blk = (rand() % (block_max - block_min + 1)) + block_min;
			
			// Ak velkost bloku a alokovana pamat presahuje region, cyklus ide od zaciatku
		if (allocated + random_blk > random_memory)
			continue;
			
			// Danu pamat je mozne alokovat, tak sa pripocita, skusi sa alokovat, ak sa alokuje, pripocita sa
		allocated += random_blk; 
		allo_count++;
		pointer[i] = memory_alloc(random_blk);
		if (pointer[i]) {
			i++;
			mallo_count++;
			mallocated += random_blk;
		}
	}
		
		// Postupne overovanie smernikov a uvolnovanie pamati
	for (int j = 0; j < i; j++) {
		if (memory_check(pointer[j])) {
			memory_free(pointer[j]);
		}
		else {
			printf("ERROR: Smernik bol poskodeny.\n");
		}
	}
	
	// Pomer medzi poctom realne alokovanych a poctom poziadanych o alokaciu
		float vn1;
		if (allo_count == 0)vn1 = 0;
		else vn1 = ((float)mallo_count / allo_count) * 100;

	// Pomer medzi poctom realne alokovanych bajtov a celkovou pamatou regionu
		float vn2 = ((float)mallocated / random_memory) * 100;

	printf("Pamat o velkosti: %d bajtov\n", random_memory);
	if (rovnake) printf("Rovnake bloky o velkosti: %d B\n", random_blk);
	printf("Alokovane bloky: %.3f%%\n", vn1);
	printf("Alokovane bajty: %d (%.3f%%) \n\n", mallocated, vn2);
}

//_________________M_A_I_N____________________//

int main() {
	
	srand(time(0));
	char region[100500];
	char* pointer[13000];
	
		// Testy 1 - 4 sluzia na splnenie potrebneho testovania podla zadania
	printf("\t\t***     Testovanie 1:     ***\n");
	memory_test(region, pointer, 1, 8, 8, 40, 50);
	memory_test(region, pointer, 1, 12, 12, 80, 100);
	memory_test(region, pointer, 1, 24, 24, 180, 200);

	printf("\n\t\t***     Testovanie 2:     ***\n");
	memory_test(region, pointer, 0, 8, 24, 40, 50);
	memory_test(region, pointer, 0, 8, 24, 80, 100);
	memory_test(region, pointer, 0, 8, 24, 180, 200);

	printf("\n\t\t***     Testovanie 3:     ***\n");
	memory_test(region, pointer, 0, 500, 5000, 1000, 5000);
	memory_test(region, pointer, 0, 500, 5000, 5000, 80000);
	memory_test(region, pointer, 0, 500, 5000, 80000, 100000);
	
	printf("\n\t\t***     Testovanie 4:     ***\n");
	memory_test(region, pointer, 0, 8, 50000, 1000, 5000);
	memory_test(region, pointer, 0, 8, 50000, 5000, 80000);
	memory_test(region, pointer, 0, 8, 50000, 80000, 100000);

		/*  
		Test testu sluzi na overenie si korektnosti:
			region o velkosti:				100 000 B
			alokuje sa blok o velkosti:			  8 B
			blok spolu s hlavickou:				 13 B
			hlavicka regionu:					  4 B
			
			Predpoklad: 
				(100 000 - 4) * (8/13) = 61 536 = 61,536%
		
	printf("\nTestovanie spravnosti testovania:\n");
	memory_test(region, pointer, 0, 8, 8, 100000, 100001);
		*/	

	return 0;
}