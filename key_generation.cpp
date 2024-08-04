/**
  * \file
  * Расширение ключа
  */

#include <array>
#include <iostream>

#include<stdint.h>

#include "AES.h"

using namespace std;

/**
  * Процедура циклического сдвига на одну ячейку вверх первого столбца текущего блока раундового ключа
  *
  * \param &mas Ссылка на массив, содержащий ключ шифрования
  * \param str_now Номер данной строки блока данных
  */
void rot_word(array <array<uint8_t, COLUMN>, STRING>& mas, uint8_t str_now)
{
	rotate(mas[str_now].begin(), mas[str_now].begin() + 1, mas[str_now].end());
}

/**
  * Процедура генерации раундовых ключей
  *
  * \param &key Ссылка на массив, содержащий ключ шифрования
  * 
  * \return Расширенный ключ
  */
array <array<array<uint8_t, COLUMN>, STRING>, (CNT_ROUND + 1)> key_generation(array <array<uint8_t, COLUMN>, STRING>& key)
{
	array <array<uint8_t, COLUMN>, STRING> round_key = {};
	array <array<array<uint8_t, COLUMN>, STRING>, (CNT_ROUND + 1)> extended_key = {};

	uint8_t str_now = 3;
	uint8_t rcon_num = 0x01;

	for (uint8_t i = 0; i <= CNT_ROUND; i++)
	{
		extended_key[i] = key;

		round_key = key;

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

		uint8_t high_bite = rcon_num & 0x80;
		rcon_num <<= 1;
		if (high_bite)
		{
			rcon_num ^= MODULE;
		}
	}

	return extended_key;
}