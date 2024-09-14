/**
  * \file
  * Реализация дешифрования AES
  */

#include "AES.h"

using namespace std;

/**
  * Процедура циклического сдвига в рядах блока состояния, обратна процедуре shift_cnt_rows
  *
  * \param &state Ссылка на массив, содержащий блок состояния
  */
void inv_shift_cnt_rows(array<array<uint8_t, CNT_COLUMN>, CNT_ROW> &state)
{
	array<uint8_t, CNT_ROW> temp{};

	for (uint8_t j = 1; j < CNT_COLUMN; j++)
	{
		temp.fill(0);

		for (uint8_t i = 0; i < CNT_ROW; i++)
		{
			temp[i] = state[i][j];
		}

		rotate(temp.rbegin(), temp.rbegin() + j, temp.rend());

		for (uint8_t i = 0; i < CNT_ROW; i++)
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
void inv_mix_columns(array<array<uint8_t, CNT_COLUMN>, CNT_ROW> &state)
{
	array<array<uint8_t, CNT_COLUMN>, CNT_ROW> state_before = state;
	array<uint8_t, CNT_COLUMN> matrix = INV_MATRIX;

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
  * Симметричный алгоритм блочного расшифрования с получением данных из массивов
  *
  * \param data Массив, содержащий блок состояния
  * \param key Массив, содержащий ключ шифрования
  *
  * \return Расашифрованный блок данных
  */ 
array<array<uint8_t, CNT_COLUMN>, CNT_ROW> decryption(array<array<uint8_t, CNT_COLUMN>, CNT_ROW> state, array<array<uint8_t, CNT_COLUMN>, CNT_ROW> key)
{
	array<array<uint8_t, CNT_COLUMN>, CNT_ROW> round_key = {};

	array<array<array<uint8_t, CNT_COLUMN>, CNT_ROW>, CNT_ROUND + 1> extended_key = key_generation(key);

	round_key = extended_key[CNT_ROUND];

	add_round_key(state, round_key);

	for (uint8_t round = CNT_ROUND - 1; round >= 1; round--)
	{
		inv_shift_cnt_rows(state);
		
		for (uint8_t i = 0; i < CNT_ROW; i++)
		{
			sub_byte(state, i, INV_SBOX);
		}

		round_key = extended_key[round];

		add_round_key(state, round_key);

		inv_mix_columns(state);
	}

	inv_shift_cnt_rows(state);

	for (uint8_t i = 0; i < CNT_ROW; i++)
	{
		sub_byte(state, i, INV_SBOX);
	}

	round_key = extended_key[0];

	add_round_key(state, round_key);
	
	return state;
}

/**
  * Симметричный алгоритм блочного расшифрования с получением данных из файлов
  *
  * \param *input_cipher Файл, содержащий данные для дешифрования
  * \param *out_decipher Файл, содержащий дешифрованные данные
  * \param *input_key Файл, содержащий ключ шифрования
  */
void decryption(FILE* input_cipher, FILE* out_decipher, FILE *input_key) //среда разработки исправляет на FILE*
{
	char buf[BYTES_IN_BLOCK] = {};
	array<array<uint8_t, CNT_COLUMN>, CNT_ROW> key = {};
	array<array<uint8_t, CNT_COLUMN>, CNT_ROW> decipher = {};
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

	while (fread_s(buf, sizeof(buf), sizeof(int8_t), BYTES_IN_BLOCK, input_cipher))
	{
		for (uint8_t i = 0; i < CNT_ROW; i++)
		{
			for (uint8_t j = 0; j < CNT_COLUMN; j++)
			{
				cipher[i][j] = buf[j + i * CNT_ROW];
			}
		}

		fill(buf, buf + sizeof(buf), 0);

		cout << "cipher" << endl;
		for (uint8_t i = 0; i < CNT_ROW; i++)
		{
			for (uint8_t j = 0; j < CNT_COLUMN; j++)
			{
				cout << hex << static_cast<unsigned>(cipher[i][j]) << "  ";
			}
			cout << endl;
		}
		cout << endl;

		for (uint8_t i = 0; i < CNT_ROW; i++)
		{
			fill(decipher[i].begin(), decipher[i].end(), 0);
		}

		decipher = decryption(cipher, key);

		for (uint8_t i = 0; i < CNT_ROW; i++)
		{
			fwrite(&decipher[i], sizeof(int8_t), sizeof(decipher[i]), out_decipher);

		}

		cout << "out_decipher" << endl;
		for (uint8_t i = 0; i < CNT_ROW; i++)
		{
			for (uint8_t j = 0; j < CNT_COLUMN; j++)
			{
				cout << hex << static_cast<unsigned>(decipher[i][j]) << "  ";
			}
			cout << endl;
		}
		cout << endl;
	}
}