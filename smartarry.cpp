#include <iostream>
#include <string>
#include <cstdlib>
#include <ctime>
using namespace std;

template <class T>
class SmarterArray
{
private:
	T* A;
	int size;

public:
	//Constructors
	SmarterArray();
	SmarterArray(const T* A, const int& size);
	SmarterArray(const SmarterArray<T>& L); //Copy constructor

	//Assignment operator
	SmarterArray<T>& operator = (const SmarterArray<T>& L);

	//Destructor
	~SmarterArray();

	//Getters, Setters, operators and other functions
	int getSize() const;
	T& operator[] (const int& index) const;
	bool& operator == (const SmarterArray<T>& L);
	int find(const T& e) const;
	void append(const T& e);
	bool remove(const T& e);
	SmarterArray<T> operator - (const SmarterArray<T>& L) const;
	SmarterArray<T> operator + (const SmarterArray<T>& L) const;

	//Friend functions need to be explicitely show as templated even inside the class declaration
	template<class T>
	friend ostream& operator << (ostream& outputStream, const SmarterArray<T>& L);
};

template <class T>
SmarterArray<T>::SmarterArray()
{
	this->size = 0;
}

template <class T>
SmarterArray<T>::SmarterArray(const T* A, const int& size)
{
	this->size = size;
	if (this->getSize() > 0)
	{
		this->A = new T[this->getSize()];
		for (int i = 0; i < this->getSize(); ++i)
			this->A[i] = A[i];
	}
}

template <class T>
SmarterArray<T>::SmarterArray(const SmarterArray<T>& L) //Copy constructor
{
	this->size = L.getSize();
	if (this->getSize() > 0)
	{
		this->A = new T[this->getSize()];
		for (int i = 0; i < this->getSize(); ++i)
			this->A[i] = L[i];
	}
}

template <class T>
SmarterArray<T>& SmarterArray<T> :: operator = (const SmarterArray<T>& L)
{
	//Check for self assginment. If so, do nothing.
	if (this == &L)
		return *this;
	//Delete the left hand side object's memory
	this->~SmarterArray();
	//Now copy the right hand side to the left
	this->size = L.getSize();
	if (this->getSize() > 0)
	{
		this->A = new T[this->getSize()];
		for (int i = 0; i < this->getSize(); ++i)
			this->A[i] = L[i];
	}
	return *this;
}

template <class T>
SmarterArray<T>::~SmarterArray()
{
	if (this->getSize() > 0)
	{
		delete[] this->A;
		this->size = 0;
	}
}

template <class T>
int SmarterArray<T>::getSize() const
{
	return this->size;
}

template <class T>
T& SmarterArray<T>::operator[](const int& index) const
{
	if (index >= 0 && index < this->getSize())
		return this->A[index];
	else
	{
		cout << "ERROR! Index out of bounds." << endl;
		system("exit");
		return this->A[index];
	}
}

template <class T>
int SmarterArray<T>::find(const T& e) const
{
	for (int i = 0; i < this->getSize(); ++i)
		if (this->A[i] == e)
			return i;	//return index if found
	return -1;	//return -1 to mean not found
}

template <class T>
void SmarterArray<T>::append(const T& e)
{
	//First create a temporary array whose size is this->size+1
	T* temp = new T[this->getSize() + 1];

	//Copy the elements of this->A to temp
	for (int i = 0; i < this->getSize(); ++i)
		temp[i] = this->A[i];

	//Copy the element to be appended to the temp
	temp[this->getSize()] = e;

	//Delete the existing array this->A
	if (this->getSize() > 0)
		delete[] this->A;

	//Make the array this->A to point to temp and increment the size
	this->A = temp;
	++this->size;
}

template <class T>
bool SmarterArray<T>::remove(const T& e)
{
	int index = this->find(e);
	if (index == -1)
		return false;
	else
	{
		//First create a temporary array whose size is this->size-1
		T* temp = new T[this->getSize() - 1];

		//Copy the elements of this->A to temp except the element at index
		for (int i = 0; i < index; ++i)
			temp[i] = this->A[i];
		for (int i = index + 1; i < this->getSize(); ++i)
			temp[i - 1] = this->A[i];
	}

	//Delete the existing array this->A
	if (this->getSize() > 0)
		delete[] this->A;

	//Make the array this->A to point to temp and decrement the size
	this->A = temp;
	--this->size;
	return true;
}

template <class T>
bool& SmarterArray<T>::operator == (const SmarterArray<T>& L)
{
	if (L.getSize() == this->getSize())
	{
		for (int i = 0; i < L.getSize(); ++i)
			return (this->A[i] == L[i]) ? continue : false;
		return true;
	}
	else
		return false;
}

template <class T>
SmarterArray<T> SmarterArray<T>::operator - (const SmarterArray<T>& L) const
{
	//Returns the elements of this that are not found in L
	SmarterArray<T> A;
	for (int i = 0; i < this->getSize(); ++i)
	{
		int index = L.find(this->A[i]);
		if (index == -1)
			A.append(this->A[i]);
	}
	return A;
}

template <class T>
ostream& operator << (ostream& outputStream, const SmarterArray<T>& L)
{
	outputStream << '[';
	for (int i = 0; i < L.getSize() - 1; ++i)
		outputStream << L[i] << ", ";
	if (L.getSize() > 0)
		outputStream << L[L.getSize() - 1];
	outputStream << ']';
	return outputStream;
}

int main()
{
	//Declare several SmarterArray objects
	SmarterArray<int> A1;					//Default array of integers
	double x[3] = { 2.4, 1.2, 5.8 };
	SmarterArray<double> A2(x, 3);			//Non-default array of doubles
	SmarterArray<string> A3;				//Default array of strings

	//Populate the SmarterArray objects
	srand(time(0));
	for (int i = 0; i < 10; ++i)
	{
		if (rand() % 2 == 0)
			A1.append(rand() % 21 + 5);
		else
		{
			int random = rand() % 5;
			A3.append(random == 0 ? "Paul" :
				(random == 1 ? "Jannet" :
				(random == 2 ? "Kevin" :
					(random == 3 ? "Sara" : "CMPT"))));
		}
	}

	//Print the objects
	cout << "The SmarterArray object A1 is " << A1 << endl;
	cout << "The SmarterArray object A2 is " << A2 << endl;
	cout << "The SmarterArray object A3 is " << A3 << endl;

	system("pause");
	return 0;
}
