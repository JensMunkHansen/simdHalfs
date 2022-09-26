#ifndef AS_SCRIPT_ARRAY_H
#define AS_SCRIPT_ARRAY_H

#include <string>
#include <memory.h>
#include <assert.h>
#include "include/AngelScript.h"

//---------------------------------------------------------------------------------------------------------------------
//	
//---------------------------------------------------------------------------------------------------------------------

template <typename T> bool gRegisterArray(asIScriptEngine * outEngine, const char * inTypeName)
{
	int result;
	std::string type_name(inTypeName);
	std::string array_name(inTypeName); array_name+="Array";

	// register array-type
	result = outEngine->RegisterObjectType(array_name.c_str(), sizeof(ASArray<T>()), asOBJ_GUESS); assert(result>=0);

	// Behaviours
	result = outEngine->RegisterTypeBehaviour(array_name.c_str(),	asBEHAVE_CONSTRUCT,		"void Construct()", asFUNCTION(ASArray<T>::ASConstruct), asCALL_CDECL_OBJLAST); assert(result>=0);
	result = outEngine->RegisterTypeBehaviour(array_name.c_str(),	asBEHAVE_ASSIGNMENT,	(array_name + " &Assign(" + array_name + "&)").c_str(), asFUNCTION(ASArray<T>::ASAssign), asCALL_CDECL_OBJLAST); assert(result>=0);
	
	result = outEngine->RegisterTypeBehaviour(0,					asBEHAVE_ADD,			(array_name + " f(const " + array_name + "&, const " + array_name + "&)").c_str(), asFUNCTION(ASArray<T>::ASOperatorAddArray), asCALL_CDECL); assert(result>=0);
	result = outEngine->RegisterTypeBehaviour(0,					asBEHAVE_ADD,			(array_name + " f(const " + array_name + "&, const " + type_name + "&)").c_str(), asFUNCTION(ASArray<T>::ASOperatorAddElement), asCALL_CDECL); assert(result>=0);

	result = outEngine->RegisterTypeBehaviour(0,					asBEHAVE_EQUAL,			(std::string("bool f(const ") + array_name + "&, const " + array_name + "&)").c_str(), asFUNCTION(ASArray<T>::ASOperatorEqual), asCALL_CDECL); assert(result>=0);
	result = outEngine->RegisterTypeBehaviour(0,					asBEHAVE_NOTEQUAL,		(std::string("bool f(const ") + array_name + "&, const " + array_name + "&)").c_str(), asFUNCTION(ASArray<T>::ASOperatorNotEqual), asCALL_CDECL); assert(result>=0);

	result = outEngine->RegisterTypeBehaviour(array_name.c_str(),	asBEHAVE_INDEX,			(type_name + std::string("& f(int)")).c_str(), asMETHOD(ASArray<T>, ASIndex), asCALL_THISCALL); assert(result>=0);

	// methods
	result = outEngine->RegisterObjectMethod(array_name.c_str(), "void SetLength(int)",											asMETHOD(ASArray<T>, SetLength), asCALL_THISCALL); assert(result>=0);
	result = outEngine->RegisterObjectMethod(array_name.c_str(), "int GetLength()",												asMETHOD(ASArray<T>, GetLength), asCALL_THISCALL); assert(result>=0);
	
	result = outEngine->RegisterObjectMethod(array_name.c_str(), (std::string("void Add(") + type_name + " &)").c_str(),		asMETHOD(ASArray<T>, ASAddElement), asCALL_THISCALL); assert(result>=0);
	result = outEngine->RegisterObjectMethod(array_name.c_str(), (std::string("void Add(") + array_name + " &)").c_str(),		asMETHOD(ASArray<T>, ASAddArray), asCALL_THISCALL); assert(result>=0);

	result = outEngine->RegisterObjectMethod(array_name.c_str(), "void Remove(int)",											asMETHOD(ASArray<T>, ASRemove), asCALL_THISCALL); assert(result>=0);
	return (result >= 0);
}


template <class T> class ASArray
{
public:
	ASArray();
	ASArray(const ASArray<T> &);
	~ASArray();

	void Allocate(int numElements, bool keepData);
	int  GetCapacity() const;

	void PushLast(const T & element);
	T    PopLast();

	void Remove(int inIndex);

	void SetLength(int numElements);
	int  GetLength() const;

	void Copy(const T*, int count);
	ASArray<T> &operator =(const ASArray<T> &);

	T &operator [](int index) const;
	T *AddressOf();


	inline bool				operator == (const ASArray<T> & inRHS) const	{ return (length == inRHS.length) && (memcmp((void*)array, (void*)inRHS.array, length) == 0); }
	inline bool				operator != (const ASArray<T> & inRHS) const	{ return (length != inRHS.length) || (memcmp((void*)array, (void*)inRHS.array, length) != 0); }
	
	inline const ASArray<T>	operator + (const ASArray<T> & inRHS) const;
	inline const ASArray<T>	operator + (const T & inRHS) const;

	inline ASArray<T>&		operator += (const ASArray<T> & inRHS);



	static void	_cdecl		ASConstruct(ASArray<T> & inThis);
	static ASArray<T> & _cdecl	ASAssign(const ASArray<T> & inSource, ASArray<T> & inDest);
	
	static ASArray<T> _cdecl	ASOperatorAddArray(const ASArray<T> & inLHS, const ASArray<T> & inRHS);
	static ASArray<T> _cdecl	ASOperatorAddElement(const ASArray<T> & inLHS, const T & inRHS);

	static bool _cdecl		ASOperatorEqual(const ASArray<T> & inLHS, const ASArray<T> &inRHS);
	static bool	_cdecl		ASOperatorNotEqual(const ASArray<T> & inLHS, const ASArray<T> &inRHS);

	T*						ASIndex(int inIndex);

	void					ASAddElement(const T & inElement)		{ PushLast(inElement); }
	void					ASAddArray(const ASArray<T> & inArray);
	void					ASRemove(int inIndex);

protected:
	T   *array;
	int  length;
	int  maxLength;
};

