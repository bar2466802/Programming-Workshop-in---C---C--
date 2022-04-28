//
// Created by BarM on 8/5/2020.
//
#include <iostream>
#ifndef VLVECTOR_VLVECTOR_HPP
#define VLVECTOR_VLVECTOR_HPP

#define DEF_CAPACITY 16
#define EMPTY 0
#define ZERO 0
#define  CALC_CAP_C(s)  ((3 * (s + 1)) / 2)

using std::distance;

template <typename T, size_t N = DEF_CAPACITY>
class VLVector
{
public:
	typedef T   valueType;
	typedef T   &refType;
	typedef size_t   sizeType;

	class iterator;
	class const_iterator;
	/**
	 * Default constructor.
	 */
	VLVector() :
				_onStack(true),
				_staticCapacity(N),
				 _size(EMPTY),
				 _capacity(N)
	{
		static valueType arr[N];
		_arrayStack = arr;
		_array = _arrayStack;
		_arrayHeap = nullptr;
	}
	/**
	 * Copy constructor.
	 * @param other - VLVector to copy info from
	 */
	VLVector(const VLVector<T, N> &other): _onStack(other._onStack),
									 _staticCapacity(N),
									 _size(other.size()),
								     _capacity(other.capacity())
	{
		static valueType arr[N];
		_arrayStack = arr;
		if (_onStack)
		{
			_arrayHeap = nullptr;
			_array = _arrayStack;
		}
		else
		{
			_arrayHeap = new valueType[_capacity];
			_array = _arrayHeap;
		}

		for (size_t i = 0; i < other.size(); i++)
		{
			_array[i] = other[i];
		}
	}
	/**
	 *  constructor-1.
	 * @param first - iterator to the first element to copy
	 * @param last - iterator to the last element to copy
	 */
	template <class InputIterator>
	VLVector(InputIterator first, InputIterator last): _staticCapacity(DEF_CAPACITY)
	{
		_size = distance(first,last); //get the needed size, meaning dis btw elements
		_capacity = _size;
		_arrayHeap = nullptr;
		static valueType arr[DEF_CAPACITY];
		_arrayStack = arr;
		_array = _arrayStack;
		_onStack = true;
		for (size_t i = 0; i < _size; i++)
		{
			_array[i] = *first;
			++first;
		}
	}
	/**
	 *  destructor
	 */
	~VLVector()
	{
		//Call current data-type destructor for each cell in vector
//		for (size_t i = 0; i < _size; i++)
//		{
//			*this[i].~T();
//		}

		if (_onStack == false) //check if we need to delete memory from heap
		{
			delete[] _arrayHeap;
		}
	}

	/**
	 *  Get current size of vector
	 *  @return the current size of vector
	 */
	size_t size() const
	{
		return _size;
	}

	/**
	 *  Get current capacity of vector
	 *  @return the current capacity of vector
	 */
	size_t capacity() const
	{
		return _capacity;
	}

	/**
	 *  Check if vector is empty or not
	 *  @return True of the vector is indeed empty, False otherwise
	 */
	bool empty() const
	{
		return _size == EMPTY;
	}
	/**
	 *  Get value at index
	 *  @param index - the given location in the vector to get the value from
	 *  @return value in the vector at the given index, checking boundaries
	 */
	valueType at(const size_t index)
	{
		if(index < ZERO || index >= _size) //check valid index
		{
			throw std::out_of_range ("Out of range");
		}
		return _array[index];
	}
	/**
	 *  Get value at index - const version
	 *  @param index - the given location in the vector to get the value from
	 *  @return value in the vector at the given index, checking boundaries
	 */
	valueType at(const size_t index) const
	{
		if(index < ZERO || index >= _size) //check valid index
		{
			throw std::out_of_range ("Out of range");
		}
		return _array[index];
	}

	/**
	 *  Get value at index
	 *  @param value - value to insert at the end of the vector
	 */
	void push_back(const valueType value)
	{
		size_t index = _size++;
		_checkCapacity();
		_array[index] = value;
	}

	/**
	 *  Get iterator to a element inside the vector and a value to add in front of it
	 *  @param it - iterator to the element to insert before
	 *  @param value - value to insert
	 *  @return iterator to the new element
	 */
	iterator insert(iterator &it, const valueType value)
	{
		_size++;
		_checkCapacity();
		iterator currIter = rend();
		iterator prevIter = rend()--;
		while (currIter != it)
		{
			*currIter = *prevIter;
			currIter--;
			prevIter--;
		}

		*it = value;
		return it;
	}

