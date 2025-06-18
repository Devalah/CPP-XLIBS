#pragma once

template<typename A, typename B>
struct tup2 {
	A a; B b;

	tup2(){}
	tup2(A aVal, B bVal)
		: a(aVal), b(bVal) {}

	void set(A aVal, B bVal) { a = aVal; b = bVal; }
};
template<typename A, typename B, typename C>
struct tup3 {
	A a; B b; C c;

	tup3() {}
	tup3(A aVal, B bVal, C cVal)
		: a(aVal), b(bVal), c(cVal) {}

	void set(A aVal, B bVal, C cVal) { a = aVal; b = bVal; c = cVal; }
};
template<typename A, typename B, typename C, typename D>
struct tup4 {
	A a; B b; C c; D d;

	tup4() {}
	tup4(A aVal, B bVal, C cVal, D dVal)
		: a(aVal), b(bVal), c(cVal), d(dVal) {
	}

	void set(A aVal, B bVal, C cVal, D dVal) { a = aVal; b = bVal; c = cVal; d = dVal; }
};
template<typename A, typename B, typename C, typename D, typename E>
struct tup5 {
	A a; B b; C c; D d; E e;

	tup5() {}
	tup5(A aVal, B bVal, C cVal, D dVal, E eVal)
		: a(aVal), b(bVal), c(cVal), d(dVal), e(eVal) {
	}

	void set(A aVal, B bVal, C cVal, D dVal, E eVal) { a = aVal; b = bVal; c = cVal; d = dVal; e = eVal; }
};
