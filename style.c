#include <stdio.h>

void changeTextblue() {
    printf("\033[0;34m");
}

void changeTextRed() {
    printf("\033[0;31m");
}

void changeTextItalic() {
    printf("\033[3m\033[38;5;208m");
}

void changeTextGreen() {
    printf("\033[0;32m");
}

void logRed(const char *message) {
    changeTextRed();
    printf("%s\n", message);
    printf("\033[0m"); // Reset to default color
}

void logGreen(const char *message) {
    changeTextGreen();
    printf("%s\n", message);
    printf("\033[0m"); // Reset to default color
}

