/**
 \file
 Реализация дешифрования AES
 */

#include <array>
#include <iostream>

#include<stdint.h>

#include "AES.h"

using namespace std;

void inv_sub_byte(array <array<uint8_t, COLUMN>, STRING>& mas, const uint8_t str_now)
{
	for (uint8_t j = 0; j < COLUMN; j++)
	{
		uint8_t s_col = mas[str_now][j] & 0xf;
		uint8_t s_str = (mas[str_now][j] >> 4) & 0xf;
		mas[str_now][j] = INV_SBOX[s_str][s_col];
	}
}

void inv_shift_rows(array <array<uint8_t, COLUMN>, STRING>& state)
{
	array<uint8_t, STRING> temp{};

	for (uint8_t j = 1; j < COLUMN; j++)
	{
		temp.fill(0);

		for (uint8_t i = 0; i < STRING; i++)
		{
			temp[i] = state[i][j];
		}

		rotate(temp.rbegin(), temp.rbegin() + j, temp.rend());

		for (uint8_t i = 0; i < STRING; i++)
		{
			state[i][j] = temp[i];
		}
	}
}

void inv_mix_columns(array <array<uint8_t, COLUMN>, STRING>& state)
{
	array <array<uint8_t, COLUMN>, STRING> state_before = state;
	array<uint8_t, COLUMN> matrix = INV_MATRIX;

	for (uint8_t i = 0; i < STRING; i++)
	{
		for (uint8_t j = 0; j < COLUMN; j++)
		{
			state[i][j] = 0;

			for (uint8_t k = 0; k < COLUMN; k++)
			{
				state[i][j] ^= multiply(state_before[i][k], matrix[k]);
			}

			rotate(matrix.rbegin(), matrix.rbegin() + 1, matrix.rend());
		}
	}
}

array <array<uint8_t, COLUMN>, STRING> inv_AES(array <array<uint8_t, COLUMN>, STRING> data, array <array<uint8_t, COLUMN>, STRING> key)
{
	array <array<uint8_t, COLUMN>, STRING> state = data;
	array <array<uint8_t, COLUMN>, STRING> round_key = {};

	array <array<array<uint8_t, COLUMN>, STRING>, (CNT_ROUND + 1)> extended_key = key_generation(key);

	round_key = extended_key[CNT_ROUND];

	add_round_key(state, round_key);

	cout << "add_round_key" << endl;
	for (uint8_t i = 0; i < STRING; i++)
	{
		for (uint8_t j = 0; j < COLUMN; j++)
		{
			cout << hex << static_cast<unsigned>(state[i][j]) << "  ";
		}
		cout << endl;
	}
	cout << endl;

	for (uint8_t round = CNT_ROUND - 1; round >= 1; round--)
	{
		inv_shift_rows(state);

		cout << int(round) << endl;
		cout << "shift" << endl;
		for (uint8_t i = 0; i < STRING; i++)
		{
			for (uint8_t j = 0; j < COLUMN; j++)
			{
				cout << hex << static_cast<unsigned>(state[i][j]) << "  ";
			}
			cout << endl;
		}
		cout << endl;
		
		for (uint8_t i = 0; i < STRING; i++)
		{
			inv_sub_byte(state, i);
		}

		cout << "sub" << endl;
		for (uint8_t i = 0; i < STRING; i++)
		{
			for (uint8_t j = 0; j < COLUMN; j++)
			{
				cout << hex << static_cast<unsigned>(state[i][j]) << "  ";
			}
			cout << endl;
		}
		cout << endl;

		round_key = extended_key[round];

		add_round_key(state, round_key);

		cout << "add_round_key" << endl;
		for (uint8_t i = 0; i < STRING; i++)
		{
			for (uint8_t j = 0; j < COLUMN; j++)
			{
				cout << hex << static_cast<unsigned>(state[i][j]) << "  ";
			}
			cout << endl;
		}
		cout << endl;

		inv_mix_columns(state);

		cout << "mix" << endl;
		for (uint8_t i = 0; i < STRING; i++)
		{
			for (uint8_t j = 0; j < COLUMN; j++)
			{
				cout << hex << static_cast<unsigned>(state[i][j]) << "  ";
			}
			cout << endl;
		}
		cout << endl;

		for (uint8_t i = 0; i < STRING; i++)
		{
			for (uint8_t j = 0; j < COLUMN; j++)
			{
				cout << hex << static_cast<unsigned>(state[i][j]) << "  ";
			}
			cout << endl;
		}
		cout << endl;
	}

	inv_shift_rows(state);

	cout << "0" << endl;
	cout << "shift" << endl;
	for (uint8_t i = 0; i < STRING; i++)
	{
		for (uint8_t j = 0; j < COLUMN; j++)
		{
			cout << hex << static_cast<unsigned>(state[i][j]) << "  ";
		}
		cout << endl;
	}
	cout << endl;

	for (uint8_t i = 0; i < STRING; i++)
	{
		inv_sub_byte(state, i);
	}

	cout << "sub" << endl;
	for (uint8_t i = 0; i < STRING; i++)
	{
		for (uint8_t j = 0; j < COLUMN; j++)
		{
			cout << hex << static_cast<unsigned>(state[i][j]) << "  ";
		}
		cout << endl;
	}
	cout << endl;

	round_key = extended_key[0];

	add_round_key(state, round_key);
	
	cout << "add" << endl;
	for (uint8_t i = 0; i < STRING; i++)
	{
		for (uint8_t j = 0; j < COLUMN; j++)
		{
			cout << hex << static_cast<unsigned>(state[i][j]) << "  ";
		}
		cout << endl;
	}
	cout << endl;

	return state;
}