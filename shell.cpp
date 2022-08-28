#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <dirent.h>
#include <bits/stdc++.h>
using namespace std;

#define MAX_INPUT_SIZE 1024
#define MAX_TOKEN_SIZE 64
#define MAX_NUM_TOKENS 64

// TODO: YOUR CODE HERE
// INITIALIZE DATA STRUCTURE TO STORE PIDS OF PROCESSES TO KILL LATER

/* Splits the string by space and returns the array of tokens
 *
 */

const int aSize = 26;

// trie node
struct TrieNode
{
    struct TrieNode *children[aSize];
    bool isEnd;
};

// Returns new trie node (init to NULLS)
struct TrieNode *
getNode(void)
{
    struct TrieNode *parentNode = new TrieNode;
    parentNode->isEnd = false;

    for (int i = 0; i < aSize; i++)
    {
        parentNode->children[i] = NULL;
    }
    return parentNode;
}

void insert(struct TrieNode *root, char *key)
{
    struct TrieNode *currNode = root;

    for (int i = 0; key[i] != '\0'; i++)
    {
        int idx = key[i] - 'a';
        if (!currNode->children[idx])
        {
            currNode->children[idx] = getNode();
        }

        currNode = currNode->children[idx];
    }
    currNode->isEnd = true;
}

bool search(struct TrieNode *root, char *key)
{
    struct TrieNode *currNode = root;

    for (int i = 0; key[i] != '\0'; i++)
    {
        int idx = key[i] - 'a';
        if (!currNode->children[idx])
        {
            return false;
        }
        currNode = currNode->children[idx];
    }
    if (!currNode->isEnd)
    {
        return false;
    }
    return true;
}

bool isLeafNode(struct TrieNode *root)
{
    for (int i = 0; i < aSize; i++)
    {
        if (root->children[i])
        {
            return false;
        }
    }
    return true;
}

void print(struct TrieNode *root, char *accum, int len)
{
    if (root->isEnd)
    {
        printf("%s\n", accum);
    }
    if (isLeafNode(root))
    {
        return;
    }

    for (int i = 0; i < aSize; i++)
    {
        if (root->children[i])
        {
            char *newString = (char *)malloc(100 * sizeof(char *));
            strcpy(newString, accum);
            newString[len] = 'a' + i;
            newString[len + 1] = '\0';

            print(root->children[i], newString, len + 1);
        }
    }

    return;
}

void suggest_dfs(struct TrieNode *root, char *key)
{
    struct TrieNode *currNode = root;
    int len = 0;
    while (key[len] != '\0')
    {
        len++;
    }
    // len++;
    for (int i = 0; key[i] != '\0'; i++)
    {
        int idx = key[i] - 'a';
        if (!currNode->children[idx])
        {
            printf("No suggestions found. Please check the string and try again!\n");
            return;
        }
        currNode = currNode->children[idx];
    }
    print(currNode, key, len);
}

char **tokenize(char *line)
{

    // tokenizer variables
    char **tokens = (char **)malloc(MAX_NUM_TOKENS * sizeof(char *));
    char *token = (char *)malloc(MAX_TOKEN_SIZE * sizeof(char));
    int i, tokenIndex = 0, tokenNo = 0;

    // loop on length of line
    for (i = 0; i < strlen(line); i++)
    {

        char readChar = line[i];

        // tokenize on any kind of space
        if (readChar == ' ' || readChar == '\n' || readChar == '\t')
        {
            token[tokenIndex] = '\0';
            if (tokenIndex != 0)
            {
                tokens[tokenNo] = (char *)malloc(MAX_TOKEN_SIZE * sizeof(char));
                strcpy(tokens[tokenNo++], token);
                tokenIndex = 0;
            }
        }
        else
        {
            token[tokenIndex++] = readChar;
        }
    }

    free(token);
    tokens[tokenNo] = NULL;
    return tokens;
}

void fire_suggest(char **tokens)
{

    printf("inside fire_suggest\n");
    int numEl = 0;
    while ((tokens[numEl] != NULL) && (strcmp(tokens[numEl], "&&")) && (strcmp(tokens[numEl], "&&&")))
    {
        numEl++;
        continue;
    }

    if (numEl != 2)
    {
        printf("Error : Too many or too few arguments. Expected 1, got %d\n", numEl - 1);
        return;
    }

    // create trie
    struct TrieNode *root = getNode();

    char *buff = (char *)malloc(100 * sizeof(char));

    char **filenames = (char **)malloc(200 * sizeof(char *));
    int fileno = 0;
    char *filename = (char *)malloc(100 * sizeof(char));

    char *query = tokens[1];
    char *cwd = getwd(buff);
    strcat(cwd, "/");
    // strcat(cwd, filename);
    printf("You've requested the directory %s\n\n", cwd);
    DIR *target_dir = opendir(cwd);
    struct dirent *dir;

    while ((dir = readdir(target_dir)) != NULL)
    {
        printf("%s\n", dir->d_name);
        filenames[fileno++] = dir->d_name;
        // insert filename in trie
        insert(root, dir->d_name);
    }
    filenames[fileno] = NULL;

    closedir(target_dir);

    printf("Efficiently printing suggestions for your query %s!\n", query);
    suggest_dfs(root, query);

    return;
}

