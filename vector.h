#ifndef VECTOR_H
#define VECTOR_H
#include<iostream>
#include<string>
#include<initializer_list>
#include<stdexcept>

using namespace std;




template<typename T = double>
class Vector {


public:
	class ConstIterator;
	class Iterator;
	using value_type = T;
	using size_type = std::size_t;
	using difference_type = std::ptrdiff_t;
	using reference = value_type&;
	using const_reference = const value_type&;
	using pointer = value_type*;
	using const_pointer = const value_type*;
	using iterator = Vector::Iterator;
	using const_iterator = Vector::ConstIterator;


	class Iterator {
	public:
		using value_type = Vector::value_type;
		using reference = Vector::reference;
		using pointer = Vector::pointer;
		using difference_type = Vector::difference_type;
		using iterator_category = std::forward_iterator_tag;
	private:

		pointer ptr;
		//Instanzvariablen
	public:
		//Methoden
		Iterator() : ptr{ nullptr } {}
		Iterator(pointer ptr) : ptr{ ptr } {}
		reference operator*() const { return *ptr; }
		pointer operator->() const { return ptr; }
		bool operator==(const const_iterator& ptra) const { return ptra == *this; }  
		operator const_iterator() const { return ptr; }           
		bool operator!=(const const_iterator& ptra) const { return ptra != *this; }
		iterator& operator++() { ++ptr; return *this; }    //
		iterator operator++(int) { pointer a{ ptr }; ptr++; return { a }; }


	};


	class ConstIterator {
	public:
		using value_type = Vector::value_type;
		using reference = Vector::const_reference;
		using pointer = Vector::const_pointer;
		using difference_type = Vector::difference_type;
		using iterator_category = std::forward_iterator_tag;
	private:
		//Instanzvariablen
		pointer ptr;

	public:
		//Methoden
		ConstIterator() : ptr{ nullptr } {}
		ConstIterator(pointer ptr) : ptr{ ptr } {}
		reference operator*() const { return *ptr; }
		pointer operator->() const { return ptr; }
		bool operator==(const const_iterator& ptra) const { return ptra.ptr == ptr; }  //

		bool operator!=(const const_iterator& ptra) const { return ptra.ptr != ptr; }
		const_iterator& operator++() { ++ptr; return *this; }
		const_iterator operator++(int) { pointer a{ ptr }; ptr++; return { a }; }

		friend Vector::difference_type operator-(const Vector::ConstIterator& lop,
			const Vector::ConstIterator& rop) {
			return lop.ptr - rop.ptr;
		}






	};



private:
	static constexpr size_t vector_sta_size{ 5 };


	size_t sz; 

	size_t max_sz;

	value_type* values;  // ist der pointer

	void fit(size_t neuerplatz) {
		if (neuerplatz <= sz) {
			return;
		}


		value_type* val_new{};  
		val_new = new value_type[neuerplatz];

		for (size_t i{ 0 }; i < sz; i++) {
			val_new[i] = values[i];
		}
		max_sz = neuerplatz;
		delete[] values;

		values = val_new;

	}



public:





	Vector(size_t big) : sz{ 0 }, max_sz{ big == 0 ? 1 : big }, values{ new value_type[max_sz] } {} 
	Vector(const initializer_list<value_type>& list) : Vector() {

		reserve(list.size());


		for (const auto& a : list) {
			this->push_back(a);
		}
	}
	Vector() : Vector(vector_sta_size) {}  //Vector default ohne big  ruft Vector(vector_st_size=10) falls mit big wird obererConstruktor aufgerufen.

	//Destruktor
	~Vector() { delete[] values; }
	
	Vector(const Vector& orginal) : Vector(orginal.max_sz) {
		sz = orginal.sz;

		for (size_t i{ 0 }; i < sz; i++) {
			values[i] = orginal.values[i];
		}

	}

	

	Vector& operator= (Vector rop) {
		swap(sz, rop.sz);
		swap(max_sz, rop.max_sz);
		swap(values, rop.values);


		return *this;
	}



	size_t size() const { return sz; }

	bool empty() const { return (sz == 0); }


	void reserve(size_t platz) {
		if (platz <= max_sz) {
			return;
		}

		fit(platz);

	}



	void push_back(value_type wert) {

		if (sz >= max_sz) {
			reserve(2 * max_sz);
		}







		values[sz] = wert; sz += 1;
	}

	void  pop_back() {
		if (empty()) {
			throw runtime_error("pop_back_fail");
		}
		sz -= 1;
	}

	value_type& operator[] (size_t index) {

		if (index >= sz) {
			throw runtime_error("operator_index_fail");
		}


		return values[index];
	}

	const value_type& operator[] (size_t index) const {

		if (index >= sz) {
			throw runtime_error("operator_index_fail");
		}


		return values[index];
	}


	bool equal(const Vector& lop) const {


		if (lop.size() != this->size()) {
			return false;
		}

		for (size_t i{ 0 }; i < lop.size(); i++) {

			if (lop.values[i] != this->values[i]) {
				return false;
			}
		}
		return true;
	}


	void clear() { sz = 0; }




	ostream& print(ostream& o) const {

		o << "[";

		bool first{ true };
		for (size_t i{ 0 }; i < sz; i++) {

			if (first) {
				o << values[i];
				first = false;
			}

			else {
				o << ", " << values[i];
			}
		}


		o << "]";

		return o;
	}

	size_t capacity()const { return max_sz; }

	void shrink_to_fit() {

		if (max_sz > sz) {
			fit(sz);
		}

	}

	iterator begin() { return Iterator{ values }; }

	iterator end() { return Iterator{ values + sz }; }

	const_iterator begin() const { return Iterator{ values }; }

	const_iterator end() const { return Iterator{ values + sz }; }

	// Aus der Folie kopiert
	iterator insert(const_iterator pos, const_reference val) {
		auto diff = pos - begin();
		if (diff<0 || static_cast<size_type>(diff)>sz)
			throw std::runtime_error("Iterator out of bounds");
		size_type current{ static_cast<size_type>(diff) };
		if (sz >= max_sz)
			reserve(max_sz * 2);
		for (size_t i{ sz }; i-- > current;)
			values[i + 1] = values[i];
		values[current] = val;
		++sz;
		return iterator{ values + current };
	}

	iterator erase(const_iterator pos) {
		auto diff = pos - begin();
		if (diff < 0 || static_cast<size_type>(diff) >= sz)
			throw std::runtime_error("Iterator_out_of_bounds");
		size_type current{ static_cast<size_type>(diff) };
		for (size_type i{ current }; i < sz - 1; ++i)
			values[i] = values[i + 1];
		--sz;
		return iterator{ values + current };
	}




	// Ende Kopiert aus den Folien







};// ENDE der Klasse




template< typename T>  bool operator== (const Vector<T>& lop, const Vector<T>& rop) {


	return lop.equal(rop);
}


template< typename T> bool operator!= (const Vector<T>& lop, const Vector<T>& rop) {
	return !(lop == rop);
}

template< typename T> ostream& operator<<  (ostream& o, const Vector<T> a) {
	return a.print(o);
}


#endif
