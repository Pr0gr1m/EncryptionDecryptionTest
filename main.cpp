#include <complex>
#include <iostream>
#include <map>
#include <cmath>
#include <vector>
#include <algorithm>

std::string messageToEncode{"attack the d point"};

inline constexpr int charactersInAlphabet = 26;
inline constexpr int startingCharacter = 'a';

std::string encryptCeaser(int c) {
    auto messageCStr = messageToEncode.c_str();
    std::string returnString;
    returnString.resize(messageToEncode.size());

    for (int i = 0; i < messageToEncode.size(); i += 1) {
        char characterAt = *(messageCStr + i);
        char encodedCharacter = static_cast<char>(startingCharacter + ((static_cast<int>(characterAt) - startingCharacter + c)%charactersInAlphabet));
        returnString[i] = encodedCharacter;
    }

    return returnString;
}

std::string decyptCeaser(std::string encrypted) {
    std::string focusWords[] = {"attack", "the", "d", "point"};
    auto encrypedCStr = encrypted.c_str();
    size_t numLetters = encrypted.size();

    std::map<char, int> characterMap;
    for (const auto& string : focusWords) {
        for (auto iterator = string.begin(); iterator != string.end(); ) {
            char stringCharacter = *iterator;
            stringCharacter = static_cast<char>(tolower(stringCharacter));

            if (characterMap.contains(stringCharacter)) {
                characterMap[stringCharacter] += 1;
            } else {
                characterMap.emplace(stringCharacter, 1);
            }

            iterator+=1;
        }
    }

    std::map<std::string, size_t> scoreForEveryEncryption;
    for (int c = 0; c < charactersInAlphabet; c += 1) {
        std::string decryptString;
        decryptString.resize(encrypted.size());

        std::map<char, int> decryptionCharacterMap;

        //Decrypt
        for (int i = 0; i < messageToEncode.size(); i += 1) {
            char characterAt = *(encrypedCStr + i);
            //int minus = -startingCharacter + static_cast<int>(characterAt) - c;
            //int signedMinus = (minus < 0)? charactersInAlphabet - minus : minus;
            //char decodedCharacter = static_cast<char>(startingCharacter + signedMinus); //static_cast<char>(static_cast<int>(startingCharacter + ((static_cast<int>(characterAt) - startingCharacter - c))));

            int offset = (static_cast<int>(characterAt) - startingCharacter - c) % charactersInAlphabet;
            if (offset < 0) offset += charactersInAlphabet;
            char decodedCharacter =static_cast<char>(startingCharacter + offset);

            if (decryptionCharacterMap.contains(decodedCharacter)) {
                decryptionCharacterMap[decodedCharacter] += 1;
            } else {
                decryptionCharacterMap.emplace(decodedCharacter, 1);
            }

            decryptString[i] = decodedCharacter;
        }

        size_t decrpytionScore = 0;
        size_t negativeScore = 0; //size t is unsigned so add negative score after accumulating positive score
        for (const auto& characterMapPair : characterMap) {
            if (!decryptionCharacterMap.contains(static_cast<char>(tolower(characterMapPair.first)))) {
                negativeScore += 10;
                continue;
            }

            //the letters in simpler messages will usually have lower frequency then expected so dont punish that strongly
            float lowerFrequencyScoreMult = 0.5f;
            float higherFrequencyScoreMult = 1.25f;

            int decrpytionCharacterScore = decryptionCharacterMap[characterMapPair.first];
            int expected = characterMapPair.second;

            decrpytionScore += static_cast<size_t>(std::min(decrpytionCharacterScore, expected)) * (charactersInAlphabet - expected);
        }

        //if (decrpytionScore>negativeScore) decrpytionScore -= negativeScore;
        //else decrpytionScore -= (negativeScore-decrpytionScore);

        scoreForEveryEncryption.emplace(decryptString, decrpytionScore);
    }

    std::vector<std::pair<std::string, size_t>> sortedDecryption;

    for (const auto& decryptionPair : scoreForEveryEncryption) {
        auto score = decryptionPair.second;

        //if (sortedDecryption.empty()) {
            // sortedDecryption[0] = (std::make_pair(decryptionPair.first, decryptionPair.second)); //use emplace to build and then insert instead of just insert
        sortedDecryption.emplace_back(decryptionPair.first, decryptionPair.second);
        //}
    }

    if (!sortedDecryption.empty()) {
        std::sort(
            sortedDecryption.begin(),
            sortedDecryption.end(),
            [](const std::pair<std::string, size_t>& a,
               const std::pair<std::string, size_t>& b) {
                return a.second > b.second; //descending score
            }
        );
    }

    if (sortedDecryption.empty()) {
        std::cout << "Empty sorted decryption vector" << std::endl;
        return std::string{};
    }

    for (int i = 0; i < 10; i += 1) {
        const auto& pair = sortedDecryption[i];
        std::cout << pair.first << " : " << pair.second << std::endl;
    }

    return sortedDecryption.front().first;
}

int main() {
    std::cout << "(Ceaser) Encrpyting and then decrypting orginal message: " << messageToEncode << std::endl;
    std::cout << decyptCeaser(encryptCeaser(2));

    /*
    Encrpyting and then decrypting orginal message: attack the d point
    attacknthendnpoint : 361
    gzzgiqtznktjtvuotz : 188
    haahjruaolukuwvpua : 148
    piiprzciwtcscedxci : 147
    leelnvyespyoyaztye : 146
    tmmtvdgmaxgwgihbgm : 143
    buubdlouifoeoqpjou : 125
    zsszbjmsgdmcmonhms : 125
    voovxfiocziyikjdio : 125
    unnuwehnbyhxhjichn : 125
    attacknthendnpoint
    */

    return 0;
}
