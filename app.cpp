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
	array <array<uint8_t, COLUMN>, STRING> key = {}; 
	char buf[BYTES_IN_BLOCK] = {};
	
	ifstream input_key(FILE_KEY, ios::binary);
	
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

	cipher(key);
	decipher(key);

	return 0;
}

