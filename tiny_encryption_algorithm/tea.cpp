#include <array>
#include <utility>
#include <iostream>
#include <stdint.h>

using namespace std;

#define ROUNDS 32

#define MAGIC_CRYPTO_NUMBER 0x9e3779b9

template<uint32_t pt1, uint32_t pt2, uint32_t k0, uint32_t k1, uint32_t k2, uint32_t k3,size_t round>
struct R {
	using PR = R<pt1, pt2, k0, k1, k2, k3, round - 1>;
	using TR = R<pt1, pt2, k0, k1, k2, k3, round >;
	static constexpr uint32_t sum = PR::sum + MAGIC_CRYPTO_NUMBER;
	static constexpr uint32_t v0 = PR::v0 + (((PR::v1 << 4) + k0) ^ (PR::v1 + TR::sum) ^ ((PR::v1 >> 5) + k1));
	static constexpr uint32_t v1 = PR::v1 + (((TR::v0 << 4) + k2) ^ (TR::v0 + TR::sum) ^ ((TR::v0 >> 5) + k3));
	static constexpr uint32_t ct1 = TR::v0;
	static constexpr uint32_t ct2 = TR::v1;
};

//TEA code taken from https://en.wikipedia.org/wiki/Tiny_Encryption_Algorithm
void encrypt(uint32_t* v, uint32_t* k) {
	uint32_t v0 = v[0], v1 = v[1], sum = 0, i;           /* set up */
	uint32_t delta = MAGIC_CRYPTO_NUMBER;                     /* a key schedule constant */
	uint32_t k0 = k[0], k1 = k[1], k2 = k[2], k3 = k[3];   /* cache key */
	for (i = 0; i < ROUNDS; i++) {                       /* basic cycle start */
		sum += delta;
		v0 += ((v1 << 4) + k0) ^ (v1 + sum) ^ ((v1 >> 5) + k1);
		v1 += ((v0 << 4) + k2) ^ (v0 + sum) ^ ((v0 >> 5) + k3);
	}                                              /* end cycle */

	v[0] = v0; v[1] = v1;
}

void decrypt(uint32_t* v, uint32_t* k) {
	uint32_t v0 = v[0], v1 = v[1], sum = 0xC6EF3720, i;  /* set up */
	uint32_t delta = 0x9e3779b9;                     /* a key schedule constant */
	uint32_t k0 = k[0], k1 = k[1], k2 = k[2], k3 = k[3];   /* cache key */
	for (i = 0; i<32; i++) {                         /* basic cycle start */
		v1 -= ((v0 << 4) + k2) ^ (v0 + sum) ^ ((v0 >> 5) + k3);
		v0 -= ((v1 << 4) + k0) ^ (v1 + sum) ^ ((v1 >> 5) + k1);
		sum -= delta;
	}                                              /* end cycle */
	v[0] = v0; v[1] = v1;
}

template<uint32_t pt1, uint32_t pt2, uint32_t k0, uint32_t k1, uint32_t k2, uint32_t k3>
struct R<pt1, pt2, k0, k1, k2, k3, 0> {
	static constexpr uint32_t sum = 0;
	static constexpr uint32_t v0 = pt1;
	static constexpr uint32_t v1 = pt2;
	static constexpr uint32_t ct1 = v0;
	static constexpr uint32_t ct2 = v1;
};

template<uint32_t pt1, uint32_t pt2, uint32_t k0, uint32_t k1, uint32_t k2, uint32_t k3>
using TEA_CRYPT = R<pt1, pt2, k0, k1, k2, k3, ROUNDS>;


int main() {
	static constexpr uint32_t k0 = 0x12345678;
	static constexpr uint32_t k1 = 0x90abcdef;
	static constexpr uint32_t k2 = 0x88888888;
	static constexpr uint32_t k3 = 0xfabfab00;

	array<uint32_t, 4> k = { k0, k1, k2, k3 };
	static constexpr uint32_t pt1 = 0xdeadbeef;
	static constexpr uint32_t pt2 = 0xcafe1776;
	array<uint32_t, 2> pt = { pt1,pt2 };
	
	/*
	encrypt(pt.data(), k.data());
	cout << "reference" << endl;
	for (auto i : pt) {
		cout << i << " ";
	}
	cout << endl;
	*/


	static constexpr array<uint32_t, 2> encrypted = { TEA_CRYPT<pt1, pt2, k0, k1, k2, k3>::ct1, TEA_CRYPT<pt1,pt2,k0,k1,k2,k3>::ct2 };

	cout << "metaprogram" << endl;
	for (auto i : encrypted) {
		cout << hex << i << " ";
	}
	cout << endl;

	array<uint32_t, 2> encrypted_mutable(encrypted);
	decrypt(encrypted_mutable.data(), k.data());

	cout << "decrypted" << endl;
	for (auto i : encrypted_mutable) {
		cout << hex << i << " ";
	}

	cout << endl;

	//system("pause");
}