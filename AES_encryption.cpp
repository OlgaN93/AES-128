/**
  * \file
  * Реализация шифрования AES
  */

#include "AES.h"

using namespace std;

/**
  * Процедура циклического сдвига в рядах блока состояния
  *
  * \param state Ссылка на массив, содержащий блок состояния
  */
void shift_cnt_rows(array<array<uint8_t, CNT_COLUMN>, CNT_ROW>& state)
{
	array<uint8_t, CNT_ROW> temp = {};

	for (uint8_t j = 1; j < CNT_COLUMN; j++)
	{
		temp.fill(0);

		for (uint8_t i = 0; i < CNT_ROW; i++)
		{
			temp[i] = state[i][j];
		}

		rotate(temp.begin(), temp.begin() + j, temp.end());

		for (uint8_t i = 0; i < CNT_ROW; i++)
		{
			state[i][j] = temp[i];
		}
	}
}


/**
  * Симметричный алгоритм блочного шифрования с получением данных из массивов
  *
  * \param state Массив, содержащий блок состояния
  * \param key Массив, содержащий ключ шифрования
  *
  * \return Зашифрованный блок данных
  */
array<array<uint8_t, CNT_COLUMN>, CNT_ROW> encryption(array<array<uint8_t, CNT_COLUMN>, CNT_ROW> state, array<array<uint8_t, CNT_COLUMN>, CNT_ROW> key)
{
	array<array<array<uint8_t, CNT_COLUMN>, CNT_ROW>, CNT_ROUND + 1> extended_key = key_generation(key);
	array<array<uint8_t, CNT_COLUMN>, CNT_ROW> round_key = key;

	add_round_key(state, round_key);

	for (uint8_t round = 1; round <= CNT_ROUND; round++)
	{
		for (uint8_t i = 0; i < CNT_ROW; i++)
		{
			sub_byte(state, i, SBOX);
		}

		shift_cnt_rows(state);

		if (round != CNT_ROUND)
		{
			mix_columns(state, MATRIX);
		}

		round_key = extended_key[round];

		add_round_key(state, round_key);
	}

	return state;
}

/**
  * Симметричный алгоритм блочного шифрования с получением данных из файлов
  * 
  * \param *input_data Файл, содержащий данные для шифрования
  * \param *out_cipher Файл для записи зашифрованных данных
  * \param *input_key Файл, содержащий ключ шифрования
  */
void encryption(FILE* input_data, FILE* out_cipher, FILE* input_key)
{
	array<array<uint8_t, CNT_COLUMN>, CNT_ROW> cipher;
	array<array<uint8_t, CNT_COLUMN>, CNT_ROW> key = {};
	array<array<uint8_t, CNT_COLUMN>, CNT_ROW> data = {};
	
	for (uint8_t i = 0; i < CNT_ROW; i++)
	{
		fread_s(&key[i], sizeof(key[i]), sizeof(uint8_t), CNT_COLUMN, input_key);
	}

	cout << "key" << endl;
	for (uint8_t i = 0; i < CNT_ROW; i++)
	{
		for (uint8_t j = 0; j < CNT_COLUMN; j++)
		{
			cout << hex << static_cast<unsigned>(key[i][j]) << "  ";
		}
		cout << endl;
	}
	cout << endl;

	while (fread_s(&data, sizeof(data), sizeof(uint8_t), BYTES_IN_BLOCK, input_data))
	{
		cout << "data" << endl;
		for (uint8_t i = 0; i < CNT_ROW; i++)
		{
			for (uint8_t j = 0; j < CNT_COLUMN; j++)
			{
				cout << hex << static_cast<unsigned>(data[i][j]) << "  ";
			}
			cout << endl;
		}
		cout << endl;

		cipher = encryption(data, key);

		for (uint8_t i = 0; i < CNT_ROW; i++)
		{
			fwrite(&cipher[i], sizeof(uint8_t), sizeof(cipher[i]), out_cipher);
		}

		cout << "out_cipher" << endl;
		for (uint8_t i = 0; i < CNT_ROW; i++)
		{
			for (uint8_t j = 0; j < CNT_COLUMN; j++)
			{
				cout << hex << static_cast<unsigned>(cipher[i][j]) << "  ";
			}
			cout << endl;
		}
		cout << endl;
	}
}

