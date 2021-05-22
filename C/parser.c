#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include"stack.h"

// html tag tokens
#define HTML_OT 1
#define HTML_CT 2
#define HEAD_OT 3
#define HEAD_CT 4
#define BODY_OT 5
#define BODY_CT 6
#define TITLE_OT 7
#define TITLE_CT 8
#define H1_OT 9
#define H1_CT 10
#define H2_OT 11
#define H2_CT 12
#define H3_OT 13
#define H3_CT 14
#define P_OT 15
#define P_CT 16
#define UL_OT 17
#define UL_CT 18
#define LI_OT 19
#define LI_CT 20
#define DIV_OT 21
#define DIV_CT 22
#define A_OT 23
#define A_CT 24
#define BR 25
#define HR 26
#define TEXT 27

#define NUM_OF_TAGS 26
#define NUM_OF_SINGLE_FREQ_TAGS 4

const char* tags[NUM_OF_TAGS] = {"<html>", "</html>", "<head>", "</head>", "<body>", "</body>", "<title>", "</title>", "<h1>", "</h1>", "<h2>", "</h2>", 
                        "<h3>", "</h3>", "<p>", "</p>", "<ul>", "</ul>", "<li>", "</li>", "<div>", "</div>", "<a>", "</a>", "<br>", "<hr>"};
const int singleFreqTags[NUM_OF_SINGLE_FREQ_TAGS] = {HTML_OT, HEAD_OT, TITLE_OT, BODY_OT};

static int tagTokenSize = 100;
static int currentTagSize = 100;
static int numOfTokens = 0;

int errorDetected() {
    printf("PARSING FAILED: 'file.html' is not a valid HTML file.\n");
    exit(1);
    return -1;
}

// --- LEXING FUNCTIONS ---

int isSpace(char c) {
    return (c == ' ' || c == '\n' || c == '\t');
}

// tokenises and stores tag
int storeTagToken(int tagTokens[], int tagTokenCounter, char currentTag[]) {
    if (*currentTag != '<') {
        tagTokens[tagTokenCounter] = TEXT;
        return ++tagTokenCounter;
    }
    for (int i = 0; i < NUM_OF_TAGS; i ++) {
        if (strcmp(currentTag, tags[i]) == 0) {
            tagTokens[tagTokenCounter] = i + 1;
            return ++tagTokenCounter;
        }
    }
    return errorDetected();
}

// checks for reallocation of memory - currentTag
char * checkCurrentTagPtrMemory(char *currentTag, int currentTagCounter) {
    if (currentTagCounter == currentTagSize) {
        currentTag = realloc(currentTag, currentTagSize * 2 * sizeof(char));
        currentTagSize *= 2;
        memset(&currentTag[currentTagCounter], 0, (currentTagSize - currentTagCounter) * sizeof(char));
    } 
    return currentTag;
}

// checks for reallocation of memory - tagTokens
int * checkTokenTagPtrMemory(int *tagTokens, int tagTokenCounter) {
    if (tagTokenCounter == tagTokenSize) {
        tagTokens = realloc(tagTokens, tagTokenSize * 2 * sizeof(int));
        tagTokenSize *= 2;
    } 
    else if (tagTokenCounter < (tagTokenSize / 2)) {
        tagTokens = realloc(tagTokens, (tagTokenSize / 2) * sizeof(int));
        tagTokenSize /= 2;
    }
    return tagTokens;
}

int resetCurrentTag(char * currentTag) {
    memset(currentTag, 0, strlen(currentTag));
    return 0;
}

void checkIfLastTag(char * currentTag) {
    if (currentTag[0] != '\0') {
        errorDetected();
    }
}

// --- PARSING FUNCTIONS ---

// checks file starts and ends with html tags
void isWrappedByHtml(int *tagTokenPtr, int counter) {
    int *tempPtr = tagTokenPtr;
    if ((counter == 0 && *tagTokenPtr != HTML_OT) || (*(++tempPtr) == 0 && *tagTokenPtr != HTML_CT)) {
        errorDetected();
    } 
}

// updates frequency of single freq tags
void updateTagFreq(int tagToken, int tagFreq[]) {
    for (int i = 0; i < NUM_OF_SINGLE_FREQ_TAGS; i ++) {
        if (tagToken == singleFreqTags[i]) {
            tagFreq[i] += 1;
        }
    }
}

// validates frequency of single freq tags
int tagFreqIsValid(int tagFreq[]) {
    return (tagFreq[0] == 1 && tagFreq[1] == 1 && tagFreq[2] < 2 && tagFreq[3] == 1);
}

int updateInP(int inP, int tagToken) {
    if (tagToken == P_OT) {
        return 1;
    } else if (tagToken == P_CT) {
        return 0;
    } else {
        return inP;
    }
}

int isHeadFollowedByBody(int tagToken, int headJustSeen) {
    if (headJustSeen) {
        if (tagToken == BODY_OT) {
            return 0;
        } else if (tagToken == TEXT) {
            return 1;
        } else {
            return errorDetected();
        }
    }
    return 0;
}

