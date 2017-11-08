///Row Sorter
///---------------
#include "txlib.h"
#include <stdio.h>
#include <string.h>
#include <cstdlib>
#include <algorithm>

struct String{
    char* str;
    int length;
};

const int Alphabet = 1, Dictionary = 2, File_name_size = 100,
            Both = 3, Empty_string_size = 3, Correction = 2;

void choose_mode        (const int mode, const char* input_file_name);
int compare_str_begin   (const void* str1, const void* str2);
int compare_str_end     (const void* str1, const void* str2);
void write_to_file      (const String* text, const int num_of_strings, FILE *fileout);
void make_text          (String* text, char* buffer);
int num_of_str          (const char* buffer);
int sizeofFILE          (FILE *filein);
void sort_strings       (const char* name_filein, const char* name_fileout, const int mode);

int main (void){
    printf ("Row Sorter (c) Dandy_Forever, 2017\n"
           "Please, write the name of input file\n");

    char* input_file_name = (char*) calloc(File_name_size, sizeof(*input_file_name));
    assert(input_file_name);

    scanf ("%s", input_file_name);
    assert (input_file_name != "");

    printf ("press '1' if you'd like to sort strings in the alphabetical order\n"
           "press '2' if you'd like to make the rhyming dictionary\n"
           "press '3' if you'd like them both\n");

    int mode = 0;
    scanf ("%d", &mode);
    assert (mode != 0);

    choose_mode (mode, input_file_name);
    return 0;
}

//----------------------------------------------------
//! Choose mode of program
//!
//! @param [in] mode             number of mode
//! @param [in] input_file_name  name of input file
//-----------------------------------------------------
void choose_mode (const int mode, const char* input_file_name){
    char* output_file_name = (char*) calloc(File_name_size, sizeof(*output_file_name));
    assert(output_file_name);

    switch (mode){
    case Alphabet:
        printf ("write the name of file with alphabetical sorted strings\n");
        scanf ("%s", output_file_name);
        assert (output_file_name != "");

        sort_strings (input_file_name, output_file_name, Alphabet);

        printf ("READY\n"
            "See you later :)");
        break;
    case Dictionary:
        printf ("write the name of file with rhyming dictionary\n");
        scanf ("%s", output_file_name);
        assert (output_file_name != "");

        sort_strings (input_file_name, output_file_name, Dictionary);

        printf ("READY\n"
           "See you later :)");
        break;
    case Both:
        printf ("write the name of file with alphabetical sorted strings\n");
        scanf ("%s", output_file_name);
        assert (output_file_name != "");

        sort_strings (input_file_name, output_file_name, Alphabet);

        printf ("write the name of file with rhyming dictionary\n");
        scanf ("%s", output_file_name);
        assert (output_file_name != "");


        sort_strings (input_file_name, output_file_name, Dictionary);
        printf ("READY\n"
           "See you later :)");
        break;
    default:
        printf ("You pressed something wrong\n"
               "See you later :)");
    }
}

//---------------------------------------------------------------------------------
//! Compare two strings
//!
//! @param [in] str1   string 1
//! @param [in] str2   string 2
//!
//! @return number > 0 if str1 > str2; number < 0 if str1 < str2; 0 if str1 == str2
//!
//! @note ignore spaces and punctuals
//---------------------------------------------------------------------------------
int compare_str_begin (const void* str1, const void* str2){
    String string1 = *(String*)str1;
    String string2 = *(String*)str2;
    while (ispunct(*string1.str) || isspace(*string1.str)) (string1.str)++;
    while (ispunct(*string2.str) || isspace(*string2.str)) (string2.str)++;
    return strcmp(string1.str, string2.str);
}