void fire_sleep(char **tokens)
{

    printf("inside fire_sleep\n");
    int numEl = 0;
    while ((tokens[numEl] != NULL) && (strcmp(tokens[numEl], "&&")) && (strcmp(tokens[numEl], "&&&")))
    {
        numEl++;
        continue;
    }

    if (numEl != 2)
    {
        printf("Error : Too many or too few arguments. Expected 1, got %d\n", numEl - 1);
        return;
    }
    int sleeptime = atoi(tokens[1]);
    printf("Going to sleep for %d... zzz...\n", sleeptime);
    sleep(sleeptime);
    printf("... zzz... Woke up after %d!\n", sleeptime);

    return;
}

void fire_ls(char **tokens)
{

    printf("inside fire_ls\n");
    int numEl = 0;
    while ((tokens[numEl] != NULL) && (strcmp(tokens[numEl], "&&")) && (strcmp(tokens[numEl], "&&&")))
    {
        numEl++;
        continue;
    }

    if (numEl != 2)
    {
        printf("Error : Too many or too few arguments. Expected 1, got %d\n", numEl - 1);
        return;
    }
    printf("Here!\n");
    // int rc = fork();
    char *buff = (char *)malloc(100 * sizeof(char));
    char *filename = tokens[1];
    char *cwd = getwd(buff);
    strcat(cwd, "/");
    strcat(cwd, filename);
    printf("You've requested the directory %s\n\n", cwd);
    DIR *target_dir = opendir(cwd);
    struct dirent *dir;
    if (target_dir)
    {
        while ((dir = readdir(target_dir)) != NULL)
        {
            printf("%s\n", dir->d_name);
        }
        closedir(target_dir);
    }
    else
    {
        printf("Invalid path\n");
    }

    return;
}
void fire_cd(char **tokens)
{
    int numEl = 0;
    while ((tokens[numEl] != NULL) && (strcmp(tokens[numEl], "&&")) && (strcmp(tokens[numEl], "&&&")))
    {
        numEl++;
        continue;
    }
    // numEl--;
    if (numEl != 2)
    {
        printf("Error : Too many or too few arguments. Expected 1, got %d\n", numEl - 1);
        return;
    }

    char *buff = (char *)malloc(100 * sizeof(char));
    char *dirname = tokens[1];

    char *newdir = getwd(buff);
    strcat(newdir, "/");
    strcat(newdir, dirname);

    printf("You've requested the directory %s\n\n", newdir);
    chdir(newdir);
    printf("Directory changed to %s! Check it out by running ls.\n", (char *)newdir);

    return;
}
// TODO
// MAKE FUNCTIONS AS REQUIRED TO AVOID REPETITIVE WORK

// void signal_handler(int signum){
//     continue;
// }

void signal_handler(int signum)
{
    printf("\nExiting the shell\n");
    exit(0);
}

void fire(char **tokens)
{

    printf("Inside fire\n");
    if (tokens[0] == NULL)
    {
        return;
    }
    if (!strcmp(tokens[0], "ls"))
    {
        fire_ls(tokens);
    }
    if (!strcmp(tokens[0], "cd"))
    {
        fire_cd(tokens);
    }
    if (!strcmp(tokens[0], "sleep"))
    {
        fire_sleep(tokens);
    }
    if (!strcmp(tokens[0], "suggest"))
    {
        fire_suggest(tokens);
    }
}

void par_exec(char **tokens)
{

    printf("inside par_exec\n");
    int i = 0;
    if (fork() == 0)
    {
        fire(tokens);
        exit(0);
    }
    while (tokens[i] != NULL)
    {
        if (!strcmp(tokens[i], "&&&"))
        {
            if (fork() == 0)
            {
                fire(tokens + i + 1);
                exit(0);
            }
        }
        i++;
    }
    // exit(0);
}

void seq_exec(char **tokens)
{

    printf("inside seq_exec\n");
    int i = 0;
    fire(tokens);

    while (tokens[i] != NULL)
    {
        if (!strcmp(tokens[i], "&&"))
        {
            fire(tokens + i + 1);
        }
        i++;
    }

    return;
}

int main(int argc, char *argv[])
{

    char line[MAX_INPUT_SIZE];
    char **tokens;

    // TODO: YOUR CODE HERE
    // INITIALIZE GLOBAL DATA STRUCTURE OF PIDS TO SOME DEFAULT VALUE
    // INITIALIZE OTHER VARIABLES AS NEEDED
    signal(SIGINT, SIG_IGN);
    signal(SIGQUIT, signal_handler);
    while (1)
    {

        /* BEGIN: TAKING INPUT */
        bzero(line, sizeof(line));
        printf("$ ");
        scanf("%[^\n]", line);
        getchar();
        /* END: TAKING INPUT */
        line[strlen(line)] = '\n'; // terminate with new line
        tokens = tokenize(line);

        int i = 0;
        while ((tokens[i] != NULL) && strcmp(tokens[i], "&&") && strcmp(tokens[i], "&&&"))
        {
            i++;
        }
        if (tokens[i] == NULL)
        {
            fire(tokens);
        }
        else if (!strcmp(tokens[i], "&&"))
        {
            seq_exec(tokens);
        }
        else
        {
            par_exec(tokens);
        }

        // TODO: YOUR CODE HERE

        // freeing the memory
        for (int i = 0; tokens[i] != NULL; i++)
        {
            free(tokens[i]);
        }

        free(tokens);
    }
    return 0;
}