void isNestingValid(struct Stack* stack, int tagToken, int inP) {
    if ((inP && (tagToken == P_OT || tagToken == DIV_OT)) || (lastValue(stack) == HEAD_OT && tagToken != TITLE_OT) || 
        (tagToken == TITLE_OT && lastValue(stack) != HEAD_OT) || (lastValue(stack) == TITLE_OT) || 
        (lastValue(stack) == HTML_OT && tagToken != HEAD_OT && tagToken != BODY_OT)) {
        errorDetected();
    }
}

// checks every opening tag has corresponding closing tag
int operateOnStack(struct Stack* stack, int tagToken, int inP) {
    if (tagToken % 2 != 0) {
        isNestingValid(stack, tagToken, inP);
        push(stack, tagToken);
        return 0;
    } else {
        int corrClosingTag = tagToken - 1;
        if (corrClosingTag == lastValue(stack)) {
            pop(stack);
            return 0;
        }
    }
    return 1;
}

// --- LEXING ---
int * executeLexing(FILE* inputFile) {
    int *tagTokens = (int*) malloc(tagTokenSize * sizeof(int));
    memset(tagTokens, 0, tagTokenSize * sizeof(int));
    int tagTokenCounter = 0;
    char *currentTag = (char*) malloc(currentTagSize * sizeof(char));
    memset(currentTag, 0, currentTagSize * sizeof(char));
    int currentTagCounter = 0;
    int inTag = 0;
    int tagReading = 0;

    char c;
    while ((c = fgetc(inputFile)) != EOF) {
        if (!inTag) {
            if (c == '<') {
                if (currentTag[0] != '\0') {
                    tagTokenCounter = storeTagToken(tagTokens, tagTokenCounter, currentTag);
                    tagTokens = checkTokenTagPtrMemory(tagTokens, tagTokenCounter);
                    currentTagCounter = resetCurrentTag(currentTag);
                }
                currentTag[currentTagCounter++] = c;
                currentTag = checkCurrentTagPtrMemory(currentTag, currentTagCounter);
                inTag = 1;
                tagReading = 1;
            } else if (!isSpace(c)) {
                if (c == '>') {
                    errorDetected();
                }
                currentTag[currentTagCounter++] = c;
                currentTag = checkCurrentTagPtrMemory(currentTag, currentTagCounter);
            }
        } else {
            if (c == '>') {
                currentTag[currentTagCounter++] = c;
                currentTag = checkCurrentTagPtrMemory(currentTag, currentTagCounter);
                inTag = 0;
                tagReading = 0;
                tagTokenCounter = storeTagToken(tagTokens, tagTokenCounter, currentTag);
                tagTokens = checkTokenTagPtrMemory(tagTokens, tagTokenCounter);
                currentTagCounter = resetCurrentTag(currentTag);
            } else if (c == '<') errorDetected();
            else if (!tagReading) continue;
            else if (c == ' ') tagReading = 0;
            else if (tagReading) {
                currentTag[currentTagCounter++] = c;
                currentTag = checkCurrentTagPtrMemory(currentTag, currentTagCounter);
            }
        }
    }
    checkIfLastTag(currentTag);
    numOfTokens = tagTokenCounter;
    free(currentTag);
    return tagTokens;
}

// --- PARSING ---
int parsing(int *tagTokensPtr) {
    int tagFreq[NUM_OF_SINGLE_FREQ_TAGS];
    memset(tagFreq, 0, NUM_OF_SINGLE_FREQ_TAGS * sizeof(int));
    int tagTokenCounter = 0;
    int headJustSeen = 0;
    int inP = 0;
    struct Stack *stack = createStack();

    while(tagTokenCounter < numOfTokens) {
        isWrappedByHtml(tagTokensPtr, tagTokenCounter);
        updateTagFreq(*tagTokensPtr, tagFreq);
        headJustSeen = isHeadFollowedByBody(*tagTokensPtr, headJustSeen);
        if (*tagTokensPtr == HEAD_CT) headJustSeen = 1;
        if (*tagTokensPtr == BR || *tagTokensPtr == HR || *tagTokensPtr == TEXT) {
            //do nothing
        } else if (operateOnStack(stack, *tagTokensPtr, inP)) errorDetected();
        inP = updateInP(inP, *tagTokensPtr);
        tagTokenCounter ++;
        tagTokensPtr ++;
    }
    freeStack(stack, stack->array);
    return tagFreqIsValid(tagFreq);
}

void isValidHTML() {
    FILE *inputFile = fopen("file.html", "r");
    if (inputFile == NULL) {
        perror("Unable to open file");
        return;
    }
    int *tagTokensPtr = executeLexing(inputFile);
    if (parsing(tagTokensPtr)) {
        free(tagTokensPtr);
        printf("PARSING SUCCESSFULL: 'file.html' is a valid HTML file.\n");
    } else {
        errorDetected();
    }
}

int main() {
    isValidHTML();
    return 0;
};