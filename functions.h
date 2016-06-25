#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include <iostream>
#include <list>
#include <vector>
#include <string>

using namespace std;

enum Type{ ELEMENT, VARIABLE, VALUE, LIST, TERM }; //тип элемента

class Element // базовый класс, от которого наследуются осстальные
{
public:

	Element(){};

	virtual Type GetType()
	{
		return Type::ELEMENT;
	};

	virtual bool Equal(Element* e)
	{
		return (this->GetType()==e->GetType());
	}

	virtual void Print()
	{
	}

};

class Variable: public Element
{
private:
	string name;
public:
	Variable(string name)
	{
		this->name=name;
	}

	string GetName()
	{
		return name;
	}

	Type GetType()
	{
		return Type::VARIABLE;
	};

	static bool IsVariable(Element* e)
	{
		return e->GetType()==Type::VARIABLE;
	}

	bool Equal(Element* e)
	{
		if (e->GetType() != Type::VARIABLE)
			return false;
		return name.compare(dynamic_cast<Variable*>(e)->GetName()) == 0;
	}

	void Print()
	{
		cout << name;
	}

};

class Value: public Element
{
private:
	string value;
public:
	Value(string value)
	{
		this->value=value;
	}

	string GetValue()
	{
		return value;
	}

	Type GetType()
	{
		return Type::VALUE;
	};

	static bool IsValue(Element* e)
	{
		return e->GetType()==Type::VALUE;
	}

	bool Equal(Element* e)
	{
		if (e->GetType() != Type::VALUE)
			return false;
		return value.compare(dynamic_cast<Value*>(e)->GetValue()) == 0;
	}

	void Print()
	{
		cout << value;
	}

};

class List: public Element // список элементов - нужен для алгоритма унификации
{
private:
	vector<Element*> params;
public:
	List(vector<Element*> params)
	{
		this->params=params;
	}

	int Size()
	{
		return params.size();
	}

	bool IsEmpty()
	{
		return (params.size()==0);
	}

	Element* First()
	{
		return params[0];
	}

	Element* Rest() // эквивалентно Tail
	{
		vector<Element*> new_params(++(params.begin()),params.end());
		Element* res = new List(new_params);
		return res;
	}

	Type GetType()
	{
		return Type::LIST;
	};

	static bool IsList(Element* e)
	{
		return e->GetType()==Type::LIST;
	}

	bool Equal(Element* e)
	{
		if (e->GetType() != Type::LIST)
			return false;
		List* l = dynamic_cast<List*>(e);
		if (params.size() != l->Size())
			return false;
		if (params.empty())
			return true;
		for (int i=0;i<params.size();++i)
			if (!(params[i]->Equal(l->params[i])))
				return false;
		return true;
	}

	void Print()
	{
		for (int i=0;i<params.size()-1;++i)
		{
			params[i]->Print();
			cout << ", ";
		}
		if (!params.empty())
			params[params.size()-1]->Print();
	}

};

class Term: public Element
{
private:
	string name;
	vector<Element*> params;
	bool negation;
public:

	Term(string name, vector<Element*> params, bool negation)//конструктор предиката
	{
		this->name=name;
		this->params=params;
		this->negation=negation;
	}

	Term(string name, vector<Element*> params)//конструктор сколемовской функции
	{
		this->name=name;
		this->params=params;
		this->negation=false;
	}

	Term(Term* t, vector<Element*>& subs)//терм, полученный из терма подстановкой
	{
		this->name=t->name;
		this->params=t->params;
		this->negation=t->negation;
		for (int i=0;i<this->params.size();++i)
		{
			bool f = false;
			for (int j=0;j<subs.size();j+=2)
				if (this->params[i]->Equal(subs[j]))
				{
					this->params[i] = subs[j+1];
					f=true;
				}
			if (f)
				--i;
		}
	}

	int GetArity()
	{
		return params.size();
	}

	string GetName()
	{
		return name;
	}

	bool GetNegation()
	{
		return negation;
	}

	Element* GetList()
	{
		Element* res = new List(params);
		return res;
	}

	Element* GetValue()
	{
		Element* res = new Value(name);
		return res;
	}

	Type GetType()
	{
		return Type::TERM;
	};

	static bool IsTerm(Element* e)
	{
		return e->GetType()==Type::TERM;
	}

	bool Equal(Element* e)
	{
		if (e->GetType() != Type::TERM)
			return false;
		Term* t = dynamic_cast<Term*>(e);
		if (name.compare(t->GetName())!=0)
			return false;
		if (params.size() != t->GetArity())
			return false;
		if (params.empty())
			return true;
		for (int i=0;i<params.size();++i)
			if (!(params[i]->Equal(t->params[i])))
				return false;
		return true;
	}

