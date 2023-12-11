#include <stdio.h>
#include <string.h>

#define MAX_RULES 10
#define MAX_LENGTH 10

// Structure to represent a production rule
struct Rule {
    char nonTerminal;
    char body[MAX_LENGTH];
};

// Function to check if a character is a terminal
int isTerminal(char ch) {
    return !(ch >= 'A' && ch <= 'Z');
}

// Function to compute First set for a given non-terminal
void computeFirst(char nonTerminal, struct Rule rules[MAX_RULES], int numRules, char firstSets[MAX_RULES][MAX_LENGTH]) {
    for (int i = 0; i < numRules; i++) {
        if (rules[i].nonTerminal == nonTerminal) {
            // If the first character is a terminal, add it to the First set
            if (isTerminal(rules[i].body[0])) {
                int len = strlen(firstSets[i]);
                firstSets[i][len] = rules[i].body[0];
                firstSets[i][len + 1] = '\0';
            }
            // If the first character is a non-terminal, recursively compute First set
            else {
                computeFirst(rules[i].body[0], rules, numRules, firstSets);
            }
        }
    }
}

// Function to compute Follow set for a given non-terminal
void computeFollow(char nonTerminal, struct Rule rules[MAX_RULES], int numRules, char followSets[MAX_RULES][MAX_LENGTH]) {
    for (int i = 0; i < numRules; i++) {
        // Find occurrences of the non-terminal in the body of production rules
        char *pos = strchr(rules[i].body, nonTerminal);
        
        while (pos != NULL) {
            // If the non-terminal is at the end, add Follow of the left-hand side
            if (*(pos + 1) == '\0') {
                // Add Follow set of the left-hand side to the current Follow set
                strcat(followSets[i], followSets[/*index of left-hand side*/]);
            } else {
                // If the next character is a terminal, add it to the Follow set
                if (isTerminal(*(pos + 1))) {
                    int len = strlen(followSets[i]);
                    followSets[i][len] = *(pos + 1);
                    followSets[i][len + 1] = '\0';
                } else {
                    // If the next character is a non-terminal, add its First set (excluding epsilon)
                    char nextNonTerminal = *(pos + 1);
                    computeFirst(nextNonTerminal, rules, numRules, followSets[i]);
                    
                    // If epsilon is in the First set, add Follow of the left-hand side
                    if (strchr(followSets[i], 'e') != NULL) {
                        // Add Follow set of the left-hand side to the current Follow set
                        strcat(followSets[i], followSets[/*index of left-hand side*/]);
                    }
                }
            }
            
            // Move to the next occurrence of the non-terminal in the body
            pos = strchr(pos + 1, nonTerminal);
        }
    }
}

int main() {
    int numRules;
    printf("Enter the number of production rules: ");
    scanf("%d", &numRules);

    struct Rule rules[MAX_RULES];

    // Input production rules
    for (int i = 0; i < numRules; i++) {
        printf("Enter production rule %d (nonTerminal -> body): ", i + 1);
        scanf(" %c -> %s", &rules[i].nonTerminal, rules[i].body);
    }

    char firstSets[MAX_RULES][MAX_LENGTH];
    char followSets[MAX_RULES][MAX_LENGTH];

    // Initialize Follow sets with the empty string
    for (int i = 0; i < numRules; i++) {
        followSets[i][0] = '\0';
    }

    // Compute First sets for each non-terminal
    for (int i = 0; i < numRules; i++) {
        computeFirst(rules[i].nonTerminal, rules, numRules, firstSets[i]);
        printf("First(%c): %s\n", rules[i].nonTerminal, firstSets[i]);
    }

    // Compute Follow sets for each non-terminal
    for (int i = 0; i < numRules; i++) {
        computeFollow(rules[i].nonTerminal, rules, numRules, followSets[i]);
        printf("Follow(%c): %s\n", rules[i].nonTerminal, followSets[i]);
    }

    return 0;
}