// Implementation

template <class T>
T *ASArray<T>::AddressOf()
{
	return array;
}

template <class T>
ASArray<T>::ASArray(void)
{
	array     = 0;
	length    = 0;
	maxLength = 0;
}

template <class T>
ASArray<T>::ASArray(const ASArray<T> &copy)
{
	array     = 0;
	length    = 0;
	maxLength = 0;

	*this = copy;
}

template <class T>
ASArray<T>::~ASArray(void)
{
	if( array )
	{
		delete[] array;
		array = 0;
	}
}

template <class T>
int ASArray<T>::GetLength() const
{
	return length;
}

template <class T>
T &ASArray<T>::operator [](int index) const
{
	assert(index >= 0);
	assert(index < maxLength);

	return array[index];
}

template <class T>
void ASArray<T>::PushLast(const T & element)
{
	if( length == maxLength )
		Allocate(int(maxLength*1.5f) + 1, true);

	array[length++] = element;
}

template <class T>
T ASArray<T>::PopLast()
{
	assert(length > 0);

	return array[--length];
}

template <class T>
void ASArray<T>::Remove(int inIndex)
{
	assert(inIndex >= 0);
	assert(inIndex < maxLength);

	int num = (length - inIndex) - 1;
	if (num > 0)
		memmove((void*) &array[inIndex], (void*) &array[inIndex+1], num);
	length--;
}

template <class T>
void ASArray<T>::Allocate(int numElements, bool keepData)
{
	assert(numElements >= 0);

	T *tmp = new T[numElements];

	for (int i=0; i<numElements; i++)
		T::ASConstruct(tmp[i]);

	if( array )
	{
		if( keepData )
		{
			if( length > numElements )
				length = numElements;

			memcpy(tmp, array, length*sizeof(T));
		}
		else
			length = 0;

		delete[] array;
	}

	array = tmp;
	maxLength = numElements;
}

template <class T>
int ASArray<T>::GetCapacity() const
{
	return maxLength;
}

template <class T>
void ASArray<T>::SetLength(int numElements)
{
	assert(numElements >= 0);

	if( numElements > maxLength )
		Allocate(numElements, true);

	length = numElements;
}

template <class T>
void ASArray<T>::Copy(const T *data, int count)
{
	if( maxLength < count )
		Allocate(count, false);

	memcpy(array, data, count*sizeof(T));
}

template <class T>
ASArray<T> &ASArray<T>::operator =(const ASArray<T> &copy)
{
	Copy(copy.array, copy.length);

	return *this;
}


template <class T>
const ASArray<T> ASArray<T>::operator +(const ASArray<T> & inRHS) const
{
	ASArray<T> result;

	result.Allocate(length + inRHS.length, false);
	memcpy((void *)result.array, (void *)array, length * sizeof(T));
	memcpy((void *)(result.array+length), (void*)inRHS.array, inRHS.length * sizeof(T));
	return result;
}

template <class T>
const ASArray<T> ASArray<T>::operator +(const T & inRHS) const
{
	ASArray<T> result;
	result.Allocate(length + 1, false);
	memcpy((void *)result.array, (void *)array, length * sizeof(T));
	result[length] = inRHS;
	return result;
}

template <class T>
ASArray<T> &ASArray<T>::operator +=(const ASArray<T> & inRHS)
{
	int old_len = length;
	Allocate(old_len + inRHS.length, true);
	memcpy((void *)(array+old_len), (void*)inRHS.array, inRHS.length * sizeof(T));
	return *this;
}




template <class T>
void ASArray<T>::ASConstruct(ASArray<T> & inThis)
{
	inThis.array     = 0;
	inThis.length    = 0;
	inThis.maxLength = 0;
}

template <class T>
ASArray<T> & ASArray<T>::ASAssign(const ASArray<T> & inSource, ASArray<T> & inDest)
{
	inDest = inSource;
	return inDest;
}
	
template <class T>
ASArray<T>	ASArray<T>::ASOperatorAddArray(const ASArray<T> & inLHS, const ASArray<T> & inRHS)
{
	return inLHS + inRHS;
}

template <class T>
ASArray<T>	ASArray<T>::ASOperatorAddElement(const ASArray<T> & inLHS, const T & inRHS)
{
	return inLHS + inRHS;
}

template <class T>
bool ASArray<T>::ASOperatorEqual(const ASArray<T> & inLHS, const ASArray<T> &inRHS)
{
	return inLHS == inRHS;
}

template <class T>
bool ASArray<T>::ASOperatorNotEqual(const ASArray<T> & inLHS, const ASArray<T> &inRHS)
{
	return inLHS != inRHS;
}

template <class T>
T* ASArray<T>::ASIndex(int inIndex)
{
	if ((inIndex >= 0) && (inIndex < GetLength()))
		return &array[inIndex];

	// The script is trying to access memory that isn't 
	// allowed so we'll throw an exception. 
	asIScriptContext *context = asGetActiveContext();
 
	// Should this function be called by the host application directly
	// then there will not be any active context to set the exception on
	if( context )
		context->SetException("Array accessed out of range.");

	return NULL;
}



template <class T>
void ASArray<T>::ASRemove(int inIndex)
{
	if ((inIndex >= 0) && (inIndex < GetLength()))
		Remove(inIndex);
}


template <class T>
void ASArray<T>::ASAddArray(const ASArray<T> & inArray)
{
	(*this) += inArray;
}


#endif