	/**
	 *  Get iterator to a element inside the vector and a range in other container to copy from
	 *  @param it - iterator to the element to insert before
	 *  @param first - iterator to the first element to copy
	 *  @param last - iterator to the last element to copy
	 *  @return iterator to the new element
	 */
	template <class InputIterator>
	iterator insert(iterator &it, InputIterator first, InputIterator last)
	{
		size_t numOfNewElements = distance(first,last);
//		iterator last = iterator + numOfNewElements;
		_size = _size + numOfNewElements; //increase size with the number of new elements
		_checkCapacity();
		iterator currIter = rend();
		iterator prevIter = currIter - numOfNewElements;
		while (currIter != prevIter)
		{
			*currIter = *prevIter;
			currIter--;
			*prevIter = *first;
			prevIter++;
			first++;
		}

		return it;
	}

	/**
	 *  Remove last element from vector
	 */
	void pop_back()
	{
//		*this[_size - 1].~T(); //call destructor of current data type
		_size--;
		_checkCapacity();
	}

	/**
	 *  remove element from the vector on the given place
	 *  @param it - iterator to the element to remove
	 *  @return iterator to the element on the right of the removed one
	 */
	iterator erase(const iterator it)
	{
		iterator currIter = it;
		currIter++;
		iterator prevIter = it;
//		*it.~T();
		//Move all the elements from the given position till the end one cell back
		while (prevIter != end())
		{
			*prevIter = *currIter;
			currIter++;
			prevIter++;
		}
		//Decrease the vector size
		_size--;
		_checkCapacity();
		return it;
	}

	/**
	 *  remove element from the vector on the given places
	 *  @param first - iterator to the first element to be removed
	 *  @param last - iterator to the last element to be removed
	 *  @return iterator to the element on the right of last element to be removed
	 */
	iterator erase(const iterator first, const iterator last)
	{
		size_t numOfNewElements = distance(first,last);
		iterator currIter = first + numOfNewElements;
		iterator prevIter = first;
		iterator end = end() - numOfNewElements;
		//Move all the elements from the given position till the end one cell back
		while (prevIter != end)
		{
//			*prevIter.~T();
			*prevIter = *currIter;
			currIter++;
			prevIter++;
		}
		//Decrease the vector size
		_size = _size - numOfNewElements;
		_checkCapacity();
		iterator returnIt = first;
		return returnIt;
	}
	/**
	 *  remove all the elements from the vector
	 */
	void clear()
	{
//		for(size_t i = 0; i < _size; i++)
//		{
////			*this[i].~T();
//		}
		_size = EMPTY;
		_checkCapacity();
	}

	/**
	 *  Get pointer to the array representing our vector
	 *  @return pointer to the array representing our vector
	 */
	valueType* data()
	{
		return _array;
	}

	/**
	 *  Get pointer to the array representing our vector
	 *  @return pointer to the array representing our vector
	 */
	valueType* data() const
	{
		return _array;
	}

	/**
	 *  redefine [] operator
	 *  @param index - given index to get value from
	 *  @return value in given index
	 */
	T& operator[](size_t index)
	{
		return _array[index];
	}

	/**
	 *  redefine [] operator - const version
	 *  @param index - given index to get value from
	 *  @return value in given index
	 */
	T operator[] (size_t index) const
	{
		return _array[index];
	}
	/**
	 *  redefine = operator
	 *  @param rhs - vector to copy from
	 *  @return the current vector after the copy
	 */
	VLVector& operator=(VLVector const &rhs)
	{
		if (this == &rhs)
		{
			return *this;
		}

		//Call current data-type destructor for each cell in vector
//		for (size_t i = 0; i < _size; i++)
//		{
//			*this[i].~T();
//		}

		if (_onStack == false) //check if we need to delete memory from heap
		{
			delete[] _arrayHeap;
		}

		_onStack = rhs._onStack;
		_staticCapacity = rhs._staticCapacity;
		_size = rhs.size();
		_capacity = rhs.capacity();

		static valueType* arr[_staticCapacity];
		_arrayStack = arr;

		if (_onStack)
		{
			_arrayHeap = nullptr;
			_array = _arrayStack;
		}
		else
		{
			_arrayHeap = new valueType[_capacity];
			_array = _arrayHeap;
		}

		for (size_t i = 0; i < rhs.size(); i++)
		{
			_array[i] = rhs[i];
		}

		return *this;
	}

//	/**
//	 *  redefine == operator
//	 *  @param rhs - vector to compare with
//	 *  @return true if the 2 vectors eq on all fields, false otherwise
//	 */
//	bool operator==(const VLVector &rhs) const
//	{
//		if(_size == rhs.size())
//		{
//			for (size_t i = 0; i < rhs.size(); i++)
//			{
//				if(_array[i] != rhs[i])
//				{
//					return false;
//				}
//			}
//			return true;
//		}
//		return false;
//	}

