#include <stdlib.h>
#include <string.h>
#include <stdio.h>

//Ryan Schlosbon - COP3502C - 4/19/2024 - Tries

// Trie structure
struct Trie
{
    struct Trie *children[26]; // Assuming only lowercase alphabets
    int isEndOfWord;
};

struct Trie *createTrie();

// Inserts the word to the trie structure
void insert(struct Trie *pTrie, char *word)
{
    struct Trie *curr = pTrie;
    for (int i = 0; word[i] != '\0'; i++) //while the word exists
    {
        int index = word[i] - 'a';
        if (!curr->children[index])
        {
            curr->children[index] = createTrie();  //create a trienode at the index in array of pointers
        }
        curr = curr->children[index]; //move the pointer forward
    }
    curr->isEndOfWord = 1;
}

// Computes the number of occurrences of the word
int numberOfOccurrences(struct Trie *pTrie, char *word)
{
    struct Trie *curr = pTrie;
    int counter = 0;
    for (int i = 0; word[i] != '\0'; i++) //while the word exists
    {
        int idx = word[i] - 'a'; //index of word
        if (!curr->children[idx])
        {
            break; //if the word does not exist, break
        }
        curr = curr->children[idx]; 
        counter++;
    }
    return counter;
}

// Deallocate the trie structure
void deallocateTrie(struct Trie *pTrie)
{
    if (!pTrie)
    {
        return;
    }
    for (int i = 0; i < 26; i++)
    {
        deallocateTrie(pTrie->children[i]); //deallocate the trie node 
    }
    free(pTrie); //free the root
}

// Initializes a trie structure
struct Trie *createTrie()
{
    struct Trie *newNode = (struct Trie *)malloc(sizeof(struct Trie)); //allocate a new trienode
    if (newNode) //if newNode exists,
    {
        newNode->isEndOfWord = 0;
        for (int i = 0; i < 26; i++) //sets each word to null in the trie
        {
            newNode->children[i] = NULL;
        }
    }
    return newNode;
}

int readDictionary(char *filename, char ***pInWords) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) { //check for file opening error
        perror("Error opening file");
        return -1;
    }

    char str[100];
    int counter = 0;
    
    while (fscanf(file, "%s", str) == 1) {
        counter++;
    }

    //Allocate memory for words
    *pInWords = (char **)malloc(counter * sizeof(char *));
    rewind(file); //rewinds file pointer to beginning

    int i = 0;
    while (fscanf(file, "%s", str) == 1) { //while there is still a word present
        (*pInWords)[i] = strdup(str);
        i++;
    }

    fclose(file);
    return counter;
}

int main(void)
{
  char *inWords[256];
  for (int i = 0; i < 256; i++) {
      inWords[i] = malloc(100 * sizeof(char)); // Allocating memory for each word
  }
  // Read the number of words in the dictionary
  int numWords = readDictionary("dictionary.txt", &inWords);
    struct Trie *pTrie = createTrie();
    for (int i = 0; i < numWords; i++)
    {
        insert(pTrie, inWords[i]);
    }

    // Words to check for occurrences
    char *pWords[] = {"notaword", "ucf", "no", "note", "corg"};
    for (int i = 0; i < 5; i++)
    {
        printf("\t%s : %d\n", pWords[i], numberOfOccurrences(pTrie, pWords[i]));
    }

    // Deallocate Trie
    deallocateTrie(pTrie);

    return 0;
}