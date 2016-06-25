#include <iostream>
#include <vector>
#include "functions.h"

using namespace std;


int main()
{
	///* 

	//Пример 1:
	//Даны утверждения:
	//1. "Сократ - человек."
	//2. "Человек - это живое существо."
	//3. "Все живые существа смертны."
	//Требуется доказать: "Сократ смертен."
	//Для этого вводим в нашу базу утверждение: "Сократ бессмертен."
	//Получение пустого дизъюнкта означает, что высказывание «Сократ бессмертен» ложно, значит истинно высказывание «Сократ смертен».

	vector<Conjunct*> kb;
	Value* val1 = new Value("Socrates");
	vector<Element*> params;
	params.push_back(val1);
	Term* p1 = new Term("Man",params,false);
	Conjunct* c1 = new Conjunct(p1);
	kb.push_back(c1);
	Variable* var1 = new Variable("x1");
	params.clear();
	params.push_back(var1);
	Term* p2 = new Term("Man",params,true);
	Term* p3 = new Term("Creature",params,false);
	Conjunct* c2 = new Conjunct(p2);
	c2->Add(p3);
	kb.push_back(c2);
	Variable* var2 = new Variable("x2");
	params.clear();
	params.push_back(var2);
	Term* p4 = new Term("Creature",params,true);
	Term* p5 = new Term("Mortality",params,false);
	Conjunct* c3 = new Conjunct(p4);
	c3->Add(p5);
	kb.push_back(c3);
	params.clear();
	params.push_back(val1);
	Term* p6 = new Term("Mortality",params,true);
	Conjunct* c4 = new Conjunct(p6);
	kb.push_back(c4);
	bool sol = Solution(kb);
	if (sol)
		cout << "Proposition is false (obtained empty conjunct)" << endl;
	else
		cout << "Proposition is true" << endl;
	//*/

	/*
	//Пример 2:
	//Пусть брадобреи бреют всех людей, которые не бреются сами и не бреют тех, кто бреется сам. Тогда брадобреи не существуют.
	vector<Conjunct*> kb;
	Variable* var1 = new Variable("x1");
	Variable* var2 = new Variable("y1");
	vector<Element*> params;
	params.push_back(var1);
	Term* p1 = new Term("Barber",params,true);
	params.push_back(var2);
	Term* p2 = new Term("Shaves",params,false);
	params.clear();
	params.push_back(var2);
	params.push_back(var2);
	Term* p3 = new Term("Shaves",params,false);
	Conjunct* c1 = new Conjunct(p1);
	c1->Add(p3);
	c1->Add(p2);
	kb.push_back(c1);
	Variable* var3 = new Variable("x2");
	Variable* var4 = new Variable("y2");
	params.clear();
	params.push_back(var3);
	Term* p4 = new Term("Barber",params,true);
	params.push_back(var4);
	Term* p5 = new Term("Shaves",params,true);
	params.clear();
	params.push_back(var4);
	params.push_back(var4);
	Term* p6 = new Term("Shaves",params,true);
	Conjunct* c2 = new Conjunct(p4);
	c2->Add(p6);
	c2->Add(p5);
	kb.push_back(c2);
	Value* val1 = new Value("A");
	params.clear();
	params.push_back(val1);
	Term* p7 = new Term("Barber",params,false);
	Conjunct* c3 = new Conjunct(p7);
	kb.push_back(c3);
	bool sol = Solution(kb);
	if (sol)
		cout << "Proposition is false (obtained empty conjunct)" << endl;
	else
		cout << "Proposition is true" << endl;
	*/

	/*
	//Пример 3:
	//Каждого, кто любит всех животных, кто-то любит.
	//Любого, кто убивает животных, никто не любит.
	//Джек любит всех животных.
	//Кота по имени Тунец убил либо Джек, либо Любопытство.
	//Действительно ли этого кота убило Любопытство?
	vector<Conjunct*> kb;
	Variable* var1 = new Variable("x1");
	vector<Element*> params;
	params.push_back(var1);
	Term* f1 = new Term("F",params,false);
	Term* f2 = new Term("G",params,false);
	params.clear();
	params.push_back(f1);
	Term* p1 = new Term("Animal",params,false);
	params.clear();
	params.push_back(f2);
	params.push_back(var1);
	Term* p2 = new Term("Loves",params,false);
	Conjunct* c1 = new Conjunct(p1);
	c1->Add(p2);
	kb.push_back(c1);
	Variable* var2 = new Variable("x2");
	params.clear();
	params.push_back(var2);
	Term* f3 = new Term("F",params,false);
	Term* f4 = new Term("G",params,false);
	params.clear();
	params.push_back(var2);
	params.push_back(f3);
	Term* p3 = new Term("Loves",params,true);
	params.clear();
	params.push_back(f4);
	params.push_back(var2);
	Term* p4 = new Term("Loves",params,false);
	Conjunct* c2 = new Conjunct(p3);
	c2->Add(p4);
	kb.push_back(c2);
	Variable* var3 = new Variable("x3");
	Variable* var4 = new Variable("y3");
	Variable* var5 = new Variable("z3");
	params.clear();
	params.push_back(var4);
	Term* p5 = new Term("Animal",params,true);
	params.clear();
	params.push_back(var3);
	params.push_back(var4);
	Term* p6 = new Term("Kills",params,true);
	params.clear();
	params.push_back(var5);
	params.push_back(var3);
	Term* p7 = new Term("Loves",params,true);
	Conjunct* c3 = new Conjunct(p5);
	c3->Add(p6);
	c3->Add(p7);
	kb.push_back(c3);
	Variable* var6 = new Variable("x4");
	Value* val1 = new Value("Jack");
	params.clear();
	params.push_back(var6);
	Term* p8 = new Term("Animal",params,true);
	params.clear();
	params.push_back(val1);
	params.push_back(var6);
	Term* p9 = new Term("Loves",params,false);
	Conjunct* c4 = new Conjunct(p8);
	c4->Add(p9);
	kb.push_back(c4);
	Value* val2 = new Value("Tuna");
	Value* val3 = new Value("Curiosity");
	params.clear();
	params.push_back(val1);
	params.push_back(val2);
	Term* p10 = new Term("Kills",params,false);
	params.clear();
	params.push_back(val3);
	params.push_back(val2);
	Term* p11 = new Term("Kills",params,false);
	Conjunct* c5 = new Conjunct(p10);
	c5->Add(p11);
	kb.push_back(c5);
	params.clear();
	params.push_back(val2);
	Term* p12 = new Term("Cat",params,false);
	Conjunct* c6 = new Conjunct(p12);
	kb.push_back(c6);
	Variable* var7 = new Variable("x7");
	params.clear();
	params.push_back(var7);
	Term* p13 = new Term("Cat",params,true);
	Term* p14 = new Term("Animal",params,false);
	Conjunct* c7 = new Conjunct(p13);
	c7->Add(p14);
	kb.push_back(c7);
	params.clear();
	params.push_back(val3);
	params.push_back(val2);
	Term* p15 = new Term("Kills",params,true);
	Conjunct* c8 = new Conjunct(p15);
	kb.push_back(c8);
	bool sol = Solution(kb);
	if (sol)
		cout << "Proposition is false (obtained empty conjunct)" << endl;
	else
		cout << "Proposition is true" << endl;
	*/

    system("pause");
	return 0;
}