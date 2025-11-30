#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <conio.h>

#include "input.h"
#include "myhashmap.h"
#include "utils.h"
#include "analysis.h"
#include "stats.h"
#include "output.h"

int main()
{
    // initialize 3 hashmaps (toxic word hashmap, unique word hashmap, stop word hashmap)
    struct hashMap *twhash = (struct hashMap *)malloc(sizeof(struct hashMap));
    struct hashMap *uwhash = (struct hashMap *)malloc(sizeof(struct hashMap));
    struct hashMap *swhash = (struct hashMap *)malloc(sizeof(struct hashMap));
    initializeHashMap(twhash, 64);
    initializeHashMap(uwhash, 256);
    initializeHashMap(swhash, 64);

    // declarations: file name, sentence buffer, token, delimiters
    char textfilename[128];
    char sentence_buff[MAX_SENTENCE_LENGTH];
    char *token;
    char delimiter[] = " :;,\n.?!{}[]()#@_/";

    // menu options, word search options
    int options;
    char searchoptions[MAX_LEN];

    // tatistic_per_sentence.csv file output
    FILE *outputptr;

    // buffer to store words for n_gram formation and analysis
    char n_gram_storage[MAX_SENTENCE_LENGTH][MAX_LEN];

    // buffer to store sentence lines from input text file
    char (*linebuffer)[MAX_SENTENCE_LENGTH] = calloc(MAX_LINES, sizeof(*linebuffer));

    // flags: analysis_is_done flag, program start/end flag, output csv of statitstic per line read
    int stat_per_line = 0;
    int analysis_flag = 0;
    int running = 1;

    // while program is running
    while (running)
    {
        printf(
            "Menu Options:\n\n"
            "1.To load text for analysis, enter \"1\"\n"
            "2.To analyze loaded text, enter \"2\"\n"
            "3.To display toxic words or unique word list, enter \"3\"\n"
            "4.For complete summary, enter \"4\"\n"
            "5.To save summary into a txt file, enter \"5\"\n"
            "6.To exit program, enter \"6\"\n\n"
            "Please enter your option:\n");

        // get user input for menu options
        scanf("%d", &options);
        // clear leftover '\n' from stdin
        getchar();

        switch (options)
        {

        case 1:
        {
            // reset analysis flag and count of lines loaded into buffer
            analysis_flag = 0;
            loadcount = 0;
            // clear screen
            system("cls");

            printf("Please enter file name of the text file (.csv / .txt) to be analyzed\n"
                   "Please make sure the the text file is located within the same directory as the program folder\n");

            fgets(textfilename, sizeof(textfilename), stdin);
            // remove trailing '\n'
            textfilename[strcspn(textfilename, "\n")] = 0;

            // update count -> numbers of lines read from input file
            loadcount = store_line_in_buffer(textfilename, linebuffer, MAX_LINES);

            // error encountered -> exit program
            if (loadcount < 0)
            {
                printf("Press any key to exit the program\n");
                getch();
                return 0;
            }

            printf("Press any key to go back to menu\n");
            getch();
            system("cls");
            break;
        }

        case 2:

            system("cls");

            if (loadcount == 0)
            {
                printf("No text file has been loaded.\n"
                       "Unable to analyze text.\n\n");

                printf("Press any key to go back to menu\n");
                getch();
                system("cls");
                break;
            }

            // reset values in word struct
            resetStruct(TWStruct, twline);
            resetStruct(WStruct, wline);

            // clear hashmap and free allocated memory
            clearHashMap(twhash);
            clearHashMap(uwhash);

            // reset counts
            totalwords = 0;
            totalsentences = 0;
            twline = 0;
            wline = 0;
            stat_per_line = 0;
            printf("Do you want to output a .csv file containing statistics for each line\n"
                   "If yes, enter \"1\" (Recommended) \n"
                   "If no, enter \"0\"\n\n");

            scanf("%d", &stat_per_line);
            getchar();
            system("cls");

            // load toxic words and stop words
            loadTWFileintoStructAndHash("Toxic Words List.txt", TWStruct, twhash, &twline);
            loadSWFileintoHash("Stop Words List.txt", swhash);

            printf("Analyzing text file...\n");

            outputptr = fopen("stats_per_line.csv", "w");
            fprintf(outputptr, "Sentence, tw_mild, tw_moderate, tw_severe, word_count, toxic_score\n");

            for (int i = 0; i < loadcount; i++)
            {
                // reset counts for each line read
                tw_mild = 0;
                tw_moderate = 0;
                tw_severe = 0;
                word_count = 0;
                toxic_score = 0;
                n_gram_count = 0;

                strcpy(sentence_buff, linebuffer[i]);
                fprintf(outputptr, "\"%s\",\n", sentence_buff);

                // convert every character in sentence to lower case
                for (int j = 0; sentence_buff[j]; j++)
                {
                    sentence_buff[j] = tolower(sentence_buff[j]);
                }

                // update total sentences count
                countSentences(sentence_buff);

                // tokenization process
                token = strtok(sentence_buff, delimiter);
                while (token)
                {
                    // checks if word contains unusual encodings, skip if true
                    if (is_Word(token) == 0)
                    {
                        token = strtok(NULL, delimiter);
                        continue;
                    }

                    // store words in n_gram_buffer and update n_gram_count
                    strcpy(n_gram_storage[n_gram_count], token);
                    n_gram_count++;

                    // checks if word is a stopword, toxic word ,repeated word or new word.
                    // update structures and hashmaps accordingly
                    // update total words count
                    if (search(swhash, token) == NULL)
                    {
                        if (search(uwhash, token) == NULL)
                        {
                            if (search(twhash, token) == NULL)
                            {
                                if (wline <= MAX_W_LINE)
                                {
                                    strcpy(WStruct[wline].word, token);
                                    WStruct[wline].frequency = 1;
                                    strcpy(WStruct[wline].severity, "None");
                                    insert(uwhash, token, 1, wline);
                                    wline++;
                                }
                            }
                            else
                            {
                                update(TWStruct, twhash, token);
                            }
                        }
                        else
                        {
                            update(WStruct, uwhash, token);
                        }
                    }
                    word_count++;
                    totalwords++;
                    token = strtok(NULL, delimiter);
                }
                analyze_n_gram(TWStruct, twhash, n_gram_storage, n_gram_count);

                // reset members in n_gram_buffer
                for (int i = 0; i < n_gram_count; i++)
                {
                    strcpy(n_gram_storage[i], "");
                }

                // printf sentence statistics to output csv file
                fprintf(outputptr, " , %d, %d, %d, %d, %d\n", tw_mild, tw_moderate, tw_severe, word_count, toxic_score);
            }

            fclose(outputptr);

            if (stat_per_line == 0)
                remove("stats_per_line.csv");

            // update analysis_is_done flag
            analysis_flag = 1;

            printf("Text analysis completed successfully.\n\n");

            printf("Press any key to go back to menu\n");
            getch();
            system("cls");
            break;

        case 3:

            system("cls");

            if (loadcount == 0)
            {
                printf("No text file has been loaded.\n"
                       "Unable to display word statistic.\n\n");

                printf("Press any key to go back to menu\n");
                getch();
                system("cls");
                break;
            }

            if (analysis_flag == 0)
            {
                printf("Loaded text file has not been analyzed.\n"
                       "Unable to display word statistic.\n\n");

                printf("Press any key to go back to menu\n");
                getch();
                system("cls");
                break;
            }

            // sort word structure in alphabetical order
            sortWordsByAlpha(TWStruct, twhash, twline);
            sortWordsByAlpha(WStruct, uwhash, wline);

            // display list of toxic words and unique words
            printf("Toxic Words:                                             Unique Words:\n\n");
            for (int i = 0, j = 0; i < twline || j < wline; i++, j++)
            {
                if (i < twline && j < wline)
                {
                    if (WStruct[j].frequency == 1)
                    {
                        printf("%-*s  %-*s\n\n", 55, TWStruct[i].word, 55, WStruct[j].word);
                    }

                    else
                        continue;
                }

                else if (j < wline)
                {
                    if (WStruct[j].frequency == 1)
                    {
                        printf("%57c%s\n\n", ' ', WStruct[j].word);
                    }

                    else
                        continue;
                }

                else if (i < twline)
                {
                    printf("%-*s\n\n", 55, TWStruct[i].word);
                }
            }

            // reset searchoptions to empty string
            searchoptions[0] = '\0';

            // Receive user input on which toxic word statistic to look up in the hashmap
            while (strcmp(searchoptions, "None") != 0)
            {
                printf("Enter toxic word to view statistic:\nEnter \"None\" to return to menu\n");
                if (fgets(searchoptions, sizeof(searchoptions), stdin) == NULL)
                {
                    break;
                }
                searchoptions[strcspn(searchoptions, "\n")] = '\0';
                display_Word_Stat(TWStruct, twhash, searchoptions);
            }

            searchoptions[0] = '\0';
            system("cls");
            break;

        case 4:

            system("cls");

            if (loadcount == 0)
            {
                printf("No text file has been loaded.\n"
                       "Unable to display general summary.\n\n");

                printf("Press any key to go back to menu\n");
                getch();
                system("cls");
                break;
            }

            if (analysis_flag == 0)
            {
                printf("Loaded text file has not been analyzed.\n"
                       "Unable to display general summary.\n\n");

                printf("Press any key to go back to menu\n");
                getch();
                system("cls");
                break;
            }

            // reset total words count
            totaltoxicwords = 0;
            totaluniquewords = 0;

            // sort word structure by frequency
            quicksort(TWStruct, twhash, 0, twline - 1);
            quicksort(WStruct, uwhash, 0, wline - 1);

            // caluclate toxic word percentage, lexical diversity index, average sentence length etc.
            calculateStatistics();

            printSummary();

            printf("Press any key to go back to menu\n");
            _getch();
            system("cls");
            break;

        case 5:

            system("cls");

            if (loadcount == 0)
            {
                printf("No text file has been loaded.\n"
                       "Unable to save analysis report file.\n\n");

                printf("Press any key to go back to menu\n");
                getch();
                system("cls");
                break;
            }

            if (analysis_flag == 0)
            {
                printf("Loaded text file has not been analyzed.\n"
                       "Unable to save analysis report file.\n\n");

                printf("Press any key to go back to menu\n");
                getch();
                system("cls");
                break;
            }

            savesummary();

            printf("Press any key to go back to menu\n");
            _getch();
            system("cls");
            break;

        case 6:
            // reset and clear every structure, hashmap and buffer used.
            resetStruct(TWStruct, twline);
            resetStruct(WStruct, wline);

            clearHashMap(twhash);
            clearHashMap(uwhash);
            clearHashMap(swhash);

            free(linebuffer);

            // close program
            running = 0;
            return 0;

        default:
            // handle cases fror invalid option input from user
            printf("Invalid option. Please try again.\n");
            system("cls");
            break;
        }
    }
}
