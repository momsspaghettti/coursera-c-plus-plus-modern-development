#include <iostream>
#include <string>
#include <vector>


using namespace std;


class Person
{
public:

	Person(const string& name) : Name(name){}

	virtual string Profession() const = 0;

	string ProfessionalName() const
	{
		return Profession() + ": " + Name;
	}

	virtual void Walk(const string& destination) const
	{
		cout << ProfessionalName() <<
			" walks to: " << destination << endl;
	}

	const string Name;
};


class Student : public Person
{
public:

	Student(const string& name, const string& favouriteSong) : Person(name),
    FavouriteSong(favouriteSong){}

    string Profession() const override
	{
		return "Student";
	}

	void Learn() const
	{
		cout << ProfessionalName() <<
			" learns" << endl;
	}

	void Walk(const string& destination) const override
	{
		Person::Walk(destination);
		SingSong();
	}

	void SingSong() const
	{
		cout << ProfessionalName() << 
			" sings a song: " << FavouriteSong << endl;
	}

private:
	const string FavouriteSong;
};


class Teacher : public Person
{
public:

	Teacher(const string& name, const string& subject) : 
    Person(name), Subject(subject){}

    string Profession() const override
	{
		return "Teacher";
	}

	void Teach() const
	{
		cout << ProfessionalName() << 
			" teaches: " << Subject << endl;
	}

private:
	const string Subject;
};


class Policeman : public Person
{
public:

	Policeman(const string& name) : Person(name){}

    string Profession() const override
	{
		return "Policeman";
	}

    void Check(Person& p) const
	{
		cout << ProfessionalName() << " checks " << p.Profession() <<
			". " << p.Profession() << "'s name is: " << p.Name << endl;
	}
};


void VisitPlaces(Person& person, const vector<string>& places) 
{
	for (auto p : places) 
	{
		person.Walk(p);
	}
}


int main() 
{
	Teacher t("Jim", "Math");
	Student s("Ann", "We will rock you");
	Policeman p("Bob");

	VisitPlaces(t, { "Moscow", "London" });
	p.Check(s);
	VisitPlaces(s, { "Moscow", "London" });

	return 0;
}