/**
  * \file
  * Реализация дешифрования AES
  */

#include <array>
#include <fstream>
#include <iostream>

#include<stdint.h>

#include "AES.h"

using namespace std;

/**
  * Процедура подмены байтов в соответствии с таблицей замены INV_SBOX
  *
  * \param &mas Ссылка на массив, содержащий блок данных
  * \param str_now Номер данной строки блока данных
  */
void inv_sub_byte(array <array<uint8_t, COLUMN>, STRING>& mas, const uint8_t str_now)
{
	for (uint8_t j = 0; j < COLUMN; j++)
	{
		uint8_t s_col = mas[str_now][j] & 0xf;
		uint8_t s_str = (mas[str_now][j] >> 4) & 0xf;
		mas[str_now][j] = INV_SBOX[s_str][s_col];
	}
}

/**
  * Процедура циклического сдвига в рядах блока состояния, обратна процедуре shift_rows
  *
  * \param &state Ссылка на массив, содержащий блок состояния
  */
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

/**
  * Процедура умножения 4 байтов столбца блока состояния на фиксированный многочлен c(x)=bx^3+dx^2+9x+e по модулю x^4+1 в поле Галуа
  *
  * \param &state Ссылка на массив, содержащий блок состояния
  */
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

/**
  * Симметричный алгоритм блочного расшифрования
  *
  * \param data Массив, содержащий блок состояния
  * \param key Массив, содержащий ключ шифрования
  *
  * \return Расашифрованный блок данных
  */
array <array<uint8_t, COLUMN>, STRING> inv_AES(array <array<uint8_t, COLUMN>, STRING> data, array <array<uint8_t, COLUMN>, STRING> key)
{
	array <array<uint8_t, COLUMN>, STRING> state = data;
	array <array<uint8_t, COLUMN>, STRING> round_key = {};

	array <array<array<uint8_t, COLUMN>, STRING>, (CNT_ROUND + 1)> extended_key = key_generation(key);

	round_key = extended_key[CNT_ROUND];

	add_round_key(state, round_key);

	for (uint8_t round = CNT_ROUND - 1; round >= 1; round--)
	{
		inv_shift_rows(state);
		
		for (uint8_t i = 0; i < STRING; i++)
		{
			inv_sub_byte(state, i);
		}

		round_key = extended_key[round];

		add_round_key(state, round_key);

		inv_mix_columns(state);
	}

	inv_shift_rows(state);

	for (uint8_t i = 0; i < STRING; i++)
	{
		inv_sub_byte(state, i);
	}

	round_key = extended_key[0];

	add_round_key(state, round_key);
	
	return state;
}

/**
  * Чтение зашифрованных данных из файла и запись расшифровки в файл
  *
  * \param key Массив, содержащий ключ шифрования
  */
void decipher(array <array<uint8_t, COLUMN>, STRING> key)
{
	char buf[BYTES_IN_BLOCK] = {};
	array <array<uint8_t, COLUMN>, STRING> data = {};
	array <array<uint8_t, COLUMN>, STRING> cipher = {};

	FILE* input_data;
	fopen_s(&input_data, FILE_CIPHER, "r");
	ofstream out_data(FILE_DATA, ios::binary);

	while (fread_s(buf, sizeof(buf), sizeof(int8_t), BYTES_IN_BLOCK, input_data))
	{
		for (uint8_t i = 0; i < STRING; i++)
		{
			for (uint8_t j = 0; j < COLUMN; j++)
			{
				cipher[i][j] = buf[j + i * STRING];
			}
		}

		fill(buf, buf + sizeof(buf), 0);

		cout << "data" << endl;
		for (uint8_t i = 0; i < STRING; i++)
		{
			for (uint8_t j = 0; j < COLUMN; j++)
			{
				cout << hex << static_cast<unsigned>(cipher[i][j]) << "  ";
			}
			cout << endl;
		}
		cout << endl;

		for (uint8_t i = 0; i < STRING; i++)
		{
			fill(data[i].begin(), data[i].end(), 0);
		}

		data = inv_AES(cipher, key);

		for (uint8_t i = 0; i < STRING; i++)
		{
			for (uint8_t j = 0; j < COLUMN; j++)
			{
				out_data << data[i][j];
			}
		}

		cout << "out_data" << endl;
		for (uint8_t i = 0; i < STRING; i++)
		{
			for (uint8_t j = 0; j < COLUMN; j++)
			{
				cout << hex << static_cast<unsigned>(data[i][j]) << "  ";
			}
			cout << endl;
		}
		cout << endl;
	}

}