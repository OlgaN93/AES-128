/**
 \file
 Расширение ключа
 */

#include <array>
#include <iostream>

#include<stdint.h>

#include "AES.h"

using namespace std;

void rot_word(array <array<uint8_t, COLUMN>, STRING>& mas, uint8_t str_now)
{
	rotate(mas[str_now].begin(), mas[str_now].begin() + 1, mas[str_now].end());
}

void sub_byte(array <array<uint8_t, COLUMN>, STRING>& mas, const uint8_t str_now)
{
	for (uint8_t j = 0; j < COLUMN; j++)
	{
		uint8_t s_col = mas[str_now][j] & 0xf;
		uint8_t s_str = (mas[str_now][j] >> 4) & 0xf;
		mas[str_now][j] = SBOX[s_str][s_col];
	}
}

void key_generation(array <array<uint8_t, COLUMN>, STRING>& key, uint8_t rcon_num)
{
	array <array<uint8_t, COLUMN>, STRING> round_key = key;

	uint8_t str_now = 3;

	rot_word(round_key, str_now);

	sub_byte(round_key, str_now);

	round_key[0][0] = key[0][0] ^ round_key[3][0] ^ rcon_num;

	for (uint8_t j = 1; j < COLUMN; j++)
	{
		round_key[0][j] = key[0][j] ^ round_key[3][j];
	}

	for (uint8_t i = 1; i < STRING; i++)
	{
		for (uint8_t j = 0; j < COLUMN; j++)
		{
			round_key[i][j] = key[i][j] ^ round_key[i - 1][j];
		}
	}

	key = round_key;
}