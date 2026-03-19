/*/
 * The goal was to make a program that convert
 * between binary, octal, decimal and hexadecimal
 * numbers. But I got a little too far...
 *
 * First, I decided to challenge myself by not
 * use another library except <stdio.h>.
 *
 * Then, I chose to make the program convert from
 * and to bases 2-16 without restriction except
 * a maximum number of digits due to limitations
 * related to memory use.
 *
 * I'm using int to store base-10 numbers and
 * char* to store numbers from other base.
 *
 * Ps.: Sorry for the messy name convention, I
 * got a little freestyle somewhere along the way.
/*/

#include <stdio.h>
// the name is good enough
#define CLEAR_INPUT_BUFFER while(getchar() != '\n')
// list with all possible digits (until base 16)
char digitsList[] = "0123456789ABCDEF";
// max number of digits with 2 bytes for each base
int digitLimit[] = {0, 0, 16, 11,
                    8, 7, 7, 6,
                    6, 6, 5, 5,
                    5, 5, 5, 5,
                    4};

// convert an integer (0-9) to ascii character
char digit2char(char digit)
{
    return (digit+'0');
}

// own implementation of strlen()
int sLength(char* s_ptr)
{
    char* s = s_ptr;
    int count = 0;
    while(s[count]!='\0') count++;
    return count;
}

// uses the digitList to convert TO base-10 number FROM your equivalent in another base
int fromDigit2Int(char digit)
{
    for (int i=0; i<16; i++)
    {
        if (digit == digitsList[i])
        {
            return i;
        }
    }
    printf("[-] ERROR: '%c' (as int: '%d') not found\n", digit, digit);
    return -1;
}

// opposite of the last function (convert FROM base-10 number TO your equivalent in another base)
char fromInt2Digit(int num)
{
    char ch_num = digit2char(num);
    for (int i=0; i<16; i++)
    {
        if (ch_num == digitsList[i])
        {
            return digitsList[i];
        }
    }
}

// convert an entire number from another base TO base-10
int convert2base10(int baseFrom, char* s_number)
{
    int number_len = sLength(s_number);
    int i_number = 0;
    for (int i=number_len; i>0; i--)
    {
        int actualDigit = fromDigit2Int(s_number[i-1]);
        // error check
        if (actualDigit == -1) return -1;
        for (int j=(number_len-i); j>0; j--)
        {
            actualDigit *= baseFrom;
        }
        i_number += actualDigit;
    }
    return i_number;
}

// the name is enough
void convertFROMbase10(int baseTo, int number, char** s_result)
{
    char converted_num[17];
    // produce a inverted string
    for(int i = 0; i < 16; i++)
    {
        if (number <= 0)
        {
            converted_num[i] = '\0';
            break;
        }
        converted_num[i] = digitsList[number%baseTo];
        number = number/baseTo;
    }
    // reorder the characters
    int number_len = sLength(converted_num);
    char buffer;
    for(int i=0; i<number_len/2; i++)
    {
        buffer = converted_num[i];
        converted_num[i] = converted_num[number_len-1-i];
        converted_num[number_len-1-i] = buffer;
    }
    *s_result = converted_num;
}

// safer scanf with validity check for get base-10 numbers
void intFromInput(int* int_ptr, int min, int max, char* prompt)
{
    printf("%s", prompt);
    while(1)
    {
        if (scanf("%d", int_ptr) == 1 && *int_ptr >= min && *int_ptr <= max) return;
        CLEAR_INPUT_BUFFER;
        printf("Please, put a valid integer from %d to %d: ", min, max);
    }
}

// safer scanf with validity check for get different base numbers
void strFromInput(char* s_number, int min, int max, int base, char* prompt)
{
    // char* max_converted;
    // convertFROMbase10(base, max, &max_converted);
    printf("%s", prompt);
    CLEAR_INPUT_BUFFER;
    while(1)
    {
        scanf("%s", s_number);
        int str_len = sLength(s_number);

        int is_valid = 1;
        if (str_len <= digitLimit[base])
        {
            char possibleDigits[base];
            for(int j=0; j<base; j++)
            {
                possibleDigits[j] = digitsList[j];
            }
            possibleDigits[base] = '\0';

            for(int i=0; i<str_len; i++)
            {
                int isDigitValid = -1;
                // upper the lower characters, assuring all possible inputs
                if (s_number[i] >= 'a' && s_number[i] <= 'z') s_number[i] -= ('a' - 'A');
                for(int j=0; j<base; j++)
                {
                    if (s_number[i] == possibleDigits[j]) isDigitValid = 1;
                }

                if (isDigitValid == -1)
                {
                    is_valid = -1;
                    break;
                }
            }
        }
        else
        {
            is_valid = -2;
        }
        if (is_valid == 1) break;
        else if (is_valid == -2) printf("Please, put a number with a maximum of %d digits: ", digitLimit[base]);
        else printf("Please, put a valid base-%d number: ", base);
        CLEAR_INPUT_BUFFER;
    }
    CLEAR_INPUT_BUFFER;
    return;
}

int main()
{
    int fromBase;
    int toBase;
    char* prompt;

    prompt = "Choose the original base of your number (2-16): ";
    intFromInput(&fromBase, 2, 16, prompt);

    prompt = "Now tell to which base you want to convert (2-16): ";
    while(1)
    {
        intFromInput(&toBase, 2, 16, prompt);
        if (fromBase == toBase) printf("I think you already know this. Let's try another base, shall we?\n");
        else break;
    }

    printf("Tell your number (in base %d): ", fromBase);
    if (fromBase == 10)
    {
        int number;
        intFromInput(&number, 0, 65535, "");
        char* convertedNumber;
        convertFROMbase10(toBase, number, &convertedNumber);
        printf("Your number (in base %d) is: %s\n", toBase, convertedNumber);
    }
    else
    {
        char* number;
        strFromInput(number, 0, 65535, fromBase, "");
        int convertedNumber = convert2base10(fromBase, number);
        if (toBase == 10)
        {
            printf("Your number (in base %d) is: %d\n", toBase, convertedNumber);
        }
        else
        {
            char* convertedNumberStr;
            convertFROMbase10(toBase, convertedNumber, &convertedNumberStr);
            printf("Your number (in base %d) is: %s\n", toBase, convertedNumberStr);
        }
    }
    return 0;
}
