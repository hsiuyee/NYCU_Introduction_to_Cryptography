#include <bits/stdc++.h>

using namespace std;
#define printf(vec)                               \
	do                                            \
	{                                             \
		for (int i = 0; i < (int)vec.size(); i++) \
		{                                         \
			cout << vec[i];                       \
			if ((i + 1) % 8 == 0)                 \
				cout << "\n";                     \
		}                                         \
	} while (0)


const int IP[64] = { 58, 50, 42, 34, 26, 18, 10, 2,
			 60, 52, 44, 36, 28, 20, 12, 4,
			 62, 54, 46, 38, 30, 22, 14, 6,
			 64, 56, 48, 40, 32, 24, 16, 8,
			 57, 49, 41, 33, 25, 17, 9,  1,
			 59, 51, 43, 35, 27, 19, 11, 3,
			 61, 53, 45, 37, 29, 21, 13, 5,
			 63, 55, 47, 39, 31, 23, 15, 7 };


const int IP_inv[64] = {40, 8, 48, 16, 56, 24, 64, 32,
						39, 7, 47, 15, 55, 23, 63, 31,
						38, 6, 46, 14, 54, 22, 62, 30,
						37, 5, 45, 13, 53, 21, 61, 29,
						36, 4, 44, 12, 52, 20, 60, 28,
						35, 3, 43, 11, 51, 19, 59, 27,
						34, 2, 42, 10, 50, 18, 58, 26,
						33, 1, 41, 9, 49, 17, 57, 25};

const int PC1[56] = {57, 49, 41, 33, 25, 17, 9,
					 1, 58, 50, 42, 34, 26, 18,
					 10, 2, 59, 51, 43, 35, 27,
					 19, 11, 3, 60, 52, 44, 36,
					 63, 55, 47, 39, 31, 23, 15,
					 7, 62, 54, 46, 38, 30, 22,
					 14, 6, 61, 53, 45, 37, 29,
					 21, 13, 5, 28, 20, 12, 4};

const int PC2[48] = {14, 17, 11, 24, 1, 5,
					 3, 28, 15, 6, 21, 10,
					 23, 19, 12, 4, 26, 8,
					 16, 7, 27, 20, 13, 2,
					 41, 52, 31, 37, 47, 55,
					 30, 40, 51, 45, 33, 48,
					 44, 49, 39, 56, 34, 53,
					 46, 42, 50, 36, 29, 32};

const int expand_table[48] = {32, 1, 2, 3, 4, 5,
							  4, 5, 6, 7, 8, 9,
							  8, 9, 10, 11, 12, 13,
							  12, 13, 14, 15, 16, 17,
							  16, 17, 18, 19, 20, 21,
							  20, 21, 22, 23, 24, 25,
							  24, 25, 26, 27, 28, 29,
							  28, 29, 30, 31, 32, 1};

const int PF[32] = {16, 7, 20, 21,
					29, 12, 28, 17,
					1, 15, 23, 26,
					5, 18, 31, 10,
					2, 8, 24, 14,
					32, 27, 3, 9,
					19, 13, 30, 6,
					22, 11, 4, 25};

