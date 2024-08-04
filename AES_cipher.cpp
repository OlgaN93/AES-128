/**
  * \file
  * Реализация шифрования AES
  */

#include <array>
#include <fstream>
#include <iostream>

#include<stdint.h>

#include "AES.h"

using namespace std;

/**
  * Процедура подмены байтов в соответствии с таблицей замены SBOX
  *
  * \param &mas Ссылка на массив, содержащий блок данных
  * \param str_now Номер данной строки блока данных 
  */
void sub_byte(array <array<uint8_t, COLUMN>, STRING>& mas, const uint8_t str_now)
{
	for (uint8_t j = 0; j < COLUMN; j++)
	{
		uint8_t s_col = mas[str_now][j] & 0xf;
		uint8_t s_str = (mas[str_now][j] >> 4) & 0xf;
		mas[str_now][j] = SBOX[s_str][s_col];
	}
}

/**
  * Процедура логического сложения по модулю 2 столбца блока состояния со столбцом блока текущего раундового ключа
  *
  * \param &state Ссылка на массив, содержащий блок состояния
  * \param &round_key Ссылка на массив, содержащий раундовый ключ
  */
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

/**
  * Процедура циклического сдвига в рядах блока состояния
  *
  * \param &state Ссылка на массив, содержащий блок состояния
  */
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
  * Процедура умножения 4 байтов столбца блока состояния на фиксированный многочлен c(x)=3x^3+x^2+x+2 по модулю x^4+1 в поле Галуа
  *
  * \param &state Ссылка на массив, содержащий блок состояния
  */
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

/**
  * Симметричный алгоритм блочного шифрования
  *
  * \param data Массив, содержащий блок состояния
  * \param key Массив, содержащий ключ шифрования
  *
  * \return Зашифрованный блок данных
  */
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

/**
  * Чтение данных из файла и запись шифра в файл
  * 
  * \param key Массив, содержащий ключ шифрования
  */
void cipher(array <array<uint8_t, COLUMN>, STRING> key)
{
	char buf[BYTES_IN_BLOCK] = {};
	array <array<uint8_t, COLUMN>, STRING> data = {};
	array <array<uint8_t, COLUMN>, STRING> cipher = {};
	
	FILE* input_data;
	fopen_s(&input_data, FILE_DATA, "r");
	ofstream out_data(FILE_CIPHER, ios::binary);

	while (fread_s(buf, sizeof(buf), sizeof(int8_t), BYTES_IN_BLOCK, input_data))
	{
		for (uint8_t i = 0; i < STRING; i++)
		{
			for (uint8_t j = 0; j < COLUMN; j++)
			{
				data[i][j] = buf[j + i * STRING];
			}
		}

		fill(buf, buf + sizeof(buf), 0);

		cout << "data" << endl;
		for (uint8_t i = 0; i < STRING; i++)
		{
			for (uint8_t j = 0; j < COLUMN; j++)
			{
				cout << hex << static_cast<unsigned>(data[i][j]) << "  ";
			}
			cout << endl;
		}
		cout << endl;

		for (uint8_t i = 0; i < STRING; i++)
		{
			fill(cipher[i].begin(), cipher[i].end(), 0);
		}

		cipher = AES(data, key);

		for (uint8_t i = 0; i < STRING; i++)
		{
			for (uint8_t j = 0; j < COLUMN; j++)
			{
				out_data << cipher[i][j];
			}
		}

		cout << "out_data" << endl;
		for (uint8_t i = 0; i < STRING; i++)
		{
			for (uint8_t j = 0; j < COLUMN; j++)
			{
				cout << hex << static_cast<unsigned>(cipher[i][j]) << "  ";
			}
			cout << endl;
		}
		cout << endl;
	}
}

