/**
  * \file
  * Основной метод
  */

#include "AES.h"

using namespace std;


void example_of_using_data_from_files()
{
	const char* file_data = "pathToFile\\file_data";
	const char* file_key = "pathToFile\\file_key";
	const char* file_cipher = "pathToFile\\cipher_data";
	const char* file_decipher = "pathToFile\\decipher_data";

	FILE* input_key;
	fopen_s(&input_key, file_key, "rb");

	FILE* input_data;
	fopen_s(&input_data, file_data, "rb");
	FILE* out_cipher;
	fopen_s(&out_cipher, file_cipher, "wb");

	encryption(input_data, out_cipher, input_key);

	fclose(out_cipher);

	FILE* input_cipher;
	fopen_s(&input_cipher, file_cipher, "rb");
	FILE* out_decipher;
	fopen_s(&out_decipher, file_decipher, "wb");

	fseek(input_key, 0, SEEK_SET);

	decryption(input_cipher, out_decipher, input_key);
}

void example_of_using_data_from_array()
{
	array<array<uint8_t, CNT_COLUMN>, CNT_ROW> i_key = {0x2b, 0x7e, 0x15, 0x16, 0x28, 0xae, 0xd2, 0xa6, 0xab, 0xf7, 0x15, 0x88, 0x09, 0xcf, 0x4f, 0x3c};
	array<array<uint8_t, CNT_COLUMN>, CNT_ROW> i_data = {0x32, 0x43, 0xf6, 0xa8, 0x88, 0x5a, 0x30, 0x8d, 0x31, 0x31, 0x98, 0xa2, 0xe0, 0x37, 0x07, 0x34};

	array<array<uint8_t, CNT_COLUMN>, CNT_ROW> o_cipher = encryption(i_data, i_key);

	cout << "out_cipher" << endl;
	for (uint8_t i = 0; i < CNT_ROW; i++)
	{
		for (uint8_t j = 0; j < CNT_COLUMN; j++)
		{
			cout << hex << static_cast<unsigned>(o_cipher[i][j]) << "  ";
		}
		cout << endl;
	}
	cout << endl;

	array<array<uint8_t, CNT_COLUMN>, CNT_ROW> i_cipher = { 0x39, 0x25, 0x84, 0x1d, 0x02, 0xdc, 0x09, 0xfb, 0xdc, 0x11, 0x85, 0x97, 0x19, 0x6a, 0x0b, 0x32 };

	array<array<uint8_t, CNT_COLUMN>, CNT_ROW> o_decipher = decryption(i_cipher, i_key);

	cout << "out_decipher" << endl;
	for (uint8_t i = 0; i < CNT_ROW; i++)
	{
		for (uint8_t j = 0; j < CNT_COLUMN; j++)
		{
			cout << hex << static_cast<unsigned>(o_decipher[i][j]) << "  ";
		}
		cout << endl;
	}
	cout << endl;
}

int main()
{
	example_of_using_data_from_files();
	
	example_of_using_data_from_array();

	return 0;
}