	bool ContainsVariable(Element* var)
	{
		for (int i=0;i<params.size();++i)
			if (var->Equal(params[i]))
				return true;
		return false;
	}

	void Print()
	{
		if (negation)
			cout << "-";
		cout << name << "(";
		for (int i=0;i<params.size()-1;++i)
		{
			params[i]->Print();
			cout << ", ";
		}
		if (!params.empty())
			params[params.size()-1]->Print();
		cout << ")";
	}

};

#define FAIL new Element() //нужно для обозначения невозможности подстановки
#define FAIL_SIZE 1

Element* Subs(Element* var, vector<Element*>& subs) //если возможна подстановка, то вернуть значение, которое нужно подставить
{
	if (subs.empty() || subs.size()==FAIL_SIZE)
		return FAIL;
	for (int i=0;i<subs.size();i+=2)
		if (var->Equal(subs[i]))
			return subs[i+1];
	return FAIL;
}

bool OccurCheck(Element* var, Element* x) //Проверка на входимость переменной в элемент х
{
	if (Term::IsTerm(x))
		return dynamic_cast<Term*>(x)->ContainsVariable(var);
	else
		return false;

}

void Unify(Element* x, Element* y, vector<Element*>& subs);

void UnifyVar(Element* var, Element* x, vector<Element*>& subs) //алгоритм унификации переменной
{
	Element* val1 = Subs(var,subs);
	Element* val2 = Variable::IsVariable(x)? Subs(x,subs) : FAIL;
	if (val1->GetType() != Type::ELEMENT)
		Unify(val1, x, subs);
	else if (val2->GetType() != Type::ELEMENT)
		Unify(var, val2, subs);
	else if (OccurCheck(var,x))
	{
		subs.clear();
		subs.push_back(FAIL);
		return; //failure
	}
	else
	{
		subs.push_back(var);
		subs.push_back(x);
	}

}

void Unify(Element* x, Element* y, vector<Element*>& subs) //алгоритм унификации действет путем поэлементного сравнения структур входных высказываний
{
	if (subs.size()==FAIL_SIZE)
		return; //failure
	else if (x->Equal(y))
		return; //subs
	else if (Variable::IsVariable(x))
		UnifyVar(x,y,subs);
	else if (Variable::IsVariable(y))
		UnifyVar(y,x,subs);
	else if (Term::IsTerm(x) && Term::IsTerm(y))
	{
		Unify(dynamic_cast<Term*>(x)->GetValue(),dynamic_cast<Term*>(y)->GetValue(),subs);
		Unify(dynamic_cast<Term*>(x)->GetList(),dynamic_cast<Term*>(y)->GetList(), subs);
	}
	else if (List::IsList(x) && List::IsList(y))
	{
		Unify(dynamic_cast<List*>(x)->First(),dynamic_cast<List*>(y)->First(),subs);
		Unify(dynamic_cast<List*>(x)->Rest(),dynamic_cast<List*>(y)->Rest(), subs);
	}
	else
	{
		subs.clear();
		subs.push_back(FAIL);
		return; //failure
	}

}

class Conjunct
{
public:
	list<Term*> predicats;
	bool contradictory; //противоречивость

	Conjunct()
	{
		contradictory=false;
	}

	Conjunct(Term* t)
	{
		contradictory=false;
		predicats.push_back(t);
	}

	Conjunct(Conjunct* c, int h, vector<Element*> subs)//конструктор факторизации с исключением дублирующегося элемента h
	{
		contradictory=false;
		for (int i=0;i<h;++i)
		{
			Term* t = new Term((*c)[i],subs);
			this->Add(t);
		}
		for (int i=h+1;i<c->Size();++i)
		{
			Term* t = new Term((*c)[i],subs);
			this->Add(t);
		}
	}

	Conjunct(Conjunct* c1, Conjunct* c2, int k, int l, vector<Element*> subs) //конструктор слияния двух конъюнктов
	{
		contradictory=false;
		for (int i=0;i<k;++i)
		{
			Term* t = new Term((*c1)[i],subs);
			this->Add(t);
		}
		for (int i=k+1;i<c1->Size();++i)
		{
			Term* t = new Term((*c1)[i],subs);
			this->Add(t);
		}
		for (int i=0;i<l;++i)
		{
			Term* t = new Term((*c2)[i],subs);
			this->Add(t);
		}
		for (int i=l+1;i<c2->Size();++i)
		{
			Term* t = new Term((*c2)[i],subs);
			this->Add(t);
		}
	}

