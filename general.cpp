/**
  * \file
  * �������, ������������ � ���������� �������
  */

#include "AES.h"

using namespace std;

/**
  * ��������� ������� ������ � ������������ � ���������� �������� ������
  *
  * \param mas ������ �� ������, ���������� ���� ������
  * \param str_now ����� ������ ������ ����� ������
  * \param mass_of_changes ������� �������� ��� ������
  */
//TODO BOX ������������� � �������� � ����
void sub_byte(array<array<uint8_t, CNT_COLUMN>, CNT_ROW>& mas, const uint8_t row_now, const array<array<uint8_t, 16>, 16> matrix_of_changes)
{
	for (uint8_t j = 0; j < CNT_COLUMN; j++)
	{
		uint8_t s_col = mas[row_now][j] & 0b1111;
		uint8_t s_row = (mas[row_now][j] >> 4) & 0b1111;
		mas[row_now][j] = matrix_of_changes[s_row][s_col];
	}
}

/**
  * ������� ����� � ���� �����
  *
  * \param num �����, ������� ����� ���������� � ���� �����
  *
  * \return ����� � ���� �����
  */
uint8_t return_to_Galois_field(uint8_t num)
{
	uint8_t hi_bit_set = (num & 0b10000000);
	num <<= 1;

	if (hi_bit_set)
	{
		num ^= MODULE;
	}

	return num;
}

/**
  * ��������� ����� � ���� ����� �� ������ x^4+1
  *
  * \param state_num ����� �� ������ ����� ���������
  * \param matrix_num ����� �� ������������� �������
  *
  * \return ������������ �����
  */
uint8_t multiply(uint8_t state_num, uint8_t matrix_num)
{
	uint8_t product = 0;

	for (uint8_t i = 0; i < BITS_IN_BYTE; i++)
	{
		if (state_num == 0)
		{
			break;
		}

		if (state_num & 1)
		{
			product ^= matrix_num;
		}

		matrix_num = return_to_Galois_field(matrix_num);

		state_num >>= 1;
	}

	return product;
}

/**
  * ��������� ����������� �������� �� ������ 2 ������� ����� ��������� �� �������� ����� �������� ���������� �����
  *
  * \param state ������ �� ������, ���������� ���� ���������
  * \param round_key ������ �� ������, ���������� ��������� ����
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

/**
  * ��������� ��������� 4 ������ ������� ����� ��������� �� ������������� ��������� �� ������ x^4+1 � ���� �����
  *
  * \param state ������ �� ������, ���������� ���� ���������
  * \param matrix ��������� � ���� �������
  */
void mix_columns(array<array<uint8_t, CNT_COLUMN>, CNT_ROW>& state, array<uint8_t, CNT_COLUMN> matrix)
{
	array<array<uint8_t, CNT_COLUMN>, CNT_ROW> state_before = state;

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
  * ��������� ��������� ��������� ������
  *
  * \param key ������, ���������� ���� ����������
  *
  * \return ����������� ����
  */
array<array<array<uint8_t, CNT_COLUMN>, CNT_ROW>, (CNT_ROUND + 1)> key_generation(array<array<uint8_t, CNT_COLUMN>, CNT_ROW> key) 
{
	array<array<uint8_t, CNT_COLUMN>, CNT_ROW> round_key;
	array<array<array<uint8_t, CNT_COLUMN>, CNT_ROW>, CNT_ROUND + 1> extended_key = {};
	uint8_t last_row = CNT_ROW - 1;
	uint8_t rcon_num = 0x01;

	for (uint8_t i = 0; i <= CNT_ROUND; i++)
	{
		extended_key[i] = key;

		round_key = key;

		rotate(round_key[last_row].begin(), round_key[last_row].begin() + 1, round_key[last_row].end());

		sub_byte(round_key, last_row, SBOX);

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

		rcon_num = return_to_Galois_field(rcon_num);
	}

	return extended_key;
}