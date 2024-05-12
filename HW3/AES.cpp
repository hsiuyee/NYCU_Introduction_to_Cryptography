#include "cryptlib.h"
#include "rijndael.h"
#include "modes.h"
#include "files.h"
#include "osrng.h"
#include "hex.h"

#include <iostream>
#include <string>

void brute_force(std::string cipher_ECB) {

    using namespace CryptoPP;


    for(int i = 0; i < 100000; i++){

        // int tempDigit[] = {0, 0, 0, 0, 0};
        // int now = i, cnt = 0;
        // while(cnt < 5) {
        //     tempDigit[cnt] = now % 10;
        //     now /= 10;
        //     cnt++;
        // }

        // byte a[] = {'0','0','0','0','0','0','0','0','0','0','0',
		//     char(tempDigit[4] + int('0')), char(tempDigit[3] + int('0')),  char(tempDigit[2] + int('0')),
        //     char(tempDigit[1] + int('0')), char(tempDigit[0] + int('0'))};

        // SecByteBlock key(a, sizeof(a));
        
        std::string temp_i = std::to_string(i), temp_0(5 - temp_i.size(), '0');
        std::string key = "00000000000" + temp_0 + temp_i;

        std::string recovered_ECB;

        recovered_ECB.clear();

        try
        {
            // ECB_Mode< AES >::Decryption d_ECB;
            // // ECB Mode does not use an IV
            // d_ECB.SetKey(key, 16);
            ECB_Mode<AES>::Decryption d_ECB((byte*)key.c_str(), 16);
            // The StreamTransformationFilter removes
            //  padding as required.

            StringSource ss2( cipher_ECB, true,
                    new HexDecoder(
                        new StreamTransformationFilter(d_ECB,
                                new StringSink(recovered_ECB),
		        BlockPaddingSchemeDef::BlockPaddingScheme::PKCS_PADDING
                        ) // HexEncoder
                )
                ); // StringSource


            bool flag = false;
            for (char c : recovered_ECB) {
                if (!std::isdigit(c) && !std::isalpha(c) && !std::isspace(c)) {
                        flag = true;
                        // std::cout << "flag is true" << std::endl;
                        break;
                }
                }
            if (!flag) {
                //std::cout << "key: ";
                //encoder.Put(a, 16);
                //encoder.MessageEnd();
                //std::cout << std::endl;
                //std::cout << std::endl;

                // std::cout << "find : key" << std::endl;
                //std::cout << "00000000000" + std::to_string(i) << std::endl;
                std::cout << key << std::endl << recovered_ECB  << std::endl;
            return;
	    }
        }
        catch( CryptoPP::Exception& e_ECB )
        {
            // std::cerr << e_ECB.what() << std::endl;
            // exit(1);
        }
    }
}