const int s_box[8][4][16] = {{{14, 4, 13, 1, 2, 15, 11, 8, 3, 10, 6, 12, 5, 9, 0, 7}, // s1
							  {0, 15, 7, 4, 14, 2, 13, 1, 10, 6, 12, 11, 9, 5, 3, 8},
							  {4, 1, 14, 8, 13, 6, 2, 11, 15, 12, 9, 7, 3, 10, 5, 0},
							  {15, 12, 8, 2, 4, 9, 1, 7, 5, 11, 3, 14, 10, 0, 6, 13}},
							 {{15, 1, 8, 14, 6, 11, 3, 4, 9, 7, 2, 13, 12, 0, 5, 10}, // s2
							  {3, 13, 4, 7, 15, 2, 8, 14, 12, 0, 1, 10, 6, 9, 11, 5},
							  {0, 14, 7, 11, 10, 4, 13, 1, 5, 8, 12, 6, 9, 3, 2, 15},
							  {13, 8, 10, 1, 3, 15, 4, 2, 11, 6, 7, 12, 0, 5, 14, 9}},
							 {{10, 0, 9, 14, 6, 3, 15, 5, 1, 13, 12, 7, 11, 4, 2, 8}, // s3
							  {13, 7, 0, 9, 3, 4, 6, 10, 2, 8, 5, 14, 12, 11, 15, 1},
							  {13, 6, 4, 9, 8, 15, 3, 0, 11, 1, 2, 12, 5, 10, 14, 7},
							  {1, 10, 13, 0, 6, 9, 8, 7, 4, 15, 14, 3, 11, 5, 2, 12}},
							 {{7, 13, 14, 3, 0, 6, 9, 10, 1, 2, 8, 5, 11, 12, 4, 15}, // s4
							  {13, 8, 11, 5, 6, 15, 0, 3, 4, 7, 2, 12, 1, 10, 14, 9},
							  {10, 6, 9, 0, 12, 11, 7, 13, 15, 1, 3, 14, 5, 2, 8, 4},
							  {3, 15, 0, 6, 10, 1, 13, 8, 9, 4, 5, 11, 12, 7, 2, 14}},
							 {{2, 12, 4, 1, 7, 10, 11, 6, 8, 5, 3, 15, 13, 0, 14, 9}, // s5
							  {14, 11, 2, 12, 4, 7, 13, 1, 5, 0, 15, 10, 3, 9, 8, 6},
							  {4, 2, 1, 11, 10, 13, 7, 8, 15, 9, 12, 5, 6, 3, 0, 14},
							  {11, 8, 12, 7, 1, 14, 2, 13, 6, 15, 0, 9, 10, 4, 5, 3}},
							 {{12, 1, 10, 15, 9, 2, 6, 8, 0, 13, 3, 4, 14, 7, 5, 11}, // s6
							  {10, 15, 4, 2, 7, 12, 9, 5, 6, 1, 13, 14, 0, 11, 3, 8},
							  {9, 14, 15, 5, 2, 8, 12, 3, 7, 0, 4, 10, 1, 13, 11, 6},
							  {4, 3, 2, 12, 9, 5, 15, 10, 11, 14, 1, 7, 6, 0, 8, 13}},
							 {{4, 11, 2, 14, 15, 0, 8, 13, 3, 12, 9, 7, 5, 10, 6, 1}, // s7
							  {13, 0, 11, 7, 4, 9, 1, 10, 14, 3, 5, 12, 2, 15, 8, 6},
							  {1, 4, 11, 13, 12, 3, 7, 14, 10, 15, 6, 8, 0, 5, 9, 2},
							  {6, 11, 13, 8, 1, 4, 10, 7, 9, 5, 0, 15, 14, 2, 3, 12}},
							 {{13, 2, 8, 4, 6, 15, 11, 1, 10, 9, 3, 14, 5, 0, 12, 7}, // s8
							  {1, 15, 13, 8, 10, 3, 7, 4, 12, 5, 6, 11, 0, 14, 9, 2},
							  {7, 11, 4, 1, 9, 12, 14, 2, 0, 6, 10, 13, 15, 3, 5, 8},
							  {2, 1, 14, 7, 4, 10, 8, 13, 15, 12, 9, 0, 3, 5, 6, 11}}};

const int num_leftShift[16] = {1, 1, 2, 2, 2, 2, 2, 2, 1, 2, 2, 2, 2, 2, 2, 1}; // number of bits to shift for each iteration

// do bits permutation
vector<int> permutation(vector<int> bits, const int P[64])
{
	vector<int> after_permutation_bits(64);
	for (int i = 0; i < 64; i++)
	{
		after_permutation_bits[i] = bits[P[i] - 1];
	}
	return after_permutation_bits;
}

vector<int> text_to_bits_64(string text)
{
	vector<int> res(64);
	for (int i = 0; i < (int)text.size(); i++)
	{
		int asciiCode = static_cast<int>(text[i]);
		for (int j = 7; j >= 0; j--)
		{
			res[i * 8 + j] = (asciiCode % 2);
			asciiCode /= 2;
		}
	}
	return res;
}

void bits_64_to_text(vector<int> bits)
{
	for (int i = 0; i * 4 < (int)bits.size(); i++)
	{
		int mul = 8;
		int total = 0;
		for(int j = 0; j < 4; j++){
			total += bits[i * 4 + j] * mul;
			mul /= 2;
		}
		cout << hex << 	uppercase << total;
	}
}

// extend 32 bits to 48 bits
vector<int> extendVector(vector<int> R)
{
	vector<int> rev(48);
	for (int i = 0; i < 48; i++)
	{
		rev[i] = R[expand_table[i] - 1];
	}
	return rev;
}

