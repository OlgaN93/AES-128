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
	char buf[16] = {};
	
	ifstream input_key(file_key, ios::binary);
	ifstream input_data(file_data, ios::binary);
	ofstream out_cipher(file_cipher, ios::binary);

	input_key.read(buf, 16);

	for (uint8_t i = 0; i < STRING; i++)
	{
		for (uint8_t j = 0; j < COLUMN; j++)
		{
			key[i][j] = buf[j + i * 4];
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

	input_data.read(buf, 16);

	for (uint8_t i = 0; i < STRING; i++)
	{
		for (uint8_t j = 0; j < COLUMN; j++)
		{
			data[i][j] = buf[j + i * 4];
		}
	}

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

	array <array<uint8_t, COLUMN>, STRING> cipher = AES(data, key);

	for (uint8_t i = 0; i < STRING; i++)
	{
		for (uint8_t j = 0; j < COLUMN; j++)
		{
			out_cipher << cipher[i][j];
		}
	}

	return 0;
}

