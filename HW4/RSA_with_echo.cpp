// ref
    // RSA concepts: https://kim85326.github.io/2019/04/17/RSA-%E5%8A%A0%E5%AF%86%E6%B5%81%E7%A8%8B/
    // Integer type in class: https://www.cryptopp.com/wiki/Integer
    // HexEncoder: https://www.cryptopp.com/wiki/HexEncoder
    // Raw RSA: https://www.cryptopp.com/wiki/Raw_RSA#Encryption
    
// enc
    // enc 64 beaadb3d839f3b5f 11 secret
    // enc 64 B14022EEF719F1BB 11 Alice
        // 73DC304C7BF6A0FD

    // enc 64 B14022EEF719F1BB 11 AliA
        // EBF147E8D9854A
        // 00EBF147E8D9854A

// dec
    // dec 64 9D001E6473DFACF9 16282B21A7866BF5 154C638CD3615216
        // Secret
    
#include "cryptlib.h"
#include "rijndael.h"
#include "modes.h"
#include "files.h"
#include "osrng.h"
#include "hex.h"
#include <cmath>
#include <cryptopp/rsa.h>
#include <sstream>
#include <iomanip>


#include <iostream>
#include <string>
#include <cctype>
using namespace std;

CryptoPP::Integer convert(string s){
    using namespace CryptoPP;
    Integer result = 0, pow = 1;
    for(int i = s.size() - 1; i >= 0; i--){
        if(isdigit(s[i])){
            result = result + pow * (s[i] - '0');
        }
        else{
            result = result + pow * (s[i] - 'A' + 10);
        }
        pow = pow * 16;
    }
    return result;
}

void env(string modulus, string message, string exp, int modulus_length){
    using namespace CryptoPP;
    try{
        Integer n = convert(modulus);
        Integer e = convert(exp);

        // echo 
        // cout << "\nn: " << n << " " << "\n";
        // cout << "e: " << e << "\n";

        // env
        RSA::PublicKey publicKey;
        publicKey.Initialize(n, e);


        // ////////////////////////////////////////////////
        // Secret to protect
        Integer m((const byte *)message.data(), message.size());
        // cout << "m: " << m << endl;

        
        // // ////////////////////////////////////////////////
        // // Encrypt
        Integer cipherText = publicKey.ApplyFunction(m);
        // std::cout << "cipherText: " << hex << cipherText << "\n";
        
        stringstream ss;
        ss << uppercase << hex << cipherText;
        string pad;
        ss >> pad;
        // cout << uppercase << hex << pad.substr(0, pad.length()-1) << endl;

        if(modulus_length % 4 == 0){
            cout << setfill('0') << setw(modulus_length / 4) 
                 << uppercase << hex << pad.substr(0, pad.length() - 1) << endl;
        }
        else{
            cout << setfill('0') << setw(modulus_length / 4 + 1) 
                 << uppercase << hex << pad.substr(0, pad.length() - 1) << endl;
        }
    }
    catch (CryptoPP::Exception& e)
	{
		return;
	}
}

void dec(string modulus, string cipherText, string dec_exp, int modulus_length){
    using namespace CryptoPP;
    try{
        // change type
        AutoSeededRandomPool prng;
        Integer n = convert(modulus);
        string exp = "10001";
        Integer e = convert(exp);
        Integer d = convert(dec_exp);

        // echo 
        // cout << "\nn: " << n << " " << "\n";
        // cout << "e: " << e << "\n";
        // cout << "d: " << d << "\n";


        // key
        RSA::PrivateKey privKey;
        privKey.Initialize(n, e, d);

        RSA::PublicKey pubKey;
        pubKey.Initialize(n, e);
        HexDecoder decoder_cipher;
        decoder_cipher.Put((const byte*)dec_exp.data(), dec_exp.size());
        decoder_cipher.MessageEnd();


        // // ////////////////////////////////////////////////
        // // Secret to protect
        HexDecoder decoder_c;
        decoder_c.Put((const byte*)cipherText.data(), cipherText.size());
        decoder_c.MessageEnd();
        Integer c;
        c.Decode(decoder_c, decoder_c.MaxRetrievable());

        Integer r = privKey.CalculateInverse(prng, c);

        // echo
        // cout << "c: " << hex << c << endl;
        // cout << "r: " << hex << r << endl;

        
        // ////////////////////////////////////////////////
        // Dencrypt
        string recovered;
        size_t req = r.MinEncodedSize();
        recovered.resize(req);

        r.Encode((byte *) &recovered[0], recovered.size());
        cout << recovered << endl;
    }
    catch (CryptoPP::Exception& e)
	{
		return;
	}
}

int main(int argc, char* argv[])
{
    using namespace CryptoPP;
    string temp;

    string md, modulus, plainText = "", exp;
    int modulus_length;
    cin >> md >> modulus_length >> modulus >> exp;
    getline(cin, temp);
    int flag = false;
    for(int i = 0; i < temp.size(); i++){
        if(flag) plainText = plainText + temp[i];
        else if(temp[i] != ' '){
            flag = true;
            plainText = temp[i];
        }
    }

    // echo inputs    
    // cout << "\nmode: " << md << "\n";
    // cout << "modulus_length: " << modulus_length << "\n";
    // cout << "modulus: " << modulus << "\n";
    // cout << "exp: " << exp << "\n";
    // cout << "plainText: " << plainText << "\n\n";

    if(md == "enc"){
        env(modulus, plainText, exp, modulus_length);
    }
    else{
        dec(modulus, plainText, exp, modulus_length);
    }

    return 0;
}