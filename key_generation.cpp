/**
  * \file
  * Расширение ключа
  */

#include "AES.h"

using namespace std;
// Может эту функцию тоже в общие запихать?
/**
  * Процедура генерации раундовых ключей
  *
  * \param &key Ссылка на массив, содержащий ключ шифрования
  * 
  * \return Расширенный ключ
  */
array<array<array<uint8_t, CNT_COLUMN>, CNT_ROW>, (CNT_ROUND + 1)> key_generation(array<array<uint8_t, CNT_COLUMN>, CNT_ROW> &key)
{
	array<array<uint8_t, CNT_COLUMN>, CNT_ROW> round_key = {};
	array<array<array<uint8_t, CNT_COLUMN>, CNT_ROW>, CNT_ROUND + 1> extended_key = {};

	uint8_t str_now = 3;
	uint8_t rcon_num = 0x01;

	for (uint8_t i = 0; i <= CNT_ROUND; i++)
	{
		extended_key[i] = key;

		round_key = key;

		rotate(round_key[str_now].begin(), round_key[str_now].begin() + 1, round_key[str_now].end());

		sub_byte(round_key, str_now, SBOX);

		round_key[0][0] = key[0][0] ^ round_key[3][0] ^ rcon_num;

		for (uint8_t j = 1; j < CNT_COLUMN; j++)
		{
			round_key[0][j] = key[0][j] ^ round_key[3][j];
		}

		for (uint8_t i = 1; i < CNT_ROW; i++)
		{
			for (uint8_t j = 0; j < CNT_COLUMN; j++)
			{
				round_key[i][j] = key[i][j] ^ round_key[i - 1][j];
			}
		}

		key = round_key;

		uint8_t high_bite = rcon_num & 0x80;
		rcon_num <<= 1;
		if (high_bite)
		{
			rcon_num ^= MODULE;
		}
	}

	return extended_key;
}