void solve(std::string plain)
{
    using namespace CryptoPP;
    
    byte a[] = {'2', '3', '5', '7', '1', '1', '1', '3', '1', '7','1', '9', '2', '3', '2', '9'};
    SecByteBlock key(a, sizeof(a));
    StringSource ss1(key.data(), key.size(), true);

    HexEncoder encoder(new FileSink(std::cout));

    // std::string plain = "AES is the US block cipher standard.";
    //std::cout << "plain text: " << plain << std::endl;
    //std::cout << std::endl;

    /*********************************\
    \*********************************/

    // ECB

    std::string cipher_ECB, encoded_ECB, recovered_ECB;

    try
    {
        ECB_Mode< AES >::Encryption e_ECB;
        e_ECB.SetKey( key, key.size() );

        // The StreamTransformationFilter adds padding
        //  as required. ECB and CBC Mode must be padded
        //  to the block size of the cipher.
        StringSource ss1_ECB( plain, true,
            new StreamTransformationFilter( e_ECB,
                new StringSink( cipher_ECB ),
		        BlockPaddingSchemeDef::BlockPaddingScheme::PKCS_PADDING
            ) // StreamTransformationFilter
        ); // StringSource
    }

    catch( CryptoPP::Exception& e_ECB )
    {
        // std::cout  << e_ECB.what() << std::endl;
        // exit(1);
    }

    /*********************************\
    \*********************************/
    
    try
    {
        ECB_Mode< AES >::Decryption d_ECB;
        // ECB Mode does not use an IV
        d_ECB.SetKey( key, key.size() );

        // The StreamTransformationFilter removes
        //  padding as required.
        StringSource ss3( cipher_ECB, true,
            new StreamTransformationFilter( d_ECB,
                new StringSink( recovered_ECB )
            ) // StreamTransformationFilter
        ); // StringSource

        // cout << "recovered text: " << recovered << endl;
    }
    catch( CryptoPP::Exception& e_ECB )
    {
        // std::cerr << e_ECB.what() << std::endl;
        // exit(1);
    }

    /*********************************\
    \*********************************/


    // CBC

    // std::cout << "plain text: " << plain << std::endl;

    std::string cipher_CBC, recovered_CBC;

    byte tempA[] = {'1','2','3','4','5','6','7','8','1','2','3','4','5','6','7','8'};
    SecByteBlock iv_CBC(tempA, sizeof(tempA));

    try
    {
        CBC_Mode< AES >::Encryption e_CBC;
        e_CBC.SetKeyWithIV(key, key.size(), iv_CBC);

        StringSource s(plain, true,
        new StreamTransformationFilter(e_CBC,
                new StringSink(cipher_CBC),
		        BlockPaddingSchemeDef::BlockPaddingScheme::ONE_AND_ZEROS_PADDING
            ) // StreamTransformationFilter
        ); // StringSource
    }
    catch(const Exception& e_CBC)
    {
        // std::cerr << e_CBC.what() << std::endl;
        // exit(1);
    }


    /*********************************\
    \*********************************/

    try
    {
        CBC_Mode< AES >::Decryption d_CBC;
        d_CBC.SetKeyWithIV(key, key.size(), iv_CBC);

        StringSource s(cipher_CBC, true,
            new StreamTransformationFilter(d_CBC,
                new StringSink(recovered_CBC),
		        BlockPaddingSchemeDef::BlockPaddingScheme::ONE_AND_ZEROS_PADDING
            ) // StreamTransformationFilter
        ); // StringSource

        // std::cout << "recovered text: " << recovered_CBC << std::endl;
    }
    catch(const Exception& e_CBC)
    {
        // std::cerr << e_CBC.what() << std::endl;
        // exit(1);
    }

    /*********************************\
    \*********************************/

    // CFB

    // std::cout << "plain text: " << plain << std::endl;

    std::string cipher_CFB, recovered_CFB, encoded_CFB;

    byte tempB[] = {'9','9','9','9','9','9','9','9','9','9','9','9','9','9','9','9'};
    SecByteBlock iv_CFB(tempB, sizeof(tempB));

    //std::cout << std::endl;
    //std::cout << std::endl;
    //std::cout << "CFB: " << std::endl;

    AlgorithmParameters params = MakeParameters(Name::FeedbackSize(), 2 /*8-bits*/)
                                      (Name::IV(), ConstByteArrayParameter(iv_CFB));

    /*********************************\
    \*********************************/

    try
    {
        CFB_Mode< AES >::Encryption e_CFB;
        e_CFB.SetKey( key, key.size(), params );

        // CFB mode must not use padding. Specifying
        //  a scheme will result in an exception
        StringSource ss1( plain, true,
            new StreamTransformationFilter(e_CFB,
                new StringSink( cipher_CFB ),
                BlockPaddingSchemeDef::BlockPaddingScheme::NO_PADDING

            ) // StreamTransformationFilter
        ); // StringSource
    }
    catch( CryptoPP::Exception& e_CFB )
    {
        // std::cerr << e_CFB.what() << std::endl;
        // exit(1);
    }

    /*********************************\
    \*********************************/

    // Pretty print cipher text
    StringSource ss2( cipher_CFB, true,
        new HexEncoder(
            new StringSink( encoded_CFB ),
            BlockPaddingSchemeDef::BlockPaddingScheme::NO_PADDING
        ) // HexEncoder
    ); // StringSource
    //std::cout << "cipher text: " << encoded_CFB << std::endl;

    /*********************************\
    \*********************************/

    try
    {
        CFB_Mode< AES >::Decryption d_CFB;
        d_CFB.SetKey( key, key.size(), params );

        // The StreamTransformationFilter removes
        //  padding as required.
        StringSource ss3( cipher_CFB, true,
            new StreamTransformationFilter(d_CFB,
                new StringSink( recovered_CFB )
            ) // StreamTransformationFilter
        ); // StringSource

        //std::cout << "recovered text: " << recovered_CFB << std::endl;
    }
    catch( CryptoPP::Exception& e_CFB )
    {
        // std::cerr << e_CFB.what() << std::endl;
        // exit(1);
    }

    //std::cout << "key: ";
    //encoder.Put(key, key.size());
    //encoder.MessageEnd();
    //std::cout << std::endl;
    //std::cout << std::endl;

    //std::cout << "iv_CBC: ";
    //encoder.Put(iv_CBC, iv_CBC.size());
    //encoder.MessageEnd();
    //std::cout << std::endl;

    //std::cout << "iv_CFB: ";
    //encoder.Put(iv_CFB, iv_CFB.size());
    //encoder.MessageEnd();
    //std::cout << std::endl;
    //std::cout << std::endl;

    //std::cout << "cipher_ECB text: ";
    encoder.Put((const byte*)&cipher_ECB[0], cipher_ECB.size());
    encoder.MessageEnd();
    std::cout << std::endl;

    //std::cout << "cipher_CBC text: ";
    encoder.Put((const byte*)&cipher_CBC[0], cipher_CBC.size());
    encoder.MessageEnd();
    std::cout << std::endl;

    //std::cout << "cipher_CFB text: ";
    encoder.Put((const byte*)&cipher_CFB[0], cipher_CFB.size());
    encoder.MessageEnd();
    std::cout << std::endl;
    //std::cout << std::endl;

    //std::cout << "recovered_ECB text: " << recovered_ECB << std::endl;
    //std::cout << "recovered_CBC text: " << recovered_CBC << std::endl;
    //std::cout << "recovered_CFB text: " << recovered_CFB << std::endl;
    //std::cout << std::endl;
    //std::cout << "hex" << std::endl;
}

int main(){    
    std::string plainText, brute_cipher;
    std::getline(std::cin, plainText);
    std::cin >> brute_cipher;
    solve("AES is the US block cipher standard.");
    brute_force("104839DE2B34D9BA96F6E054F79F865890B827381D22FC3388690794F0D08EB3");
    solve(plainText);
    brute_force(brute_cipher);

    return 0;
}