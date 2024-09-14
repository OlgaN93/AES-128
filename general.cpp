/**
  * \file
  * Функции, используемые в нескольких модулях
  */

#include "AES.h"

using namespace std;

/**
  * Процедура подмены байтов в соответствии с таблицей замены SBOX или INV_SBOX
  *
  * \param &mas Ссылка на массив, содержащий блок данных
  * \param str_now Номер данной строки блока данных
  */
void sub_byte(array<array<uint8_t, CNT_COLUMN>, CNT_ROW>& mas, const uint8_t str_now, const std::array<std::array<uint8_t, 16>, 16> BOX)
{
	for (uint8_t j = 0; j < CNT_COLUMN; j++)
	{
		uint8_t s_col = mas[str_now][j] & 0xf;
		uint8_t s_str = (mas[str_now][j] >> 4) & 0xf;
		mas[str_now][j] = BOX[s_str][s_col];
	}
}

/**
  * Умножение чисел в поле Галуа по модулю x^4+1
  *
  * \param state_num Число из данных блока состояния
  * \param matrix_num Число из фиксированной матрицы
  *
  * \return Произведение чисел
  */
uint8_t multiply(uint8_t state_num, uint8_t matrix_num)
{
	uint8_t product = 0;
	uint8_t hi_bit_set;

	for (uint8_t i = 0; i < 8; i++)
	{
		if (state_num & 1)
		{
			product ^= matrix_num;
		}

		hi_bit_set = (matrix_num & 0x80);
		matrix_num <<= 1;

		if (hi_bit_set)
		{
			matrix_num ^= 0x1b;
		}
		state_num >>= 1;
	}

	return product;
}

/**
  * Процедура логического сложения по модулю 2 столбца блока состояния со столбцом блока текущего раундового ключа
  *
  * \param &state Ссылка на массив, содержащий блок состояния
  * \param &round_key Ссылка на массив, содержащий раундовый ключ
  */
void add_round_key(array<array<uint8_t, CNT_COLUMN>, CNT_ROW>& state, array<array<uint8_t, CNT_COLUMN>, CNT_ROW>& round_key)
{
	for (uint8_t i = 0; i < CNT_ROW; i++)
	{
		for (uint8_t j = 0; j < CNT_COLUMN; j++)
		{
			state[i][j] ^= round_key[i][j];
		}
	}
}