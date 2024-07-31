/**
 \file
 Реализация шифрования AES
 */

#include <array>
#include <iostream>

#include<stdint.h>

#include "AES.h"

using namespace std;

//void sub_byte(array <array<uint8_t, COLUMN>, STRING>& mas, const uint8_t str_now)
//{
//	for (uint8_t j = 0; j < COLUMN; j++)
//	{
//		uint8_t s_col = mas[str_now][j] & 0xf;
//		uint8_t s_str = (mas[str_now][j] >> 4) & 0xf;
//		mas[str_now][j] = SBOX[s_str][s_col];
//	}
//}


void add_round_key(array <array<uint8_t, COLUMN>, STRING>& state, array <array<uint8_t, COLUMN>, STRING>& round_key)
{
	for (uint8_t i = 0; i < STRING; i++)
	{
		for (uint8_t j = 0; j < COLUMN; j++)
		{
			state[i][j] ^= round_key[i][j];
		}
	}
}

void shift_rows(array <array<uint8_t, COLUMN>, STRING>& state)
{
	array<uint8_t, STRING> temp{};

	for (uint8_t j = 1; j < COLUMN; j++)
	{
		temp.fill(0);

		for (uint8_t i = 0; i < STRING; i++)
		{
			temp[i] = state[i][j];
		}

		rotate(temp.begin(), temp.begin() + j, temp.end());

		for (uint8_t i = 0; i < STRING; i++)
		{
			state[i][j] = temp[i];
		}
	}
}

uint8_t multiply(uint8_t state_num, uint8_t matrix_num) // лучше сделать с highbit?
{
	if (matrix_num == 1)
	{
		return state_num;
	}
	else
	{
		uint8_t num_mult = state_num << 1;

		if (state_num > 0x7f)
		{
			num_mult ^= MODULE;
		}

		if (matrix_num == 3)
		{
			num_mult ^= state_num;
		}

		return num_mult;
	}
}

void mix_columns(array <array<uint8_t, COLUMN>, STRING>& state)
{
	array <array<uint8_t, COLUMN>, STRING> state_before = state;
	array<uint8_t, COLUMN> matrix = MATRIX;

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

array <array<uint8_t, COLUMN>, STRING> AES(array <array<uint8_t, COLUMN>, STRING> data, array <array<uint8_t, COLUMN>, STRING> key)
{
	array <array<uint8_t, COLUMN>, STRING> state = data;
	array <array<uint8_t, COLUMN>, STRING> round_key = {};

	add_round_key(state, key);

	array <array<array<uint8_t, COLUMN>, STRING>, (CNT_ROUND + 1)> extended_key = key_generation(key);

	for (uint8_t round = 1; round < CNT_ROUND; round++)
	{
		for (uint8_t i = 0; i < STRING; i++)
		{
			sub_byte(state, i);
		}

		shift_rows(state);

		mix_columns(state);

		round_key = extended_key[round];

		add_round_key(state, round_key);
	}

	for (uint8_t i = 0; i < STRING; i++)
	{
		sub_byte(state, i);
	}

	shift_rows(state);

	round_key = extended_key[CNT_ROUND];

	add_round_key(state, round_key);

	return state;
}