//---------------------------------------------------------------------------------
//! Compare two strings from the end
//!
//! @param [in] str1   string 1
//! @param [in] str2   string 2
//!
//! @return number > 0 if str1 > str2; number < 0 if str1 < str2; 0 if str1 == str2
//!
//! @note ignore spaces and punctuals
//---------------------------------------------------------------------------------
int compare_str_end(const void* str1, const void* str2){
    String string1 = *(String*)str1;
    String string2 = *(String*)str2;
    int is1 = 0, js2 = 0;
    if (string1.str == string2.str) return 0;
    const int max_len = std::max(string1.length, string2.length);
    while (is1 < max_len && js2 < max_len){
        if (ispunct(string1.str[string1.length - is1 - Correction]) ||
            isspace(string1.str[string1.length - is1 - Correction])) is1++;
        else if (ispunct(string2.str[string2.length - js2 - Correction]) ||
                 isspace(string2.str[string2.length - js2 - Correction])) js2++;
        else{
            const int diference = string1.str[string1.length - is1 - Correction] -
                string2.str[string2.length - js2 - Correction];
            if (diference != 0) return diference;
            else{
                is1++;
                js2++;
            }
        }
    }
    if (is1 == string1.length) return -1;
    else return 1;
}

//------------------------------------------------------------------
//! Write sorted strings into output file
//!
//! @param [in]  text             array of Srings
//! @param [in]  num_of_strings   number of strings of input file
//! @param [out] fileout          a file variable f output file
//!
//! @note ignore empty strings
//-------------------------------------------------------------------
void write_to_file (const String* text, const int num_of_strings, FILE *fileout){
    for (int i = 0; i < num_of_strings; i++){
        assert (text + i != 0);
        assert (text[i].str != 0);
        if (text[i].length > Empty_string_size) fprintf (fileout, "%s\n", text[i].str);
    }
}

//------------------------------------------------------------------------
//! Make array of pointers on beginnings of the strings
//!
//! @param [in] buffer     pointer on first symbol of text from input file
//! @param [out] text      array of Strings
//------------------------------------------------------------------------
void make_text (String* text, char* buffer){
    int num_of_strings = 0;
    text[num_of_strings++].str = buffer;
    int i = 0;
    while (buffer[i] != '\0'){
        if (buffer[i] == '\n'){
            buffer[i] = '\0';
            assert (buffer[i] != '\n');
            assert (buffer[i] == '\0');
            text[num_of_strings - 1].length = buffer + i - text[num_of_strings - 1].str + 1;
            text[num_of_strings++].str = buffer + i + 1;
        }
        i++;
    }
    text[num_of_strings - 1].length = buffer + i - text[num_of_strings - 1].str + 1;
    assert (num_of_strings != 0);
}

//-----------------------------------------------------------------------
//! Number of rows of input file
//!
//! @param [in] buffer     pointer on first symbol of text from input file
//!
//! @return number of strings in input file
//-----------------------------------------------------------------------
int num_of_str (const char* buffer){
    int num_str = 1, i = 0;
    while (buffer[i] != '\0'){
        if (buffer[i] == '\n') num_str++;
        i++;
    }
    return num_str;
}

//---------------------------------------------------
//! Size of input file
//!
//! @param [in] filein     file variable of input file
//!
//! @return number of symbols in input file
//----------------------------------------------------
int sizeofFILE (FILE *filein){
    fseek (filein, 0, SEEK_END);
    const int file_length = ftell (filein);
    rewind (filein);
    return file_length;
}

//----------------------------------------------------------------------
//! Sort rows of input file
//!
//! @param [in] name_filein    name of input file
//! @param [in] name_fileout   name of output file
//! @param [in] number         mode of function
//!
//! @note take rows from input file, sort them and put into output file
//----------------------------------------------------------------------
void sort_strings (const char* name_filein, const char* name_fileout, const int mode){
    FILE* filein = fopen (name_filein, "rb");
    assert(filein);
    FILE* fileout = fopen (name_fileout, "wb");
    assert(fileout);
    assert (filein != NULL);
    assert (fileout != NULL);

    const int file_length = sizeofFILE (filein);
    assert (file_length != -1);

    char* buffer = (char*) calloc (file_length + 1, sizeof (*buffer));
    assert(buffer);
    fread (buffer, file_length, sizeof (*buffer), filein);

    const int num_of_strings = num_of_str (buffer);
    assert (num_of_strings != -1);

    String* text = (String*) calloc(num_of_strings, sizeof (*text));
    assert(text);
    make_text (text, buffer);

    switch (mode){
        case Alphabet:
            qsort (text, num_of_strings, sizeof (*text), compare_str_begin);
            break;
        case Dictionary:
            qsort (text, num_of_strings, sizeof (*text), compare_str_end);
            break;
    }
    write_to_file (text, num_of_strings, fileout);

    fclose (filein);
    fclose (fileout);
}
