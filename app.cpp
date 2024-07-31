/**
 * \file
 * Основной метод
 */

#include <array>
#include <fstream>
#include <iostream>

#include<stdint.h>

#include "AES.h"

using namespace std;

int main()
{
	const char file_data[100] = "D:\\For_Olga\\lern_C++\\projects\\AES\\file_data.bin"; //"pathToFile\\file_data";
	const char file_key[100] = "D:\\For_Olga\\lern_C++\\projects\\AES\\file_key.bin"; //"pathToFile\\file_key";
	const char file_cipher[100] = "D:\\For_Olga\\lern_C++\\projects\\AES\\cipher_data.bin"; //"pathToFile\\cipher_data";
	
	array <array<uint8_t, COLUMN>, STRING> data{};
	array <array<uint8_t, COLUMN>, STRING> key{}; 
	array <array<uint8_t, COLUMN>, STRING> cipher{};
	char buf[BYTES_IN_BLOCK] = {};
	
	ifstream input_key(file_key, ios::binary);
	ofstream out_cipher(file_cipher, ios::binary);
	FILE* input_data;
	fopen_s(&input_data, file_data, "r");

	input_key.read(buf, BYTES_IN_BLOCK);

	for (uint8_t i = 0; i < STRING; i++)
	{
		for (uint8_t j = 0; j < COLUMN; j++)
		{
			key[i][j] = buf[j + i * STRING];
		}
	}

	cout << "key" << endl;
	for (uint8_t i = 0; i < STRING; i++)
	{
		for (uint8_t j = 0; j < COLUMN; j++)
		{
			cout << hex << static_cast<unsigned>(key[i][j]) << "  ";
		}
		cout << endl;
	}
	cout << endl;
	
	fill(buf, buf + sizeof(buf), 0);

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
				out_cipher << cipher[i][j];
			}
		}

		cout << "cipher" << endl;
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

	return 0;
}

