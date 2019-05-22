#include <iostream>
#include <string>
#include <vector>

using namespace std;


class Person {
public:
    Person(const string& name) : Name(name) {}
    virtual string Type() const {
        return "Person";
    }
    void Introduce() const {
        cout << Type() << ": " << Name;
    }
    virtual void Walk(const string& destination) const {
        Introduce();
        cout << " walks to: " << destination << endl;
    }
    const string Name;
};


class Student : public Person {
public:
    Student(const string& name, const string &favouriteSong) : Person(name), FavouriteSong(favouriteSong) {}

    string Type() const override {
        return "Student";
    }

    void Learn() const {
        Introduce();
        cout << " learns" << endl;
    }

    void SingSong() const {
        Introduce();
        cout << " sings a song: " << FavouriteSong << endl;
    }

    void Walk(const string& destination) const override {
        Person::Walk(destination);
        SingSong();
    }

    const string FavouriteSong;
};


class Teacher : public Person {
public:

    Teacher(const string& name, const string& subject): Person(name), Subject(subject) {}

    string Type() const override {
        return "Teacher";
    }
    void Teach() const {
        Introduce();
        cout << " teaches: " << Subject << endl;
    }

    const string Subject;
};


class Policeman : public Person {
public:
    Policeman(const string& name) : Person(name) {}

    string Type() const override {
        return "Policeman";
    }
    void Check(const Person &p) const {
        Introduce();
        cout << " checks " << p.Type() << ". " << p.Type() << "'s name is: " << p.Name << endl;
    }
};


void VisitPlaces(Person& pr, const vector<string>& places) {
    for (const auto& p : places) {
        pr.Walk(p);
    }
}


int main() {
    Teacher t("Jim", "Math");
    Student s("Ann", "We will rock you");
    Policeman p("Bob");

    VisitPlaces(t, {"Moscow", "London"});
    p.Check(s);
    VisitPlaces(s, {"Moscow", "London"});

	getchar();
	getchar();

    return 0;
}
