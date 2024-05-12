#include "cryptlib.h"
#include "rijndael.h"
#include "modes.h"
#include "files.h"
#include "osrng.h"
#include "hex.h"

#include <iostream>
#include <string>

std::string transform(int w){
    if(w < 10) return std::to_string(w);
    else if(w == 10) return "A";
    else if(w == 11) return "B";
    else if(w == 12) return "C";
    else if(w == 13) return "D";
    else if(w == 14) return "E";
    else if(w == 15) return "F";
}


void solve(std::string cipher_CFB,int k)
{
    using namespace CryptoPP;
    int maxi = 16 * 16 * 16 *16;
    for(int i = 0; i < maxi; i++){
        std::string temp_i = "Our key is: ";
        int now = i;
        for(int j = 0; j < 4; j++){
            temp_i += transform(now % 16);
            now /= 16;  
        }
        // // CFB
        byte a[] = {temp_i[0], temp_i[1], temp_i[2], temp_i[3],
                    temp_i[4], temp_i[5], temp_i[6], temp_i[7],
                    temp_i[8], temp_i[9], temp_i[10], temp_i[11],
                    temp_i[12], temp_i[13], temp_i[14], temp_i[15]};
        // reverse(a);
        SecByteBlock key(a, sizeof(a));
        StringSource ss1(key.data(), key.size(), true);

        byte tempB[] = {'0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0'};
        SecByteBlock iv_CFB(tempB, sizeof(tempB));


        AlgorithmParameters params = MakeParameters(Name::FeedbackSize(), k /*8-bits*/)
                                        (Name::IV(), ConstByteArrayParameter(iv_CFB));

        // /*********************************\
        // \*********************************/
        std::string recovered_CFB;  
        recovered_CFB.clear();


        try
        {
            CFB_Mode< AES >::Decryption d_CFB;
            d_CFB.SetKey( key, key.size(), params );
            // StringSource ss3( cipher_CFB, true, 
            //     new StreamTransformationFilter( d_CFB,
            //         new StringSink( recovered_CFB ),
		    //     BlockPaddingSchemeDef::BlockPaddingScheme::NO_PADDING
            //     ) // StreamTransformationFilter
            // ); // StringSource

            StringSource ss2( cipher_CFB, true,
                    new HexDecoder(
                        new StreamTransformationFilter(d_CFB,
                                new StringSink(recovered_CFB)
                        ) 
                    ) // HexEncoder
                ); // StringSource

            bool flag = false;
            for (char c : recovered_CFB) {
                if (!std::isdigit(c) && !std::isalpha(c) && !std::isspace(c) && (c != '.') && (c != ',') ){
                        flag = true;
                        break;
                }
                }
            if (!flag) {
                std::cout << recovered_CFB  << std::endl;
                return;
            }
        }
        catch( CryptoPP::Exception& d_CFB )
        {
        }
    }
}

int main(){    
    std::string brute_cipher;
    getline(std::cin, brute_cipher);
    // solve(brute_cipher, 2);
    solve(brute_cipher, 4);
    // solve(brute_cipher, 8);
    return 0;
}