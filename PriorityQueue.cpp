#include "PriorityQueue.h"
#include <iostream>
#include <vector>
#include <exception>

using namespace std;

//thata(1)
PriorityQueue::PriorityQueue(Relation r) {
	rel = r;
	cap = 50;
	length = 0;
	elm = new Element[cap];

}

//O(n)
void PriorityQueue::push(TElem e, TPriority p) {
	// daca array-ul este plin => marim capacitatea
	if (length == cap) {
		cap *= 2;
		// creare array nou de dimensiune cap marita
		auto* new_elm = new Element[cap];
		// copiere elemente in noul array
		for (int i = 0; i < cap; i++) {
			new_elm[i] = elm[i];
		}
		// stergere array vechi din memorie
		delete[] elm;

		// redenumire
		elm = new_elm;
	}
	// daca array-ul e gol => se adauga primul elm pe prima pozitie
	if (isEmpty()) {
		elm[0].first = e;
		elm[0].second = p;
		length++;
		return;
	}

	// elementul se adauga pe prima poz libera
	elm[length].first = e;
	elm[length].second = p;
	int poz = length; // poz = pozitia elementului pe care il adaugam
	length++;
	

	// cazul in care elm trebuie adaugat pe poz 1/2/3 (primii copii ai radacinii) va fi tratat separat
	// verificare prioritate
	// daca nu respecta relatia cu parintele => interschimbare (bubble-up)
	// pt un nod de indice i, parintele acestuia are indicele [(i - 1) / 3] (ternar Heap)
	while (!rel(elm[(poz - 1) / 3].second, elm[poz].second) && poz > 3) {
		Element aux = elm[(poz - 1) / 3];
		elm[(poz - 1) / 3] = elm[poz];
		elm[poz] = aux;
		poz = (poz - 1) / 3;
	}

	// cazul in care elm pe care il adaugam este (/a ajuns) pe pozitia primilor copii ai radacinii si nu respecta relatia cu radacina
	// => se interschimba radacina cu elm respectiv
	if (1 <= poz <= 3 && !rel(elm[0].second, elm[poz].second)) {
		Element aux = elm[0];
		elm[0] = elm[poz];
		elm[poz] = aux;
	}
}

//theta(1)
Element PriorityQueue::top()  const {
	if (!isEmpty()) {
		return elm[0];
	}
	
	throw exception();
	
}

//O(n)
Element PriorityQueue::pop() {
	if (isEmpty()) {
		throw exception();
	}
	Element del_elm;

	//daca len = 1 => se sterge radacina
	if (length == 1) {
		del_elm = elm[0];
		elm[0] = NULL_ELEMENT;
		length--;
		return del_elm;
	}

	// daca len = 2 => radacina va lua valoarea singurului copil 
	if (length == 2) {
		del_elm = elm[0];
		elm[0] = elm[1];
		elm[1] = NULL_ELEMENT;
		length --;
		return del_elm;
	}
	
	length--;
	del_elm = elm[0];
	elm[0] = elm[length]; // ultimul element va ajunge radacina
	elm[length] = NULL_ELEMENT; // stergere ultimul elm / NULL_TELEM
	int poz = 0; // pozitia elementului pe care il mutam (bubble-down)
	int poz_aux = 1; // folosita pentru compararea copiilor de pe acelasi nivel (la inceput poz_aux e pe primul copil al parintelui)
	int poz_curent = 0; // pozitia nodului cu cea mai mare prioritate
	Element aux;

	while (poz_aux < length) {
		aux = elm[poz]; // aux ia valoarea elementului de pe poz (valoarea cu care schimbam); aux - parinte
		// poz_aux <= 3 * poz + 3 adica se plimba doar printre copiii parintelui elm[poz]
		while (poz_aux <= 3 * poz + 3) {
			// verificare nod cu cea mai mare prioritate
			if (!rel(aux.second, elm[poz_aux].second)) {
				aux = elm[poz_aux];
				poz_curent = poz_aux;
			}
			poz_aux++;
			// daca suntem pe copiii ultimului nod parinte, si sunt mai putin de 3 copii => poz_aux = length
			if (poz_aux == length) {
				break;
			}
		}
		// daca nodul parinte are cea mai mare prioritate
		if (poz_curent == poz) {
			break;
		}
		// interschimbare valori
		elm[poz_curent] = elm[poz];
		elm[poz] = aux;
		poz = poz_curent; // pozitia unde urmeaza sa se faca bubble-down este poz_curent
		poz_aux = poz_curent * 3 + 1; // poz_aux = pimul copil al parintelui poz_curent
	}
	return del_elm;
}

//theta(1)
bool PriorityQueue::isEmpty() const {
	if (length == 0) {
		return true;
	}
	return false;
}

//theta(1)
PriorityQueue::~PriorityQueue() {
	delete[] elm; // stergere vector dinamic
}