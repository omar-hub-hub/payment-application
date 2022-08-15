#ifndef _CARD_C_
#define _CARD_C_
#include "card.h"
#include <stdio.h>
#include<random>
int getNeededLuhnDigit(uint8_t* cardPan) {
    int startindex = 17;
    int sum = 0;
    char cp[21];
    for (int i = 0; i <= startindex; i++) {
        cp[i] = cardPan[i];
    }
    cp[startindex + 1] = '\0';
    for (int i = startindex; i >= 0; i -= 2) {
        int digit = (cp[i] - '0') * 2;
        if (digit < 10)cp[i] = (char)(digit + '0');
        else cp[i] = (char)(digit - 9 + '0');
    }
    for (int i = 0; i <= startindex; i++) {
        sum += cp[i] - '0';
    }

    return sum % 10;
}

uint8_t* GLN() {
    time_t t1;
    srand((unsigned)time(&t1));
    uint8_t* cardpan = new uint8_t[21];
    cardpan[0] = (char)('0' + rand() % 9 + 1);
    for (int i = 1; i <= 17; i++) {
        cardpan[i] = (char)('0' + rand() % 10);
    }
    cardpan[18] = '\0';
    int valid = getNeededLuhnDigit(cardpan);
    cardpan[18] = (char)('0' + valid);
    cardpan[19] = '\0';
    return cardpan;
}

EN_cardError_t getCardHolderName(ST_cardData_t* cardData) {
    char cardname[100];
    for (int i = 0; i < strlen(cardname); i++)
    {
        cardname[i] = '\0';
    }
    printf("card holder name  ");
    fseek(stdin, 0, SEEK_END);
    scanf("%100[0-9a-zA-Z ]", &cardname);
    fseek(stdin, 0, SEEK_END);

    if (!cardname)return WRONG_NAME;
    if (strlen(cardname) > 20 && strlen(cardname) < 24) {
        for (int i = 0; i < 25; i++) {
            cardData->cardHolderName[i] = cardname[i];
        }
        printf("Read name: %s\n", cardData->cardHolderName);
    }
    else return WRONG_NAME;
    return OK_cardError;

}
EN_cardError_t getCardExpiryDate(ST_cardData_t* cardData)
{
    char cardexp[100];
    for (int i = 0; i < 100; i++) { cardexp[i] = '\0'; }
    uint32_t month, year;
    printf("Enter expiry date [MM/YY]: ");
    fseek(stdin, 0, SEEK_END);
    scanf("%s", cardexp);
    if (!cardexp)return WRONG_EXP_DATE;
    if (cardexp[6] != '\0' || cardexp[3] == '\0')return WRONG_EXP_DATE;
    month = (cardexp[0] - '0') * 10 + (cardexp[1] - '0');
    year = (cardexp[3] - '0') * 10 + (cardexp[4] - '0');
    if (month < 1 || month>12 || cardexp[2] != '/' || (year > 99 || year < 0))return WRONG_EXP_DATE;
    for (int i = 0; i < 6; i++) {
        cardData->cardExpirationDate[i] = cardexp[i];
    }
    printf("Read expiry date: %s\n", cardData->cardExpirationDate);
    return (EN_cardError_t)OK_cardError;
}
EN_cardError_t getCardPAN(ST_cardData_t* cardData) {
    char a = '\0';
    printf("wanna to genrate card number or not[y/n] ");
    fseek(stdin, 0, SEEK_END);
    scanf("%c", &a);
    if (a == 'y' || a == 'Y') {
        uint8_t* cardpin = GLN();
        for (int i = 0; i < 20; i++) {
            cardData->primaryAccountNumber[i] = cardpin[i];
        }
        printf("Generated card number: %s\n", cardData->primaryAccountNumber);
    }
    else if (a == 'n' || a == 'N') {
        char cardPin[100];

        printf("card pan number  ");
        scanf("%s", &cardPin);
        if (!cardPin)return WRONG_PAN;

        if (strlen(cardPin) >= 16 && strlen(cardPin) <= 20) {
            for (int i = 0; i < 25; i++) {
                cardData->primaryAccountNumber[i] = cardPin[i];
            }
            printf("Read pan : %s\n", cardData->primaryAccountNumber);
            return OK_cardError;
        }
        else return WRONG_PAN;

    }

}
#endif // !_CARD_C_