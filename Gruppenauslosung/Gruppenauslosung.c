// Gruppenarbeit 1
// Gruppe "Fantastische Nichtskönner"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

#define MAX_STRING_SIZE 64

// ===== Start Structures ======

struct element
{
    int index;
    char inhalt[MAX_STRING_SIZE];
    struct element *next;
};

typedef struct
{
    int gr_type1;
    int quantity1;
    int gr_type2;
    int quantity2;
} type_options;

struct element *anker = NULL;
struct element *zeiger = NULL;

//====== End Structers ======

//====== Start Prototyps ======

int numberOfLines(char *filepath);

void ausgabe(void);
void anhaengen(char mail[MAX_STRING_SIZE], int index);
void loeschen(int index);
char *getElementAtIndex(int index);

void groupSuggestion(type_options *options, int option_size, int groupSize, int numberOfMails);
void groupSize(type_options *options, int option_size, int array_size);
void printGroups(char *path, char *randomMails[], int numberOfStudents, type_options *options, int selectedOption);

//====== End Prototyps ========

int main(int argc, char *argv[])
{
    char *pathToMails = argv[1];
    char *pathToCSV = argv[2];

    int countLines = numberOfLines(argv[1]) + 1;
    printf("Insgesamte Personen: %i\n", countLines);

    type_options options[5];
    int option_size = sizeof(options) / (sizeof(int) * 4);

    FILE *file = fopen(argv[1], "r"); //"r" for read
    if (file == NULL)
    {
        perror("Unable to open file!");
        return 1;
    }
    else
    {
        for (int i = 0; i < countLines; i++)
        {
            char line[MAX_STRING_SIZE] = " ";
            //Read string from file
            fscanf(file, "%s", line);
            //Save string in linked list
            anhaengen(line, i);
        }
        fclose(file);

        //Initialize random generator
        int numberOfStudents = countLines;
        char *randomMails[numberOfStudents];
        char mail[MAX_STRING_SIZE];

        for (int number = 0; number < countLines; number++)
        {
            time_t t;
            srand((unsigned)time(&t));
            int zufallszahl = rand() % (numberOfStudents - number); //zahl [0,numberofstudents - number[

            strcpy(mail, getElementAtIndex(zufallszahl));

            randomMails[number] = malloc(strlen(mail) + 1);
            strcpy(randomMails[number], mail);

            loeschen(zufallszahl);
        }

        int choosen_option;

        do
        {
            groupSize(options, option_size, countLines);

            printf("\nBitte waehlen sie eine Nummer (Falls neue Gruppengroesse die 0 eintippen): ");
            scanf("%i", &choosen_option);

        } while (choosen_option == 0);

        printGroups(pathToCSV, randomMails, numberOfStudents, options, choosen_option - 1);
        printf("\nCSV wurde in %s gespeichert!\n", pathToCSV);
    }

    system("PAUSE");
    return 0;
}

void anhaengen(char mail[MAX_STRING_SIZE], int index)
{
    // Haengt eine weitere Mail an die Verkette Liste
    struct element *zgr;
    if (anker == NULL)
    {
        if ((anker = malloc(sizeof(struct element))) == NULL)
        {
            fprintf(stderr, "Kein Speicherplatz vorhanden.");
            return;
        }
        strcpy(anker->inhalt, mail);
        anker->index = index;
        anker->next = NULL;
    }
    else
    {
        zeiger = anker;
        while (zeiger->next != NULL)
        {
            zeiger = zeiger->next;
        }
        zeiger->next = malloc(sizeof(struct element));
        zeiger = zeiger->next; //Zeiger auf neuen Speicherplatz
        strcpy(zeiger->inhalt, mail);
        zeiger->index = index;
        zeiger->next = NULL;
    }
}

void loeschen(int index)
{
    // Loescht eine Mail am index aus der Verkette Liste

    struct element *zeiger, *zeiger1;
    //Ist überhaupt ein Element vorhanden?
    if (anker != NULL)
    {
        //Ist das erste Element das zu loeschende Element?
        if (index == 0)
        {
            zeiger = anker->next;
            free(anker);
            anker = zeiger;
        }
        else
        {
            zeiger = anker;

            int indexCounter = 1;
            while (zeiger->next != NULL)
            {
                zeiger1 = zeiger->next;
                if (indexCounter == index)
                {
                    break;
                }
                //Die Zuweisung ist für die Schleife
                zeiger = zeiger1;
                indexCounter++;
            }
            zeiger->next = zeiger1->next;
            // printf("Zeiger: %x\n", zeiger->next);
            // printf("Zeiger1: %x\n", zeiger1 ->next);
            free(zeiger1);
        }
    }
    else
    {
        printf("Es sind keine Daten zum Löschen da.");
    }
}

char *getElementAtIndex(int index)
{
    // Gibt den Inhalt eins Elements am index zurück
    if (anker != NULL)
    {
        zeiger = anker;
        int zeigerCounter = 0;
        while (zeiger->next != NULL)
        {
            if (zeigerCounter == index)
            {
                return zeiger->inhalt;
            }
            zeiger = zeiger->next;
            zeigerCounter++;
        }
        return zeiger->inhalt;
    }
    else
    {
        printf("Es gibt keine Elemente.");
    }
}

void ausgabe(void)
{
    // Zeigt alle vorhandenen Elemente auf der Konsole an
    struct element *zeiger = anker;
    while (zeiger != NULL)
    {
        printf("(%d)\t%s \n", zeiger->index, zeiger->inhalt);
        zeiger = zeiger->next;
    }
}