vector<int> S(int k, vector<int> slice_R)
{
	int num = s_box[k][slice_R[0] * 2 + slice_R[5]][slice_R[1] * 8 + slice_R[2] * 4 + slice_R[3] * 2 + slice_R[4]];
	vector<int> rev(4);
	for (int i = 3; i >= 0; i--)
	{
		rev[i] = num % 2;
		num /= 2;
	}
	return rev;
}

// do F function
vector<int> F_and_permutation(vector<int> R, vector<int> subkey)
{
	R = extendVector(R);
	for (int i = 0; i < 48; i++)
	{
		R[i] ^= subkey[i];
	}

	vector<int> rev(32);
	for (int i = 0; i < 8; i++)
	{
		vector<int> slice_R(6);
		for (int j = 0; j < 6; j++)
		{
			slice_R[j] = R[i * 6 + j];
		}
		vector<int> temp_S = S(i, slice_R);
		for (int j = 0; j < 4; j++)
		{
			rev[i * 4 + j] = temp_S[j];
		}
	}
	vector<int> rev_permutation(32);
	for (int i = 0; i < 32; i++)
	{
		rev_permutation[i] = rev[PF[i] - 1];
	}
	return rev_permutation;
}

pair<vector<int>, vector<int> > round(vector<int> L, vector<int> R, vector<int> subkey)
{
	vector<int> rev_L(32), rev_R(32), temp_F_and_permutation(32);
	rev_L = R;
	temp_F_and_permutation = F_and_permutation(R, subkey);
	// debuger temp_F = L;
	for (int i = 0; i < 32; i++)
	{
		rev_R[i] = L[i] ^ temp_F_and_permutation[i];
	}
	return make_pair(rev_L, rev_R);
}

void solve(vector<int> mainKey, vector<int> plainText)
{
	// // Step 1: permutate the plainText OK
	plainText = permutation(plainText, IP);

	// Step 2: Build subkey
	vector<int> subkey[16];

	// 			 initial C, D
	vector<vector<int> > C(17), D(17);
	for (int i = 0; i < 16; i++)
	{
		C[i].resize(28);
		D[i].resize(28);
		subkey[i].resize(48);
	}
	C[16].resize(28);
	D[16].resize(28);
	for (int i = 0; i < 28; i++)
	{
		C[0][i] = mainKey[PC1[i] - 1];
		D[0][i] = mainKey[PC1[i + 28] - 1];
	}
	  		//   iteration get C_i before permutation
	vector<int> CD(56);
	for (int i = 1; i < 17; i++)
	{
		for (int j = 0; j < 28 - num_leftShift[i - 1]; j++)
		{
			C[i][j] = C[i - 1][j + num_leftShift[i - 1]];
			D[i][j] = D[i - 1][j + num_leftShift[i - 1]];
		}
		for (int j = 28 - num_leftShift[i - 1], k = 0;
			 k < num_leftShift[i - 1]; j++, k++)
		{
			C[i][j] = C[i - 1][k];
			D[i][j] = D[i - 1][k];
		}
		for (int j = 0; j < 28; j++)
		{
			CD[j] = C[i][j];
			CD[j + 28] = D[i][j];
		}
		for (int j = 0; j < 48; j++)
		{
			subkey[i - 1][j] = CD[PC2[j] - 1];
		}
	}

	// Step 3: calculate the L and R
	// 		3-1: Build initial L0, R0
	vector<int> L(32), R(32);
	for (int i = 0; i < 32; i++)
	{
		L[i] = plainText[i];
		R[i] = plainText[i + 32];
	}
		
	for (int m = 0; m < 16; m++)
	{
		pair<vector<int>, vector<int> > upd_L_R = round(L, R, subkey[m]);
		L = upd_L_R.first;
		R = upd_L_R.second;
	}

	// Step 4: Get the final L_15 and R_15,
	// 		   and get cipherText before permutation
	for (int i = 0; i < 32; i++)
	{
		plainText[i] = R[i];
		plainText[i + 32] = L[i];
	}

	// Step 5: Get cipherText
	vector<int> cipherText(64);
	cipherText = permutation(plainText, IP_inv);
	bits_64_to_text(cipherText);
	cout << "\n";
}

signed main()
{
	int T = 5;
	string key, text;
	while (T--)
	{
		cin >> key >> text;
		solve(text_to_bits_64(key), text_to_bits_64(text));
	}
}