	void Add(Term* t)
	{
		for (list<Term*>::iterator it=predicats.begin();it!=predicats.end();++it)
			if ((*it)->GetName().compare(t->GetName())==0)
				if ((*it)->GetNegation()!=t->GetNegation()) // обработка контрарных предикатов
				{
					vector<Element*> subs;
					Unify(*it,t,subs);
					if (subs.size()!=FAIL_SIZE)
						contradictory=true; //противоричивый конъюнкт
				}
				else
					if ((*it)->Equal(t)) //обработка добавления одинаковых предикатов
						return;
		predicats.push_back(t);
	}

	int Size()
	{
		return predicats.size();
	}

	bool Empty()
	{
		return (predicats.size()==0);
	}

	bool Contradictory()
	{
		return contradictory;
	}

	Term* operator[](int i)
	{
		if (i<0)
			return predicats.front();
		int j=0;
		for (list<Term*>::iterator it=predicats.begin();it!=predicats.end();++it)
			if (i==j)
				return *it;
			else
				++j;
		return predicats.back();
	}

	void Print()
	{
		if (predicats.empty())
		{
			cout << "Empty";
			return;
		}
		for (list<Term*>::iterator it=predicats.begin();it!=(--predicats.end());++it)
		{
			(*it)->Print();
			cout << " || ";
		}
		predicats.back()->Print();
	}

};

bool Solution(vector<Conjunct*> kb)
{
	for (int i=0;i<kb.size();++i)
	{
		//факторизация конъюнктов
		for (int s=0;s<kb[i]->Size();++s)
			for (int h=s+1;h<kb[i]->Size();++h)
				if ((*kb[i])[s]->GetName().compare((*kb[i])[h]->GetName())==0 && (*kb[i])[s]->GetNegation()==(*kb[i])[h]->GetNegation())
				{
					vector<Element*> subs;
					Unify((*kb[i])[s],(*kb[i])[h],subs);
					if (subs.size()!=FAIL_SIZE)
					{
						Conjunct* c_new = new Conjunct(kb[i],h,subs);
						if (!c_new->Contradictory())
						{
							///*
							cout << "Knowledge Base:" << endl;
							for (int z=0;z<kb.size();++z)
							{
								kb[z]->Print();
								cout << endl;
							}
							cout << endl;
							cout << "Conjunct: ";
							(*kb[i]).Print();
							cout << endl << "Substitution: " << endl;
							for (int z=0;z<subs.size();z+=2)
							{
								subs[z]->Print();
								cout << "->";
								subs[z+1]->Print();
								cout << endl;
							}
							cout << "New conjunct: ";
							c_new->Print();
							cout << endl << endl << endl << endl;
							system("pause");
							//*/
							kb.push_back(c_new);
						}
					}
				}
		//склейка конъюнктов
		for (int j=i-1;j>=0;--j)
		{
			if (j<0)
				continue;

			for (int k=0;k<kb[i]->Size();++k)
				for (int l=0;l<kb[j]->Size();++l)
					if ((*kb[i])[k]->GetName().compare((*kb[j])[l]->GetName())==0 && (*kb[i])[k]->GetNegation()!=(*kb[j])[l]->GetNegation())
					{
						vector<Element*> subs;
						Unify((*kb[i])[k],(*kb[j])[l],subs);
						if (subs.size()!=FAIL_SIZE)
						{
							Conjunct* c_new = new Conjunct(kb[i],kb[j],k,l,subs);
							if (!c_new->Contradictory())
							{
								///*
								cout << "Knowledge Base:" << endl;
								for (int z=0;z<kb.size();++z)
								{
									kb[z]->Print();
									cout << endl;
								}
								cout << endl;
								cout << "Conjunct 1: ";
								(*kb[i]).Print();
								cout << endl << "Conjunct 2: ";
								(*kb[j]).Print();
								cout << endl << "Gluing by: ";
								(*kb[i])[k]->Print();
								cout << "      ";
								(*kb[j])[l]->Print();
								cout << endl << "Substitution: " << endl;
								for (int z=0;z<subs.size();z+=2)
								{
									subs[z]->Print();
									cout << "->";
									subs[z+1]->Print();
									cout << endl;
								}
								cout << "New conjunct: ";
								c_new->Print();
								cout << endl << endl << endl << endl;
								system("pause");
								//*/
								kb.push_back(c_new);
								if (c_new->Empty())
									return true;
							}
						}
					}
		}
	}
	return false;
}

#endif