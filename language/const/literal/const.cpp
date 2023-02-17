int foo(int i) {
    return i + 109909;
}
int main() {
    int i = 1090090;
    char* t = (char *)"Why are you here so that the program can detect the difference of small string that can never be held in the assembly code and have to be place in the const degment";
    foo(i);
    int *p = &i;

}
