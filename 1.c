#include <stdio.h>
#include <stdlib.h>
#include <string.h>
void int_to_bytes(unsigned long int n, unsigned char *p){
        for(int i = 0; i < 4; i++){
           p[i] =  (n << (8*i)) >> 24;
        }
};
int bytes_to_int(unsigned char* p){
    unsigned int n = 0;
    for(int i = 0; i < 4; i++){
        n += p[3-i] << (8*i);
    }
    return n;
};
int get_size(FILE* f){
    unsigned long int signal;
    fseek(f, 0, SEEK_END);
    signal = ftell(f);
    fseek(f, 0, SEEK_SET);
    return signal;
};
void create_arc(FILE* a, char* entrance){
    FILE* ent = fopen(entrance, "rb");
    unsigned long int size_of_file;
    size_of_file = get_size(ent);
    unsigned char size_of_file_in_bytes[4];
    int_to_bytes(size_of_file, size_of_file_in_bytes);
    unsigned long int size_of_name = strlen(entrance);
    unsigned char size_of_name_in_bytes[4];
    int_to_bytes(size_of_name, size_of_name_in_bytes);
    fwrite(size_of_name_in_bytes, sizeof(char), 4, a);
    fwrite(entrance, sizeof(char), size_of_name, a);
    fwrite(size_of_file_in_bytes, sizeof(size_of_file_in_bytes), 1, a);
    char symbol;
    int i = 0;
    while(size_of_file > i){
       symbol = fgetc(ent);
       fputc(symbol, a);
       i++;
    }
    fclose(ent);

};
void list_arc(FILE* a){
    char size_of_name_in_bytes[4];
    int size_of_name;
    unsigned char size_of_file_in_bytes[4];
    unsigned long int size_of_file;
    long int size_to_chek;
    size_to_chek = get_size(a);
    printf("list of names in:\n");
    while(ftell(a) < size_to_chek){
        fread(size_of_name_in_bytes, sizeof(char), 4, a);
        size_of_name = bytes_to_int(size_of_name_in_bytes);
        char* name = NULL;
        name = (char*)malloc(size_of_name*sizeof(char));
        fread(name, sizeof(char), size_of_name, a);
        printf("%s\n", name);
        fread(size_of_file_in_bytes, sizeof(size_of_file_in_bytes), 1, a);
        size_of_file = bytes_to_int(size_of_file_in_bytes);
        char* read = NULL;
        read = (char*)malloc(size_of_file* sizeof(char)+1);
        fgets(read, sizeof(char)* size_of_file + 1, a);
        free(read);
        free(name);
    }

}
void extract_arc(FILE* a){
    unsigned long int size_to_chek;
    size_to_chek = get_size(a);
    unsigned long int size_of_file;
    unsigned char size_of_file_in_bytes[4];
    unsigned long int size_of_name;
    unsigned char size_of_name_in_bytes[4];
    char simvol;
    while(ftell(a) < size_to_chek){
       fread(size_of_name_in_bytes, sizeof(char), 4, a);
       size_of_name = bytes_to_int(size_of_name_in_bytes);
       char* name = NULL;
       name = (char*)malloc(size_of_name* sizeof(char));
       fread(name, sizeof(char), size_of_name, a);
       FILE* newe = fopen(name, "ab");
       fread(size_of_file_in_bytes, sizeof(char), 4, a);
       size_of_file = bytes_to_int(size_of_file_in_bytes);
       for(int i = 0; i < size_of_file; i++){
            simvol = fgetc(a);
            fputc(simvol, newe);
       }
       fclose(newe);
       free(name);
    }
};
int main(int argc, char* argv[])
{
    if(argc < 4){
        printf("uncorrect number of arguments");
        return 1;
    };
    char* arc_name;
    FILE* arc;
    if(strcmp(argv[1], "--file") == 0) arc_name = argv[2];
    else printf("uncorect format of argument --file");
    int j = 4;
    if(strcmp(argv[3], "--create") == 0){
        while(j <= argc - 1){
            arc = fopen(arc_name, "ab");
            create_arc(arc, argv[j]);
            j++;
            fclose(arc);
        }

    } else
    if(strcmp(argv[3], "--extract") == 0){
        arc = fopen(arc_name, "rb");
        extract_arc(arc);
        fclose(arc);

    } else
    if(strcmp(argv[3], "--list") == 0){
        arc = fopen(arc_name, "rb");
        list_arc(arc);
        fclose(arc);
        return 0;
    } else printf("error of argument argv[3]");
    return 0;
}