	friend bool operator==(const VLVector& v1, const VLVector& v2)
	{
		if(v1.size() == v2.size())
		{
			for (size_t i = 0; i < v2.size(); i++)
			{
				if(v1[i] != v2[i])
				{
					return false;
				}
			}
			return true;
		}
		return false;
	}

	/**
	 *  redefine != operator
	 *  @param rhs - vector to compare with
	 *  @return true if the 2 vectors eq on all fields, false otherwise
	 */
	bool operator!=(VLVector const &rhs) const
	{
		return !this->operator==(&rhs);
	}

	iterator begin()
	{
		return iterator(&_array[ZERO]);
	}

	const_iterator begin() const
	{
		return const_iterator(&_array[ZERO]);
	}

	iterator end()
	{
		return iterator(&_array[_size]);
	}

	iterator rend() const
	{
		return iterator(&_array[_size - 1]);
	}

	const_iterator end() const
	{
		return const_iterator(&_array[_size]);
	}

	const_iterator cbegin() const
	{
		return const_iterator(&_array[ZERO]);
	}

	const_iterator cend() const
	{
		return const_iterator(&_array[_size]);
	}

	const_iterator crend() const
	{
		return const_iterator(&_array[_size - 1]);
	}

	class iterator
	{
	private:
		valueType* _vector;
//		sizeType _index;
//		sizeType _index;
	public:
		typedef T value_type;
		typedef T &reference;
		typedef T *pointer;
		typedef T difference_type;
		typedef std::random_access_iterator_tag iterator_category;

		iterator() : _vector(nullptr)
		{

		}

//		iterator(VLVector<T, N> *v, sizeType pos) : _vector(v->data()), _index(pos)
//		{
//
//		}

//		explicit iterator(VLVector<T, N> *v) : _vector(v->data()), _index(ZERO)
//		{
//
//		}

		explicit iterator(pointer p) : _vector(p)
		{

		}

		iterator(reference p, sizeType pos) : _vector(*p)
		{
			_vector + pos;
		}

		explicit iterator(reference p) : _vector(*p)
		{

		}

		iterator(pointer p, sizeType pos) : _vector(p)
		{

		}

		valueType operator*() const
		{
			return *_vector;
		}

		reference &operator*()
		{
			return *_vector;
		}

		pointer operator->()
		{
			return _vector;
		}

		pointer operator->() const
		{
			return _vector;
		}

		reference operator[](difference_type off) const
		{
			return _vector[off];
		}

		iterator &operator++()
		{
			++_vector;
			return *this;
		}

		iterator operator++(int)
		{
			iterator temp(*this);
			++_vector;
			return temp;
		}

		iterator &operator--()
		{
			_vector--;
			return *this;
		}

		iterator operator--(int)
		{
			iterator temp(*this);
			_vector--;
			return temp;
		}

		bool operator ==(const iterator &other) const
		{
			return _vector ==  other._vector;
		}

//		bool operator !=(const iterator &other) const
//		{
//			return _vector[_index] != other._vector[other._index];
//		}

		friend bool operator!=(const iterator& it1, const iterator& it2)
		{
			return it1._vector != it2._vector;
		}

		bool operator >(const iterator &other) const
		{
			return _vector > other._vector;
		}

		bool operator <(const iterator &other) const
		{
			return _vector < other._vector;
		}

		bool operator >=(const iterator &other) const
		{
			return _vector >= other._vector;
		}

		bool operator <=(const iterator &other) const
		{
			return _vector <= other._vector;
		}

		iterator& operator+=(difference_type off)
		{
			_vector += off;
			return *this;
		}

		iterator operator+(difference_type off) const
		{
			return iterator(this->_vector + off);
		}

		friend iterator operator+(difference_type off, const iterator& right)
		{
			return iterator(right._vector+ off);
		}

		iterator& operator-=(difference_type off)
		{
			_vector -= off;
			return *this;
		}

		iterator operator-(difference_type off) const
		{
			return iterator(_vector - off);
		}

		difference_type operator-(const const_iterator& rhs) const
		{
			return Iterator(this->_vector - rhs->_vector);
		}
	};

