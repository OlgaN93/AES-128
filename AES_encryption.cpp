/**
  * \file
  * Реализация шифрования AES
  */

#include "AES.h"

using namespace std;

/**
  * Процедура циклического сдвига в рядах блока состояния
  *
  * \param &state Ссылка на массив, содержащий блок состояния
  */
void shift_cnt_rows(array<array<uint8_t, CNT_COLUMN>, CNT_ROW> &state)
{
	array<uint8_t, CNT_ROW> temp{};

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
  * Процедура умножения 4 байтов столбца блока состояния на фиксированный многочлен c(x)=3x^3+x^2+x+2???????(не сходится с матрицей) по модулю x^4+1 в поле Галуа //так написано в пункте 4.3
  *
  * \param &state Ссылка на массив, содержащий блок состояния
  */
void mix_columns(array<array<uint8_t, CNT_COLUMN>, CNT_ROW> &state)
{
	array<array<uint8_t, CNT_COLUMN>, CNT_ROW> state_before = state;
	array<uint8_t, CNT_COLUMN> matrix = MATRIX;

	for (uint8_t i = 0; i < CNT_ROW; i++)
	{
		for (uint8_t j = 0; j < CNT_COLUMN; j++)
		{
			state[i][j] = 0;

			for (uint8_t k = 0; k < CNT_COLUMN; k++)
			{
				state[i][j] ^= multiply(state_before[i][k], matrix[k]);
			}
			
			rotate(matrix.rbegin(), matrix.rbegin() + 1, matrix.rend());
		}
	}
}

/**
  * Симметричный алгоритм блочного шифрования с получением данных из массивов
  *
  * \param data Массив, содержащий блок состояния
  * \param key Массив, содержащий ключ шифрования
  *
  * \return Зашифрованный блок данных
  */
array<array<uint8_t, CNT_COLUMN>, CNT_ROW> encryption(array<array<uint8_t, CNT_COLUMN>, CNT_ROW> state, array<array<uint8_t, CNT_COLUMN>, CNT_ROW> key)
{
	array<array<uint8_t, CNT_COLUMN>, CNT_ROW> round_key = {};

	add_round_key(state, key);

	array<array<array<uint8_t, CNT_COLUMN>, CNT_ROW>, CNT_ROUND + 1> extended_key = key_generation(key);

	for (uint8_t round = 1; round < CNT_ROUND; round++)
	{
		for (uint8_t i = 0; i < CNT_ROW; i++)
		{
			sub_byte(state, i, SBOX);
		}

		shift_cnt_rows(state);

		mix_columns(state);

		round_key = extended_key[round];

		add_round_key(state, round_key);
	}

	for (uint8_t i = 0; i < CNT_ROW; i++)
	{
		sub_byte(state, i, SBOX);
	}

	shift_cnt_rows(state);

	round_key = extended_key[CNT_ROUND];

	add_round_key(state, round_key);

	return state;
}

/**
  * Симметричный алгоритм блочного шифрования с получением данных из файлов
  * 
  * \param *input_data Файл, содержащий данные для шифрования
  * \param *out_cipher Файл, содержащий зашифрованные данные
  * \param *input_key Файл, содержащий ключ шифрования
  */
void encryption(FILE *input_data, FILE *out_cipher, FILE *input_key)
{
	char buf[BYTES_IN_BLOCK] = {};
	array<array<uint8_t, CNT_COLUMN>, CNT_ROW> key = {};
	array<array<uint8_t, CNT_COLUMN>, CNT_ROW> data = {};
	array<array<uint8_t, CNT_COLUMN>, CNT_ROW> cipher = {};
	
	for (uint8_t i = 0; i < CNT_ROW; i++)
	{
		fread_s(&key[i], sizeof(key[i]), sizeof(int8_t), CNT_COLUMN, input_key);

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

	while (fread_s(buf, sizeof(buf), sizeof(int8_t), BYTES_IN_BLOCK, input_data))
	{
		for (uint8_t i = 0; i < CNT_ROW; i++)
		{
			for (uint8_t j = 0; j < CNT_COLUMN; j++)
			{
				data[i][j] = buf[j + i * CNT_ROW];
			}
		}

		fill(buf, buf + sizeof(buf), 0);

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

		for (uint8_t i = 0; i < CNT_ROW; i++)
		{
			fill(cipher[i].begin(), cipher[i].end(), 0);
		}

		cipher = encryption(data, key);

		for (uint8_t i = 0; i < CNT_ROW; i++)
		{
			fwrite(&cipher[i], sizeof(int8_t), sizeof(cipher[i]), out_cipher);

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

