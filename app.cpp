/**
  * \file
  * Основной метод
  */

#include "AES.h"

using namespace std;

int main()
{
	const char file_data[100] = /*"D:\\For_Olga\\lern_C++\\projects\\AES\\testOrig.jpg";*/ "D:\\For_Olga\\lern_C++\\projects\\AES\\file_data.bin"; //"pathToFile\\file_data";
	const char file_key[100] = "D:\\For_Olga\\lern_C++\\projects\\AES\\file_key.bin"; //"pathToFile\\file_key";
	const char file_cipher[100] = "D:\\For_Olga\\lern_C++\\projects\\AES\\cipher_data.bin"; //"pathToFile\\cipher_data";
	const char file_decipher[100] = /*"D:\\For_Olga\\lern_C++\\projects\\AES\\test.jpg";*/ "D:\\For_Olga\\lern_C++\\projects\\AES\\decipher_data.bin"; //"pathToFile\\cipher_data";

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

	return 0;
}

