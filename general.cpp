/**
  * \file
  * �������, ������������ � ���������� �������
  */

#include "AES.h"

using namespace std;

/**
  * ��������� ������� ������ � ������������ � �������� ������ SBOX ��� INV_SBOX
  *
  * \param &mas ������ �� ������, ���������� ���� ������
  * \param str_now ����� ������ ������ ����� ������
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
  * ��������� ����������� �������� �� ������ 2 ������� ����� ��������� �� �������� ����� �������� ���������� �����
  *
  * \param &state ������ �� ������, ���������� ���� ���������
  * \param &round_key ������ �� ������, ���������� ��������� ����
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