int numberOfLines(char *filepath)
{
    // Ermittelt die Anzahl an New Lines in der Datei

    FILE *fileptr;
    int count_lines = 0;
    char chr;

    fileptr = fopen(filepath, "r");
    //extract character from file and store in chr
    chr = getc(fileptr);
    while (chr != EOF)
    {
        //Count whenever new line is encountered
        if (chr == '\n')
        {
            count_lines += 1;
        }
        //take next character from file.
        chr = getc(fileptr);
    }
    fclose(fileptr); //close file.

    return count_lines;
}

void groupSuggestion(type_options *options, int option_size, int groupSize, int numberOfMails)
{
    // Ermittel der Optionen falls die eingegebene Gruppengroesse kein Teiler der Kursgroesse ist
    int restOfMails = numberOfMails % groupSize;
    int numberOfGroups = (numberOfMails - restOfMails) / groupSize;

    options[0].quantity1 = numberOfGroups;
    options[0].gr_type1 = groupSize;

    options[0].quantity2 = numberOfGroups;
    options[0].gr_type2 = groupSize;

    int toggel = 0;
    int quantityCounter = 1;

    switch (restOfMails)
    {
    case 1:
        options[0].gr_type1 = options[0].gr_type1 + 1;
        options[0].quantity1 = 1;
        options[0].quantity2 = options[0].quantity2 - 1;
        break;
    case 2:
        options[0].gr_type1 = options[0].gr_type1;
        options[0].quantity1 = options[0].quantity1 - 1;
        options[0].gr_type2 = options[0].gr_type1 + restOfMails;
        options[0].quantity2 = 1;
        break;
    case 3:
        options[0].gr_type1 = options[0].gr_type1;
        options[0].quantity1 = options[0].quantity1 - 2;
        options[0].gr_type2 = options[0].gr_type1 + restOfMails;
        options[0].quantity2 = 2;
        break;
    case 4:
        options[0].gr_type1 = options[0].gr_type1;
        options[0].quantity1 = options[0].quantity1 - 3;
        options[0].gr_type2 = options[0].gr_type1 + restOfMails;
        options[0].quantity2 = 3;
        break;

    default:
        options[0].quantity1 = 0;
        options[0].gr_type1 = 0;

        options[0].quantity2 = 0;
        options[0].gr_type2 = 0;
        printf("\n!!! ERROR bei Option 1 !!!\n");
        break;
    }

    options[1].gr_type1 = groupSize;
    options[1].quantity1 = numberOfGroups;

    options[1].gr_type2 = restOfMails;
    options[1].quantity2 = 1;

    for (int i = 0; i < 2; i++)
    {
        printf("\nOption %i: \n%i Gruppen mit je %i Personen und %i Gruppen mit je %i Personen\n", i + 1, options[i].quantity1, options[i].gr_type1, options[i].quantity2, options[i].gr_type2);
    }
}

void groupSize(type_options *options, int option_size, int array_size)
{
    // Ermittel ob die Gruppengroesse ein Teiler der Kursgroesse ist, ansonsten werden alternativen angeboten
    int group_size;
    int rest;
    printf("\nBitte die Gruppengroesse eingeben: ");
    scanf("%d", &group_size);
    rest = array_size % group_size;
    if (rest != 0)
    {
        printf("\nDie eingegebene Gruppengroesse ist kein Teiler der Kursgroesse!\n");
        printf("\n---------------------------------------------------------------\n");
        printf("Vorschlaege: \n");
        groupSuggestion(options, option_size, group_size, array_size);
    }
    else
    {
        printf("%d Gruppen der Groesse %d: \n", array_size / group_size, group_size); //Optional - to test it set "array_size" to an even number or use another file with even number of lines!
    }
}

void printGroups(char *path, char *randomMails[], int numberOfStudents, type_options *options, int selectedOption)
{
    // Ausgeben der Gruppen auf dem Terminal und der CSV Datei
    int numberOfGroups = options[selectedOption].quantity1 + options[selectedOption].quantity2;

    FILE *csv = fopen(path, "a");
    int mailCounter = 0;
    printf("\n");
    for (int i = 0; i < options[selectedOption].quantity1; i++)
    {
        fprintf(csv, "Gruppe %i;", i + 1);
        printf("=== Gruppe %-i ===\n", i + 1);

        for (int j = 0; j < options[selectedOption].gr_type1 - 1; j++)
        {
            fprintf(csv, "%s;", randomMails[mailCounter]);
            printf("%-s\n", randomMails[mailCounter]);

            mailCounter++;
        }
        fprintf(csv, "%s\n", randomMails[mailCounter]);
        printf("%-s\n\n", randomMails[mailCounter]);
        mailCounter++;
    }

    for (int i = 0; i < options[selectedOption].quantity2; i++)
    {
        fprintf(csv, "Gruppe %i;", i + 1 + options[selectedOption].quantity1);
        printf("=== Gruppe %-i ===\n", i + 1 + options[selectedOption].quantity1);

        for (int j = 0; j < options[selectedOption].gr_type2 - 1; j++)
        {
            fprintf(csv, "%s;", randomMails[mailCounter]);
            printf("%-s\n", randomMails[mailCounter]);
            mailCounter++;
        }
        fprintf(csv, "%s\n", randomMails[mailCounter]);
        printf("%-s\n\n", randomMails[mailCounter]);
        mailCounter++;
    }
    fclose(csv);
}