	class const_iterator
	{
	private:
		const valueType* _vector;
	public:
		typedef T value_type;
		typedef const T &reference;
		typedef const T *pointer;
		typedef T difference_type;
		typedef std::random_access_iterator_tag iterator_category;

		const_iterator() : _vector(nullptr)
		{

		}

		explicit const_iterator(pointer p)
		{
			_vector = p;
		}

		const_iterator(reference p, sizeType pos)
		{
			_vector = &p;
			_vector+pos;
		}

		explicit const_iterator(reference p) : _vector(p)
		{

		}

		const_iterator(pointer p, sizeType pos) : _vector(p)
		{

		}

		valueType operator*() const
		{
			return *_vector;
		}

		pointer operator->() const
		{
			return *_vector;
		}

		reference operator[](difference_type off) const
		{
			return _vector[off];
		}

		const_iterator &operator++()
		{
			_vector++;
			return *this;
		}

		const_iterator &operator++(int)
		{
			const_iterator* temp = this;
			_vector++;
			return &temp;
		}

		const_iterator &operator--()
		{
			_vector--;
			return *this;
		}

		const_iterator &operator--(int)
		{
			const_iterator* temp = this;
			_vector--;
			return temp;
		}

		bool operator ==(const const_iterator &other) const
		{
			return _vector == other._vector;
		}

		bool operator !=(const const_iterator &other) const
		{
			return _vector != other._vector;
		}

		bool operator >(const const_iterator &other) const
		{
			return this->_vector > other._vector;
		}

		bool operator <(const const_iterator &other) const
		{
			return this->_vector < other._vector;
		}

		bool operator >=(const const_iterator &other) const
		{
			return this->_vector >= other._vector;
		}

		bool operator <=(const const_iterator &other) const
		{
			return this->_vector <= other._vector;
		}

		const_iterator& operator+=(difference_type off)
		{
			_vector += off;
			return *this;
		}

		const_iterator operator+(difference_type off) const
		{
			return const_iterator(this->_vector + off);
		}

		friend const_iterator operator+(difference_type off, const const_iterator& right)
		{
			return const_iterator(right._vector + off);
		}

		const_iterator& operator-=(difference_type off)
		{
			_vector -= off;
			return *this;
		}

		const_iterator operator-(difference_type off) const
		{
			return const_iterator(this->_vector - off);
		}

		difference_type operator-(const const_iterator& rhs) const
		{
			return const_iterator(this->_vector - rhs._vector);
		}
	};

private:
	/**
	 * Flag if we are currently using the stack or thr heap
	 * @property onStack
	 */
	bool _onStack;
	/**
	 * Capacity of vector on the stack
	 * @property staticCapacity
	 */
	 const size_t _staticCapacity;
	/**
	* The actual size of the vector
	* @property size
	*/
	size_t _size;
	/**
	 * Actual current Capacity of vector
	 * @property staticCapacity
	 */
	size_t _capacity;
	/**
	 * Array on Stack memory
	 * @property _arrayStack
	 */
	valueType* _arrayStack;
	/**
	 * Array on Heap memory
	 * @property _arrayHeap
	 */
	valueType* _arrayHeap;

	/**
	 * Current array
	 * @property _arrayHeap
	 */
	valueType* _array;

	/**
	 * Clac the new capacity on the heap
	 */
	void _calcNewCapacity()
	{
		_capacity =  CALC_CAP_C(_size);
	}

	/**
	 * Check if we need to switch btw heap & stack or vice versa
	 */
	void _checkCapacity()
	{
		if(_size > _staticCapacity)
		{
			_calcNewCapacity();
			_arrayHeap = new valueType[_capacity];
			if (_onStack == true)
			{
				for (size_t i = 0; i < _staticCapacity; i++)
				{
					_arrayHeap[i] = _arrayStack[i];
				}
			}
			_onStack = false;
			_array = _arrayHeap;

		}
		else if (_onStack == false && _size <= _staticCapacity)
		{
			for (size_t i = 0; i < _staticCapacity; i++)
			{
				_arrayStack[i] = _arrayHeap[i];
			}

//			for (size_t i = 0; i < _size; i++)
//			{
//				_arrayHeap[i].~T();
//			}

			delete[] _arrayHeap;
			_onStack = true;
			_arrayHeap = nullptr;
			_array = _arrayStack;
			_capacity = _staticCapacity;
		}
	}
};


#endif //VLVECTOR_VLVECTOR_HPP
