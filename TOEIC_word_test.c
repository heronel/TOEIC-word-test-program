// TOEIC word test program Ver. 1.5.
// Made by yeohj0710 (Heronel)

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<time.h>
#include<conio.h>
#include<io.h>
#define LINE 50
#define MAX 5000
#define SCREENLINE 13

int word_count, check_count, removed_count, rotation_count;
char file_name[50], temp_line[100], print_line[30][100], rotation_count_string[10], next_key = '/', remove_key = '.';

void initailize_screen() {
    for(int i=0; i<=SCREENLINE; i++) print_line[i][0] = '\0';
}

void build_screen() {
    for(int i=0; i<SCREENLINE; i++) strcpy(print_line[i], print_line[i+1]);
    strcpy(print_line[SCREENLINE], temp_line);
    temp_line[0] = '\0';
}

void print_screen() {
    system("cls");
    printf("\n   %s ", file_name);
    // for(int i=0; i<13-strlen(file_name); i++) printf(" ");
    printf("| processivity : %2d/%2d | Rotation Count : %2d\n\n", removed_count, word_count, rotation_count);
    printf("   %c : Next word | %c : Remove word  | esc : Back to main menu\n\n", next_key, remove_key);
    for(int i=0; i<=SCREENLINE; i++) printf("%s\n", print_line[i]);
    printf("\n     ");
    for(int i=0; i<55; i++) printf("-");
    printf("\n");
}

int main() {
    int random, input, check[MAX], removed[MAX], esc, all_mode;
    char word_eng[MAX][LINE], word_kor[MAX][LINE];
    struct _finddata_t file_data; long handle; int result;
    srand(time(NULL));
    FILE *fp, *settings;

    while(1) { // program

        word_count = 0, random = 0, input = 0, check_count = 0, removed_count = 0, rotation_count = 0, esc = 0, all_mode = 0;
        temp_line[0] = '\0', initailize_screen();

        for(int i=0; i<MAX; i++) {
            check[i] = 0;
            removed[i] = 0;
            word_eng[i][0] = '\0';
            word_kor[i][0] = '\0';
        }

        settings = fopen("./Settings/Settings.txt", "r"); // key settings
        if(settings != NULL) {
            fscanf(settings, "Next Key : %c\n", &next_key);
            fscanf(settings, "Remove Key : %c", &remove_key);
            fclose(settings);
        }

        do { // load file
            system("cls");

            result = 1; // find txt files
            handle = _findfirst(".\\*.txt",&file_data);
            if (handle == -1) {
                printf("\n   There are no test(.txt) files!!\n\n");
                return 0;
            }
            printf("\n   [Test File Lists]\n\n");
            for(int i=0; result!=-1; i++) {
                strcpy(temp_line, file_data.name);
                temp_line[strlen(temp_line)-4] = '\0';
                printf("   - %s ", file_data.name);
                if(!file_data.size) printf("(Empty)");
                else {
                    int j;
                    fp = fopen(file_data.name, "r");
                    for(j=0; !feof(fp); j++) fgets(temp_line, LINE, fp);
                    printf("(%d words)", j/2);
                    fclose(fp);
                }
                temp_line[0] = '\0';
                if(i%2) printf("\n");
                else for(int j=0; j<20-strlen(file_data.name); j++) printf(" ");
                result = _findnext(handle, &file_data);
            }
            _findclose(handle);

            printf("\n\n   If you input \"all\", You can take all the words in one test.\n");
            printf("\n   Which file do you want to test? (without .txt) : ");
            gets(file_name);
            if(!strcmp(file_name, "all")) {
                all_mode = 1;
                break;
            }
            strcat(file_name, ".txt");
            fp = fopen(file_name, "r");
        } while(fp == NULL);

        if(all_mode) { // for "all mode"
            result = 1; // find txt files
            handle = _findfirst(".\\*.txt",&file_data);
            for(int i=0; result!=-1; i++) {
                fp = fopen(file_data.name, "r");
                while(!feof(fp)) { // arrange words
                    fgets(word_eng[word_count], LINE, fp);
                    fgets(word_kor[word_count], LINE, fp);
                    word_eng[word_count][strlen(word_eng[word_count])-1] = '\0';
                    word_kor[word_count][strlen(word_kor[word_count])-1] = '\0';
                    word_count++;
                }
                fclose(fp);
                result = _findnext(handle, &file_data);
            }
        }
        else { // for normal input (file name)
            while(!feof(fp)) { // arrange words
                fgets(word_eng[word_count], LINE, fp);
                fgets(word_kor[word_count], LINE, fp);
                word_eng[word_count][strlen(word_eng[word_count])-1] = '\0';
                word_kor[word_count][strlen(word_kor[word_count])-1] = '\0';
                word_count++;
            }
        }

        print_screen();
        while(1) { // each test (each word)

            if(removed_count >= word_count) {
                printf("\n%42s", "TEST OVER!!");
                Sleep(3000);
                break;
            }

            do { // check duplication or removal
                random = rand()%word_count;
            } while(check[random] || removed[random]);
            build_screen(); build_screen();
            for(int i=0; i<30-strlen(word_eng[random]); i++) strcat(temp_line, " ");
            strcat(temp_line, word_eng[random]);
            printf("\n%30s  ", word_eng[random]);

            while(1) { // get input
                input = getch();
                if(input == next_key) break;
                else if(input == 27) {
                    esc = 1;
                    break;
                }
                else if(input == remove_key) {
                    removed[random] = 1;
                    break;
                }
            }
            if(esc) break;

            strcat(temp_line, "  "); strcat(temp_line, word_kor[random]);
            for(int i=0; i<30-strlen(word_kor[random]); i++) strcat(temp_line, " ");

            if(removed[random]) strcat(temp_line, "\t"), strcat(temp_line, "Removed!!"), removed_count++;
            else check[random] = 1, check_count++;
            build_screen(); print_screen();

            if(check_count >= word_count - removed_count) {
                for(int i=0; i<word_count; i++) check[i] = 0;
                rotation_count++;
                itoa(rotation_count, rotation_count_string, 10);
                print_line[1][55] = '\0';
                strcat(print_line[1], rotation_count_string);
                check_count = 0;
            }
        }

    }
}
